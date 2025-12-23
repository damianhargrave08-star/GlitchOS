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
    
    ; Jump to kernel at 0x8C00 (segment 0x08C0, offset 0x0000)
    ; Memory layout at 0x7C00: boot.bin(512B) + loader.bin(4096B) + kernel.bin
    jmp 0x08C0:0x0000

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
