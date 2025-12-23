#!/bin/bash
# GlitchOS QEMU Boot Script for Linux Terminal
# Run the bootable ISO in QEMU with serial output

set -e

ISO_FILE="${1:-./ glitchos.iso}"
LOG_FILE="${2:-serial.log}"

echo "================================================"
echo "  GlitchOS - Linux Terminal Boot"
echo "================================================"
echo ""
echo "ISO: $ISO_FILE"
echo "Serial log: $LOG_FILE"
echo ""
echo "Starting QEMU..."
echo "(Close the QEMU window or press Ctrl+C to stop)"
echo ""

# Run QEMU with serial output to file
qemu-system-x86_64 \
    -m 64M \
    -cdrom "$ISO_FILE" \
    -boot d \
    -display gtk \
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

echo "To view boot messages without QEMU GUI, run:"
echo "  tail -f $LOG_FILE"
