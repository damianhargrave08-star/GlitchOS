bits 16
org 0x7C00

; FAT12-compatible boot sector (BPB for 1.44MB floppy)

jmp short start
nop
OEM db 'MSDOS5.0'        ; OEM Name (8)

BytesPerSector dw 512
SectorsPerCluster db 1
ReservedSectors dw 1
NumFATs db 2
RootEntries dw 224
TotalSectors16 dw 2880
Media db 0xF0
SectorsPerFAT dw 9
SectorsPerTrack dw 18
NumberOfHeads dw 2
HiddenSectors dd 0
TotalSectors32 dd 0

DriveNumber db 0
Reserved db 0
BootSignature db 0x29
VolumeID dd 0x12345678
VolumeLabel db 'GLITCH OS    '
FileSystemType db 'FAT12   '

start:
    cli
    xor ax, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    mov si, boot_msg
    call print_string

    mov si, loading_msg
    call print_string

    ; Jump to kernel at 0x1000 (assumes kernel is already loaded there)
    mov ax, 0x1000
    mov ds, ax
    mov es, ax
    jmp 0x1000:0x0000

print_string:
    mov ah, 0x0E
.print_loop:
    lodsb
    cmp al, 0
    je .print_done
    int 0x10
    jmp .print_loop
.print_done:
    ret

boot_msg db 'GlitchOS Bootloader - 16-bit', 0x0D, 0x0A, 0
loading_msg db 'Loading kernel...', 0x0D, 0x0A, 0

; Pad to 510 bytes and add boot signature
times 510 - ($ - $$) db 0
dw 0xAA55
