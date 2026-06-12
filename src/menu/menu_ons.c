/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** menu_ons
*/

#include "graphics/engine.h"
#include "graphics/resources.h"
#include "graphics/sprite_anim.h"
#include "menu.h"
#include <SFML/Graphics/Sprite.h>
#include <SFML/System/Clock.h>
#include <stdlib.h>

static void destroy_menu_buttons(menu_data_t *data)
{
    ui_button_t *buttons[] = {data->play_btn, data->settings_btn,
        data->difficulty_btn, data->exit_btn, data->controls_btn,
        data->win_mode_btn, data->music_btn, data->sounds_btn};

    destroy_buttons(buttons, sizeof(buttons) / sizeof(buttons[0]));
}

void menu_on_enter(engine_t *engine)
{
    menu_data_t *data = engine->scene->data;

    init_sprites(engine, data);
    init_buttons(engine, data);
    data->page = MENU_PAGE_MAIN;
    sfRenderWindow_setMouseCursorVisible(engine->window, sfTrue);
    sfRenderWindow_setMouseCursorGrabbed(engine->window, sfFalse);
    if (engine->end_clock) {
        sfClock_destroy(engine->end_clock);
        engine->end_clock = nullptr;
    }
}

void menu_on_resize(engine_t *engine)
{
    menu_data_t *data = engine->scene->data;
    menu_page_t page = data->page;

    sfSprite_destroy(data->bg);
    sprite_anim_destroy(data->logo);
    destroy_menu_buttons(data);
    init_sprites(engine, data);
    init_buttons(engine, data);
    data->page = page;
}

void menu_on_exit(engine_t *engine)
{
    menu_data_t *data = engine->scene->data;

    sfSprite_destroy(data->bg);
    sprite_anim_destroy(data->logo);
    destroy_menu_buttons(data);
    free(data);
}
