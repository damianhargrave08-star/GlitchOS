#include "apps.h"

static AppManager g_app_manager;

void app_manager_initialize() {
    /* Initialize all apps */
    g_app_manager.apps[APP_TERMINAL].type = APP_TERMINAL;
    g_app_manager.apps[APP_TERMINAL].name = "Terminal";
    g_app_manager.apps[APP_TERMINAL].icon = "T";
    g_app_manager.apps[APP_TERMINAL].is_running = 0;
    
    g_app_manager.apps[APP_SETTINGS].type = APP_SETTINGS;
    g_app_manager.apps[APP_SETTINGS].name = "Settings";
    g_app_manager.apps[APP_SETTINGS].icon = "S";
    g_app_manager.apps[APP_SETTINGS].is_running = 0;
    
    g_app_manager.apps[APP_FILES].type = APP_FILES;
    g_app_manager.apps[APP_FILES].name = "Files";
    g_app_manager.apps[APP_FILES].icon = "F";
    g_app_manager.apps[APP_FILES].is_running = 0;
    
    g_app_manager.apps[APP_MINECRAFT].type = APP_MINECRAFT;
    g_app_manager.apps[APP_MINECRAFT].name = "TLauncher";
    g_app_manager.apps[APP_MINECRAFT].icon = "M";
    g_app_manager.apps[APP_MINECRAFT].is_running = 0;
    
    g_app_manager.apps[APP_XTERM].type = APP_XTERM;
    g_app_manager.apps[APP_XTERM].name = "XTerm";
    g_app_manager.apps[APP_XTERM].icon = "X";
    g_app_manager.apps[APP_XTERM].is_running = 0;
    
    g_app_manager.apps[APP_ROOT_TERMINAL].type = APP_ROOT_TERMINAL;
    g_app_manager.apps[APP_ROOT_TERMINAL].name = "Root Term";
    g_app_manager.apps[APP_ROOT_TERMINAL].icon = "R";
    g_app_manager.apps[APP_ROOT_TERMINAL].is_running = 0;
    
    g_app_manager.apps[APP_WEB_SEARCH].type = APP_WEB_SEARCH;
    g_app_manager.apps[APP_WEB_SEARCH].name = "Web Search";
    g_app_manager.apps[APP_WEB_SEARCH].icon = "W";
    g_app_manager.apps[APP_WEB_SEARCH].is_running = 0;
    
    g_app_manager.active_app = NULL;
}

void app_launch(AppType type) {
    if (type >= APP_COUNT) return;
    
    App *app = &g_app_manager.apps[type];
    app->is_running = 1;
    g_app_manager.active_app = app;
    
    switch (type) {
        case APP_TERMINAL:
            terminal_app_init();
            break;
        case APP_SETTINGS:
            settings_app_init();
            break;
        case APP_FILES:
            files_app_init();
            break;
        case APP_MINECRAFT:
            minecraft_app_init();
            break;
        case APP_XTERM:
            xterm_app_init();
            break;
        case APP_ROOT_TERMINAL:
            root_terminal_app_init();
            break;
        case APP_WEB_SEARCH:
            web_search_app_init();
            break;
        default:
            break;
    }
}

void app_close(AppType type) {
    if (type >= APP_COUNT) return;
    
    App *app = &g_app_manager.apps[type];
    app->is_running = 0;
    
    switch (type) {
        case APP_TERMINAL:
            terminal_app_close();
            break;
        case APP_SETTINGS:
            settings_app_close();
            break;
        case APP_FILES:
            files_app_close();
            break;
        case APP_MINECRAFT:
            minecraft_app_close();
            break;
        case APP_XTERM:
            xterm_app_close();
            break;
        case APP_ROOT_TERMINAL:
            root_terminal_app_close();
            break;
        case APP_WEB_SEARCH:
            web_search_app_close();
            break;
        default:
            break;
    }
    
    g_app_manager.active_app = NULL;
}

void app_draw_launcher() {
    uint16_t *vram = (uint16_t *)VIDEO_MEMORY;
    uint16_t attr = (MAGENTA << 4) | LIGHT_MAGENTA;
    
    /* Draw app launcher grid */
    uint16_t start_x = 5;
    uint16_t start_y = 2;
    uint16_t icon_size = 8;
    uint16_t col = 0;
    
    for (int i = 0; i < APP_COUNT; i++) {
        App *app = &g_app_manager.apps[i];
        
        uint16_t x = start_x + (col * (icon_size + 2));
        uint16_t y = start_y;
        
        /* Draw icon box */
        for (int dy = 0; dy < icon_size; dy++) {
            for (int dx = 0; dx < icon_size; dx++) {
                uint16_t index = (y + dy) * SCREEN_WIDTH + (x + dx);
                if (index < SCREEN_WIDTH * (SCREEN_HEIGHT - 1)) {
                    vram[index] = (attr << 8) | app->icon[0];
                }
            }
        }
        
        /* Draw app name below icon */
        uint16_t name_attr = (MAGENTA << 4) | WHITE;
        uint16_t name_y = y + icon_size + 1;
        uint16_t name_x = x;
        
        for (int j = 0; app->name[j] && name_x < SCREEN_WIDTH; j++) {
            uint16_t index = name_y * SCREEN_WIDTH + name_x;
            if (index < SCREEN_WIDTH * (SCREEN_HEIGHT - 1)) {
                vram[index] = (name_attr << 8) | app->name[j];
            }
            name_x++;
        }
        
        col++;
        if (col >= 8) {
            col = 0;
            start_y += icon_size + 3;
        }
    }
}

void app_manager_draw() {
    app_draw_launcher();
    
    if (g_app_manager.active_app) {
        switch (g_app_manager.active_app->type) {
            case APP_TERMINAL:
                terminal_app_draw();
                break;
            case APP_SETTINGS:
                settings_app_draw();
                break;
            case APP_FILES:
                files_app_draw();
                break;
            case APP_MINECRAFT:
                minecraft_app_draw();
                break;
            case APP_XTERM:
                xterm_app_draw();
                break;
            case APP_ROOT_TERMINAL:
                root_terminal_app_draw();
                break;
            case APP_WEB_SEARCH:
                web_search_app_draw();
                break;
            default:
                break;
        }
    }
}

void app_manager_update() {
    if (g_app_manager.active_app) {
        switch (g_app_manager.active_app->type) {
            case APP_TERMINAL:
                terminal_app_update();
                break;
            case APP_SETTINGS:
                settings_app_update();
                break;
            case APP_FILES:
                files_app_update();
                break;
            case APP_MINECRAFT:
                minecraft_app_update();
                break;
            case APP_XTERM:
                xterm_app_update();
                break;
            case APP_ROOT_TERMINAL:
                root_terminal_app_update();
                break;
            case APP_WEB_SEARCH:
                web_search_app_update();
                break;
            default:
                break;
        }
    }
}
