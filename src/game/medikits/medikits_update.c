/*
** EPITECH PROJECT, 2026
** medikits_update.c
** File description:
** update medikits
*/

#include <SFML/Audio/Sound.h>
#include <math.h>

#include "game.h"

void update_medikits(game_data_t *d)
{
    sfVector2f dist_vector;
    float dist;

    for (size_t i = 0; i < d->medikit_count; i++) {
        if (!d->medikits[i].active || d->player.hp >= d->player.max_hp)
            continue;
        dist_vector = (sfVector2f) {d->medikits[i].pos.x - d->player.pos.x,
            d->medikits[i].pos.y - d->player.pos.y};
        dist = hypotf(dist_vector.x, dist_vector.y);
        if (dist > MEDIKIT_COLLISION_RADIUS)
            continue;
        d->player.hp += MEDIKIT_HEAL;
        if (d->player.hp > d->player.max_hp)
            d->player.hp = d->player.max_hp;
        d->medikits[i].active = false;
        sfSound_play(d->player.heal_sound);
    }
}
