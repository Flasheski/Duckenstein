/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** menu_clicks
*/

#include "game.h"
#include "graphics/engine.h"
#include "menu.h"

void on_menu_difficulty_click(void *data)
{
    engine_t *engine = data;
    menu_data_t *menu = engine->scene->data;

    settings_toggle_difficulty(engine);
    settings_apply_difficulty_texture(engine, menu->difficulty_btn);
}

void on_play_click(void *data)
{
    engine_t *engine = data;

    sfRenderWindow_setMouseCursorGrabbed(engine->window, sfTrue);
    sfRenderWindow_setMouseCursorVisible(engine->window, sfFalse);
    engine_set_scene(engine, game_create(), true);
}

void on_exit_click(void *data)
{
    engine_t *engine = data;

    sfRenderWindow_close(engine->window);
}

void on_settings_click(void *data)
{
    engine_t *engine = data;
    menu_data_t *menu = engine->scene->data;

    menu->page = MENU_PAGE_SETTINGS;
}
