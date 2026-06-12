/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** pause_settings
*/

#include <SFML/Audio/Music.h>
#include <SFML/Audio/Sound.h>
#include <SFML/Graphics/Color.h>

#include "game.h"
#include "graphics/engine.h"
#include "graphics/resources.h"
#include "graphics/ui.h"
#include "menu.h"
#include "wolf3d.h"

void set_pause_page(engine_t *engine, pause_page_t page)
{
    game_data_t *data = engine->scene->data;

    if (!data)
        return;
    data->pause.page = page;
}

void settings_apply_keyboard_texture(engine_t *engine, ui_button_t *button)
{
    const char *path = nullptr;

    if (!engine || !button)
        return;
    path = engine->is_fr ? FR_BTN_PATH : EN_BTN_PATH;
    sfRectangleShape_setTexture(button->background,
        resources_load_texture(engine->resources, path), true);
}

void settings_apply_toggle_color(ui_button_t *button, bool enabled)
{
    sfColor color = enabled ? sfWhite : sfRed;

    if (!button)
        return;
    sfRectangleShape_setFillColor(button->background, color);
}

void settings_set_music_enabled(engine_t *engine, bool enabled)
{
    sfMusic *music = nullptr;
    sfSoundStatus status = sfStopped;

    if (!engine || !engine->resources)
        return;
    engine->music_enabled = enabled;
    music = resources_load_music(engine->resources, MUSIC_PATH);
    if (!music)
        return;
    sfMusic_setVolume(music, 40);
    status = sfMusic_getStatus(music);
    if (!enabled && status == sfPlaying)
        sfMusic_pause(music);
    if (enabled && status != sfPlaying)
        sfMusic_play(music);
}

void settings_set_sounds_enabled(engine_t *engine, bool enabled)
{
    sound_resource_t *resource = nullptr;

    if (!engine || !engine->resources)
        return;
    engine->sounds_enabled = enabled;
    for (resource = engine->resources->sound_list; resource;
        resource = resource->next) {
        sfSound_setVolume(resource->sound, enabled ? 100.0F : 0.0F);
    }
}
