bits 16
org 0x7C00

start:
	cli
	xor ax, ax
	mov ss, ax
	mov sp, 0x7C00
	sti

	mov si, msg
	call print_string

    mov si, loading_msg
    call print_string

    ; Load kernel from disk (simplified: assumes kernel at sector 1)
    ; In a real bootloader this would use BIOS int 13h to read sectors
    ; For now, we'll jump directly to kernel memory (0x1000)
    ; assuming the ISO has placed the kernel there
    
    mov ax, 0x1000
    mov ds, ax
    mov es, ax
    jmp 0x1000:0x0000

print_string:
    mov ah, 0x0E
.next:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .next
.done:
    ret

msg db 'GlitchOS Bootloader - 16-bit', 0x0D, 0x0A, 0
loading_msg db 'Loading kernel...', 0x0D, 0x0A, 0
