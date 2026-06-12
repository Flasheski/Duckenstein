/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** pause_destroy
*/

#include "game.h"
#include "graphics/ui.h"

void pause_destroy(game_data_t *data)
{
    ui_button_t *buttons[] = {data->pause.resume, data->pause.save,
        data->pause.exit, data->pause.controls, data->pause.sounds,
        data->pause.hud, data->pause.controls_mv, data->pause.win_mode,
        data->pause.music_btn, data->pause.sounds_btn, data->pause.hud_minimap,
        data->pause.hud_timer};

    sfSprite_destroy(data->pause.bg);
    sfRectangleShape_destroy(data->pause.separator);
    destroy_buttons(buttons, sizeof(buttons) / sizeof(buttons[0]));
}
