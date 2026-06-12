/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** pause_create
*/

#include "game.h"
#include "graphics/engine.h"
#include "graphics/maths.h"
#include "graphics/resources.h"
#include "graphics/ui.h"
#include "menu.h"

static sfVector2f get_main_column_pos(engine_t *engine, bool is_right,
    float x_offset, float y_offset)
{
    float x = engine->window_size.x / 2.0F;
    float y = engine->window_size.y / 2.0F - y_offset;

    if (is_right)
        x += x_offset;
    else
        x -= x_offset;
    return (sfVector2f) {x, y};
}

static void init_buttons_column(engine_t *engine,
    buttons_group_config_t *config, const sfVector2f *start_pos,
    float y_offset)
{
    sfVector2f positions[config->count];
    sfVector2f pos = *start_pos;

    for (size_t i = 0; i < config->count; i++) {
        positions[i] = pos;
        pos.y += y_offset;
    }
    config->positions = positions;
    create_buttons_group(engine, config);
}

static void init_pause_bg(engine_t *engine, pause_menu_t *pause)
{
    sfTexture *texture = resources_load_texture(engine->resources, BG_PATH);
    sfVector2u text_size = sfTexture_getSize(texture);
    sfColor line_c = {0, 0, 0, 100};
    float sep_h = (float) engine->window_size.y * 0.70F;

    if (sep_h < 280.0F)
        sep_h = 280.0F;
    pause->bg = sfSprite_create();
    sfSprite_setTexture(pause->bg, texture, sfTrue);
    sfSprite_setScale(pause->bg,
        (sfVector2f) {(float) engine->window_size.x / (float) text_size.x,
            (float) engine->window_size.y / (float) text_size.y});
    pause->separator = sfRectangleShape_create();
    sfRectangleShape_setSize(pause->separator, (sfVector2f) {4.0F, sep_h});
    sfRectangleShape_setFillColor(pause->separator, line_c);
    sfRectangleShape_setOrigin(pause->separator,
        (sfVector2f) {2.0F, sep_h / 2});
    sfRectangleShape_setPosition(pause->separator,
        (sfVector2f) {engine->window_size.x / 2.0F,
            engine->window_size.y / 2.0F});
}

static void init_main_left(engine_t *engine, pause_menu_t *pause,
    float x_offset, float y_offset)
{
    sfVector2f left = get_main_column_pos(engine, false, x_offset, y_offset);
    ui_button_t **buttons[] = {&pause->resume, &pause->save, &pause->exit};
    const char *textures[] = {RESUME_BTN_PATH, SAVE_BTN_PATH, EXIT_BTN_PATH};
    void (*on_clicks[])(
        void *) = {&pause_on_resume, &pause_on_save, &pause_on_exit};
    buttons_group_config_t config = {buttons, textures, on_clicks, nullptr,
        sizeof(buttons) / sizeof(buttons[0]), nullptr};

    init_buttons_column(engine, &config, &left, y_offset);
}

static void init_main_right(engine_t *engine, pause_menu_t *pause,
    float x_offset, float y_offset)
{
    sfVector2f right = get_main_column_pos(engine, true, x_offset, y_offset);
    ui_button_t **buttons[] = {&pause->controls, &pause->sounds, &pause->hud};
    const char *textures[] = {KEYS_BTN_PATH, SOUNDS_BTN_PATH, PARAMS_BTN_PATH};
    void (*on_clicks[])(
        void *) = {&pause_on_controls, &pause_on_sounds, &pause_on_hud};
    buttons_group_config_t config = {buttons, textures, on_clicks, nullptr,
        sizeof(buttons) / sizeof(buttons[0]), nullptr};

    init_buttons_column(engine, &config, &right, y_offset);
}

static void init_controls_page(engine_t *engine, pause_menu_t *pause)
{
    sfVector2f positions[] = {
        {engine->window_size.x / 2.0F, engine->window_size.y / 2.0F}};
    ui_button_t **buttons[] = {&pause->controls_mv};
    const char *textures[] = {FR_BTN_PATH};
    void (*on_clicks[])(void *) = {&pause_on_controls_keys};
    buttons_group_config_t config = {buttons, textures, on_clicks, positions,
        sizeof(buttons) / sizeof(buttons[0]), nullptr};

    create_buttons_group(engine, &config);
    settings_apply_keyboard_texture(engine, pause->controls_mv);
}

static void init_sounds_page(engine_t *engine, pause_menu_t *pause)
{
    sfVector2f positions[] = {
        {engine->window_size.x / 4.0F, engine->window_size.y / 2.0F},
        {(engine->window_size.x / 4.0F) * 3.F, engine->window_size.y / 2.0F}};
    ui_button_t **buttons[] = {&pause->music_btn, &pause->sounds_btn};
    const char *textures[] = {MUSICS_BTN_PATH, SOUNDS_BTN_PATH};
    void (*on_clicks[])(void *) = {&pause_on_sound_music, &pause_on_sound_sfx};
    buttons_group_config_t config = {buttons, textures, on_clicks, positions,
        sizeof(buttons) / sizeof(buttons[0]), nullptr};

    create_buttons_group(engine, &config);
    settings_apply_toggle_color(pause->music_btn, engine->music_enabled);
    settings_apply_toggle_color(pause->sounds_btn, engine->sounds_enabled);
}

static void apply_hud_button_colors(engine_t *engine, game_data_t *data,
    pause_menu_t *pause)
{
    settings_apply_toggle_color(pause->win_mode, engine->is_fullscreen);
    if (data->hud && !data->hud->show_minimap)
        sfRectangleShape_setTexture(pause->hud_minimap->background,
            resources_load_texture(engine->resources, MAP_KO_BTN_PATH), true);
    if (data->hud && !data->hud->show_timer)
        sfRectangleShape_setTexture(pause->hud_timer->background,
            resources_load_texture(engine->resources, TIME_KO_BTN_PATH), true);
    settings_apply_toggle_color(pause->hud_minimap,
        !data->hud || data->hud->show_minimap);
    settings_apply_toggle_color(pause->hud_timer,
        !data->hud || data->hud->show_timer);
}

static void init_hud_page(engine_t *engine, game_data_t *data)
{
    pause_menu_t *pause = &data->pause;
    sfVector2f positions[] = {
        {engine->window_size.x / 4.0F, engine->window_size.y / 2.0F - 60.0F},
        {(engine->window_size.x / 4.0F) * 3.F,
            engine->window_size.y / 2.0F - 60.0F},
        {engine->window_size.x / 2.0F, engine->window_size.y / 2.0F + 60.0F}};
    ui_button_t **buttons[] = {&pause->hud_minimap, &pause->hud_timer,
        &pause->win_mode};
    const char *textures[] = {MAP_OK_BTN_PATH, TIME_OK_BTN_PATH,
        RESIZE_BTN_PATH};
    void (*on_clicks[])(void *) = {&pause_on_hud_minimap, &pause_on_hud_timer,
        &pause_on_controls_resize};
    buttons_group_config_t config = {buttons, textures, on_clicks, positions,
        sizeof(buttons) / sizeof(buttons[0]), nullptr};

    create_buttons_group(engine, &config);
    apply_hud_button_colors(engine, data, pause);
}

void pause_init(engine_t *engine, game_data_t *data)
{
    float y_offset =
        restrain_value((float) engine->window_size.y * 0.15F, 78.0F, 150.0F);
    float x_offset =
        restrain_value((float) engine->window_size.x * 0.20F, 170.0F, 330.0F);

    init_pause_bg(engine, &data->pause);
    init_main_left(engine, &data->pause, x_offset, y_offset);
    init_main_right(engine, &data->pause, x_offset, y_offset);
    init_controls_page(engine, &data->pause);
    init_sounds_page(engine, &data->pause);
    init_hud_page(engine, data);
    data->pause.page = PAUSE_PAGE_MAIN;
}
