/*
** EPITECH PROJECT, 2026
** enemies_init.c
** File description:
** Init enemy
*/

#include <SFML/Audio/Sound.h>
#include <SFML/Graphics/Texture.h>
#include <SFML/System/Vector2.h>

#include "graphics/engine.h"
#include "graphics/resources.h"

#include "enemies.h"
#include "game.h"

static int get_enemy_hp(enemy_type_t type)
{
    if (type == ENEMY_BOSS)
        return ENEMY_BOSS_HP;
    if (type == ENEMY_ELITE)
        return ENEMY_ELITE_HP;
    return ENEMY_NORMAL_HP;
}

static int get_enemy_damage(enemy_type_t type)
{
    if (type == ENEMY_BOSS)
        return ENEMY_BOSS_DAMAGE;
    if (type == ENEMY_ELITE)
        return ENEMY_ELITE_DAMAGE;
    return ENEMY_NORMAL_DAMAGE;
}

static float get_enemy_delay(enemy_type_t type)
{
    if (type == ENEMY_BOSS)
        return ENEMY_BOSS_SHOOT_DELAY;
    if (type == ENEMY_ELITE)
        return ENEMY_ELITE_SHOOT_DELAY;
    return ENEMY_NORMAL_SHOOT_DELAY;
}

void init_enemy_assets(engine_t *engine, game_data_t *d)
{
    d->enemy_textures[ENEMY_NORMAL] =
        resources_load_texture(engine->resources, ENEMY_NORMAL_SPRITE);
    d->enemy_textures[ENEMY_ELITE] =
        resources_load_texture(engine->resources, ENEMY_ELITE_SPRITE);
    d->enemy_textures[ENEMY_BOSS] =
        resources_load_texture(engine->resources, ENEMY_BOSS_SPRITE);
    d->medikit_texture =
        resources_load_texture(engine->resources, MEDIKIT_SPRITE);
    d->key_anim = sprite_anim_create(KEY_SPRITE,
        &(sprite_frame_info_t) {{0, 0, KEY_WIDTH, KEY_HEIGHT}, KEY_FRAME_COUNT,
            KEY_ANIMATION_FPS},
        engine);
    d->enemy_hit_sound =
        resources_load_sound(engine->resources, ENEMY_HIT_SOUND);
}

void add_enemy(game_data_t *d, enemy_type_t type, sfVector2i *pos,
    bool drops_key)
{
    enemy_t *enemy;

    if (d->enemy_count >= MAX_ENEMIES)
        return;
    enemy = &d->enemies[d->enemy_count];
    enemy->pos = (sfVector2f) {(float) pos->x + 0.5F, (float) pos->y + 0.5F};
    enemy->type = type;
    enemy->max_hp = get_enemy_hp(type);
    enemy->hp = enemy->max_hp;
    enemy->damage = get_enemy_damage(type);
    enemy->shoot_delay = get_enemy_delay(type);
    enemy->shoot_cooldown = 0;
    enemy->alive = true;
    enemy->drops_key = drops_key;
    d->enemy_count++;
}
