/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** menu_update
*/

#include "graphics/engine.h"
#include "graphics/resources.h"
#include "graphics/sprite_anim.h"
#include "menu.h"
#include <stdlib.h>

void menu_update(engine_t *engine)
{
    menu_data_t *data = engine->scene->data;
    ui_button_t *buttons[MAX_MAIN_MENU_BUTTONS];
    size_t count = menu_get_page_buttons(data, buttons);

    update_buttons(engine, buttons, count);
    sprite_anim_update(data->logo, engine->dt);
}
