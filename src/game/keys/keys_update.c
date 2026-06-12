/*
** EPITECH PROJECT, 2026
** keys_update.c
** File description:
** update keys and doors
*/

#include <SFML/Audio/Sound.h>
#include <math.h>

#include "graphics/sprite_anim.h"

#include "game.h"

static float distance_to_player(game_data_t *d, float x, float y)
{
    sfVector2f dist = {x - d->player.pos.x, y - d->player.pos.y};

    return hypotf(dist.x, dist.y);
}

void update_keys(engine_t *engine, game_data_t *d)
{
    float dist;

    sprite_anim_update(d->key_anim, engine->dt);
    for (size_t i = 0; i < d->key_count; i++) {
        if (!d->keys[i].active)
            continue;
        dist = distance_to_player(d, d->keys[i].pos.x, d->keys[i].pos.y);
        if (dist > KEY_COLLISION_RADIUS)
            continue;
        d->keys[i].active = false;
        d->player.keys++;
    }
}

static void close_neighbor_doors(game_data_t *d, int x, int y)
{
    if (x > 0 && d->map[x - 1][y] == DOOR_TEXTURE_INDEX)
        d->map[x - 1][y] = 0;
    if (x < MAP_WIDTH - 1 && d->map[x + 1][y] == DOOR_TEXTURE_INDEX)
        d->map[x + 1][y] = 0;
    if (y > 0 && d->map[x][y - 1] == DOOR_TEXTURE_INDEX)
        d->map[x][y - 1] = 0;
    if (y < MAP_HEIGHT - 1 && d->map[x][y + 1] == DOOR_TEXTURE_INDEX)
        d->map[x][y + 1] = 0;
}

static bool update_doors_loop(game_data_t *d, int y)
{
    float dist;

    for (int x = 0; x < MAP_WIDTH; x++) {
        if (d->map[x][y] != DOOR_TEXTURE_INDEX)
            continue;
        dist = distance_to_player(d, (float) x + 0.5F, (float) y + 0.5F);
        if (dist > DOOR_OPEN_RADIUS)
            continue;
        d->map[x][y] = 0;
        close_neighbor_doors(d, x, y);
        d->player.keys--;
        if (d->player.door_open_sound)
            sfSound_play(d->player.door_open_sound);
        return false;
    }
    return true;
}

void update_doors(game_data_t *d)
{
    if (d->player.keys <= 0)
        return;
    for (int y = 0; y < MAP_HEIGHT; y++)
        if (!update_doors_loop(d, y))
            return;
}
