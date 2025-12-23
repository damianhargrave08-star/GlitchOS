#ifndef DESKTOP_H
#define DESKTOP_H

#include "kernel.h"

#define MAX_WINDOWS 10

typedef enum {
    WINDOW_STATE_CLOSED,
    WINDOW_STATE_OPEN,
    WINDOW_STATE_MINIMIZED,
    WINDOW_STATE_FOCUSED
} WindowState;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    const char *title;
    WindowState state;
    VGA_Color bg_color;
} Window;

typedef struct {
    uint16_t x;
    uint16_t y;
    char cursor_char;
    VGA_Color color;
} Mouse;

/* Desktop state */
typedef struct {
    Window windows[MAX_WINDOWS];
    uint16_t window_count;
    Mouse mouse;
    VGA_Color bg_color;
} Desktop;

/* Function declarations */
void desktop_initialize();
void desktop_draw();
void desktop_draw_taskbar();
void desktop_draw_background();
void desktop_open_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *title);
void desktop_close_window(uint16_t index);
void window_draw(Window *win);
void mouse_draw(uint16_t x, uint16_t y);
void desktop_update();

#endif
