/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** menu_loop
*/

#include "graphics/engine.h"
#include "graphics/resources.h"
#include "menu.h"
#include <stdlib.h>

void handle_menu_events(engine_t *engine, sfEvent *event)
{
    menu_data_t *data = engine->scene->data;
    ui_button_t *buttons[MAX_MAIN_MENU_BUTTONS];
    size_t count = menu_get_page_buttons(data, buttons);

    handle_buttons_events(engine, event, buttons, count);
    if (event->type == sfEvtKeyPressed && event->key.code == sfKeyEscape)
        data->page = MENU_PAGE_MAIN;
}
