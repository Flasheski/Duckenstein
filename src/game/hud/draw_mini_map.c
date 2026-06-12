/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** mini-map
*/

#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/View.h>
#include <math.h>

#include "graphics/engine.h"

#include "game.h"
#include "wolf3d.h"

static void set_cell_color(sfRectangleShape *cell, int tile)
{
    switch (tile) {
        case 0:
            sfRectangleShape_setFillColor(cell, sfTransparent);
            break;
        case DOOR_TEXTURE_INDEX:
            sfRectangleShape_setFillColor(cell, sfColor_fromRGB(244, 164, 96));
            break;
        default:
            sfRectangleShape_setFillColor(cell, sfWhite);
            break;
    }
}

static void draw_mm_player(hud_t *hud, game_data_t *d)
{
    sfCircleShape *player = sfCircleShape_create();
    float radius = 2 * MINIMAP_RATIO;
    float cell_size = MINIMAP_TILE_SIZE * MINIMAP_RATIO;

    sfCircleShape_setRadius(player, radius);
    sfCircleShape_setFillColor(player, sfRed);
    sfCircleShape_setPosition(player,
        (sfVector2f) {d->player.pos.x * cell_size - radius,
            d->player.pos.y * cell_size - radius});
    sfRenderTexture_drawCircleShape(hud->mini_map_render, player, nullptr);
    sfCircleShape_destroy(player);
}

static sfColor get_enemy_color(enemy_type_t type)
{
    if (type == ENEMY_BOSS)
        return sfColor_fromRGB(138, 43, 226);
    if (type == ENEMY_ELITE)
        return sfColor_fromRGB(0, 0, 139);
    return sfYellow;
}

static void draw_mm_enemies(hud_t *hud, game_data_t *d)
{
    sfCircleShape *enemy = sfCircleShape_create();
    float radius = 1.8F * MINIMAP_RATIO;
    float cell_size = MINIMAP_TILE_SIZE * MINIMAP_RATIO;

    if (!enemy)
        return;
    sfCircleShape_setRadius(enemy, radius);
    for (size_t i = 0; i < d->enemy_count; i++) {
        if (!d->enemies[i].alive)
            continue;
        sfCircleShape_setFillColor(enemy, get_enemy_color(d->enemies[i].type));
        sfCircleShape_setPosition(enemy,
            (sfVector2f) {d->enemies[i].pos.x * cell_size - radius,
                d->enemies[i].pos.y * cell_size - radius});
        sfRenderTexture_drawCircleShape(hud->mini_map_render, enemy, nullptr);
    }
    sfCircleShape_destroy(enemy);
}

static void draw_mm_medikits(hud_t *hud, game_data_t *d)
{
    sfCircleShape *medikit = sfCircleShape_create();
    float radius = 1.5F * MINIMAP_RATIO;
    float cell_size = MINIMAP_TILE_SIZE * MINIMAP_RATIO;

    if (!medikit)
        return;
    sfCircleShape_setRadius(medikit, radius);
    for (size_t i = 0; i < d->medikit_count; i++) {
        if (!d->medikits[i].active)
            continue;
        sfCircleShape_setFillColor(medikit, sfColor_fromRGB(0, 153, 51));
        sfCircleShape_setPosition(medikit,
            (sfVector2f) {d->medikits[i].pos.x * cell_size - radius,
                d->medikits[i].pos.y * cell_size - radius});
        sfRenderTexture_drawCircleShape(hud->mini_map_render, medikit,
            nullptr);
    }
    sfCircleShape_destroy(medikit);
}

static void draw_mm_keys(hud_t *hud, game_data_t *d)
{
    sfCircleShape *key = sfCircleShape_create();
    float radius = 1.5F * MINIMAP_RATIO;
    float cell_size = MINIMAP_TILE_SIZE * MINIMAP_RATIO;

    if (!key)
        return;
    sfCircleShape_setRadius(key, radius);
    sfCircleShape_setFillColor(key, sfColor_fromRGB(255, 140, 0));
    for (size_t i = 0; i < d->key_count; i++) {
        if (!d->keys[i].active)
            continue;
        sfCircleShape_setPosition(key,
            (sfVector2f) {d->keys[i].pos.x * cell_size - radius,
                d->keys[i].pos.y * cell_size - radius});
        sfRenderTexture_drawCircleShape(hud->mini_map_render, key, nullptr);
    }
    sfCircleShape_destroy(key);
}

static void draw_mini_map(hud_t *hud, game_data_t *d)
{
    sfRectangleShape *cell = sfRectangleShape_create();
    float cell_size = MINIMAP_TILE_SIZE * MINIMAP_RATIO;

    sfRectangleShape_setSize(cell, (sfVector2f) {cell_size, cell_size});
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            set_cell_color(cell, d->map[x][y]);
            sfRectangleShape_setPosition(cell,
                (sfVector2f) {(float) x * cell_size, (float) y * cell_size});
            sfRenderTexture_drawRectangleShape(hud->mini_map_render, cell,
                nullptr);
        }
    }
    sfRectangleShape_destroy(cell);
}

static void draw_minimap_texture(hud_t *hud, game_data_t *d, float angle)
{
    float cell_size = MINIMAP_TILE_SIZE * MINIMAP_RATIO;
    float map_w = MAP_WIDTH * cell_size;
    float map_h = MAP_HEIGHT * cell_size;
    float size = sqrtf(map_w * map_w + map_h * map_h);

    sfView_setCenter(hud->mini_map,
        (sfVector2f) {d->player.pos.x * cell_size,
            d->player.pos.y * cell_size});
    sfView_setSize(hud->mini_map, (sfVector2f) {size, size});
    sfView_setRotation(hud->mini_map, angle + 90);
    sfView_zoom(hud->mini_map, 1 / (float) MINIMAP_TILE_SIZE);
    sfRenderTexture_setView(hud->mini_map_render, hud->mini_map);
    sfRenderTexture_clear(hud->mini_map_render,
        sfColor_fromRGBA(0, 0, 0, MINIMAP_OPACITY));
    draw_mini_map(hud, d);
    draw_mm_enemies(hud, d);
    draw_mm_medikits(hud, d);
    draw_mm_keys(hud, d);
    draw_mm_player(hud, d);
    sfRenderTexture_display(hud->mini_map_render);
}

static sfVector2f get_minimap_pos(engine_t *engine, hud_t *hud,
    const sfFloatRect *viewport)
{
    float width = (float) engine->window_size.x * viewport->width;
    float height = (float) engine->window_size.y * viewport->height;
    float left = (float) engine->window_size.x * viewport->left;
    float top = (float) engine->window_size.y * viewport->top;

    return (sfVector2f) {left + (width - (float) hud->mini_map_size.x) / 2,
        top + (height - (float) hud->mini_map_size.y) / 2};
}

void view_mini_map(engine_t *engine, game_data_t *d, hud_t *hud)
{
    sfFloatRect viewport = MINIMAP_VIEWPORT;
    float angle = atan2f(d->player.view_dir.y, d->player.view_dir.x) * 180
        / (float) M_PI;
    sfVector2f minimap_pos;

    draw_minimap_texture(hud, d, angle);
    minimap_pos = get_minimap_pos(engine, hud, &viewport);
    minimap_pos.x += (float) hud->mini_map_size.x;
    sfCircleShape_setPosition(hud->mini_map_shape, minimap_pos);
    sfCircleShape_setTexture(hud->mini_map_shape,
        sfRenderTexture_getTexture(hud->mini_map_render), true);
    sfRenderWindow_drawCircleShape(engine->window, hud->mini_map_shape,
        nullptr);
}
