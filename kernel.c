#include "kernel.h"
#include "apps.h"
#include "desktop.h"

/* Global terminal state */
static Terminal g_terminal;

/* Helper to write to video memory */
static void write_to_vram(uint16_t x, uint16_t y, char c) {
    uint16_t *vram = (uint16_t *)VIDEO_MEMORY;
    uint16_t index = y * SCREEN_WIDTH + x;
    uint16_t attr = (g_terminal.background << 4) | g_terminal.foreground;
    vram[index] = (attr << 8) | c;
}

void terminal_initialize(VGA_Color fg, VGA_Color bg) {
    g_terminal.cursor_x = 0;
    g_terminal.cursor_y = 0;
    g_terminal.foreground = fg;
    g_terminal.background = bg;
    terminal_clear();
}

void terminal_clear() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            write_to_vram(x, y, ' ');
        }
    }
    g_terminal.cursor_x = 0;
    g_terminal.cursor_y = 0;
}

void terminal_newline() {
    g_terminal.cursor_x = 0;
    g_terminal.cursor_y++;
    
    if (g_terminal.cursor_y >= SCREEN_HEIGHT) {
        g_terminal.cursor_y = SCREEN_HEIGHT - 1;
        /* TODO: Implement scrolling */
    }
}

void terminal_write_char(char c) {
    if (c == '\n') {
        terminal_newline();
        return;
    }
    
    if (g_terminal.cursor_x >= SCREEN_WIDTH) {
        terminal_newline();
    }
    
    write_to_vram(g_terminal.cursor_x, g_terminal.cursor_y, c);
    g_terminal.cursor_x++;
}

void terminal_write_string(const char *str) {
    while (*str) {
        terminal_write_char(*str);
        str++;
    }
}

void kernel_main() {
    terminal_initialize(WHITE, BLACK);
    terminal_write_string("GlitchOS Kernel Starting...\n");
    terminal_write_string("Welcome to GlitchOS!\n");
    
    /* Initialize desktop and launch app manager */
    desktop_initialize();
    app_manager_initialize();
    
    /* Launch a few default apps to show off the desktop */
    app_launch(APP_TERMINAL);
    app_launch(APP_SETTINGS);
    app_launch(APP_FILES);
    
    /* Draw the desktop with all windows */
    app_manager_draw();
    
    /* Main loop */
    while (1) {
        app_manager_update();
        asm("hlt");
    }
}
