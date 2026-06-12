/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** menu
*/

// clang-format off
#ifndef MENU_H_
    #define MENU_H_

    #include "graphics/engine.h"
    #include "graphics/sprite_anim.h"
    #include "graphics/ui.h"

    #define LOGO_FRAMES 5
    #define LOGO_FPS 12.0F
    #define LOGO_WIDTH 381
    #define LOGO_HEIGHT 112

    #define MAX_PAUSE_BUTTONS 6
    #define MAX_MAIN_MENU_BUTTONS 4

    #define BUTTONS_PATH SPRITES_PATH "main_menu/button_"
    #define BG_PATH SPRITES_PATH "main_menu/bg_menu_pause.png"
    #define BG_MAIN_PATH SPRITES_PATH "main_menu/background.jpg"
    #define LOGO_PATH SPRITES_PATH "main_menu/logo.png"
    #define SAVE_BTN_PATH BUTTONS_PATH "save.png"
    #define RESUME_BTN_PATH BUTTONS_PATH "resume.png"
    #define FR_BTN_PATH BUTTONS_PATH "fr.png"
    #define EN_BTN_PATH BUTTONS_PATH "en.png"
    #define EXIT_BTN_PATH BUTTONS_PATH "exit.png"
    #define KEYS_BTN_PATH BUTTONS_PATH "keys.png"
    #define SOUNDS_BTN_PATH BUTTONS_PATH "sounds.png"
    #define PARAMS_BTN_PATH BUTTONS_PATH "params.png"
    #define MUSICS_BTN_PATH BUTTONS_PATH "music.png"
    #define MAP_KO_BTN_PATH BUTTONS_PATH "map_ko.png"
    #define TIME_KO_BTN_PATH BUTTONS_PATH "time_ko.png"
    #define MAP_OK_BTN_PATH BUTTONS_PATH "map_ok.png"
    #define TIME_OK_BTN_PATH BUTTONS_PATH "time_ok.png"
    #define RESIZE_BTN_PATH BUTTONS_PATH "resize.png"
    #define EASY_BTN_PATH BUTTONS_PATH "easy.png"
    #define MEDIUM_BTN_PATH BUTTONS_PATH "medium.png"
    #define HARD_BTN_PATH BUTTONS_PATH "hard.png"
    #define PLAY_BTN_PATH BUTTONS_PATH "play.png"

    #define END_SCREEN_PATH SPRITES_PATH "end_screen/"
    #define MISSION_SUCESS_PATH END_SCREEN_PATH "mission_success_msg.png"
    #define MISSION_FAILED_PATH END_SCREEN_PATH "mission_failed_msg.png"
// clang-format on

typedef enum menu_page {
    MENU_PAGE_MAIN,
    MENU_PAGE_SETTINGS,
} menu_page_t;

typedef struct menu_data {
    sfSprite *bg;
    sprite_anim_t *logo;
    menu_page_t page;
    ui_button_t *play_btn;
    ui_button_t *settings_btn;
    ui_button_t *difficulty_btn;
    ui_button_t *exit_btn;
    ui_button_t *controls_btn;
    ui_button_t *win_mode_btn;
    ui_button_t *music_btn;
    ui_button_t *sounds_btn;
} menu_data_t;

typedef enum pause_page {
    PAUSE_PAGE_MAIN,
    PAUSE_PAGE_CONTROLS,
    PAUSE_PAGE_SOUNDS,
    PAUSE_PAGE_HUD,
} pause_page_t;

typedef struct buttons_group_config {
    ui_button_t ***buttons;
    const char **textures;
    void (**on_clicks)(void *);
    sfVector2f *positions;
    size_t count;
    const sfVector2f *size;
} buttons_group_config_t;

typedef struct pause_menu {
    sfSprite *bg;
    sfRectangleShape *separator;
    pause_page_t page;
    ui_button_t *resume;
    ui_button_t *save;
    ui_button_t *exit;
    ui_button_t *controls;
    ui_button_t *sounds;
    ui_button_t *hud;
    ui_button_t *controls_mv;
    ui_button_t *win_mode;
    ui_button_t *music_btn;
    ui_button_t *sounds_btn;
    ui_button_t *hud_minimap;
    ui_button_t *hud_timer;
} pause_menu_t;

// Main_menu

typedef struct game_s game_data_t;

scene_t *menu_create(void);
void menu_on_enter(engine_t *engine);
void menu_on_resize(engine_t *engine);
void menu_update(engine_t *engine);
void menu_draw(engine_t *engine);
void handle_menu_events(engine_t *engine, sfEvent *event);
void menu_on_exit(engine_t *engine);
void init_sprites(engine_t *engine, menu_data_t *data);
void init_buttons(engine_t *engine, menu_data_t *data);
void on_play_click(void *data);
void on_exit_click(void *data);
void on_settings_click(void *data);
void on_menu_keys_click(void *data);
void on_menu_fullscreen_click(void *data);
void on_menu_music_click(void *data);
void on_menu_sound_click(void *data);
void on_menu_difficulty_click(void *data);
// Pause_menu
void pause_on_resume(void *data);
void pause_on_save(void *data);
void pause_on_exit(void *data);
void pause_init(engine_t *engine, game_data_t *data);
void pause_destroy(game_data_t *data);
void draw_btn(engine_t *engine, ui_button_t *button);
void pause_draw(engine_t *engine, game_data_t *data);
void pause_events(engine_t *engine, game_data_t *data, sfEvent *event);
void pause_on_hud(void *data);
void pause_on_sounds(void *data);
void pause_on_controls(void *data);
void pause_on_sound_music(void *data);
void pause_on_sound_sfx(void *data);
void pause_on_hud_minimap(void *data);
void pause_on_hud_timer(void *data);
void pause_on_controls_keys(void *data);
void pause_on_controls_resize(void *data);
void pause_update(engine_t *engine, game_data_t *data);

ui_button_t *create_btn(engine_t *engine, const char *tex, sfVector2f *pos);
void set_pause_page(engine_t *engine, pause_page_t page);
void set_btn(ui_button_t *button, void (*f)(void *), engine_t *engine,
    char *text);
void create_buttons_group(engine_t *engine, buttons_group_config_t *config);
void settings_apply_keyboard_texture(engine_t *engine, ui_button_t *button);
void settings_apply_toggle_color(ui_button_t *button, bool enabled);
void settings_set_music_enabled(engine_t *engine, bool enabled);
void settings_set_sounds_enabled(engine_t *engine, bool enabled);
void settings_toggle_music(engine_t *engine);
void settings_toggle_sounds(engine_t *engine);
void settings_toggle_difficulty(engine_t *engine);
const char *settings_get_difficulty_texture(game_difficulty_t difficulty);
void settings_apply_difficulty_texture(engine_t *engine, ui_button_t *button);
size_t menu_get_page_buttons(menu_data_t *menu, ui_button_t **buttons);
size_t pause_get_page_buttons(pause_menu_t *pause, ui_button_t **buttons);
void draw_buttons(engine_t *engine, ui_button_t **buttons, size_t count);
void update_buttons(engine_t *engine, ui_button_t **buttons, size_t count);
void handle_buttons_events(engine_t *engine, const sfEvent *event,
    ui_button_t **buttons, size_t count);
void destroy_buttons(ui_button_t **buttons, size_t count);

#endif /* !MENU_H_ */
