/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** menu_toggle_clicks
*/

#include "game.h"
#include "graphics/engine.h"
#include "menu.h"

void on_menu_music_click(void *data)
{
    engine_t *engine = data;
    menu_data_t *menu = engine->scene->data;

    settings_toggle_music(engine);
    settings_apply_toggle_color(menu->music_btn, engine->music_enabled);
}

void on_menu_sound_click(void *data)
{
    engine_t *engine = data;
    menu_data_t *menu = engine->scene->data;

    settings_toggle_sounds(engine);
    settings_apply_toggle_color(menu->sounds_btn, engine->sounds_enabled);
}

void on_menu_keys_click(void *data)
{
    engine_t *engine = data;
    menu_data_t *menu = engine->scene->data;

    engine->is_fr = !engine->is_fr;
    settings_apply_keyboard_texture(engine, menu->controls_btn);
}

void on_menu_fullscreen_click(void *data)
{
    engine_t *engine = data;

    switch_fullscreen(engine);
    sfRenderWindow_setMouseCursorVisible(engine->window, sfTrue);
    sfRenderWindow_setMouseCursorGrabbed(engine->window, sfFalse);
}
