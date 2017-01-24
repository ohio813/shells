
#define X84_SIZE 206

char X84[] = {
  /* 0000 */ "\x51"                     /* push rcx                    */
  /* 0001 */ "\x52"                     /* push rdx                    */
  /* 0002 */ "\x31\xc0"                 /* xor eax, eax                */
  /* 0004 */ "\x99"                     /* cdq                         */
  /* 0005 */ "\xac"                     /* lodsb                       */
  /* 0006 */ "\x0c\x20"                 /* or al, 0x20                 */
  /* 0008 */ "\x30\xc2"                 /* xor dl, al                  */
  /* 000A */ "\x6a\x08"                 /* push 0x8                    */
  /* 000C */ "\x59"                     /* pop rcx                     */
  /* 000D */ "\xd1\xea"                 /* shr edx, 1                  */
  /* 000F */ "\x73\x06"                 /* jae 0x17                    */
  /* 0011 */ "\x81\xf2\x78\x3b\xf6\x82" /* xor edx, 0x82f63b78         */
  /* 0017 */ "\xe2\xf4"                 /* loop 0xd                    */
  /* 0019 */ "\x3c\x20"                 /* cmp al, 0x20                */
  /* 001B */ "\x75\xe8"                 /* jnz 0x5                     */
  /* 001D */ "\x92"                     /* xchg edx, eax               */
  /* 001E */ "\x5a"                     /* pop rdx                     */
  /* 001F */ "\x59"                     /* pop rcx                     */
  /* 0020 */ "\xc3"                     /* ret                         */
  /* 0021 */ "\x57"                     /* push rdi                    */
  /* 0022 */ "\x53"                     /* push rbx                    */
  /* 0023 */ "\x52"                     /* push rdx                    */
  /* 0024 */ "\x56"                     /* push rsi                    */
  /* 0025 */ "\x03\x55\x3c"             /* add edx, [rbp+0x3c]         */
  /* 0028 */ "\x8b\x4c\x15\x28"         /* mov ecx, [rbp+rdx+0x28]     */
  /* 002C */ "\xe3\x4d"                 /* jrcxz 0x7b                  */
  /* 002E */ "\x56"                     /* push rsi                    */
  /* 002F */ "\x8b\x74\x0d\x0c"         /* mov esi, [rbp+rcx+0xc]      */
  /* 0033 */ "\x48\x01\xee"             /* add rsi, rbp                */
  /* 0036 */ "\xe8\xc5\xff\xff\xff"     /* call 0x0                    */
  /* 003B */ "\x97"                     /* xchg edi, eax               */
  /* 003C */ "\x8b\x74\x0d\x1c"         /* mov esi, [rbp+rcx+0x1c]     */
  /* 0040 */ "\x48\x01\xee"             /* add rsi, rbp                */
  /* 0043 */ "\x8b\x54\x0d\x20"         /* mov edx, [rbp+rcx+0x20]     */
  /* 0047 */ "\x48\x01\xea"             /* add rdx, rbp                */
  /* 004A */ "\x8b\x5c\x0d\x24"         /* mov ebx, [rbp+rcx+0x24]     */
  /* 004E */ "\x48\x01\xeb"             /* add rbx, rbp                */
  /* 0051 */ "\x8b\x4c\x0d\x18"         /* mov ecx, [rbp+rcx+0x18]     */
  /* 0055 */ "\x58"                     /* pop rax                     */
  /* 0056 */ "\xe3\x23"                 /* jrcxz 0x7b                  */
  /* 0058 */ "\x53"                     /* push rbx                    */
  /* 0059 */ "\x56"                     /* push rsi                    */
  /* 005A */ "\x93"                     /* xchg ebx, eax               */
  /* 005B */ "\x8b\x74\x8a\xfc"         /* mov esi, [rdx+rcx*4-0x4]    */
  /* 005F */ "\x48\x01\xee"             /* add rsi, rbp                */
  /* 0062 */ "\xe8\x99\xff\xff\xff"     /* call 0x0                    */
  /* 0067 */ "\x01\xf8"                 /* add eax, edi                */
  /* 0069 */ "\x39\xd8"                 /* cmp eax, ebx                */
  /* 006B */ "\xe0\xee"                 /* loopne 0x5b                 */
  /* 006D */ "\x5e"                     /* pop rsi                     */
  /* 006E */ "\x5b"                     /* pop rbx                     */
  /* 006F */ "\x75\x0a"                 /* jnz 0x7b                    */
  /* 0071 */ "\x0f\xb7\x1c\x4b"         /* movzx ebx, word [rbx+rcx*2] */
  /* 0075 */ "\x8b\x0c\x9e"             /* mov ecx, [rsi+rbx*4]        */
  /* 0078 */ "\x48\x01\xe9"             /* add rcx, rbp                */
  /* 007B */ "\x5e"                     /* pop rsi                     */
  /* 007C */ "\x5a"                     /* pop rdx                     */
  /* 007D */ "\x5b"                     /* pop rbx                     */
  /* 007E */ "\x5f"                     /* pop rdi                     */
  /* 007F */ "\xc3"                     /* ret                         */
  /* 0080 */ "\x53"                     /* push rbx                    */
  /* 0081 */ "\x57"                     /* push rdi                    */
  /* 0082 */ "\x56"                     /* push rsi                    */
  /* 0083 */ "\x55"                     /* push rbp                    */
  /* 0084 */ "\x51"                     /* push rcx                    */
  /* 0085 */ "\x5e"                     /* pop rsi                     */
  /* 0086 */ "\x31\xdb"                 /* xor ebx, ebx                */
  /* 0088 */ "\xf7\xe3"                 /* mul ebx                     */
  /* 008A */ "\xb3\x30"                 /* mov bl, 0x30                */
  /* 008C */ "\x48\x79\x13"             /* jns 0xa2                    */
  /* 008F */ "\x8b\x74\x24\x14"         /* mov esi, [rsp+0x14]         */
  /* 0093 */ "\x64\x8b\x3b"             /* mov edi, [fs:rbx]           */
  /* 0096 */ "\x8b\x7f\x0c"             /* mov edi, [rdi+0xc]          */
  /* 0099 */ "\x8b\x7f\x0c"             /* mov edi, [rdi+0xc]          */
  /* 009C */ "\xb3\x18"                 /* mov bl, 0x18                */
  /* 009E */ "\xb2\x50"                 /* mov dl, 0x50                */
  /* 00A0 */ "\xeb\x1d"                 /* jmp 0xbf                    */
  /* 00A2 */ "\xb2\x60"                 /* mov dl, 0x60                */
  /* 00A4 */ "\x65\x48\x8b\x3a"         /* mov rdi, [gs:rdx]           */
  /* 00A8 */ "\x8b\x7f\x18"             /* mov edi, [rdi+0x18]         */
  /* 00AB */ "\x8b\x7f\x10"             /* mov edi, [rdi+0x10]         */
  /* 00AE */ "\xeb\x0f"                 /* jmp 0xbf                    */
  /* 00B0 */ "\xe8\x6c\xff\xff\xff"     /* call 0x21                   */
  /* 00B5 */ "\x48\x85\xc9"             /* test rcx, rcx               */
  /* 00B8 */ "\x51"                     /* push rcx                    */
  /* 00B9 */ "\x58"                     /* pop rax                     */
  /* 00BA */ "\x75\x0d"                 /* jnz 0xc9                    */
  /* 00BC */ "\x48\x8b\x3f"             /* mov rdi, [rdi]              */
  /* 00BF */ "\x48\x8b\x2c\x1f"         /* mov rbp, [rdi+rbx]          */
  /* 00C3 */ "\x48\x85\xed"             /* test rbp, rbp               */
  /* 00C6 */ "\x75\xe8"                 /* jnz 0xb0                    */
  /* 00C8 */ "\x95"                     /* xchg ebp, eax               */
  /* 00C9 */ "\x5d"                     /* pop rbp                     */
  /* 00CA */ "\x5e"                     /* pop rsi                     */
  /* 00CB */ "\x5f"                     /* pop rdi                     */
  /* 00CC */ "\x5b"                     /* pop rbx                     */
  /* 00CD */ "\xc3"                     /* ret                         */
};