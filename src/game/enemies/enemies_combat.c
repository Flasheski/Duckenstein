/*
** EPITECH PROJECT, 2026
** enemies_combat.c
** File description:
** hit enemies
*/

#include <math.h>

#include "game.h"

static int get_weapon_damage(weapon_type_t type)
{
    if (type == WEAPON_BAZOOKA)
        return BAZOOKA_DAMAGE;
    if (type == WEAPON_REVOLVER)
        return REVOLVER_DAMAGE;
    if (type == WEAPON_GRENADE)
        return GRENADE_DAMAGE;
    return AK_DAMAGE;
}

static bool enemy_is_close_to_cursor(engine_t *engine, game_data_t *d,
    size_t i, world_object_t *obj)
{
    sfTexture *texture = d->enemy_textures[d->enemies[i].type];
    sfVector2f center = {(float) engine->window_size.x / 2.0F,
        (float) engine->window_size.y / 2.0F};

    if (!texture)
        return false;
    obj->texture_size = sfTexture_getSize(texture);
    if (!get_world_object_coords(engine, d, &d->enemies[i].pos, obj))
        return false;
    if (center.x < (float) obj->start_x || center.x > (float) obj->end_x
        || center.y < (float) obj->start_y || center.y > (float) obj->end_y)
        return false;
    if (!can_see_other_point(d, &d->player.pos, &d->enemies[i].pos))
        return false;
    return true;
}

static enemy_t *find_enemy(engine_t *engine, game_data_t *d,
    world_object_t *obj, bool *is_critical)
{
    sfVector2f center = {(float) engine->window_size.x / 2.0F,
        (float) engine->window_size.y / 2.0F};
    enemy_t *target = nullptr;
    float closest_dist = INFINITY;

    for (size_t i = 0; i < d->enemy_count; i++) {
        if (!d->enemies[i].alive)
            continue;
        if (!enemy_is_close_to_cursor(engine, d, i, obj)
            || obj->transform_y >= closest_dist)
            continue;
        closest_dist = obj->transform_y;
        target = &d->enemies[i];
        *is_critical = center.y
            < (float) obj->start_y + (float) obj->height * ENEMY_HEAD_SIZE;
    }
    return target;
}

void hit_aimed_enemy(engine_t *engine, game_data_t *d)
{
    world_object_t obj = {0};
    enemy_t *target;
    bool is_critical = false;
    int damage;

    obj.size_mult = 1;
    target = find_enemy(engine, d, &obj, &is_critical);
    if (!target)
        return;
    damage = get_weapon_damage(d->weapons[d->active_weapon].type);
    if (is_critical)
        damage = (int) ((float) damage * 1.5F);
    damage_enemy(d, target, damage, engine->difficulty);
}
