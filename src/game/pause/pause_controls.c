/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** pause_controls
*/

#include <SFML/Audio/Sound.h>
#include <SFML/Graphics/Color.h>

#include "game.h"
#include "graphics/engine.h"
#include "menu.h"

void pause_on_controls(void *data)
{
    engine_t *engine = data;

    set_pause_page(engine, PAUSE_PAGE_CONTROLS);
}

void pause_on_controls_keys(void *data)
{
    engine_t *engine = data;
    game_data_t *game_data = engine->scene->data;

    if (!game_data)
        return;
    engine->is_fr = !engine->is_fr;
    settings_apply_keyboard_texture(engine, game_data->pause.controls_mv);
}
