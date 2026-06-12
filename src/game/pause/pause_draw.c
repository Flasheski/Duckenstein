/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** pause_draw
*/

#include "game.h"
#include "graphics/engine.h"
#include "graphics/ui.h"
#include "menu.h"

void draw_btn(engine_t *engine, ui_button_t *button)
{
    sfRenderWindow_drawRectangleShape(engine->window, button->background,
        nullptr);
    if (button->text && sfText_getString(button->text))
        sfRenderWindow_drawText(engine->window, button->text, nullptr);
}

void pause_draw(engine_t *engine, game_data_t *data)
{
    ui_button_t *buttons[MAX_PAUSE_BUTTONS];
    size_t count = pause_get_page_buttons(&data->pause, buttons);

    sfRenderWindow_drawSprite(engine->window, data->pause.bg, nullptr);
    sfRenderWindow_drawRectangleShape(engine->window, data->pause.separator,
        nullptr);
    draw_buttons(engine, buttons, count);
}
