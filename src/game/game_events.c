/*
** EPITECH PROJECT, 2026
** game_events.c
** File description:
** Handle events during the game
*/

#include <SFML/Graphics/RectangleShape.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Window/Event.h>
#include <SFML/Window/Keyboard.h>
#include <SFML/Window/Mouse.h>
#include <SFML/Window/VideoMode.h>

#include "graphics/engine.h"

#include "game.h"
#include "weapons.h"
#include "wolf3d.h"

static void switch_fullscreen_restore(engine_t *engine,
    sfVideoMode *video_mode, bool activate_fullscreen)
{
#ifndef DEBUG
    sfRenderWindow_setFramerateLimit(engine->window, FPS);
#endif
    engine->is_fullscreen = activate_fullscreen;
    engine->window_size = (sfVector2u) {video_mode->width, video_mode->height};
    if (engine->transition_rect)
        sfRectangleShape_setSize(engine->transition_rect,
            (sfVector2f) {(float) video_mode->width,
                (float) video_mode->height});
    if (engine->scene && engine->scene->on_resize)
        engine->scene->on_resize(engine);
}

void switch_fullscreen(engine_t *engine)
{
    bool activate_fullscreen = !engine->is_fullscreen;
    sfVideoMode video_mode;

    if (activate_fullscreen)
        engine->windowed_size = engine->window_size;
    video_mode = activate_fullscreen
        ? sfVideoMode_getDesktopMode()
        : (sfVideoMode) {engine->windowed_size.x, engine->windowed_size.y, 32};
    sfRenderWindow_destroy(engine->window);
    engine->window = sfRenderWindow_create(video_mode, WIN_TITLE,
        activate_fullscreen ? sfFullscreen : (sfClose | sfResize), nullptr);
    switch_fullscreen_restore(engine, &video_mode, activate_fullscreen);
}

static void weapons_events(engine_t *engine, game_data_t *d, sfEvent *event)
{
    weapon_t *w = &d->weapons[d->active_weapon];

    if (event->type == sfEvtMouseButtonPressed
        && event->mouseButton.button == sfMouseLeft
        && !w->weapon_data->is_auto)
        weapon_shoot(engine, d);
    if (event->type == sfEvtKeyPressed && event->key.code == sfKeyR)
        weapon_reload(d);
    if (event->type == sfEvtKeyPressed && event->key.code == sfKeyTab)
        weapon_change(d);
}

static void handle_escape(engine_t *engine, game_data_t *data)
{
    if (data->is_paused && data->pause.page != PAUSE_PAGE_MAIN) {
        data->pause.page = PAUSE_PAGE_MAIN;
        return;
    }
    data->is_paused = !data->is_paused;
    sfRenderWindow_setMouseCursorVisible(engine->window, data->is_paused);
    sfRenderWindow_setMouseCursorGrabbed(engine->window, !data->is_paused);
}

void game_event(engine_t *engine, sfEvent *event)
{
    game_data_t *data = (game_data_t *) engine->scene->data;

    if (!data || !event)
        return;
    if (event->type == sfEvtKeyPressed && event->key.code == sfKeyEscape)
        handle_escape(engine, data);
    if (data->is_paused) {
        pause_events(engine, data, event);
        return;
    }
    if (event->type == sfEvtMouseButtonPressed
        && event->mouseButton.button == sfMouseRight)
        data->player.is_zooming = true;
    if (event->type == sfEvtMouseButtonReleased
        && event->mouseButton.button == sfMouseRight)
        data->player.is_zooming = false;
    weapons_events(engine, data, event);
}
