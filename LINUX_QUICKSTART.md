# GlitchOS - Linux Terminal Quick Start

## Prerequisites

Install QEMU on your Linux system:

```bash
# Ubuntu/Debian
sudo apt-get install qemu-system

# Fedora/RHEL
sudo dnf install qemu-system

# Arch
sudo pacman -S qemu-system

# macOS (Homebrew)
brew install qemu
```

## Boot the ISO

### Option 1: Using the boot script (easiest)

```bash
cd GlitchOS
./run.sh
```

This will:
1. Launch QEMU with the ISO
2. Display a QEMU window showing boot output
3. Save serial output to `serial.log`
4. Print the boot messages to your terminal when done

### Option 2: Direct QEMU command (32-bit)

```bash
qemu-system-i386 -m 64M -cdrom glitchos.iso -boot d -serial file:serial.log
```

### Option 3: Direct QEMU command (64-bit)

```bash
qemu-system-x86_64 -m 64M -cdrom glitchos.iso -boot d -serial file:serial.log
```

Both 32-bit and 64-bit QEMU will work—the bootloader is 16-bit real mode code.

### Option 4: Headless mode (no display)

For servers or remote Linux systems without X11:

```bash
qemu-system-x86_64 \
    -m 64M \
    -cdrom glitchos.iso \
    -boot d \
    -display none \
    -serial file:serial.log &

sleep 30

# View boot output
cat serial.log
```

## Write to USB (Real Hardware)

To boot on actual x86_64 hardware:

```bash
# Find your USB device
lsblk

# Unmount if mounted
sudo umount /dev/sdX*

# Write the ISO (replace sdX with your device)
sudo dd if=glitchos.iso of=/dev/sdX bs=4M && sync

# Eject
sudo eject /dev/sdX
```

⚠️ **Warning**: Double-check the device name! `dd` can overwrite your hard drive!

## View Boot Messages

After running QEMU, check the serial output:

```bash
cat serial.log
```

Example output:
```
GlitchOS stage1: loading loader...
Reading loader (max 20 sectors)...
Disk read error (falling back)
Loader: jumping to kernel...
```

## Troubleshooting

### QEMU not found
```bash
which qemu-system-x86_64
# If not found, install it (see Prerequisites section)
```

### Permission denied on USB
Use `sudo` with `dd`:
```bash
sudo dd if=glitchos.iso of=/dev/sdX bs=4M
```

### No serial output
Make sure you're using the correct command with `-serial file:serial.log`:
```bash
qemu-system-x86_64 -m 64M -cdrom glitchos.iso -boot d -serial file:serial.log
```

### Can't close QEMU window
Press `Ctrl+C` in the terminal or click the window's close button.

## Build from Source

To rebuild the ISO:

```bash
cd GlitchOS
bash build.sh
```

Requirements:
- `nasm` (assembler)
- `gcc` (C compiler)
- `ld` (linker)
- `xorriso` (ISO creation)

Install on Ubuntu/Debian:
```bash
sudo apt-get install nasm gcc binutils xorriso
```

---

**Enjoy GlitchOS!** 🎉

also see readme.md to see if this OS Is even operable.
**ENJOY!**