/* memory.h - Memory layout symbols and helpers for GlitchOS */
#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

/* Kernel load address (set in linker script) */
#define KERNEL_BASE 0x1000

/* Video RAM */
#define VIDEO_MEMORY_ADDR 0xB8000

/* Stack */
#define KERNEL_STACK_TOP 0x90000
#define KERNEL_STACK_SIZE 0x4000

/* Linker-provided symbols (see linker.ld)
 * Use these to get section boundaries and sizes from C.
 */
extern unsigned char __kernel_start[];
extern unsigned char __kernel_end[];
extern unsigned char __text_start[];
extern unsigned char __text_end[];
extern unsigned char __rodata_start[];
extern unsigned char __rodata_end[];
extern unsigned char __data_start[];
extern unsigned char __data_end[];
extern unsigned char __bss_start[];
extern unsigned char __bss_end[];
extern unsigned char __stack_top[];

static inline uintptr_t kernel_size(void) {
    return (uintptr_t)__kernel_end - (uintptr_t)__kernel_start;
}

#endif
