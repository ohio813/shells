AS=nasm
OUTPUT=bin
EXT=bin
all: 
  $(AS) -f$(OUTPUT) x64.asm -ox64.$(EXT)
  $(AS) -f$(OUTPUT) x86.asm -ox86.$(EXT)
  $(AS) -f$(OUTPUT) x84.asm -ox84.$(EXT)
  $(AS) -f$(OUTPUT) rs.asm -ors.$(EXT)
  $(AS) -f$(OUTPUT) bs.asm -obs.$(EXT)
clean:
  rm -rf *.bin *.obj *.o  