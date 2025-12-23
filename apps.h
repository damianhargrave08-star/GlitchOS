#ifndef APPS_H
#define APPS_H

#include "kernel.h"

typedef enum {
    APP_TERMINAL,
    APP_SETTINGS,
    APP_FILES,
    APP_MINECRAFT,
    APP_XTERM,
    APP_ROOT_TERMINAL,
    APP_WEB_SEARCH,
    APP_COUNT
} AppType;

typedef struct {
    AppType type;
    const char *name;
    const char *icon;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    int is_running;
} App;

typedef struct {
    App apps[APP_COUNT];
    App *active_app;
} AppManager;

/* App lifecycle functions */
void app_manager_initialize();
void app_launch(AppType type);
void app_close(AppType type);
void app_draw_launcher();
void app_manager_draw();
void app_manager_update();

/* Individual app function declarations */
void terminal_app_init();
void terminal_app_draw();
void terminal_app_update();
void terminal_app_close();

void settings_app_init();
void settings_app_draw();
void settings_app_update();
void settings_app_close();

void files_app_init();
void files_app_draw();
void files_app_update();
void files_app_close();

void minecraft_app_init();
void minecraft_app_draw();
void minecraft_app_update();
void minecraft_app_close();

void xterm_app_init();
void xterm_app_draw();
void xterm_app_update();
void xterm_app_close();

void root_terminal_app_init();
void root_terminal_app_draw();
void root_terminal_app_update();
void root_terminal_app_close();

void web_search_app_init();
void web_search_app_draw();
void web_search_app_update();
void web_search_app_close();

#endif
