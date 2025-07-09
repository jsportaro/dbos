ORG 0x7c00 
BITS 16

_start:
    jmp short start
    nop

times 33 db 0       ; BPB (all zeros - who cares)

start:
    jmp 0:step_2

step_2:
                    ; The following defensively sets
                    ; the segment (and SS register) 
                    ; to known values
    cli             ; Clear Interrupts
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti             ; Enables Interrupts


    mov si, message
    call print
    jmp $

print:
    mov bx, 0
.loop:
    lodsb
    cmp al, 0
    je .done
    call print_char
    jmp .loop
.done:
    ret

print_char:
    mov ah, 0Eh
    int 0x10
    ret

message: db 'Hello world!', 0
times 510-($ - $$) db 0
dw 0xAA55
