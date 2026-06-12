/*
** EPITECH PROJECT, 2026
** add_draw_objects.c
** File description:
** Draw objects
*/

#include <SFML/Graphics/Color.h>
#include <stddef.h>
#include <stdlib.h>

#include "enemies.h"
#include "game.h"

void add_enemies_objects(game_data_t *d, world_item_t *items, size_t *count)
{
    sfVector2f dist_vector;

    for (size_t i = 0; i < d->enemy_count; i++) {
        if (!d->enemies[i].alive)
            continue;
        dist_vector = (sfVector2f) {d->enemies[i].pos.x - d->player.pos.x,
            d->enemies[i].pos.y - d->player.pos.y};
        items[*count].type = OBJECT_ENEMY;
        items[*count].index = i;
        items[*count].dist =
            dist_vector.x * dist_vector.x + dist_vector.y * dist_vector.y;
        (*count)++;
    }
}

void add_medikits_objects(game_data_t *d, world_item_t *items, size_t *count)
{
    sfVector2f dist_vector;

    for (size_t i = 0; i < d->medikit_count; i++) {
        if (!d->medikits[i].active)
            continue;
        dist_vector = (sfVector2f) {d->medikits[i].pos.x - d->player.pos.x,
            d->medikits[i].pos.y - d->player.pos.y};
        items[*count].type = OBJECT_MEDIKIT;
        items[*count].index = i;
        items[*count].dist =
            dist_vector.x * dist_vector.x + dist_vector.y * dist_vector.y;
        (*count)++;
    }
}

void add_keys_objects(game_data_t *d, world_item_t *items, size_t *count)
{
    sfVector2f dist_vector;

    for (size_t i = 0; i < d->key_count; i++) {
        if (!d->keys[i].active)
            continue;
        dist_vector = (sfVector2f) {d->keys[i].pos.x - d->player.pos.x,
            d->keys[i].pos.y - d->player.pos.y};
        items[*count].type = OBJECT_KEY;
        items[*count].index = i;
        items[*count].dist =
            dist_vector.x * dist_vector.x + dist_vector.y * dist_vector.y;
        (*count)++;
    }
}
