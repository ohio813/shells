@echo off
yasm -DBIN x86.asm -ox86.bin
yasm -fwin32 x86.asm -ox86.obj
cl /DTEST getapi.c x86.obj
del *.obj