/*
** EPITECH PROJECT, 2026
** game_enter.c
** File description:
** Initialize the game scene
*/

#include <SFML/Audio/Sound.h>
#include <SFML/Graphics/Glsl.h>
#include <SFML/Graphics/PrimitiveType.h>
#include <SFML/Graphics/RectangleShape.h>
#include <SFML/Graphics/RenderTexture.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/Shader.h>
#include <SFML/Graphics/Types.h>
#include <SFML/Graphics/VertexArray.h>
#include <SFML/System/Vector2.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "graphics/engine.h"
#include "graphics/resources.h"

#include "game.h"
#include "menu.h"
#include "weapons.h"

static constexpr char WORLD_MAP[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "1111111111111111111111111111111111111111",
    "100000000000000000000000000000000020e001",
    "1000000000003555555555555500000000222001",
    "100000b000003000500000000000500d00200001",
    "10000000000030E0400000000000500000203001",
    "1333333333993000400000000000500000003331",
    "1000600000000000400000000000500000003331",
    "1000600000444444400000005555555555993001",
    "10006000000000000000000060e0000000000001",
    "1000666666666660000000666000000e00000001",
    "1000000000000000000000006000000000000001",
    "10000044444444444000005060000000000d0001",
    "1000004000000000000000506000000000000001",
    "1000004000040000000000506000777770000001",
    "1000004000040000000000506000000070007771",
    "1000004444440000000000606000600070000001",
    "100000000004000444444460666660007000E001",
    "1000000000000000000000600000000070000001",
    "1888998888888888888888888888888888888881",
    "1000000000000000000000000000000000000001",
    "1000003333000000000000000000020000E00001",
    "1000000000000000010000000000022222222221",
    "13300330000000000100d0000000000000000001",
    "1330333333300000011111111000000000000001",
    "13003300e0300000010000000002220022288881",
    "1300330000300000010000000002220022200001",
    "1003330000300000000000000000000000000d01",
    "10000000d0300000000000000002220022200001",
    "11333333333000000P000000000222002220e001",
    "1000000000000000000000000000000000000001",
    "1777777777777777777777777777777777777771",
};

static void set_up_floor_ceil_shader(engine_t *engine, game_data_t *data,
    sfShader *s)
{
    sfGlslVec2 textures_size = {NB_WALL_TEXTURES * WALL_TEXTURE_WIDTH,
        WALL_TEXTURE_HEIGHT};

    sfShader_setTextureUniform(s, "u_textures", data->wall_textures);
    sfShader_setVec2Uniform(s, "u_resolution",
        (sfGlslVec2) {(float) engine->window_size.x,
            (float) engine->window_size.y});
    sfShader_setVec2Uniform(s, "u_textures_size", textures_size);
    sfShader_setFloatUniform(s, "u_tile_size", (float) WALL_TEXTURE_WIDTH);
    sfShader_setFloatUniform(s, "u_floor_index", FLOOR_TILE_INDEX);
    sfShader_setFloatUniform(s, "u_ceil_index", CEIL_TILE_INDEX);
    sfShader_setFloatUniform(s, "u_flashlight_enabled", ENABLE_FLASHLIGHT);
    sfShader_setFloatUniform(s, "u_flashlight_range", FLASHLIGHT_RANGE);
    sfShader_setFloatUniform(s, "u_flashlight_min_light",
        FLASHLIGHT_MIN_LIGHT);
}

static void set_up_floor_ceil(engine_t *engine, game_data_t *data)
{
    data->floor_ceil_shader =
        sfShader_createFromFile(nullptr, nullptr, FLOOR_CEIL_SHADER_PATH);
    data->floor_ceil = sfRectangleShape_create();
    sfRectangleShape_setSize(data->floor_ceil,
        (sfVector2f) {(float) engine->window_size.x,
            (float) engine->window_size.y});
    set_up_floor_ceil_shader(engine, data, data->floor_ceil_shader);
}

static void init_vignette_shader(engine_t *engine, game_data_t *data)
{
    data->vignette_shader =
        sfShader_createFromFile(nullptr, nullptr, VIGNETTE_SHADER_PATH);
    sfShader_setFloatUniform(data->vignette_shader, "u_inner_radius",
        VIGNETTE_INNER_RADIUS);
    sfShader_setFloatUniform(data->vignette_shader, "u_outer_radius",
        VIGNETTE_OUTER_RADIUS);
    sfShader_setFloatUniform(data->vignette_shader, "u_alpha", VIGNETTE_ALPHA);
    sfShader_setVec2Uniform(data->vignette_shader, "u_resolution",
        (sfGlslVec2) {(float) engine->window_size.x,
            (float) engine->window_size.y});
}

static void init_player(engine_t *engine, game_data_t *data)
{
    data->player.view_dir = (sfVector2f) {-1, 0};
    data->camera_plane = (sfVector2f) {0, DEFAULT_FOV};
    data->camera_plane_base = (sfVector2f) {0, 1.0F};
    data->fov = DEFAULT_FOV;
    data->target_fov = DEFAULT_FOV;
    data->player.max_hp = PLAYER_MAX_HP;
    data->player.hp = PLAYER_MAX_HP;
    data->player.steps = resources_load_sound(engine->resources, STEPS_SOUND);
    data->player.dead_sound =
        resources_load_sound(engine->resources, PLAYER_DEAD_SOUND);
    data->player.heal_sound =
        resources_load_sound(engine->resources, PLAYER_HEAL_SOUND);
    data->player.door_open_sound =
        resources_load_sound(engine->resources, DOOR_OPEN_SOUND);
    sfSound_setLoop(data->player.steps, true);
}

static void init_rendering(engine_t *engine, game_data_t *data)
{
    data->rays = sfVertexArray_create();
    sfVertexArray_resize(data->rays, (size_t) engine->window_size.x * 2);
    sfVertexArray_setPrimitiveType(data->rays, sfLines);
    data->wall_textures =
        resources_load_texture(engine->resources, WALL_TEXTURES_PATH);
    data->render_texture = sfRenderTexture_create(engine->window_size.x,
        engine->window_size.y, false);
    data->depth_buffer =
        calloc((size_t) engine->window_size.x + 1, sizeof(float));
    init_enemy_assets(engine, data);
    set_up_floor_ceil(engine, data);
    init_vignette_shader(engine, data);
}

static void add_special_char(game_data_t *data, char tile, int x, int y)
{
    sfVector2i pos = {x, y};

    if (tile == 'd')
        add_enemy(data, ENEMY_NORMAL, &pos, false);
    if (tile == 'D')
        add_enemy(data, ENEMY_NORMAL, &pos, true);
    if (tile == 'e')
        add_enemy(data, ENEMY_ELITE, &pos, false);
    if (tile == 'E')
        add_enemy(data, ENEMY_ELITE, &pos, true);
    if (tile == 'b')
        add_enemy(data, ENEMY_BOSS, &pos, false);
    if (tile == 'P')
        data->player.pos = (sfVector2f) {(float) x, (float) y};
}

static void parse_map(game_data_t *data)
{
    char tile = '\0';

    for (size_t y = 0; y < MAP_HEIGHT; y++) {
        for (size_t x = 0; x < MAP_WIDTH; x++) {
            tile = WORLD_MAP[y][x];
            data->map[x][y] = (tile >= '1' && tile <= '9') ? tile - '0' : 0;
            add_special_char(data, tile, (int) x, (int) y);
        }
    }
}

static weapon_type_t get_difficulty_weapon(game_difficulty_t difficulty)
{
    if (difficulty == GAME_DIFFICULTY_MEDIUM)
        return WEAPON_REVOLVER;
    if (difficulty == GAME_DIFFICULTY_HARD)
        return WEAPON_BAZOOKA;
    return WEAPON_AK;
}

static void setup_mouse_and_pause(engine_t *engine, game_data_t *data)
{
    sfRenderWindow_setMouseCursorVisible(engine->window, false);
    sfMouse_setPositionRenderWindow(
        (sfVector2i) {(int) engine->window_size.x / 2,
            (int) engine->window_size.y / 2},
        engine->window);
    data->is_paused = false;
    data->is_end = false;
    pause_init(engine, data);
}

void game_enter(engine_t *engine)
{
    game_data_t *data = (game_data_t *) engine->scene->data;
    weapon_type_t weapon = get_difficulty_weapon(engine->difficulty);

    if (!data)
        return;
    srand(time(nullptr));
    parse_map(data);
    init_player(engine, data);
    init_rendering(engine, data);
    setup_mouse_and_pause(engine, data);
    if (init_hud(engine, data) == ERROR)
        return;
    if (init_weapons(engine, data, weapon) == ERROR)
        return;
    settings_set_sounds_enabled(engine, engine->sounds_enabled);
}
