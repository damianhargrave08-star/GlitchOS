ENTRY(kernel_main)

/*
 * Linker script for GlitchOS
 * - Kernel is linked to run at physical address 0x1000 (real-mode jump target)
 * - Define symbols for section boundaries so the kernel can inspect sizes
 */
SECTIONS
{
    /* Load address / execution base for the kernel
     * Kernel is concatenated into the El Torito boot image after
     * boot.bin (0x200) and loader.bin (0x1000) when the ISO is
     * built. When the BIOS loads the full boot image at 0x7C00,
     * the kernel's physical load address is 0x8E00. Link the kernel
     * to that address so runtime pointers and absolute references
     * match the loader's jump target. */
    . = 0x9000;

    /* Mark start of the kernel image */
    __kernel_start = .;

    .text : ALIGN(0x10)
    {
        __text_start = .;
        *(.text .text.*)
        *(.rodata .rodata.*)
        __text_end = .;
    }

    .rodata : ALIGN(0x10)
    {
        __rodata_start = .;
        *(.rodata .rodata.*)
        __rodata_end = .;
    }

    .data : ALIGN(0x10)
    {
        __data_start = .;
        *(.data .data.*)
        __data_end = .;
    }

    .bss : ALIGN(0x10)
    {
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    }

    /* Mark end of kernel image */
    __kernel_end = .;

    /* Provide a stack top symbol; kernel will use this as a reference for
     * setting up the initial stack. This is a virtual address and can be
     * adjusted as needed. */
    PROVIDE(__stack_top = 0x90000);
}

/* Provide some useful aliases for C code */
PROVIDE(_kernel_start = __kernel_start);
PROVIDE(_kernel_end = __kernel_end);
PROVIDE(_stack_top = __stack_top);

