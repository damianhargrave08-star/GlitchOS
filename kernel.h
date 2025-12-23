#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>

/* Video memory address */
#define VIDEO_MEMORY 0xB8000

/* Screen dimensions */
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

/* VGA text mode colors */
typedef enum {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GRAY = 7,
    DARK_GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
} VGA_Color;

/* Terminal state */
typedef struct {
    uint16_t cursor_x;
    uint16_t cursor_y;
    VGA_Color foreground;
    VGA_Color background;
} Terminal;

void kernel_main();
void terminal_initialize(VGA_Color fg, VGA_Color bg);
void terminal_write_char(char c);
void terminal_write_string(const char *str);
void terminal_clear();
void terminal_newline();

#endif
