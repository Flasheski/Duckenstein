/*
** EPITECH PROJECT, 2026
** wolf3d
** File description:
** free_hud
*/

#include "game.h"
#include "graphics/sprite_anim.h"

static void free_shapes(hud_t *hud)
{
    if (hud->mini_map_shape)
        sfCircleShape_destroy(hud->mini_map_shape);
    if (hud->mini_map_render)
        sfRenderTexture_destroy(hud->mini_map_render);
    if (hud->hp_background)
        sfRectangleShape_destroy(hud->hp_background);
    if (hud->hp_bar)
        sfRectangleShape_destroy(hud->hp_bar);
}

static void free_texts(hud_t *hud)
{
    if (hud->timer)
        sfText_destroy(hud->timer);
    if (hud->ammo_text)
        sfText_destroy(hud->ammo_text);
    if (hud->hp_text)
        sfText_destroy(hud->hp_text);
}

void free_hud(hud_t *hud)
{
    if (!hud)
        return;
    free_shapes(hud);
    free_texts(hud);
    if (hud->mini_map)
        sfView_destroy(hud->mini_map);
    if (hud->cursor)
        sprite_anim_destroy(hud->cursor);
    if (hud->ammo_sprite)
        sfSprite_destroy(hud->ammo_sprite);
    free(hud);
}
