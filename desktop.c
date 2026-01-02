#include "desktop.h"

/* Global desktop state */
static Desktop g_desktop;

void desktop_initialize() {
    g_desktop.window_count = 0;
    g_desktop.bg_color = MAGENTA;
    g_desktop.mouse.x = SCREEN_WIDTH / 2;
    g_desktop.mouse.y = SCREEN_HEIGHT / 2;
    g_desktop.mouse.cursor_char = 'X';
    g_desktop.mouse.color = WHITE;
    
    /* Initialize window array */
    for (int i = 0; i < MAX_WINDOWS; i++) {
        g_desktop.windows[i].state = WINDOW_STATE_CLOSED;
    }
    
    desktop_draw();
}

void desktop_draw_background() {
    uint16_t *vram = (uint16_t *)VIDEO_MEMORY;
    uint16_t attr = (g_desktop.bg_color << 4) | LIGHT_MAGENTA;
    
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        vram[i] = (attr << 8) | '.';
    }
}

void desktop_draw_taskbar() {
    uint16_t *vram = (uint16_t *)VIDEO_MEMORY;
    uint16_t taskbar_y = SCREEN_HEIGHT - 1;
    uint16_t attr = (BLACK << 4) | LIGHT_GRAY;
    
    /* Draw taskbar background */
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        uint16_t index = taskbar_y * SCREEN_WIDTH + x;
        vram[index] = (attr << 8) | ' ';
    }
    
    /* Draw taskbar text */
    const char *taskbar_text = "[GlitchOS Desktop] Windows: ";
    uint16_t x = 1;
    for (int i = 0; taskbar_text[i]; i++) {
        uint16_t index = taskbar_y * SCREEN_WIDTH + x;
        vram[index] = (attr << 8) | taskbar_text[i];
        x++;
    }
    
    /* Display window count */
    char count_char = '0' + g_desktop.window_count;
    uint16_t index = taskbar_y * SCREEN_WIDTH + x;
    vram[index] = (attr << 8) | count_char;
}

void window_draw(Window *win) {
    if (win->state == WINDOW_STATE_CLOSED) return;
    
    uint16_t *vram = (uint16_t *)VIDEO_MEMORY;
    uint16_t attr = (win->bg_color << 4) | WHITE;
    uint16_t title_attr = (win->bg_color << 4) | YELLOW;
    
    /* Draw window border and fill */
    for (uint16_t y = win->y; y < win->y + win->height; y++) {
        for (uint16_t x = win->x; x < win->x + win->width; x++) {
            if (y >= SCREEN_HEIGHT - 1) break; /* Don't draw over taskbar */
            
            uint16_t index = y * SCREEN_WIDTH + x;
            char c = ' ';
            uint16_t use_attr = attr;
            
            /* Draw borders */
            if (y == win->y || y == win->y + win->height - 1) {
                c = '-';
            } else if (x == win->x || x == win->x + win->width - 1) {
                c = '|';
            }
            
            /* Draw corners */
            if ((x == win->x || x == win->x + win->width - 1) &&
                (y == win->y || y == win->y + win->height - 1)) {
                c = '+';
            }
            
            /* Draw title bar */
            if (y == win->y) {
                c = '-';
                use_attr = title_attr;
            }
            
            vram[index] = (use_attr << 8) | c;
        }
    }
    
    /* Draw title text */
    if (win->title) {
        uint16_t title_x = win->x + 2;
        uint16_t title_y = win->y;
        uint16_t idx = 0;
        
        for (uint16_t x = title_x; x < win->x + win->width - 2 && win->title[idx]; x++) {
            uint16_t index = title_y * SCREEN_WIDTH + x;
            uint16_t use_attr = title_attr;
            vram[index] = (use_attr << 8) | win->title[idx];
            idx++;
        }
    }
}

void mouse_draw(uint16_t x, uint16_t y) {
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT - 1) return;
    
    uint16_t *vram = (uint16_t *)VIDEO_MEMORY;
    uint16_t index = y * SCREEN_WIDTH + x;
    uint16_t attr = (BLACK << 4) | g_desktop.mouse.color;
    vram[index] = (attr << 8) | g_desktop.mouse.cursor_char;
}

void desktop_open_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *title) {
    if (g_desktop.window_count >= MAX_WINDOWS) return;
    
    uint16_t idx = g_desktop.window_count;
    g_desktop.windows[idx].x = x;
    g_desktop.windows[idx].y = y;
    g_desktop.windows[idx].width = w;
    g_desktop.windows[idx].height = h;
    g_desktop.windows[idx].title = title;
    g_desktop.windows[idx].state = WINDOW_STATE_OPEN;
    g_desktop.windows[idx].bg_color = LIGHT_MAGENTA;
    
    g_desktop.window_count++;
}

void desktop_close_window(uint16_t index) {
    if (index >= g_desktop.window_count) return;
    
    g_desktop.windows[index].state = WINDOW_STATE_CLOSED;
    
    /* Shift remaining windows */
    for (uint16_t i = index; i < g_desktop.window_count - 1; i++) {
        g_desktop.windows[i] = g_desktop.windows[i + 1];
    }
    
    g_desktop.window_count--;
}

void desktop_draw() {
    desktop_draw_background();
    
    /* Draw all open windows */
    for (int i = 0; i < g_desktop.window_count; i++) {
        if (g_desktop.windows[i].state != WINDOW_STATE_CLOSED) {
            window_draw(&g_desktop.windows[i]);
        }
    }
    
    /* Draw taskbar */
    desktop_draw_taskbar();
    
    /* Draw mouse cursor */
    mouse_draw(g_desktop.mouse.x, g_desktop.mouse.y);
}

void desktop_update() {
    desktop_draw();
}
/* --- DESKTOP MAGIC BOOT NUMBER 4d 5a --- */
/*jump to Global desktop state*/
/* --- IGNORE --- */