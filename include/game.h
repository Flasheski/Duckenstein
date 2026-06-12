/*
** EPITECH PROJECT, 2026
** game.h
** File description:
** game scene
*/

#ifndef GAME_H
// clang-format off
    #define GAME_H

    #include <SFML/Audio/Types.h>
    #include <SFML/Graphics/Rect.h>
    #include <SFML/Graphics/Types.h>
    #include <SFML/System/Vector2.h>
    #include <stdlib.h>

    #include "graphics/engine.h"

    #include "weapons.h"
    #include "menu.h"

    #define MAP_HEIGHT 35
    #define MAP_WIDTH 43
    #define WALL_TEXTURE_WIDTH 64
    #define WALL_TEXTURE_HEIGHT 64
    #define WALL_TEXTURES_PATH SPRITES_PATH "wall_textures.png"
    #define NB_WALL_TEXTURES 9
    #define DOOR_TEXTURE_INDEX 9

    #define VIGNETTE_SHADER_PATH SHADERS_PATH "vignette.frag"
    #define VIGNETTE_INNER_RADIUS 0.85F
    #define VIGNETTE_OUTER_RADIUS 1.3F
    #define VIGNETTE_ALPHA 0.5F

    #define FLOOR_CEIL_SHADER_PATH SHADERS_PATH "floor_ceil.frag"
    #define FLOOR_TILE_INDEX 3
    #define CEIL_TILE_INDEX 6

    #define ENABLE_FLASHLIGHT true
    #define FLASHLIGHT_RANGE 2
    #define FLASHLIGHT_MIN_LIGHT 0

    #define WALL_HEIGHT_MULT 1

    #define MOVE_SPEED 5
    #define SPRINT_MULT 1.5F
    #define CROUCH_MULT 0.5F
    #define COLLISION_MARGIN 0.1F

    #define CAMERA_ROT_SPEED 1
    #define CAMERA_HEIGHT_SPEED 1000
    #define ZOOM_FACTOR 1.5F

    #define MOUSE_X_SENSITIVITY 0.002F
    #define MOUSE_Y_SENSITIVITY 2.0F

    #define DEFAULT_FOV 0.66F
    #define SPRINT_FOV 0.8F
    #define CROUCH_FOV 0.5F

    #define MINIMAP_TILE_SIZE 2
    #define MINIMAP_RATIO 3
    #define MINIMAP_VIEWPORT (sfFloatRect) {0.75F, 0.05F, 0.25F, 0.25F}
    #define MINIMAP_OPACITY 160

    #define TIMER_POS (sfVector2f) {50, 50}

    #define STEPS_SOUND SFXS_PATHS "player/steps.mp3"
    #define DOOR_OPEN_SOUND SFXS_PATHS "player/door_open.mp3"

    #define NORMAL_BOBBING 5000.0F

    #define CURSOR_PATH SPRITES_PATH "cursor.png"
    #define HUD_FONT_PATH FONTS_PATH "old_stamper.ttf"
    #define CURSOR_SIZE 48.0F
    #define CURSOR_WIDTH 20
    #define CURSOR_HEIGHT 20
    #define CURSOR_FRAME_COUNT 6
    #define CURSOR_ANIMATION_FPS 12.0F

    #define ENEMY_NORMAL_SPRITE SPRITES_PATH "enemies/duck.png"
    #define ENEMY_ELITE_SPRITE SPRITES_PATH "enemies/duck_epitech.png"
    #define ENEMY_BOSS_SPRITE SPRITES_PATH "enemies/duck_boss.png"
    #define MEDIKIT_SPRITE SPRITES_PATH "props/medikit.png"
    #define KEY_SPRITE SPRITES_PATH "props/key.png"
    #define ENEMY_HIT_SOUND SFXS_PATHS "enemies/hit.mp3"
    #define PLAYER_DEAD_SOUND SFXS_PATHS "player/dead.mp3"
    #define PLAYER_HEAL_SOUND SFXS_PATHS "player/heal.mp3"

    #define MAX_ENEMIES 32
    #define MAX_MEDIKITS MAX_ENEMIES
    #define MAX_KEYS MAX_ENEMIES
    #define PLAYER_MAX_HP 150
    #define MEDIKIT_HEAL 45
    #define MEDIKIT_COLLISION_RADIUS 0.55F
    #define MEDIKIT_SIZE_MULT 0.45F
    #define KEY_COLLISION_RADIUS 0.55F
    #define KEY_SIZE_MULT 0.35F
    #define KEY_WIDTH 31
    #define KEY_HEIGHT 37
    #define KEY_FRAME_COUNT 4
    #define KEY_ANIMATION_FPS 8.0F
    #define DOOR_OPEN_RADIUS 0.75F
    #define ENEMY_SHOOT_RANGE 25.0F
    #define ENEMY_HEAD_SIZE 0.3F
    #define HP_BAR_POS (sfVector2f) {50, 980}
    #define HP_BAR_SIZE (sfVector2f) {320, 30}
    #define HP_BAR_COLOR sfColor_fromRGB(45, 220, 70)

    #define GAME_VICTORY_PATH SFXS_PATHS "victory.mp3"
// clang-format on

typedef enum { ENEMY_NORMAL, ENEMY_ELITE, ENEMY_BOSS } enemy_type_t;

typedef struct {
    sfSound *steps;
    sfSound *dead_sound;
    sfSound *heal_sound;
    sfSound *door_open_sound;
    sfVector2f pos;
    sfVector2f view_dir;
    int hp;
    int max_hp;
    bool has_moved;
    bool is_sprinting;
    bool is_crouching;
    bool is_zooming;
    bool is_dead;
    int keys;
} player_t;

typedef struct hud_s {
    sfView *mini_map;
    sfRenderTexture *mini_map_render;
    sfCircleShape *mini_map_shape;
    sfVector2u mini_map_size;
    sfText *timer;
    float timer_time;
    sprite_anim_t *cursor;
    sfFont *hud_font;
    sfSprite *ammo_sprite;
    sfText *ammo_text;
    bool show_minimap;
    bool show_timer;
    sfRectangleShape *hp_background;
    sfRectangleShape *hp_bar;
    sfText *hp_text;
} hud_t;

typedef struct {
    sfVector2f pos;
    enemy_type_t type;
    int hp;
    int max_hp;
    int damage;
    float shoot_cooldown;
    float shoot_delay;
    bool alive;
    bool drops_key;
} enemy_t;

typedef struct {
    sfVector2f pos;
    bool active;
} medikit_t;

typedef struct {
    sfVector2f pos;
    bool active;
} key_item_t;

typedef struct {
    float transform_y;
    int screen_x;
    int start_x;
    int end_x;
    int start_y;
    int end_y;
    int width;
    int height;
    sfVector2u texture_size;
    float size_mult;
    sfIntRect rect;
} world_object_t;

typedef struct {
    int type;
    size_t index;
    float dist;
} world_item_t;

typedef struct {
    sfVector2f pos;
    sfFloatRect bounds;
    float scale;
} hud_layout_t;

typedef struct game_s {
    int map[MAP_WIDTH][MAP_HEIGHT];
    sfRenderTexture *render_texture;
    sfVertexArray *rays;
    sfRectangleShape *floor_ceil;
    sfShader *floor_ceil_shader;
    sfShader *vignette_shader;
    sfTexture *wall_textures;
    sfTexture *enemy_textures[3];
    sfTexture *medikit_texture;
    sprite_anim_t *key_anim;
    sfSound *enemy_hit_sound;
    float *depth_buffer;
    player_t player;
    enemy_t enemies[MAX_ENEMIES];
    size_t enemy_count;
    medikit_t medikits[MAX_MEDIKITS];
    size_t medikit_count;
    key_item_t keys[MAX_KEYS];
    size_t key_count;
    bool game_won;
    bool game_over;
    sfVector2f camera_plane;
    sfVector2f camera_plane_base;
    float camera_height;
    float fov;
    float target_fov;
    float bobbing_clock;
    hud_t *hud;
    weapon_t weapons[WEAPON_SLOT_COUNT];
    int active_weapon;
    float recoil_vel;
    bool is_paused;
    pause_menu_t pause;
    bool is_end;
} game_data_t;

typedef struct ray_s {
    float camera_plane_x;
    sfVector2f ray_dir;
    sfVector2i map_tile;
    sfVector2f side_distance;
    sfVector2f delta_distance;
    sfVector2i step_dir;
    float perpendicular_dist;
    bool was_wall_hit;
    bool was_x_side_hit;
    float camera_plane_length;
    int line_height;
    int horizon_y;
    int draw_start_y;
    int line_start;
    int line_end;
    float wall_hit_coord;
    int wall_texture_ind;
    float texture_x;
    float texture_y_top;
    float texture_y_bottom;
    sfColor tint;
} ray_t;

scene_t *game_create(void);
void game_enter(engine_t *engine);
void game_exit(engine_t *engine);
void game_draw(engine_t *engine);
void game_update(engine_t *engine);
void game_event(engine_t *engine, sfEvent *event);
void game_on_resize(engine_t *engine);

void switch_fullscreen(engine_t *engine);

void cast_wall_ray(engine_t *engine, game_data_t *d, size_t x);
void draw_floor_and_ceil(engine_t *engine, game_data_t *d);

void init_enemy_assets(engine_t *engine, game_data_t *d);
void add_enemy(game_data_t *d, enemy_type_t type, sfVector2i *pos,
    bool drops_key);
bool get_world_object_coords(engine_t *engine, game_data_t *d,
    const sfVector2f *pos, world_object_t *obj);
bool can_see_other_point(game_data_t *d, const sfVector2f *a,
    const sfVector2f *b);
void damage_player(game_data_t *d, int damage);
void damage_enemy(game_data_t *d, enemy_t *enemy, int damage,
    game_difficulty_t difficulty);
void update_enemies(engine_t *engine, game_data_t *d);
void update_medikits(game_data_t *d);
void update_keys(engine_t *engine, game_data_t *d);
void update_doors(game_data_t *d);
void hit_aimed_enemy(engine_t *engine, game_data_t *d);
void add_enemies_objects(game_data_t *d, world_item_t *items, size_t *count);
void add_medikits_objects(game_data_t *d, world_item_t *items, size_t *count);
void add_keys_objects(game_data_t *d, world_item_t *items, size_t *count);
void draw_objects_sprites(engine_t *engine, game_data_t *d);

int init_hud(engine_t *engine, game_data_t *data);
void free_hud(hud_t *hud);
void view_mini_map(engine_t *engine, game_data_t *d, hud_t *hud);
void draw_timer(engine_t *engine, hud_t *hud);
void draw_hp_bar(engine_t *engine, game_data_t *d);

void display_ending_screen(engine_t *engine);

#endif /* !GAME_H */
