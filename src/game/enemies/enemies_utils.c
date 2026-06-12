/*
** EPITECH PROJECT, 2026
** enemies_utils.c
** File description:
** enemies utils
*/

#include <SFML/Audio/Sound.h>
#include <math.h>
#include <stdlib.h>

#include "enemies.h"
#include "game.h"
#include "graphics/engine.h"

bool can_see_other_point(game_data_t *d, const sfVector2f *a,
    const sfVector2f *b)
{
    sfVector2f dist_vector = {b->x - a->x, b->y - a->y};
    float dist = hypotf(dist_vector.x, dist_vector.y);
    int nb_steps = (int) (dist / ENEMY_TARGET_DDA_STEP);
    sfVector2f pos;
    int x;
    int y;

    if (nb_steps <= 0)
        return true;
    for (int i = 1; i < nb_steps; i++) {
        pos.x = a->x + dist_vector.x * (float) i / (float) nb_steps;
        pos.y = a->y + dist_vector.y * (float) i / (float) nb_steps;
        x = (int) pos.x;
        y = (int) pos.y;
        if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT
            || d->map[x][y] > 0)
            return false;
    }
    return true;
}

void damage_player(game_data_t *d, int damage)
{
    if (d->player.is_dead || d->game_won)
        return;
    sfSound_play(d->enemy_hit_sound);
    sfSound_play(d->weapons[0].shoot_sound);
    d->player.hp -= damage;
    if (d->player.hp > 0)
        return;
    d->player.hp = 0;
    d->player.is_dead = true;
    d->game_over = true;
    if (d->player.dead_sound)
        sfSound_play(d->player.dead_sound);
}

static void drop_key(game_data_t *d, enemy_t *enemy)
{
    if (!enemy->drops_key || d->key_count >= MAX_KEYS)
        return;
    d->keys[d->key_count].pos = enemy->pos;
    d->keys[d->key_count].active = true;
    d->key_count++;
}

static void drop_medikit(game_data_t *d, enemy_t *enemy, int chance,
    game_difficulty_t difficulty)
{
    float multiplier = 1;

    if (difficulty == GAME_DIFFICULTY_MEDIUM)
        multiplier = 0.75F;
    if (difficulty == GAME_DIFFICULTY_HARD)
        multiplier = 0.5F;
    if (d->medikit_count < MAX_MEDIKITS
        && rand() % 100 < (int) ((float) chance * multiplier)) {
        d->medikits[d->medikit_count].pos = enemy->pos;
        d->medikits[d->medikit_count].active = true;
        d->medikit_count++;
    }
}

void damage_enemy(game_data_t *d, enemy_t *enemy, int damage,
    game_difficulty_t difficulty)
{
    int chance = MEDIKIT_DROP_NORMAL;

    enemy->hp -= damage;
    if (d->enemy_hit_sound)
        sfSound_play(d->enemy_hit_sound);
    if (enemy->hp > 0)
        return;
    enemy->hp = 0;
    enemy->alive = false;
    drop_key(d, enemy);
    if (enemy->type == ENEMY_BOSS)
        d->game_won = true;
    if (enemy->type == ENEMY_ELITE)
        chance = MEDIKIT_DROP_ELITE;
    if (enemy->type == ENEMY_BOSS)
        chance = MEDIKIT_DROP_BOSS;
    drop_medikit(d, enemy, chance, difficulty);
}
