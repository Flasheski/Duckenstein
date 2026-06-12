/*
** EPITECH PROJECT, 2026
** draw_objects.c
** File description:
** Draw objects
*/

#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/Types.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include "enemies.h"
#include "game.h"

static bool get_object_transform(game_data_t *d, const sfVector2f *pos,
    float *transform_x, world_object_t *obj)
{
    sfVector2f sprite = {pos->x - d->player.pos.x, pos->y - d->player.pos.y};
    float det = d->camera_plane.x * d->player.view_dir.y
        - d->player.view_dir.x * d->camera_plane.y;
    float inv_det;

    if (fabsf(det) < 0.0001F)
        return false;
    inv_det = 1 / det;
    *transform_x = inv_det
        * (d->player.view_dir.y * sprite.x - d->player.view_dir.x * sprite.y);
    obj->transform_y = inv_det
        * (-d->camera_plane.y * sprite.x + d->camera_plane.x * sprite.y);
    return obj->transform_y > OBJECT_MIN_SIZE_TO_SHOW;
}

static bool get_object_size(engine_t *engine, game_data_t *d,
    world_object_t *obj, float transform_x)
{
    float camera_plane_len = hypotf(d->camera_plane.x, d->camera_plane.y);
    float object_height = fabsf((float) engine->window_size.x
        / (2 * camera_plane_len * obj->transform_y));

    obj->screen_x = (int) ((float) engine->window_size.x / 2
        * (1 + transform_x / obj->transform_y));
    obj->height = (int) (object_height * obj->size_mult);
    obj->width = (int) ((float) obj->height * (float) obj->texture_size.x
        / (float) obj->texture_size.y);
    return obj->width > 0 && obj->height > 0;
}

bool get_world_object_coords(engine_t *engine, game_data_t *d,
    const sfVector2f *pos, world_object_t *obj)
{
    int horizon_y;
    float transform_x;

    if (!get_object_transform(d, pos, &transform_x, obj))
        return false;
    if (!get_object_size(engine, d, obj, transform_x))
        return false;
    horizon_y = (int) engine->window_size.y / 2 + (int) d->camera_height;
    obj->start_y = -obj->height / 2 + horizon_y;
    obj->end_y = obj->height / 2 + horizon_y;
    obj->start_x = -obj->width / 2 + obj->screen_x;
    obj->end_x = obj->width / 2 + obj->screen_x;
    return true;
}

static void add_sprite_col(sfVertexArray *vertices, world_object_t *obj, int i,
    int texture_x)
{
    float x = (float) i + 0.5F;
    float texture_coord_x = (float) obj->rect.left + (float) texture_x
        + (float) obj->texture_size.x / (float) obj->width * 0.5F;
    float texture_top = (float) obj->rect.top;
    float texture_bottom = (float) obj->rect.top + (float) obj->rect.height;

    sfVertexArray_append(vertices,
        (sfVertex) {{x, (float) obj->start_y}, sfWhite,
            {texture_coord_x, texture_top}});
    sfVertexArray_append(vertices,
        (sfVertex) {{x, (float) obj->end_y}, sfWhite,
            {texture_coord_x, texture_bottom}});
}

static void draw_object(engine_t *engine, game_data_t *d, world_object_t *obj,
    sfTexture *texture)
{
    sfVertexArray *vertices = sfVertexArray_create();
    sfRenderStates states = sfRenderStates_default();
    int draw_start = obj->start_x < 0 ? 0 : obj->start_x;
    int draw_end = obj->end_x > (int) engine->window_size.x
        ? (int) engine->window_size.x
        : obj->end_x;
    int texture_x;

    if (!vertices)
        return;
    sfVertexArray_setPrimitiveType(vertices, sfLines);
    for (int i = draw_start; i < draw_end; i++) {
        texture_x = (int) ((float) (i - obj->start_x)
            * (float) obj->texture_size.x / (float) obj->width);
        if (obj->transform_y > 0 && d->depth_buffer[i] > obj->transform_y)
            add_sprite_col(vertices, obj, i, texture_x);
    }
    states.texture = texture;
    sfRenderTexture_drawVertexArray(d->render_texture, vertices, &states);
    sfVertexArray_destroy(vertices);
}

static void setup_up_rect_and_texture_size(world_object_t *obj,
    const world_item_t *item, sfTexture *texture)
{
    if (item->type != OBJECT_KEY) {
        obj->texture_size = sfTexture_getSize(texture);
        obj->rect = (sfIntRect) {0, 0, (int) obj->texture_size.x,
            (int) obj->texture_size.y};
    } else
        obj->texture_size = (sfVector2u) {(unsigned int) obj->rect.width,
            (unsigned int) obj->rect.height};
}

static float get_size_mult(const world_item_t *item)
{
    if (item->type == OBJECT_ENEMY)
        return 1;
    if (item->type == OBJECT_MEDIKIT)
        return MEDIKIT_SIZE_MULT;
    return KEY_SIZE_MULT;
}

static void draw_item(engine_t *engine, game_data_t *d,
    const world_item_t *item)
{
    world_object_t obj = {0};
    sfTexture *texture = nullptr;
    sfVector2f *pos = nullptr;

    obj.size_mult = get_size_mult(item);
    if (item->type == OBJECT_ENEMY) {
        texture = d->enemy_textures[d->enemies[item->index].type];
        pos = &d->enemies[item->index].pos;
    } else if (item->type == OBJECT_MEDIKIT) {
        texture = d->medikit_texture;
        pos = &d->medikits[item->index].pos;
    } else {
        texture = d->key_anim->texture;
        pos = &d->keys[item->index].pos;
        obj.rect = d->key_anim->frame_info.frame_rect;
    }
    setup_up_rect_and_texture_size(&obj, item, texture);
    if (!get_world_object_coords(engine, d, pos, &obj))
        return;
    draw_object(engine, d, &obj, texture);
}

static int compare_objects(const void *a, const void *b)
{
    const world_item_t *wa = a;
    const world_item_t *wb = b;

    if (wa->dist < wb->dist)
        return 1;
    if (wa->dist > wb->dist)
        return -1;
    return 0;
}

void draw_objects_sprites(engine_t *engine, game_data_t *d)
{
    world_item_t items[MAX_ENEMIES + MAX_MEDIKITS + MAX_KEYS];
    size_t count = 0;

    add_enemies_objects(d, items, &count);
    add_medikits_objects(d, items, &count);
    add_keys_objects(d, items, &count);
    qsort(items, count, sizeof(*items), compare_objects);
    for (size_t i = 0; i < count; i++)
        draw_item(engine, d, &items[i]);
}
