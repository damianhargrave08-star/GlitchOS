; pm_entry.asm
; 16-bit real-mode stub that switches CPU to 32-bit protected mode
; and then jumps to the kernel linked at 0x9000.

bits 16
org 0x0000

start:
    cli
    xor ax, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Setup GDT
    lgdt [gdt_descriptor]

    ; Enable protected mode (set CR0.PE)
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump to flush prefetch and enter protected mode
    jmp 0x08:pm_protected

; GDT: null, code (base=0, limit=4GB, access=0x9A), data (access=0x92)
gdt_start:
    ; NULL descriptor
    dd 0x00000000
    dd 0x00000000

    ; Code descriptor
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9A
    db 0xCF
    db 0x00

    ; Data descriptor
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0xCF
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; Protected-mode code
bits 32
pm_protected:
    ; Set up data segments to the flat data selector (0x10)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Set up a stack in higher memory
    mov esp, 0x90000

    ; Jump to kernel at 0x9000 (kernel is linked to this address)
    jmp 0x08:0x00009000

; Pad to 512 bytes so this stub occupies one sector
times 512 - ($ - $$) db 0
; kernel shows desktop here
 jmp 0x42 ; Jumps to magic boot number 4d 5a in desktop.c and starts desktop.c at global desktop state