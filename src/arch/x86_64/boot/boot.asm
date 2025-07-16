ORG 0x7c00
BITS 16

CODE_SEL equ gdt_code - gdt_start
DATA_SEL equ gdt_data - gdt_start

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
    jmp CODE_SEL:load32

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

[BITS 32]
load32:
    mov eax, 1          ; Sector to start reading
    mov ecx, 100        ; Number of sectors to read - Must be less than
                        ; BINARY_SECTORS in the Makefile
    mov edi, 0x0100000 ; Must match the linker script - Where we're loading
                        ; sectors too
    call ata_lba_read
    jmp CODE_SEL:0x0100000

ata_lba_read:
    mov ebx, eax    ; Backup the LBA
    ; Send the highest 8 bits of the lba to the hard disk controller
    shr eax, 24
    or eax, 0xE0 ; Select the  master drive
    mov dx, 0x1F6
    out dx, al

    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1f2
    out dx, al

    ; Send more bits to the LBA
    mov eax, ebx    ; Restore the backup LBA
    mov dx, 0x1f3
    out dx, al

    mov eax, ebx   ; Restore the backup LBA
    mov dx, 0x1f4
    shr eax, 8
    out dx, al

    ; Send upper 16 bits of the LBA
    mov eax, ebx
    mov dx, 0x1f5
    shr eax, 16
    out dx, al

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

    ; Read all sectors into memory
.next_sector:
    push ecx

; Checking if we need to read
.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

; We need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1f0
    rep insw
    pop ecx
    loop .next_sector

    ret

times 510-($ - $$) db 0
dw 0xAA55