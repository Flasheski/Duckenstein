/*
** EPITECH PROJECT, 2026
** wolf3d [WSL: Ubuntu]
** File description:
** menu_create
*/

#include "graphics/engine.h"
#include "graphics/maths.h"
#include "graphics/resources.h"
#include "graphics/sprite_anim.h"
#include "menu.h"
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Text.h>
#include <SFML/System/Vector2.h>
#include <stdlib.h>

static sfVector2f get_main_button_size(engine_t *engine)
{
    sfTexture *texture =
        resources_load_texture(engine->resources, PLAY_BTN_PATH);
    sfVector2u text_size = sfTexture_getSize(texture);
    float width = (float) engine->window_size.x * 0.2F;

    return (
        sfVector2f) {width, width * (float) text_size.y / (float) text_size.x};
}

// clang-format off
static void init_main_buttons(engine_t *engine, menu_data_t *data)
{
    sfVector2f size = get_main_button_size(engine);
    float x_offset =
        restrain_value((float) engine->window_size.x * 0.20F, 170.0F, 330.0F);
    float y_gap = size.y * 0.35F;
    float top_y = (float) engine->window_size.y * 0.60F - size.y - y_gap / 2;
    sfVector2f positions[] = {{engine->window_size.x / 2.0F - x_offset, top_y},
        {engine->window_size.x / 2.0F + x_offset, top_y},
        {engine->window_size.x / 2.0F - x_offset, top_y + size.y + y_gap},
        {engine->window_size.x / 2.0F + x_offset, top_y + size.y + y_gap}};
    ui_button_t **buttons[] = {&data->play_btn, &data->settings_btn,
        &data->difficulty_btn, &data->exit_btn};
    const char *textures[] = {PLAY_BTN_PATH, PARAMS_BTN_PATH,
        settings_get_difficulty_texture(engine->difficulty), EXIT_BTN_PATH};
    void (*on_clicks[])(void *) = {&on_play_click, &on_settings_click,
        &on_menu_difficulty_click, &on_exit_click};
    buttons_group_config_t config = {buttons, textures, on_clicks, positions,
        sizeof(buttons) / sizeof(buttons[0]), &size};

    create_buttons_group(engine, &config);
}

static void init_settings_buttons(engine_t *engine, menu_data_t *data)
{
    sfVector2f positions[] = {{(float) engine->window_size.x * 0.35F,
            (float) engine->window_size.y * 0.45F}, {(float)
            engine->window_size.x * 0.65F, (float) engine->window_size.y *
            0.45F}, {(float) engine->window_size.x * 0.35F, (float)
            engine->window_size.y * 0.60F}, {(float) engine->window_size.x *
            0.65F, (float) engine->window_size.y * 0.60F}};
    ui_button_t **buttons[] = {&data->controls_btn, &data->win_mode_btn,
        &data->music_btn, &data->sounds_btn};
    const char *textures[] = {FR_BTN_PATH, RESIZE_BTN_PATH, MUSICS_BTN_PATH,
        SOUNDS_BTN_PATH};
    void (*on_clicks[])(void *) = {&on_menu_keys_click,
        &on_menu_fullscreen_click, &on_menu_music_click, &on_menu_sound_click};
    buttons_group_config_t config = {buttons, textures, on_clicks, positions,
        sizeof(buttons) / sizeof(buttons[0]), nullptr};

    create_buttons_group(engine, &config);
    settings_apply_keyboard_texture(engine, data->controls_btn);
    settings_apply_toggle_color(data->win_mode_btn, engine->is_fullscreen);
    settings_apply_toggle_color(data->music_btn, engine->music_enabled);
    settings_apply_toggle_color(data->sounds_btn, engine->sounds_enabled);
}
// clang-format on

void init_buttons(engine_t *engine, menu_data_t *data)
{
    init_main_buttons(engine, data);
    init_settings_buttons(engine, data);
}

void init_sprites(engine_t *engine, menu_data_t *data)
{
    sfTexture *texture_bg =
        resources_load_texture(engine->resources, BG_MAIN_PATH);
    sfVector2u bg_size = sfTexture_getSize(texture_bg);
    const sprite_frame_info_t info = {{0, 0, LOGO_WIDTH, LOGO_HEIGHT},
        LOGO_FRAMES, LOGO_FPS};

    data->bg = sfSprite_create();
    sfSprite_setTexture(data->bg, texture_bg, sfTrue);
    sfSprite_setScale(data->bg,
        (sfVector2f) {(float) engine->window_size.x / (float) bg_size.x,
            (float) engine->window_size.y / (float) bg_size.y});
    data->logo = sprite_anim_create(LOGO_PATH, &info, engine);
    sfSprite_setOrigin(data->logo->sprite,
        (sfVector2f) {LOGO_WIDTH / 2.0F, LOGO_HEIGHT / 2.0F});
    sfSprite_setScale(data->logo->sprite,
        (sfVector2f) {(float) engine->window_size.x / LOGO_WIDTH * 0.65F,
            (float) engine->window_size.x / LOGO_WIDTH * 0.65F});
    sfSprite_setPosition(data->logo->sprite,
        (sfVector2f) {engine->window_size.x / 2.0F, 150.0F});
}

scene_t *menu_create(void)
{
    scene_t *scene = malloc(sizeof(scene_t));
    menu_data_t *data = calloc(1, sizeof(menu_data_t));

    if (!scene || !data)
        return nullptr;
    scene->data = data;
    scene->on_enter = &menu_on_enter;
    scene->update = &menu_update;
    scene->on_resize = &menu_on_resize;
    scene->draw = &menu_draw;
    scene->handle_events = &handle_menu_events;
    scene->on_exit = &menu_on_exit;
    return scene;
}
