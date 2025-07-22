section .asm

extern Int21hHandler
extern NoInterruptHandler

global IDTLoad
global Int21h
global NoInterrupt


IDTLoad:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+8]
    lidt [ebx]
    pop ebp
    ret

Int21h:
    cli
    pushad
    call Int21hHandler
    popad
    sti
    iret

NoInterrupt:
    cli
    pushad
    call NoInterruptHandler
    popad
    sti
    iret