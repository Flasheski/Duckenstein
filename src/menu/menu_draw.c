/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** menu_draw
*/

#include "graphics/engine.h"
#include "graphics/resources.h"
#include "menu.h"
#include <stdlib.h>

void menu_draw(engine_t *engine)
{
    menu_data_t *data = engine->scene->data;
    ui_button_t *buttons[MAX_MAIN_MENU_BUTTONS];
    size_t count = menu_get_page_buttons(data, buttons);

    sfRenderWindow_drawSprite(engine->window, data->bg, nullptr);
    sfRenderWindow_drawSprite(engine->window, data->logo->sprite, nullptr);
    draw_buttons(engine, buttons, count);
}
