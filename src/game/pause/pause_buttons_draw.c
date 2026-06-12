/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** pause_buttons_draw
*/

#include "graphics/engine.h"
#include "graphics/ui.h"
#include "menu.h"

void draw_buttons(engine_t *engine, ui_button_t **buttons, size_t count)
{
    for (size_t i = 0; i < count; i++)
        draw_btn(engine, buttons[i]);
}

void update_buttons(engine_t *engine, ui_button_t **buttons, size_t count)
{
    for (size_t i = 0; i < count; i++)
        ui_button_update(engine, buttons[i]);
}

void handle_buttons_events(engine_t *engine, const sfEvent *event,
    ui_button_t **buttons, size_t count)
{
    bool fullscreen = engine->is_fullscreen;

    for (size_t i = 0; i < count; i++) {
        ui_button_events(buttons[i], event, engine);
        if (engine->is_fullscreen != fullscreen)
            return;
    }
}

void destroy_buttons(ui_button_t **buttons, size_t count)
{
    for (size_t i = 0; i < count; i++)
        ui_button_destroy(buttons[i]);
}
