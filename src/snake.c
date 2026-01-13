#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "snake.h"

bool snake_init(Snake* p_snake, int32_t start_x, int32_t start_y) {
    if (p_snake == NULL) {
        return false;
    }

    // 초기 용량 설정
    p_snake->capacity = INITIAL_SNAKE_LENGTH + 10; // 여유 공간 확보
    p_snake->p_body = (Position*)malloc(sizeof(Position) * p_snake->capacity);
    
    if (p_snake->p_body == NULL) {
        return false;
    }

    // 초기 위치 설정 (수평으로 배치)
    p_snake->length = INITIAL_SNAKE_LENGTH;
    for (uint32_t i = 0; i < p_snake->length; i++) {
        p_snake->p_body[i].x = start_x - (int32_t)i;
        p_snake->p_body[i].y = start_y;
    }

    p_snake->direction = DIRECTION_RIGHT;
    p_snake->next_direction = DIRECTION_RIGHT;

    return true;
}

void snake_destroy(Snake* p_snake) {
    if (p_snake != NULL && p_snake->p_body != NULL) {
        free(p_snake->p_body);
        p_snake->p_body = NULL;
        p_snake->length = 0;
        p_snake->capacity = 0;
    }
}

void snake_set_direction(Snake* p_snake, Direction direction) {
    if (p_snake == NULL || direction == DIRECTION_NONE) {
        return;
    }

    // 반대 방향으로는 이동할 수 없음
    if ((p_snake->direction == DIRECTION_UP && direction == DIRECTION_DOWN) ||
        (p_snake->direction == DIRECTION_DOWN && direction == DIRECTION_UP) ||
        (p_snake->direction == DIRECTION_LEFT && direction == DIRECTION_RIGHT) ||
        (p_snake->direction == DIRECTION_RIGHT && direction == DIRECTION_LEFT)) {
        return;
    }

    // 같은 방향이면 무시
    if (p_snake->direction == direction) {
        return;
    }

    p_snake->next_direction = direction;
}

void snake_move(Snake* p_snake) {
    if (p_snake == NULL || p_snake->p_body == NULL || p_snake->length == 0) {
        return;
    }

    // 방향 업데이트
    p_snake->direction = p_snake->next_direction;

    // 모든 몸통을 한 칸씩 뒤로 이동
    for (uint32_t i = p_snake->length - 1; i > 0; i--) {
        p_snake->p_body[i] = p_snake->p_body[i - 1];
    }

    // 머리를 새로운 방향으로 이동
    Position* p_head = &p_snake->p_body[0];
    switch (p_snake->direction) {
        case DIRECTION_UP:
            p_head->y--;
            break;
        case DIRECTION_DOWN:
            p_head->y++;
            break;
        case DIRECTION_LEFT:
            p_head->x--;
            break;
        case DIRECTION_RIGHT:
            p_head->x++;
            break;
        case DIRECTION_NONE:
            break;
    }
}

void snake_grow(Snake* p_snake) {
    if (p_snake == NULL || p_snake->p_body == NULL) {
        return;
    }

    // 최대 길이 체크
    if (p_snake->length >= MAX_SNAKE_LENGTH) {
        return;
    }

    // 용량이 부족하면 재할당
    if (p_snake->length >= p_snake->capacity) {
        uint32_t new_capacity = p_snake->capacity * 2;
        if (new_capacity > MAX_SNAKE_LENGTH) {
            new_capacity = MAX_SNAKE_LENGTH;
        }
        
        Position* p_new_body = (Position*)realloc(p_snake->p_body, 
                                                   sizeof(Position) * new_capacity);
        if (p_new_body == NULL) {
            return; // 메모리 할당 실패
        }
        p_snake->p_body = p_new_body;
        p_snake->capacity = new_capacity;
    }

    // 꼬리 위치에 새 몸통 추가
    Position tail = p_snake->p_body[p_snake->length - 1];
    p_snake->p_body[p_snake->length] = tail;
    p_snake->length++;
}

Position snake_get_head(const Snake* p_snake) {
    Position head = {0, 0};
    if (p_snake != NULL && p_snake->p_body != NULL && p_snake->length > 0) {
        head = p_snake->p_body[0];
    }
    return head;
}

Position snake_get_position_at(const Snake* p_snake, uint32_t index) {
    Position pos = {0, 0};
    if (p_snake != NULL && p_snake->p_body != NULL && index < p_snake->length) {
        pos = p_snake->p_body[index];
    }
    return pos;
}

bool snake_check_self_collision(const Snake* p_snake) {
    if (p_snake == NULL || p_snake->p_body == NULL || p_snake->length < 4) {
        return false; // 길이가 4 미만이면 자기 자신과 충돌 불가능
    }

    Position head = snake_get_head(p_snake);

    // 머리가 몸통의 어느 부분과도 겹치는지 확인
    for (uint32_t i = 1; i < p_snake->length; i++) {
        if (p_snake->p_body[i].x == head.x && p_snake->p_body[i].y == head.y) {
            return true;
        }
    }

    return false;
}

bool snake_check_boundary_collision(const Snake* p_snake) {
    if (p_snake == NULL || p_snake->p_body == NULL || p_snake->length == 0) {
        return false;
    }

    Position head = snake_get_head(p_snake);

    return (head.x < 0 || head.x >= GRID_WIDTH ||
            head.y < 0 || head.y >= GRID_HEIGHT);
}

