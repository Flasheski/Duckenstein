/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** pause_buttons_utils
*/

#include "graphics/engine.h"
#include "graphics/resources.h"
#include "graphics/ui.h"
#include "menu.h"

ui_button_t *create_btn(engine_t *engine, const char *tex, sfVector2f *pos)
{
    sfVector2f size = {(float) engine->window_size.x * 0.17F, 60.0F};
    sfVector2u text_size;

    if (tex) {
        text_size =
            sfTexture_getSize(resources_load_texture(engine->resources, tex));
        size.y = size.x * (float) text_size.y / (float) text_size.x;
    }
    return ui_button_create(engine, tex, pos, &size);
}

void set_btn(ui_button_t *button, void (*f)(void *), engine_t *engine,
    char *text)
{
    if (button) {
        button->on_click = f;
        button->data = engine;
        if (text)
            ui_button_set_text(button, text, 40,
                &(sfColor) {255, 255, 255, 255});
    }
}

void create_buttons_group(engine_t *engine, buttons_group_config_t *config)
{
    sfVector2f custom_size = {0};

    for (size_t i = 0; i < config->count; i++) {
        if (config->size) {
            custom_size = *config->size;
            *config->buttons[i] = ui_button_create(engine, config->textures[i],
                &config->positions[i], &custom_size);
        } else {
            *config->buttons[i] =
                create_btn(engine, config->textures[i], &config->positions[i]);
        }
        set_btn(*config->buttons[i], config->on_clicks[i], engine, nullptr);
    }
}

size_t menu_get_page_buttons(menu_data_t *menu, ui_button_t **buttons)
{
    if (!menu || !buttons)
        return 0;
    if (menu->page == MENU_PAGE_MAIN) {
        buttons[0] = menu->play_btn;
        buttons[1] = menu->settings_btn;
        buttons[2] = menu->difficulty_btn;
        buttons[3] = menu->exit_btn;
        return 4;
    }
    if (menu->page == MENU_PAGE_SETTINGS) {
        buttons[0] = menu->controls_btn;
        buttons[1] = menu->win_mode_btn;
        buttons[2] = menu->music_btn;
        buttons[3] = menu->sounds_btn;
        return 4;
    }
    return 0;
}

static size_t get_pause_page_main(pause_menu_t *pause, ui_button_t **buttons)
{
    buttons[0] = pause->resume;
    buttons[1] = pause->save;
    buttons[2] = pause->exit;
    buttons[3] = pause->controls;
    buttons[4] = pause->sounds;
    buttons[5] = pause->hud;
    return 6;
}

size_t pause_get_page_buttons(pause_menu_t *pause, ui_button_t **buttons)
{
    if (!pause || !buttons)
        return 0;
    if (pause->page == PAUSE_PAGE_MAIN)
        return get_pause_page_main(pause, buttons);
    if (pause->page == PAUSE_PAGE_CONTROLS) {
        buttons[0] = pause->controls_mv;
        return 1;
    }
    if (pause->page == PAUSE_PAGE_SOUNDS) {
        buttons[0] = pause->music_btn;
        buttons[1] = pause->sounds_btn;
        return 2;
    }
    if (pause->page == PAUSE_PAGE_HUD) {
        buttons[0] = pause->hud_minimap;
        buttons[1] = pause->hud_timer;
        buttons[2] = pause->win_mode;
        return 3;
    }
    return 0;
}
