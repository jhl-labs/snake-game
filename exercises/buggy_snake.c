/**
 * @file buggy_snake.c
 * @brief 버그 수정 실습용 파일
 *
 * 이 파일에는 의도적으로 여러 버그가 포함되어 있습니다.
 * AI Agent를 활용하여 버그를 찾고 수정하는 실습을 진행합니다.
 *
 * 버그 개수: 7개
 * 난이도: 초급 ~ 중급
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
// BUG #1: NULL 포인터 체크 누락
// ========================================
bool snake_init(Snake* p_snake, int start_x, int start_y) {
    // 여기에 NULL 체크가 없음!

    p_snake->body[0].x = start_x;
    p_snake->body[0].y = start_y;
    p_snake->length = 1;
    p_snake->direction = DIRECTION_RIGHT;

    return true;
}

// ========================================
// BUG #2: 배열 경계 체크 누락
// ========================================
bool snake_grow(Snake* p_snake) {
    if (p_snake == NULL) {
        return false;
    }

    // BUG: MAX_SNAKE_LENGTH 체크 없이 증가!
    p_snake->length++;

    // 마지막 위치 복사
    int last_idx = p_snake->length - 1;
    p_snake->body[last_idx] = p_snake->body[last_idx - 1];

    return true;
}

// ========================================
// BUG #3: 잘못된 조건문 (off-by-one error)
// ========================================
bool is_wall_collision(int x, int y) {
    // BUG: >= 대신 > 사용으로 경계에서 충돌 감지 실패
    if (x < 0 || x > GRID_WIDTH || y < 0 || y > GRID_HEIGHT) {
        return true;
    }
    return false;
}

// ========================================
// BUG #4: 방향 반전 체크 누락
// ========================================
bool snake_set_direction(Snake* p_snake, Direction new_direction) {
    if (p_snake == NULL) {
        return false;
    }

    // BUG: 현재 방향과 반대 방향으로 바로 전환 가능!
    // 예: 오른쪽으로 가다가 바로 왼쪽으로 전환하면 자기 몸과 충돌
    p_snake->direction = new_direction;

    return true;
}

// ========================================
// BUG #5: 이동 로직 오류
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
            // BUG: x-- 대신 x++ (왼쪽으로 가야 하는데 오른쪽으로 감)
            p_snake->body[0].x++;
            break;
        case DIRECTION_RIGHT:
            p_snake->body[0].x++;
            break;
    }

    return true;
}

// ========================================
// BUG #6: 자기 충돌 체크 범위 오류
// ========================================
bool is_self_collision(Snake* p_snake) {
    if (p_snake == NULL) {
        return false;
    }

    Position head = p_snake->body[0];

    // BUG: i = 0부터 시작하면 머리와 머리를 비교 (항상 true)
    // i = 1부터 시작해야 함
    for (int i = 0; i < p_snake->length; i++) {
        if (head.x == p_snake->body[i].x && head.y == p_snake->body[i].y) {
            return true;
        }
    }

    return false;
}

// ========================================
// BUG #7: 메모리 누수
// ========================================
char* get_direction_name(Direction dir) {
    // BUG: malloc한 메모리를 반환하지만 caller가 free하지 않음
    // 정적 문자열을 반환하거나, caller에게 메모리 관리 책임을 명시해야 함
    char* name = (char*)malloc(10 * sizeof(char));

    switch (dir) {
        case DIRECTION_UP:
            sprintf(name, "UP");
            break;
        case DIRECTION_DOWN:
            sprintf(name, "DOWN");
            break;
        case DIRECTION_LEFT:
            sprintf(name, "LEFT");
            break;
        case DIRECTION_RIGHT:
            sprintf(name, "RIGHT");
            break;
    }

    return name;
}

// ========================================
// 테스트용 main 함수
// ========================================
int main(void) {
    printf("=== Buggy Snake Test ===\n\n");

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

    // 테스트 4: 방향 변경
    printf("Test 4: Change direction to LEFT\n");
    snake_set_direction(&snake, DIRECTION_LEFT);
    snake_move(&snake);
    printf("  Position: (%d, %d)\n\n", snake.body[0].x, snake.body[0].y);

    // 테스트 5: 벽 충돌 체크
    printf("Test 5: Wall collision at (20, 10)\n");
    printf("  Collision: %s\n\n", is_wall_collision(20, 10) ? "YES" : "NO");

    // 테스트 6: 방향 이름 (메모리 누수 발생!)
    printf("Test 6: Direction name\n");
    char* dir_name = get_direction_name(snake.direction);
    printf("  Direction: %s\n", dir_name);
    // BUG: free(dir_name) 호출 안함!

    printf("\n=== Tests Complete ===\n");
    printf("(Note: This code contains intentional bugs for practice)\n");

    return 0;
}

/*
 * ========================================
 * 실습 가이드
 * ========================================
 *
 * AI Agent에게 다음과 같이 요청해보세요:
 *
 * 1. "이 파일의 버그를 모두 찾아줘"
 * 2. "snake_move 함수의 버그를 설명해줘"
 * 3. "메모리 누수가 있는지 확인해줘"
 * 4. "버그를 수정한 코드를 보여줘"
 * 5. "각 버그의 심각도를 평가해줘"
 *
 * 학습 포인트:
 * - NULL 포인터 체크의 중요성
 * - 배열 경계 검사
 * - Off-by-one 에러
 * - 메모리 관리
 * - 로직 오류 디버깅
 */
