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

hang:
	jmp hang

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

msg db 'GlitchOS Bootloader - 16-bit', 0

times 510 - ($ - $$) db 0
dw 0xAA55

