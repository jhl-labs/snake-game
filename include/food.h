#ifndef SNAKE_GAME_FOOD_H
#define SNAKE_GAME_FOOD_H

#include <stdbool.h>
#include <stdint.h>
#include "snake.h"

// 음식 구조체
typedef struct {
    Position position;
    bool is_active;
} Food;

/**
 * 음식 객체를 초기화합니다.
 * 
 * @param p_food 음식 객체 포인터
 */
void food_init(Food* p_food);

/**
 * 음식을 랜덤한 위치에 생성합니다.
 * 뱀의 몸과 겹치지 않는 위치를 선택합니다.
 * 
 * @param p_food 음식 객체 포인터
 * @param p_snake 뱀 객체 포인터 (겹침 체크용)
 * @return 생성 성공 여부
 */
bool food_spawn(Food* p_food, const Snake* p_snake);

/**
 * 음식의 위치를 반환합니다.
 * 
 * @param p_food 음식 객체 포인터
 * @return 음식 위치
 */
Position food_get_position(const Food* p_food);

/**
 * 음식이 활성화되어 있는지 확인합니다.
 * 
 * @param p_food 음식 객체 포인터
 * @return 활성화 여부
 */
bool food_is_active(const Food* p_food);

/**
 * 음식을 비활성화합니다.
 *
 * @param p_food 음식 객체 포인터
 */
void food_deactivate(Food* p_food);

/**
 * 음식을 랜덤한 위치에 생성합니다 (2인용 모드).
 * 두 뱀의 몸과 겹치지 않는 위치를 선택합니다.
 *
 * @param p_food 음식 객체 포인터
 * @param p_snake1 P1 뱀 객체 포인터
 * @param p_snake2 P2 뱀 객체 포인터
 * @return 생성 성공 여부
 */
bool food_spawn_versus(Food* p_food, const Snake* p_snake1, const Snake* p_snake2);

#endif // SNAKE_GAME_FOOD_H

