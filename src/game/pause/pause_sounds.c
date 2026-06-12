/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** pause_sounds
*/

#include <SFML/Graphics/Color.h>

#include "graphics/engine.h"

#include "game.h"
#include "menu.h"

void pause_on_sounds(void *data)
{
    engine_t *engine = data;

    set_pause_page(engine, PAUSE_PAGE_SOUNDS);
}

void pause_on_sound_music(void *data)
{
    engine_t *engine = data;
    game_data_t *game_data = engine->scene->data;

    settings_toggle_music(engine);
    if (!game_data)
        return;
    settings_apply_toggle_color(game_data->pause.music_btn,
        engine->music_enabled);
}

void pause_on_sound_sfx(void *data)
{
    engine_t *engine = data;
    game_data_t *game_data = engine->scene->data;

    settings_toggle_sounds(engine);
    if (!game_data)
        return;
    settings_apply_toggle_color(game_data->pause.sounds_btn,
        engine->sounds_enabled);
}
