/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** pause_hud
*/

#include <SFML/Audio/Sound.h>
#include <SFML/Graphics/Color.h>

#include "game.h"
#include "graphics/engine.h"
#include "graphics/resources.h"
#include "graphics/ui.h"

void pause_on_hud(void *data)
{
    engine_t *engine = data;

    set_pause_page(engine, PAUSE_PAGE_HUD);
}

void pause_on_hud_minimap(void *data)
{
    engine_t *engine = data;
    game_data_t *game_data = engine->scene->data;

    if (!game_data || !game_data->hud || !game_data->pause.hud_minimap)
        return;
    game_data->hud->show_minimap = !game_data->hud->show_minimap;
    settings_apply_toggle_color(game_data->pause.hud_minimap,
        game_data->hud->show_minimap);
    if (game_data && game_data->pause.hud_minimap)
        sfRectangleShape_setTexture(game_data->pause.hud_minimap->background,
            game_data->hud->show_minimap
            ? resources_load_texture(engine->resources, MAP_OK_BTN_PATH)
            : resources_load_texture(engine->resources, MAP_KO_BTN_PATH),
            true);
}

void pause_on_hud_timer(void *data)
{
    engine_t *engine = data;
    game_data_t *game_data = engine->scene->data;

    if (!game_data || !game_data->hud)
        return;
    game_data->hud->show_timer = !game_data->hud->show_timer;
    settings_apply_toggle_color(game_data->pause.hud_timer,
        game_data->hud->show_timer);
    if (game_data && game_data->pause.hud_timer)
        sfRectangleShape_setTexture(game_data->pause.hud_timer->background,
            game_data->hud->show_timer
            ? resources_load_texture(engine->resources, TIME_OK_BTN_PATH)
            : resources_load_texture(engine->resources, TIME_KO_BTN_PATH),
            true);
}

void pause_on_controls_resize(void *data)
{
    engine_t *engine = data;
    game_data_t *game_data = engine->scene->data;

    if (!game_data)
        return;
    switch_fullscreen(engine);
    if (engine->is_fullscreen) {
        sfRenderWindow_setMouseCursorVisible(engine->window, true);
        sfRenderWindow_setMouseCursorGrabbed(engine->window, false);
    }
    if (game_data && game_data->pause.win_mode)
        settings_apply_toggle_color(game_data->pause.win_mode,
            engine->is_fullscreen);
}
