/**
 * @file buggy_snake_fixed.c
 * @brief 버그 수정 실습 - 모범 답안
 *
 * buggy_snake.c의 7개 버그를 모두 수정한 버전입니다.
 * 교육자용 - 수강생에게는 비공개
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SNAKE_LENGTH 100
#define GRID_WIDTH 20
#define GRID_HEIGHT 20

typedef enum {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT
} Direction;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position body[MAX_SNAKE_LENGTH];
    int length;
    Direction direction;
} Snake;

// ========================================
// FIX #1: NULL 포인터 체크 추가
// ========================================
bool snake_init(Snake* p_snake, int start_x, int start_y) {
    // FIX: NULL 체크 추가
    if (p_snake == NULL) {
        fprintf(stderr, "snake_init: NULL pointer\n");
        return false;
    }

    p_snake->body[0].x = start_x;
    p_snake->body[0].y = start_y;
    p_snake->length = 1;
    p_snake->direction = DIRECTION_RIGHT;

    return true;
}

// ========================================
// FIX #2: 배열 경계 체크 추가
// ========================================
bool snake_grow(Snake* p_snake) {
    if (p_snake == NULL) {
        return false;
    }

    // FIX: MAX_SNAKE_LENGTH 체크 추가
    if (p_snake->length >= MAX_SNAKE_LENGTH) {
        fprintf(stderr, "snake_grow: Max length reached\n");
        return false;
    }

    // 마지막 위치 복사
    int last_idx = p_snake->length;
    p_snake->body[last_idx] = p_snake->body[last_idx - 1];
    p_snake->length++;

    return true;
}

// ========================================
// FIX #3: 올바른 경계 조건 (>= 사용)
// ========================================
bool is_wall_collision(int x, int y) {
    // FIX: > 대신 >= 사용
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
        return true;
    }
    return false;
}

// ========================================
// FIX #4: 방향 반전 체크 추가
// ========================================
static Direction get_opposite_direction(Direction dir) {
    switch (dir) {
        case DIRECTION_UP:    return DIRECTION_DOWN;
        case DIRECTION_DOWN:  return DIRECTION_UP;
        case DIRECTION_LEFT:  return DIRECTION_RIGHT;
        case DIRECTION_RIGHT: return DIRECTION_LEFT;
        default:              return dir;
    }
}

bool snake_set_direction(Snake* p_snake, Direction new_direction) {
    if (p_snake == NULL) {
        return false;
    }

    // FIX: 반대 방향 체크 추가 (길이가 1보다 클 때만)
    if (p_snake->length > 1) {
        Direction opposite = get_opposite_direction(p_snake->direction);
        if (new_direction == opposite) {
            return false;  // 반대 방향으로 전환 불가
        }
    }

    p_snake->direction = new_direction;
    return true;
}

// ========================================
// FIX #5: 왼쪽 이동 로직 수정
// ========================================
bool snake_move(Snake* p_snake) {
    if (p_snake == NULL) {
        return false;
    }

    // 몸통 이동 (뒤에서부터)
    for (int i = p_snake->length - 1; i > 0; i--) {
        p_snake->body[i] = p_snake->body[i - 1];
    }

    // 머리 이동
    switch (p_snake->direction) {
        case DIRECTION_UP:
            p_snake->body[0].y--;
            break;
        case DIRECTION_DOWN:
            p_snake->body[0].y++;
            break;
        case DIRECTION_LEFT:
            // FIX: x-- 로 수정 (원래 x++였음)
            p_snake->body[0].x--;
            break;
        case DIRECTION_RIGHT:
            p_snake->body[0].x++;
            break;
    }

    return true;
}

// ========================================
// FIX #6: 자기 충돌 체크 범위 수정
// ========================================
bool is_self_collision(Snake* p_snake) {
    if (p_snake == NULL) {
        return false;
    }

    Position head = p_snake->body[0];

    // FIX: i = 1부터 시작 (머리와 머리 비교 방지)
    for (int i = 1; i < p_snake->length; i++) {
        if (head.x == p_snake->body[i].x && head.y == p_snake->body[i].y) {
            return true;
        }
    }

    return false;
}

// ========================================
// FIX #7: 정적 문자열 반환 (메모리 누수 방지)
// ========================================
const char* get_direction_name(Direction dir) {
    // FIX: 정적 문자열 반환으로 변경 (malloc 제거)
    switch (dir) {
        case DIRECTION_UP:    return "UP";
        case DIRECTION_DOWN:  return "DOWN";
        case DIRECTION_LEFT:  return "LEFT";
        case DIRECTION_RIGHT: return "RIGHT";
        default:              return "UNKNOWN";
    }
}

// ========================================
// 테스트용 main 함수
// ========================================
int main(void) {
    printf("=== Fixed Snake Test ===\n\n");

    Snake snake;

    // 테스트 1: 초기화
    printf("Test 1: Initialize snake at (5, 5)\n");
    snake_init(&snake, 5, 5);
    printf("  Position: (%d, %d)\n", snake.body[0].x, snake.body[0].y);
    printf("  Length: %d\n\n", snake.length);

    // 테스트 2: 이동
    printf("Test 2: Move right\n");
    snake_move(&snake);
    printf("  Position: (%d, %d)\n\n", snake.body[0].x, snake.body[0].y);

    // 테스트 3: 성장
    printf("Test 3: Grow snake\n");
    snake_grow(&snake);
    printf("  Length: %d\n\n", snake.length);

    // 테스트 4: 방향 변경 (수정됨!)
    printf("Test 4: Change direction to LEFT\n");
    snake_set_direction(&snake, DIRECTION_LEFT);
    snake_move(&snake);
    printf("  Position: (%d, %d) (should be 5, 5)\n\n",
           snake.body[0].x, snake.body[0].y);

    // 테스트 5: 벽 충돌 체크 (수정됨!)
    printf("Test 5: Wall collision at (20, 10)\n");
    printf("  Collision: %s (should be YES)\n\n",
           is_wall_collision(20, 10) ? "YES" : "NO");

    // 테스트 6: 방향 이름 (메모리 누수 없음!)
    printf("Test 6: Direction name\n");
    const char* dir_name = get_direction_name(snake.direction);
    printf("  Direction: %s\n", dir_name);
    // 메모리 해제 불필요 (정적 문자열)

    printf("\n=== All bugs fixed! ===\n");

    return 0;
}

/*
 * ========================================
 * 수정 요약
 * ========================================
 *
 * BUG #1: snake_init에 NULL 체크 추가
 * BUG #2: snake_grow에 MAX_SNAKE_LENGTH 체크 추가
 * BUG #3: is_wall_collision에서 > 를 >= 로 수정
 * BUG #4: snake_set_direction에 반대 방향 체크 추가
 * BUG #5: snake_move에서 LEFT 케이스의 x++ 를 x-- 로 수정
 * BUG #6: is_self_collision에서 i=0 을 i=1 로 수정
 * BUG #7: get_direction_name을 정적 문자열 반환으로 변경
 */
