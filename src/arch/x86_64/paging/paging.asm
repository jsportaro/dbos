[BITS 32]

section .asm

global PagingLoadDirectory
global PagingEnable

PagingLoadDirectory:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr3, eax
    pop ebp
    ret

PagingEnable:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    pop ebp
    ret