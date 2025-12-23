bits 16
org 0x7C00

; FAT12-compatible boot sector (BPB for 1.44MB floppy)

jmp short start
bits 16
org 0x7C00

; Minimal stage1 boot sector with FAT12 BPB for 1.44MB floppy

jmp short start
nop
OEM db 'MSDOS5.0'

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

    ; Save drive (use DL if valid, otherwise default to CD/floppy 0x00)
    ; Under El Torito, DL might not be valid; use 0x00 as safe default for INT 0x13 CDROM emulation
    cmp dl, 0x00
    je .drive_ok
    cmp dl, 0x80
    jl .drive_ok      ; 0x00-0x7F are floppy, OK
    cmp dl, 0xFF
    jbe .drive_ok     ; 0x80-0xFF are hard disk, OK
    mov dl, 0x00      ; default to first floppy/CD
.drive_ok:
    mov [boot_drive], dl

    ; Load up to 20 sectors starting at LBA=1 into 0x8000 (segment 0x0800)
    mov word [load_count], 20
    mov ax, 0x0800
    mov es, ax
    xor bx, bx
    mov si, 1            ; starting LBA (sector 1)
load_loop:
    push si
    push bx
    push es
    ; compute CHS for LBA in SI
    mov ax, si
    xor dx, dx
    mov bx, [SectorsPerTrack]
    div bx                ; AX = temp, DX = rem
    mov di, ax            ; temp
    mov al, dl            ; sector-1 in AL
    mov ah, 0
    mov bx, [NumberOfHeads]
    mov ax, di
    xor dx, dx
    div bx                ; AX = cylinder, DX = head
    mov ch, al
    mov dh, dl
    mov cl, al            ; temporarily put sector in CL
    ; compute sector properly: remainder from first div is in DL from earlier; recompute
    mov ax, si
    xor dx, dx
    mov bx, [SectorsPerTrack]
    div bx
    mov dx, dx
    mov cl, dl
    inc cl
    and cl, 0x3F

    mov dl, [boot_drive]
    mov ah, 0x02
    mov al, 1
    int 0x13
    jc disk_fail

    pop es
    pop bx
    pop si
    inc si
    dec word [load_count]
    jnz load_loop

    ; jump to loader at 0x8000
    jmp 0x0800:0x0000

disk_fail:
    mov si, ioerr_msg
    call print_string
    ; If disk reads fail (common under El Torito no-emulation),
    ; the BIOS may have already loaded the full boot image at 0x7C00.
    ; Stage2 is at offset 0x200 from start of boot image.
    ; So jump to 0x7E00 (0x7C00 + 0x200 = physical 0x7E00 = segment 0x07E0:0000)
    jmp 0x07E0:0x0000

print_string:
    mov ah, 0x0E
.print_loop:
    lodsb
    cmp al, 0
    je .print_done
    int 0x10
    ; also output to serial port (COM1: port 0x3F8)
    mov dx, 0x3F8
    out dx, al
    jmp .print_loop
.print_done:
    ret

boot_msg db 'GlitchOS stage1: loading loader...',0x0D,0x0A,0
loading_msg db 'Reading loader (max 20 sectors)...',0x0D,0x0A,0
ioerr_msg db 'Disk read error (falling back)',0x0D,0x0A,0

; small variables
boot_drive db 0
load_count dw 0

; Pad to 510 bytes and add boot signature
times 510 - ($ - $$) db 0
dw 0xAA55
    ; Compute starting LBA = first_data_sector + (start_cluster - 2) * SectorsPerCluster
