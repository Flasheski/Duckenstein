/*
** EPITECH PROJECT, 2026
** enemies_update.c
** File description:
** update enemies
*/

#include <math.h>

#include "game.h"

void update_enemies(engine_t *engine, game_data_t *d)
{
    sfVector2f dist_vector;
    float dist;

    for (size_t i = 0; i < d->enemy_count; i++) {
        if (!d->enemies[i].alive)
            continue;
        d->enemies[i].shoot_cooldown -= engine->dt;
        if (d->enemies[i].shoot_cooldown > 0)
            continue;
        dist_vector = (sfVector2f) {d->player.pos.x - d->enemies[i].pos.x,
            d->player.pos.y - d->enemies[i].pos.y};
        dist = hypotf(dist_vector.x, dist_vector.y);
        if (dist <= ENEMY_SHOOT_RANGE
            && can_see_other_point(d, &d->enemies[i].pos, &d->player.pos)) {
            d->enemies[i].shoot_cooldown = d->enemies[i].shoot_delay;
            damage_player(d, d->enemies[i].damage);
        }
    }
}
