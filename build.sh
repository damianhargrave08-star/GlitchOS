#!/bin/bash
# GlitchOS Build Script

set -e

echo "=== GlitchOS Build System ==="
echo ""

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Check for required tools
echo -e "${BLUE}[*] Checking required tools...${NC}"
command -v nasm >/dev/null 2>&1 || { echo "Error: nasm not found"; exit 1; }
command -v gcc >/dev/null 2>&1 || { echo "Error: gcc not found"; exit 1; }
command -v ld >/dev/null 2>&1 || { echo "Error: ld not found"; exit 1; }
command -v mkisofs >/dev/null 2>&1 || { echo "Error: mkisofs not found"; exit 1; }
echo -e "${GREEN}[+] All required tools found${NC}"
echo ""

# Step 1: Assemble bootloader
echo -e "${BLUE}[*] Assembling bootloader...${NC}"
nasm -f bin main.asm -o boot.bin
echo -e "${GREEN}[+] Bootloader created: boot.bin (512 bytes)${NC}"
echo ""

# Step 2: Compile C source files
echo -e "${BLUE}[*] Compiling C source files...${NC}"
gcc -c kernel.c -o kernel.o -ffreestanding -Wall -Wextra -Werror
echo -e "${GREEN}[+] kernel.c compiled${NC}"

gcc -c desktop.c -o desktop.o -ffreestanding -Wall -Wextra -Werror
echo -e "${GREEN}[+] desktop.c compiled${NC}"

gcc -c apps.c -o apps.o -ffreestanding -Wall -Wextra -Werror
echo -e "${GREEN}[+] apps.c compiled${NC}"

gcc -c app_implementations.c -o app_impl.o -ffreestanding -Wall -Wextra -Werror
echo -e "${GREEN}[+] app_implementations.c compiled${NC}"
echo ""

# Step 3: Link kernel
echo -e "${BLUE}[*] Linking kernel...${NC}"
ld -T linker.ld -o kernel.elf kernel.o desktop.o apps.o app_impl.o
echo -e "${GREEN}[+] Kernel linked: kernel.elf${NC}"
echo ""

# Step 4: Extract kernel binary
echo -e "${BLUE}[*] Extracting kernel binary...${NC}"
objcopy -O binary kernel.elf kernel.bin
echo -e "${GREEN}[+] Kernel binary created: kernel.bin${NC}"
echo ""

# Step 5: Create ISO directory structure
echo -e "${BLUE}[*] Creating ISO directory structure...${NC}"
rm -rf iso_root
mkdir -p iso_root/boot
cp boot.bin iso_root/boot/
cp kernel.bin iso_root/boot/
echo -e "${GREEN}[+] ISO directory created${NC}"
echo ""

# Step 6: Create bootable ISO
echo -e "${BLUE}[*] Creating bootable ISO image...${NC}"
cd iso_root && mkisofs -R -b boot/boot.bin -no-emul-boot -boot-load-size 4 -o ../glitchos.iso . && cd ..
echo -e "${GREEN}[+] ISO image created: glitchos.iso${NC}"
echo ""

# Step 7: Display build summary
echo -e "${GREEN}=== Build Complete ===${NC}"
echo ""
echo "Artifacts created:"
ls -lh boot.bin kernel.elf kernel.bin glitchos.iso 2>/dev/null || true
echo ""
echo "To test the ISO:"
echo "  qemu-system-x86_64 -cdrom glitchos.iso"
echo ""
