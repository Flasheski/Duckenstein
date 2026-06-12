/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** pause_events
*/

#include "game.h"
#include "graphics/engine.h"
#include "graphics/ui.h"

void pause_events(engine_t *engine, game_data_t *data, sfEvent *event)
{
    ui_button_t *buttons[MAX_PAUSE_BUTTONS];
    size_t count = 0;

    if (!data || !event)
        return;
    count = pause_get_page_buttons(&data->pause, buttons);
    handle_buttons_events(engine, event, buttons, count);
}
