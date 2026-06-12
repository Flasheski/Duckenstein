/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** pause_settings_utils
*/

#include <SFML/Graphics/Color.h>

#include "graphics/engine.h"
#include "graphics/resources.h"
#include "graphics/ui.h"
#include "menu.h"

void settings_toggle_music(engine_t *engine)
{
    if (!engine)
        return;
    settings_set_music_enabled(engine, !engine->music_enabled);
}

void settings_toggle_sounds(engine_t *engine)
{
    if (!engine)
        return;
    settings_set_sounds_enabled(engine, !engine->sounds_enabled);
}

void settings_toggle_difficulty(engine_t *engine)
{
    int difficulty = 0;

    if (!engine)
        return;
    difficulty = ((int) engine->difficulty + 1) % GAME_DIFFICULTY_COUNT;
    engine->difficulty = (game_difficulty_t) difficulty;
}

const char *settings_get_difficulty_texture(game_difficulty_t difficulty)
{
    if (difficulty == GAME_DIFFICULTY_MEDIUM)
        return MEDIUM_BTN_PATH;
    if (difficulty == GAME_DIFFICULTY_HARD)
        return HARD_BTN_PATH;
    return EASY_BTN_PATH;
}

void settings_apply_difficulty_texture(engine_t *engine, ui_button_t *button)
{
    if (!engine || !button)
        return;
    sfRectangleShape_setTexture(button->background,
        resources_load_texture(engine->resources,
            settings_get_difficulty_texture(engine->difficulty)),
        true);
}
