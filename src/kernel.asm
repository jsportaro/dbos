[BITS 32]
global _start       ; Exports _start symbol

extern KernelMain

CODE_SEL equ 0x08
DATA_SEL equ 0x10

_start:
    mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; enable A20
    in al, 0x92
    or al, 2
    out 0x92, al

    call KernelMain
    jmp $

; As mentioned in the linker file,
; we want all the assembly to go AFTER all C code
; to keep the alignment correct.  In other words,
; the C compiler will get all the alignment by default.
; But, if the assembly (that could be any arbitary size)
; is before it'll mess that alignment up.  So, we stick
; all the assembly at the end and not have to worry about it
;
; BUT - we need to jump to some assembly here.  So we pad the
; file with 0's till we get to c
times 512-($ - $$) db 0