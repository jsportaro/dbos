section .asm

global EnableInterrupts
global DisableInterrupts
global insb
global insw
global outb
global outw

insb:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov edx, [ebp+8]
    in al, dx
    ; Remember - EAX is always the return value in C

    pop ebp
    ret

insw:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov edx, [ebp+8]
    in ax, dx
    ; Remember - EAX is always the return value in C

    pop ebp
    ret

outb:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, al
    ; Remember - EAX is always the return value in C

    pop ebp
    ret

outw:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, ax
    ; Remember - EAX is always the return value in C

    pop ebp
    ret


EnableInterrupts:
    sti
    ret

DisableInterrupts:
    cli
    ret