/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** pause_update
*/

#include "game.h"
#include "graphics/engine.h"
#include "graphics/ui.h"

void pause_update(engine_t *engine, game_data_t *d)
{
    ui_button_t *buttons[MAX_PAUSE_BUTTONS];
    size_t count = pause_get_page_buttons(&d->pause, buttons);

    update_buttons(engine, buttons, count);
}
