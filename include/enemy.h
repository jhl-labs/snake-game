/**
 * @file enemy.h
 * @brief 적 뱀 모델 헤더
 *
 * 적 뱀의 데이터 구조와 동작을 정의합니다.
 * 기존 Snake 구조체를 재사용하여 구현합니다.
 */

#ifndef SNAKE_GAME_ENEMY_H
#define SNAKE_GAME_ENEMY_H

#include <stdbool.h>
#include <stdint.h>
#include "snake.h"
#include "game_constants.h"

// 적 뱀 구조체
typedef struct {
    Snake snake;           // 적 뱀 (기존 Snake 재사용)
    uint32_t move_delay;   // 이동 지연 (프레임 단위)
    uint32_t move_counter; // 이동 카운터
    bool is_active;        // 활성화 여부
} Enemy;

/**
 * 적 뱀 객체를 초기화합니다.
 * 
 * @param p_enemy 적 뱀 객체 포인터
 * @param p_player_snake 플레이어 뱀 포인터 (겹치지 않는 위치 선택용)
 * @return 초기화 성공 여부
 */
bool enemy_init(Enemy* p_enemy, const Snake* p_player_snake);

/**
 * 적 뱀 객체를 해제합니다.
 * 
 * @param p_enemy 적 뱀 객체 포인터
 */
void enemy_destroy(Enemy* p_enemy);

/**
 * 적 뱀을 업데이트합니다 (랜덤 이동).
 * 
 * @param p_enemy 적 뱀 객체 포인터
 */
void enemy_update(Enemy* p_enemy);

/**
 * 적 뱀을 리스폰합니다.
 * 
 * @param p_enemy 적 뱀 객체 포인터
 * @param p_player_snake 플레이어 뱀 포인터 (겹치지 않는 위치 선택용)
 * @return 리스폰 성공 여부
 */
bool enemy_respawn(Enemy* p_enemy, const Snake* p_player_snake);

/**
 * 적 뱀의 Snake 포인터를 반환합니다 (읽기 전용).
 * 
 * @param p_enemy 적 뱀 객체 포인터
 * @return 내부 Snake 포인터 (const)
 */
const Snake* enemy_get_snake(const Enemy* p_enemy);

/**
 * 적 뱀이 활성화 상태인지 확인합니다.
 * 
 * @param p_enemy 적 뱀 객체 포인터
 * @return 활성화 여부
 */
bool enemy_is_active(const Enemy* p_enemy);

/**
 * 플레이어 뱀과 적 뱀의 충돌을 검사합니다.
 * 
 * @param p_enemy 적 뱀 객체 포인터
 * @param p_player_snake 플레이어 뱀 포인터
 * @return 충돌 여부 (플레이어 머리가 적 뱀 몸통과 충돌)
 */
bool enemy_check_collision_with_player(const Enemy* p_enemy, const Snake* p_player_snake);

#endif // SNAKE_GAME_ENEMY_H
