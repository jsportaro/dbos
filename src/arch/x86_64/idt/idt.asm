section .asm

extern Int21hHandler
extern NoInterruptHandler

global IDTEnableInterrupts
global IDTDisableInterrupts
global IDTLoad
global Int21h
global NoInterrupt

IDTEnableInterrupts:
    sti
    ret

IDTDisableInterrupts:
    cli
    ret

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