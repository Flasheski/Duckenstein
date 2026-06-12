/*
** EPITECH PROJECT, 2026
** game_resize.c
** File description:
** resize the game
*/

#include <SFML/Graphics/Glsl.h>
#include <SFML/Graphics/RectangleShape.h>
#include <SFML/Graphics/RenderTexture.h>
#include <SFML/Graphics/VertexArray.h>
#include <SFML/Graphics/View.h>
#include <SFML/Window/Mouse.h>

#include "graphics/engine.h"

#include "game.h"
#include "menu.h"
#include "weapons.h"

static void resize_render(game_data_t *d, unsigned int w, unsigned int h)
{
    sfVertexArray_resize(d->rays, (size_t) w * 2);
    sfRectangleShape_setSize(d->floor_ceil,
        (sfVector2f) {(float) w, (float) h});
    sfShader_setVec2Uniform(d->floor_ceil_shader, "u_resolution",
        (sfGlslVec2) {(float) w, (float) h});
    sfShader_setVec2Uniform(d->vignette_shader, "u_resolution",
        (sfGlslVec2) {(float) w, (float) h});
    sfRenderTexture_destroy(d->render_texture);
    d->render_texture = sfRenderTexture_create(w, h, false);
    free(d->depth_buffer);
    d->depth_buffer = nullptr;
    d->depth_buffer = calloc((size_t) w + 1, sizeof(float));
}

static void save_game_state(game_data_t *d, float *timer, bool *show_minimap,
    bool *show_timer)
{
    *timer = d->hud->timer_time;
    *show_minimap = d->hud->show_minimap;
    *show_timer = d->hud->show_timer;
}

static void restore_game_state(game_data_t *d, float timer, bool show_minimap,
    bool show_timer)
{
    d->hud->timer_time = timer;
    d->hud->show_minimap = show_minimap;
    d->hud->show_timer = show_timer;
}

static void reinit_pause_menu(engine_t *engine, game_data_t *d,
    pause_page_t pause_page)
{
    pause_destroy(d);
    d->pause = (pause_menu_t) {0};
    pause_init(engine, d);
    d->pause.page = pause_page;
}

void game_on_resize(engine_t *engine)
{
    game_data_t *d = (game_data_t *) engine->scene->data;
    unsigned int w = engine->window_size.x;
    unsigned int h = engine->window_size.y;
    float timer = 0;
    bool show_minimap = true;
    bool show_timer = true;
    pause_page_t pause_page = d->pause.page;

    resize_render(d, w, h);
    save_game_state(d, &timer, &show_minimap, &show_timer);
    free_hud(d->hud);
    init_hud(engine, d);
    restore_game_state(d, timer, show_minimap, show_timer);
    reinit_pause_menu(engine, d, pause_page);
    resize_weapons(engine, d);
    if (!d->is_paused) {
        sfRenderWindow_setMouseCursorVisible(engine->window, false);
        sfMouse_setPositionRenderWindow(
            (sfVector2i) {(int) w / 2, (int) h / 2}, engine->window);
    }
}
