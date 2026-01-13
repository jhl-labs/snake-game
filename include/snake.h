#ifndef SNAKE_GAME_SNAKE_H
#define SNAKE_GAME_SNAKE_H

#include <stdbool.h>
#include <stdint.h>
#include "game_constants.h"

// 위치 구조체
typedef struct {
    int32_t x;
    int32_t y;
} Position;

// 뱀 구조체
typedef struct {
    Position* p_body;        // 뱀의 몸통 위치 배열
    uint32_t length;        // 현재 길이
    uint32_t capacity;       // 배열 용량
    Direction direction;     // 현재 이동 방향
    Direction next_direction; // 다음 이동 방향
} Snake;

/**
 * 뱀 객체를 초기화합니다.
 * 
 * @param p_snake 뱀 객체 포인터
 * @param start_x 시작 X 좌표
 * @param start_y 시작 Y 좌표
 * @return 초기화 성공 여부
 */
bool snake_init(Snake* p_snake, int32_t start_x, int32_t start_y);

/**
 * 뱀 객체를 해제합니다.
 * 
 * @param p_snake 뱀 객체 포인터
 */
void snake_destroy(Snake* p_snake);

/**
 * 뱀의 이동 방향을 설정합니다.
 * 반대 방향으로는 이동할 수 없습니다.
 * 
 * @param p_snake 뱀 객체 포인터
 * @param direction 새로운 이동 방향
 */
void snake_set_direction(Snake* p_snake, Direction direction);

/**
 * 뱀을 한 칸 이동시킵니다.
 * 
 * @param p_snake 뱀 객체 포인터
 */
void snake_move(Snake* p_snake);

/**
 * 뱀의 길이를 증가시킵니다 (음식을 먹었을 때).
 * 
 * @param p_snake 뱀 객체 포인터
 */
void snake_grow(Snake* p_snake);

/**
 * 뱀의 머리 위치를 반환합니다.
 * 
 * @param p_snake 뱀 객체 포인터
 * @return 머리 위치
 */
Position snake_get_head(const Snake* p_snake);

/**
 * 뱀의 특정 인덱스 위치를 반환합니다.
 * 
 * @param p_snake 뱀 객체 포인터
 * @param index 인덱스 (0이 머리)
 * @return 위치
 */
Position snake_get_position_at(const Snake* p_snake, uint32_t index);

/**
 * 뱀이 자신의 몸과 충돌하는지 확인합니다.
 * 
 * @param p_snake 뱀 객체 포인터
 * @return 충돌 여부
 */
bool snake_check_self_collision(const Snake* p_snake);

/**
 * 뱀이 그리드 경계를 벗어났는지 확인합니다.
 * 
 * @param p_snake 뱀 객체 포인터
 * @return 경계 벗어남 여부
 */
bool snake_check_boundary_collision(const Snake* p_snake);

#endif // SNAKE_GAME_SNAKE_H

