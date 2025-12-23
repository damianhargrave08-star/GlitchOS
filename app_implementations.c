#include "apps.h"

typedef struct {
    char buffer[256];
    uint16_t buffer_pos;
    uint16_t cursor_x;
    uint16_t cursor_y;
} TerminalState;

static TerminalState g_terminal_state;

void terminal_app_init() {
    g_terminal_state.buffer_pos = 0;
    g_terminal_state.cursor_x = 5;
    g_terminal_state.cursor_y = 3;
}

void terminal_app_draw() {
    uint16_t *vram = (uint16_t *)VIDEO_MEMORY;
    uint16_t attr = (CYAN << 4) | WHITE;
    uint16_t title_attr = (CYAN << 4) | YELLOW;
    
    /* Draw window border */
    uint16_t win_x = 2, win_y = 2, win_w = 40, win_h = 15;
    
    for (uint16_t y = win_y; y < win_y + win_h; y++) {
        for (uint16_t x = win_x; x < win_x + win_w; x++) {
            uint16_t index = y * SCREEN_WIDTH + x;
            char c = ' ';
            uint16_t use_attr = attr;
            
            if (y == win_y || y == win_y + win_h - 1) c = '-';
            else if (x == win_x || x == win_x + win_w - 1) c = '|';
            
            if ((x == win_x || x == win_x + win_w - 1) && 
                (y == win_y || y == win_y + win_h - 1)) c = '+';
            
            if (y == win_y) {
                c = '-';
                use_attr = title_attr;
            }
            
            vram[index] = (use_attr << 8) | c;
        }
    }
    
    /* Draw title */
    const char *title = " Terminal ";
    uint16_t title_x = win_x + 2;
    for (int i = 0; title[i]; i++) {
        uint16_t index = win_y * SCREEN_WIDTH + title_x + i;
        vram[index] = (title_attr << 8) | title[i];
    }
    
    /* Draw terminal content */
    const char *msg = "user@glitchos:~$ ";
    uint16_t content_x = win_x + 2;
    uint16_t content_y = win_y + 2;
    
    for (int i = 0; msg[i]; i++) {
        uint16_t index = content_y * SCREEN_WIDTH + content_x + i;
        vram[index] = (attr << 8) | msg[i];
    }
    
    /* Draw buffer content */
    uint16_t buf_x = content_x + 17;
    for (int i = 0; i < g_terminal_state.buffer_pos && buf_x < win_x + win_w - 1; i++) {
        uint16_t index = content_y * SCREEN_WIDTH + buf_x + i;
        vram[index] = (attr << 8) | g_terminal_state.buffer[i];
    }
    
    /* Draw cursor */
    uint16_t cursor_x = buf_x + g_terminal_state.buffer_pos;
    if (cursor_x < win_x + win_w - 1) {
        uint16_t index = content_y * SCREEN_WIDTH + cursor_x;
        vram[index] = (attr << 8) | '_';
    }
}

void terminal_app_update() {
    /* Terminal input handling would go here */
}

void terminal_app_close() {
    g_terminal_state.buffer_pos = 0;
}

/* Settings App */
void settings_app_init() {
}

void settings_app_draw() {
    uint16_t *vram = (uint16_t *)VIDEO_MEMORY;
    uint16_t attr = (GREEN << 4) | WHITE;
    uint16_t title_attr = (GREEN << 4) | YELLOW;
    
    uint16_t win_x = 5, win_y = 4, win_w = 35, win_h = 13;
    
    for (uint16_t y = win_y; y < win_y + win_h; y++) {
        for (uint16_t x = win_x; x < win_x + win_w; x++) {
            uint16_t index = y * SCREEN_WIDTH + x;
            char c = ' ';
            uint16_t use_attr = attr;
            
            if (y == win_y || y == win_y + win_h - 1) c = '-';
            else if (x == win_x || x == win_x + win_w - 1) c = '|';
            if ((x == win_x || x == win_x + win_w - 1) && (y == win_y || y == win_y + win_h - 1)) c = '+';
            if (y == win_y) use_attr = title_attr;
            
            vram[index] = (use_attr << 8) | c;
        }
    }
    
    const char *title = " Settings ";
    uint16_t title_x = win_x + 2;
    for (int i = 0; title[i]; i++) {
        uint16_t index = win_y * SCREEN_WIDTH + title_x + i;
        vram[index] = (title_attr << 8) | title[i];
    }
    
    const char *options[] = {
        "Display",
        "Sound",
        "Network",
        "System",
        NULL
    };
    
    uint16_t opt_y = win_y + 2;
    for (int i = 0; options[i]; i++) {
        uint16_t opt_x = win_x + 2;
        for (int j = 0; options[i][j]; j++) {
            uint16_t index = opt_y * SCREEN_WIDTH + opt_x + j;
            vram[index] = (attr << 8) | options[i][j];
        }
        opt_y++;
    }
}

void settings_app_update() {
}

void settings_app_close() {
}

/* Files App */
void files_app_init() {
}

void files_app_draw() {
    uint16_t *vram = (uint16_t *)VIDEO_MEMORY;
    uint16_t attr = (BROWN << 4) | WHITE;
    uint16_t title_attr = (BROWN << 4) | YELLOW;
    
    uint16_t win_x = 8, win_y = 3, win_w = 30, win_h = 14;
    
    for (uint16_t y = win_y; y < win_y + win_h; y++) {
        for (uint16_t x = win_x; x < win_x + win_w; x++) {
            uint16_t index = y * SCREEN_WIDTH + x;
            char c = ' ';
            uint16_t use_attr = attr;
            
            if (y == win_y || y == win_y + win_h - 1) c = '-';
            else if (x == win_x || x == win_x + win_w - 1) c = '|';
            if ((x == win_x || x == win_x + win_w - 1) && (y == win_y || y == win_y + win_h - 1)) c = '+';
            if (y == win_y) use_attr = title_attr;
            
            vram[index] = (use_attr << 8) | c;
        }
    }
    
    const char *title = " File Manager ";
    uint16_t title_x = win_x + 2;
    for (int i = 0; title[i]; i++) {
        uint16_t index = win_y * SCREEN_WIDTH + title_x + i;
        vram[index] = (title_attr << 8) | title[i];
    }
    
    const char *files[] = {
        "[boot.bin]",
        "[kernel.c]",
        "[kernel.h]",
        "[desktop.c]",
        "[apps.c]",
        NULL
    };
    
    uint16_t file_y = win_y + 2;
    for (int i = 0; files[i]; i++) {
        uint16_t file_x = win_x + 2;
        for (int j = 0; files[i][j]; j++) {
            uint16_t index = file_y * SCREEN_WIDTH + file_x + j;
            vram[index] = (attr << 8) | files[i][j];
        }
        file_y++;
    }
}

void files_app_update() {
}

void files_app_close() {
}

/* Minecraft Launcher */
void minecraft_app_init() {
}

void minecraft_app_draw() {
    uint16_t *vram = (uint16_t *)VIDEO_MEMORY;
    uint16_t attr = (RED << 4) | WHITE;
    uint16_t title_attr = (RED << 4) | YELLOW;
    
    uint16_t win_x = 10, win_y = 5, win_w = 35, win_h = 12;
    
    for (uint16_t y = win_y; y < win_y + win_h; y++) {
        for (uint16_t x = win_x; x < win_x + win_w; x++) {
            uint16_t index = y * SCREEN_WIDTH + x;
            char c = ' ';
            uint16_t use_attr = attr;
            
            if (y == win_y || y == win_y + win_h - 1) c = '-';
            else if (x == win_x || x == win_x + win_w - 1) c = '|';
            if ((x == win_x || x == win_x + win_w - 1) && (y == win_y || y == win_y + win_h - 1)) c = '+';
            if (y == win_y) use_attr = title_attr;
            
            vram[index] = (use_attr << 8) | c;
        }
    }
    
    const char *title = " TLauncher - Minecraft ";
    uint16_t title_x = win_x + 2;
    for (int i = 0; title[i]; i++) {
        uint16_t index = win_y * SCREEN_WIDTH + title_x + i;
        vram[index] = (title_attr << 8) | title[i];
    }
    
    const char *launcher_text = "Loading versions...";
    uint16_t text_x = win_x + 5;
    uint16_t text_y = win_y + 4;
    for (int i = 0; launcher_text[i]; i++) {
        uint16_t index = text_y * SCREEN_WIDTH + text_x + i;
        vram[index] = (attr << 8) | launcher_text[i];
    }
    
    const char *button = "[ PLAY ]";
    uint16_t button_x = win_x + 13;
    uint16_t button_y = win_y + 8;
    for (int i = 0; button[i]; i++) {
        uint16_t index = button_y * SCREEN_WIDTH + button_x + i;
        vram[index] = (attr << 8) | button[i];
    }
}

void minecraft_app_update() {
}

void minecraft_app_close() {
}

/* XTerm App */
void xterm_app_init() {
}

void xterm_app_draw() {
    uint16_t *vram = (uint16_t *)VIDEO_MEMORY;
    uint16_t attr = (BLACK << 4) | LIGHT_GRAY;
    uint16_t title_attr = (BLACK << 4) | WHITE;
    
    uint16_t win_x = 3, win_y = 1, win_w = 45, win_h = 16;
    
    for (uint16_t y = win_y; y < win_y + win_h; y++) {
        for (uint16_t x = win_x; x < win_x + win_w; x++) {
            uint16_t index = y * SCREEN_WIDTH + x;
            char c = ' ';
            uint16_t use_attr = attr;
            
            if (y == win_y || y == win_y + win_h - 1) c = '-';
            else if (x == win_x || x == win_x + win_w - 1) c = '|';
            if ((x == win_x || x == win_x + win_w - 1) && (y == win_y || y == win_y + win_h - 1)) c = '+';
            if (y == win_y) use_attr = title_attr;
            
            vram[index] = (use_attr << 8) | c;
        }
    }
    
    const char *title = " XTerm ";
    uint16_t title_x = win_x + 2;
    for (int i = 0; title[i]; i++) {
        uint16_t index = win_y * SCREEN_WIDTH + title_x + i;
        vram[index] = (title_attr << 8) | title[i];
    }
    
    const char *xterm_msg = "XTerm Terminal Emulator";
    uint16_t msg_x = win_x + 10;
    uint16_t msg_y = win_y + 3;
    for (int i = 0; xterm_msg[i]; i++) {
        uint16_t index = msg_y * SCREEN_WIDTH + msg_x + i;
        vram[index] = (attr << 8) | xterm_msg[i];
    }
}

void xterm_app_update() {
}

void xterm_app_close() {
}

/* Root Terminal App */
void root_terminal_app_init() {
}

void root_terminal_app_draw() {
    uint16_t *vram = (uint16_t *)VIDEO_MEMORY;
    uint16_t attr = (LIGHT_RED << 4) | WHITE;
    uint16_t title_attr = (LIGHT_RED << 4) | YELLOW;
    
    uint16_t win_x = 6, win_y = 2, win_w = 38, win_h = 15;
    
    for (uint16_t y = win_y; y < win_y + win_h; y++) {
        for (uint16_t x = win_x; x < win_x + win_w; x++) {
            uint16_t index = y * SCREEN_WIDTH + x;
            char c = ' ';
            uint16_t use_attr = attr;
            
            if (y == win_y || y == win_y + win_h - 1) c = '-';
            else if (x == win_x || x == win_x + win_w - 1) c = '|';
            if ((x == win_x || x == win_x + win_w - 1) && (y == win_y || y == win_y + win_h - 1)) c = '+';
            if (y == win_y) use_attr = title_attr;
            
            vram[index] = (use_attr << 8) | c;
        }
    }
    
    const char *title = " Root Terminal (ADMIN) ";
    uint16_t title_x = win_x + 2;
    for (int i = 0; title[i]; i++) {
        uint16_t index = win_y * SCREEN_WIDTH + title_x + i;
        vram[index] = (title_attr << 8) | title[i];
    }
    
    const char *root_msg = "root@glitchos:~# ";
    uint16_t msg_x = win_x + 2;
    uint16_t msg_y = win_y + 3;
    for (int i = 0; root_msg[i]; i++) {
        uint16_t index = msg_y * SCREEN_WIDTH + msg_x + i;
        vram[index] = (attr << 8) | root_msg[i];
    }
}

void root_terminal_app_update() {
}

void root_terminal_app_close() {
}

/* Web Search App */
void web_search_app_init() {
}

void web_search_app_draw() {
    uint16_t *vram = (uint16_t *)VIDEO_MEMORY;
    uint16_t attr = (BLUE << 4) | WHITE;
    uint16_t title_attr = (BLUE << 4) | YELLOW;
    
    uint16_t win_x = 15, win_y = 6, win_w = 40, win_h = 11;
    
    for (uint16_t y = win_y; y < win_y + win_h; y++) {
        for (uint16_t x = win_x; x < win_x + win_w; x++) {
            uint16_t index = y * SCREEN_WIDTH + x;
            char c = ' ';
            uint16_t use_attr = attr;
            
            if (y == win_y || y == win_y + win_h - 1) c = '-';
            else if (x == win_x || x == win_x + win_w - 1) c = '|';
            if ((x == win_x || x == win_x + win_w - 1) && (y == win_y || y == win_y + win_h - 1)) c = '+';
            if (y == win_y) use_attr = title_attr;
            
            vram[index] = (use_attr << 8) | c;
        }
    }
    
    const char *title = " Custom Search Engine ";
    uint16_t title_x = win_x + 2;
    for (int i = 0; title[i]; i++) {
        uint16_t index = win_y * SCREEN_WIDTH + title_x + i;
        vram[index] = (title_attr << 8) | title[i];
    }
    
    const char *search_msg = "Custom CSE Search:";
    uint16_t msg_x = win_x + 5;
    uint16_t msg_y = win_y + 2;
    for (int i = 0; search_msg[i]; i++) {
        uint16_t index = msg_y * SCREEN_WIDTH + msg_x + i;
        vram[index] = (attr << 8) | search_msg[i];
    }
    
    const char *search_box = "[                    ]";
    uint16_t box_y = msg_y + 2;
    for (int i = 0; search_box[i]; i++) {
        uint16_t index = box_y * SCREEN_WIDTH + msg_x + i;
        vram[index] = (attr << 8) | search_box[i];
    }
}

void web_search_app_update() {
}

void web_search_app_close() {
}
