bits 16
org 0x0000

; Stage2 loader for El Torito bootable ISO
; When El Torito boots, the BIOS loads the full boot image at physical 0x7C00:
; - boot.bin (512B) at 0x7C00
; - loader.bin (4096B) at 0x7E00 (where we execute)
; - kernel.bin (variable size) at 0x7E00 + 0x1000 = 0x8E00
;
; This loader simply prints a startup message and jumps to the kernel.

start:
    cli
    xor ax, ax
    mov ss, ax
    mov sp, 0x7C00
    sti
    
    ; Set data segment to where code is executing (critical!)
    mov ax, cs
    mov ds, ax

    ; Print startup message via serial and BIOS
    mov si, loader_msg
    call print
    
    ; Prepare to enter protected mode and jump to kernel at 0x9000
    ; Build a small GDT (null, code, data) and enable PE in CR0.
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:pm_protected

; GDT: null, code (base=0, limit=4GB, access=0x9A), data (access=0x92)
gdt_start:
    dd 0x00000000
    dd 0x00000000
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9A
    db 0xCF
    db 0x00
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

bits 32
pm_protected:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000
    ; Debug: output 'PM' over COM1 to verify protected mode active
    mov dx, 0x3F8
    mov al, 'P'
    out dx, al
    mov al, 'M'
    out dx, al
    jmp 0x08:0x00009000
bits 16

; print: Output SI string via BIOS INT 10h and serial port 0x3F8
print:
    push ax
    push si
    mov ah, 0x0E
.loop:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    ; Also output to COM1 serial port for debugging
    mov dx, 0x3F8
    out dx, al
    jmp .loop
.done:
    pop si
    pop ax
    ret

; Halt the system
halt:
    cli
    hlt
    jmp halt

; Data
loader_msg db 'Loader: jumping to kernel...', 0x0D, 0x0A, 0

; Pad to 4096 bytes
times 4096 - ($ - $$) db 0
