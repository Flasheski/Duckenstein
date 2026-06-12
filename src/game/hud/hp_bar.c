/*
** EPITECH PROJECT, 2026
** hp_bar.c
** File description:
** draw hp bar
*/

#include <SFML/Graphics/RectangleShape.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/Text.h>
#include <math.h>
#include <stdio.h>

#include "graphics/engine.h"

#include "game.h"

static void update_hp_bar_text(game_data_t *d, const sfVector2f *pos,
    const sfVector2f *size)
{
    char buffer[32];
    sfFloatRect bounds;
    sfVector2f text_pos;

    snprintf(buffer, sizeof(buffer), "HP %d/%d", d->player.hp,
        d->player.max_hp);
    sfText_setString(d->hud->hp_text, buffer);
    bounds = sfText_getLocalBounds(d->hud->hp_text);
    text_pos.x = pos->x + size->x / 2.0F - bounds.width / 2.0F - bounds.left;
    text_pos.y = pos->y + size->y / 2.0F - bounds.height / 2.0F - bounds.top;
    sfText_setPosition(d->hud->hp_text, text_pos);
}

static void update_hp_bar(game_data_t *d, const sfVector2f *pos,
    const sfVector2f *size, float percentage)
{
    sfRectangleShape_setPosition(d->hud->hp_background, *pos);
    sfRectangleShape_setSize(d->hud->hp_background, *size);
    sfRectangleShape_setPosition(d->hud->hp_bar, *pos);
    sfRectangleShape_setSize(d->hud->hp_bar,
        (sfVector2f) {size->x * percentage, size->y});
    update_hp_bar_text(d, pos, size);
}

void draw_hp_bar(engine_t *engine, game_data_t *d)
{
    float scale = HUD_SCALE;
    float percentage = (float) d->player.hp / (float) d->player.max_hp;
    sfVector2f size = {HP_BAR_SIZE.x * scale, HP_BAR_SIZE.y * scale};
    float bottom = (float) engine->window_size.y
        - (HUD_BASE_HEIGHT - HP_BAR_POS.y) * scale;
    sfVector2f pos = {HP_BAR_POS.x * scale, bottom};

    percentage = fminf(1.0F, fmaxf(0.0F, percentage));
    update_hp_bar(d, &pos, &size, percentage);
    sfRenderWindow_drawRectangleShape(engine->window, d->hud->hp_background,
        nullptr);
    sfRenderWindow_drawRectangleShape(engine->window, d->hud->hp_bar, nullptr);
    sfRenderWindow_drawText(engine->window, d->hud->hp_text, nullptr);
}
