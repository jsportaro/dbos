ORG 0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short start
    nop

times 33 db 0       ; BPB (all zeros - who cares)

start:
    jmp 0x00:step_2

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

.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or  eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32

; GDT
gdt_start:
gdt_null:
    dd 0x00000000
    dd 0x00000000

; offset 0x08 - default values
; just need this to get into paging
gdt_code:       ; CS should point to this
    dw 0xffff   ; Segment limit 
    dw 0x0000   ; Base
    db 0x00     ;
    db 0x9a     ; Access Byte
    db 0xcf     ; 11001111
    db 0x00

; offset 0x10
gdt_data:       ; DS, SS, ES, FS, GS
    dw 0xffff   ; Segment limit 
    dw 0x0000   ; Base
    db 0x00     ;
    db 0x92     ; Access Byte
    db 0xcf     ; 11001111
    db 0x00

gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

BITS 32
load32:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp
    jmp $

times 510-($ - $$) db 0
dw 0xAA55