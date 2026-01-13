#include <stdlib.h>
#include <time.h>
#include "food.h"

// 뱀의 특정 위치와 겹치는지 확인하는 헬퍼 함수
static bool is_position_in_snake(const Position* p_pos, const Snake* p_snake) {
    if (p_snake == NULL || p_snake->p_body == NULL) {
        return false;
    }

    for (uint32_t i = 0; i < p_snake->length; i++) {
        if (p_snake->p_body[i].x == p_pos->x && 
            p_snake->p_body[i].y == p_pos->y) {
            return true;
        }
    }

    return false;
}

void food_init(Food* p_food) {
    if (p_food == NULL) {
        return;
    }

    p_food->position.x = 0;
    p_food->position.y = 0;
    p_food->is_active = false;
}

bool food_spawn(Food* p_food, const Snake* p_snake) {
    if (p_food == NULL || p_snake == NULL) {
        return false;
    }

    // 랜덤 시드 초기화 (한 번만)
    static bool s_seed_initialized = false;
    if (!s_seed_initialized) {
        srand((unsigned int)time(NULL));
        s_seed_initialized = true;
    }

    // 최대 시도 횟수 (무한 루프 방지)
    const uint32_t max_attempts = GRID_WIDTH * GRID_HEIGHT * 2;
    uint32_t attempts = 0;

    Position new_pos;
    do {
        new_pos.x = rand() % GRID_WIDTH;
        new_pos.y = rand() % GRID_HEIGHT;
        attempts++;

        if (attempts >= max_attempts) {
            // 모든 위치를 시도했지만 실패한 경우
            return false;
        }
    } while (is_position_in_snake(&new_pos, p_snake));

    p_food->position = new_pos;
    p_food->is_active = true;

    return true;
}

Position food_get_position(const Food* p_food) {
    Position pos = {0, 0};
    if (p_food != NULL) {
        pos = p_food->position;
    }
    return pos;
}

bool food_is_active(const Food* p_food) {
    if (p_food == NULL) {
        return false;
    }
    return p_food->is_active;
}

void food_deactivate(Food* p_food) {
    if (p_food != NULL) {
        p_food->is_active = false;
    }
}

