bits 32
; if ZF is 1, we're 64-bit, else 32-bit
test_64: 
  xor    eax, eax
  inc    eax
  xchg   eax, edx
  jz     is_64
  ; 32-bit code here
is_64: