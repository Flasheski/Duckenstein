/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** display_end_screen
*/

#include "graphics/engine.h"
#include "graphics/resources.h"

#include "game.h"
#include "menu.h"
#include "weapons.h"

#include <SFML/Audio/Sound.h>
#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Types.h>
#include <SFML/System/Clock.h>
#include <stdio.h>
#include <unistd.h>

static void display_mission_failed(engine_t *engine, sfSprite *end_sprite,
    sfTexture *end_texture)
{
    end_texture =
        resources_load_texture(engine->resources, MISSION_FAILED_PATH);
    sfSprite_setTexture(end_sprite, end_texture, true);
    sfSprite_setScale(end_sprite, (sfVector2f) {3.F, 3.F});
    sfSprite_setPosition(end_sprite,
        (sfVector2f) {engine->window_size.x / 2.F
            - (sfTexture_getSize(end_texture).x / 2.F * 3.F),
            engine->window_size.y / 8.F});
    sfRenderWindow_drawSprite(engine->window, end_sprite, NULL);
    sfSprite_destroy(end_sprite);
}

static void display_mission_success(engine_t *engine, sfSprite *end_sprite,
    sfTexture *end_texture)
{
    end_texture =
        resources_load_texture(engine->resources, MISSION_SUCESS_PATH);
    sfSprite_setTexture(end_sprite, end_texture, true);
    sfSprite_setScale(end_sprite, (sfVector2f) {3.F, 3.F});
    sfSprite_setPosition(end_sprite,
        (sfVector2f) {engine->window_size.x / 2.F
            - (sfTexture_getSize(end_texture).x / 2.F * 3.F),
            engine->window_size.y / 8.F});
    sfRenderWindow_drawSprite(engine->window, end_sprite, NULL);
    sfSprite_destroy(end_sprite);
}

void display_end_type(engine_t *engine, game_data_t *d)
{
    sfSprite *end_sprite = sfSprite_create();
    sfTexture *end_texture = NULL;

    if (d->game_over)
        return display_mission_failed(engine, end_sprite, end_texture);
    if (d->game_won)
        return display_mission_success(engine, end_sprite, end_texture);
}

static void center_text(sfText *text, engine_t *engine, float offset)
{
    sfFloatRect text_rect = sfText_getLocalBounds(text);

    sfText_setOrigin(text,
        (sfVector2f) {text_rect.width / 2.F, text_rect.height / 2.F});
    sfText_setPosition(text,
        (sfVector2f) {engine->window_size.x / 2.F,
            engine->window_size.y / 2.F + text_rect.height + offset});
}

static void display_end_time(engine_t *engine, game_data_t *d)
{
    sfText *time_taken = sfText_create();
    char time_str[64] = {0};
    float time = d->hud->timer_time * 1000;
    int cs = ((int) time % 1000) / 10;
    int sec = ((int) time / 1000) % 60;
    int min = (int) time / 60000;

    if (!time_taken)
        return;
    snprintf(time_str, sizeof(time_str), "Time taken: %02d:%02d.%02d", min,
        sec, cs);
    sfText_setString(time_taken, time_str);
    sfText_setFont(time_taken, d->hud->hud_font);
    sfText_setCharacterSize(time_taken, engine->window_size.y / 20);
    sfText_setFillColor(time_taken, sfWhite);
    center_text(time_taken, engine, 0.F);
    sfRenderWindow_drawText(engine->window, time_taken, NULL);
    sfText_destroy(time_taken);
}

static void display_end_diff(engine_t *engine, game_data_t *d)
{
    sfText *diff = sfText_create();
    sfColor color;

    if (engine->difficulty == GAME_DIFFICULTY_EASY) {
        sfText_setString(diff, "EASY");
        color = sfColor_fromRGB(0, 204, 0);
    } else if (engine->difficulty == GAME_DIFFICULTY_MEDIUM) {
        sfText_setString(diff, "MEDIUM");
        color = sfColor_fromRGB(255, 140, 0);
    } else {
        sfText_setString(diff, "HARD");
        color = sfColor_fromRGB(198, 40, 40);
    }
    sfText_setFont(diff, d->hud->hud_font);
    sfText_setCharacterSize(diff, engine->window_size.y / 20);
    sfText_setFillColor(diff, color);
    center_text(diff, engine, 150.f);
    sfRenderWindow_drawText(engine->window, diff, NULL);
    sfText_destroy(diff);
}

void end_draw(engine_t *engine)
{
    game_data_t *data = (game_data_t *) engine->scene->data;

    if (engine->end_clock
        && sfClock_getElapsedTime(engine->end_clock).microseconds > 5000000) {
        sfClock_destroy(engine->end_clock);
        engine->end_clock = nullptr;
        engine_set_scene(engine, menu_create(), true);
        return;
    }
    if (data->is_end && !engine->end_clock) {
        if (data->game_won)
            sfSound_play(
                resources_load_sound(engine->resources, GAME_VICTORY_PATH));
        engine->end_clock = sfClock_create();
    }
    display_end_type(engine, engine->scene->data);
    display_end_time(engine, engine->scene->data);
    display_end_diff(engine, engine->scene->data);
}

void display_ending_screen(engine_t *engine)
{
    game_data_t *data = (game_data_t *) engine->scene->data;

    if (!data->is_end) {
        sfRenderWindow_setMouseCursorVisible(engine->window, sfTrue);
        sfRenderWindow_setMouseCursorGrabbed(engine->window, sfFalse);
        data->is_end = !data->is_end;
        engine->scene->draw = &end_draw;
    } else {
        return;
    }
}
