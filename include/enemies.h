/*
** EPITECH PROJECT, 2026
** enemies.h
** File description:
** enemy hp, damage, ...
*/

#ifndef ENEMIES_H
// clang-format off
    #define ENEMIES_H

    #define ENEMY_NORMAL_HP 55
    #define ENEMY_ELITE_HP 110
    #define ENEMY_BOSS_HP 300

    #define ENEMY_NORMAL_DAMAGE 7
    #define ENEMY_ELITE_DAMAGE 12
    #define ENEMY_BOSS_DAMAGE 20

    #define ENEMY_NORMAL_SHOOT_DELAY 1.8F
    #define ENEMY_ELITE_SHOOT_DELAY 1.25F
    #define ENEMY_BOSS_SHOOT_DELAY 1.15F

    #define ENEMY_TARGET_DDA_STEP 0.05F
    #define OBJECT_MIN_SIZE_TO_SHOW 0.05F

    #define OBJECT_ENEMY 0
    #define OBJECT_MEDIKIT 1
    #define OBJECT_KEY 2

    #define MEDIKIT_DROP_NORMAL 45
    #define MEDIKIT_DROP_ELITE 65
    #define MEDIKIT_DROP_BOSS 100
// clang-format on

#endif /* !ENEMIES_H */
