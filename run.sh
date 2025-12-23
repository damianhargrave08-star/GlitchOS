#!/bin/bash
# GlitchOS QEMU Boot Script for Linux Terminal
# Run the bootable ISO in QEMU with serial output

set -e

ISO_FILE="${1:-./glitchos.iso}"
LOG_FILE="${2:-serial.log}"
DISPLAY_MODE="${3:-none}"

echo "================================================"
echo "  GlitchOS - Linux Terminal Boot"
echo "================================================"
echo ""
echo "ISO: $ISO_FILE"
echo "Serial log: $LOG_FILE"
echo "Display: $DISPLAY_MODE"
echo ""
echo "Starting QEMU..."
echo "(Press Ctrl+C to stop)"
echo ""

# Run QEMU with serial output to file
qemu-system-x86_64 \
    -m 64M \
    -cdrom "$ISO_FILE" \
    -boot d \
    -display "$DISPLAY_MODE" \
    -serial file:"$LOG_FILE" \
    -no-reboot

echo ""
echo "================================================"
echo "Boot completed!"
echo "================================================"
echo ""
echo "Serial output captured in: $LOG_FILE"
echo ""
if [ -f "$LOG_FILE" ]; then
    echo "--- Serial Log Contents ---"
    cat "$LOG_FILE"
    echo "--- End Serial Log ---"
    echo ""
fi

echo "Display modes available:"
echo "  ./run.sh  glitchos.iso serial.log none     (headless, no display)"
echo "  ./run.sh  glitchos.iso serial.log curses   (terminal display)"
echo "  ./run.sh  glitchos.iso serial.log sdl      (SDL window - requires X11)"
echo "  ./run.sh  glitchos.iso serial.log gtk      (GTK window - requires X11)"
