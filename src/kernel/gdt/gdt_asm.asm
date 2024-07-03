[bits 32]
global load_gdt
gdtr    dw 0
        dd 0

load_gdt:
    mov eax, DWORD [esp + 4]
    lgdt [eax]
    call reloadSegments
    ret
reloadSegments:
   jmp   0x08:.reload_CS 
.reload_CS:
   mov   ax, 0x10 
   mov   ds, ax
   mov   es, ax
   mov   fs, ax
   mov   gs, ax
   mov   ss, ax
   ret