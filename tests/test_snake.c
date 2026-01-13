#include <stdio.h>
#include <stdlib.h>
#include "snake.h"

// 테스트 매크로
#define TEST(name) static int test_##name(void)
#define ASSERT(cond) do { \
    if (!(cond)) { \
        printf("  FAIL: %s (line %d)\n", #cond, __LINE__); \
        return 0; \
    } \
} while(0)
#define PASS() return 1

// 테스트 실행 매크로
#define RUN_TEST(name) do { \
    printf("[TEST] %s... ", #name); \
    if (test_##name()) { \
        printf("PASS\n"); \
        passed++; \
    } else { \
        failed++; \
    } \
    total++; \
} while(0)

// 테스트: snake_init
TEST(snake_init) {
    Snake snake;
    ASSERT(snake_init(&snake, 10, 10) == true);
    ASSERT(snake.length == INITIAL_SNAKE_LENGTH);
    ASSERT(snake.direction == DIRECTION_RIGHT);
    ASSERT(snake.p_body != NULL);

    Position head = snake_get_head(&snake);
    ASSERT(head.x == 10);
    ASSERT(head.y == 10);

    snake_destroy(&snake);
    PASS();
}

// 테스트: snake_init with NULL
TEST(snake_init_null) {
    ASSERT(snake_init(NULL, 0, 0) == false);
    PASS();
}

// 테스트: snake_move
TEST(snake_move) {
    Snake snake;
    snake_init(&snake, 10, 10);

    // 기본 방향은 RIGHT
    snake_move(&snake);
    Position head = snake_get_head(&snake);
    ASSERT(head.x == 11);
    ASSERT(head.y == 10);

    snake_destroy(&snake);
    PASS();
}

// 테스트: snake_set_direction
TEST(snake_set_direction) {
    Snake snake;
    snake_init(&snake, 10, 10);

    // UP으로 방향 변경
    snake_set_direction(&snake, DIRECTION_UP);
    snake_move(&snake);
    Position head = snake_get_head(&snake);
    ASSERT(head.x == 10);
    ASSERT(head.y == 9);

    snake_destroy(&snake);
    PASS();
}

// 테스트: 반대 방향 무시
TEST(snake_opposite_direction) {
    Snake snake;
    snake_init(&snake, 10, 10);

    // RIGHT에서 LEFT로는 변경 불가
    snake_set_direction(&snake, DIRECTION_LEFT);
    snake_move(&snake);
    Position head = snake_get_head(&snake);
    ASSERT(head.x == 11);  // 여전히 RIGHT로 이동

    snake_destroy(&snake);
    PASS();
}

// 테스트: snake_grow
TEST(snake_grow) {
    Snake snake;
    snake_init(&snake, 10, 10);

    uint32_t initial_length = snake.length;
    snake_grow(&snake);
    ASSERT(snake.length == initial_length + 1);

    snake_destroy(&snake);
    PASS();
}

// 테스트: snake_check_boundary_collision
TEST(snake_boundary_collision) {
    Snake snake;

    // 위쪽 경계 (y=0에서 UP으로 이동하면 y=-1)
    snake_init(&snake, 10, 0);
    snake_set_direction(&snake, DIRECTION_UP);
    snake_move(&snake);
    ASSERT(snake_check_boundary_collision(&snake) == true);
    snake_destroy(&snake);

    // 오른쪽 경계 (기본 방향 RIGHT로 계속 이동)
    snake_init(&snake, GRID_WIDTH - 1, 10);
    snake_move(&snake);  // x = GRID_WIDTH
    ASSERT(snake_check_boundary_collision(&snake) == true);
    snake_destroy(&snake);

    PASS();
}

// 테스트: snake_check_self_collision
TEST(snake_self_collision) {
    Snake snake;
    snake_init(&snake, 10, 10);

    // 충분히 길게 만들기
    for (int i = 0; i < 5; i++) {
        snake_grow(&snake);
    }

    // 자기 몸에 부딪히도록 이동
    snake_set_direction(&snake, DIRECTION_UP);
    snake_move(&snake);
    snake_set_direction(&snake, DIRECTION_LEFT);
    snake_move(&snake);
    snake_set_direction(&snake, DIRECTION_DOWN);
    snake_move(&snake);
    snake_set_direction(&snake, DIRECTION_RIGHT);
    snake_move(&snake);

    ASSERT(snake_check_self_collision(&snake) == true);

    snake_destroy(&snake);
    PASS();
}

int main(void) {
    int total = 0;
    int passed = 0;
    int failed = 0;

    printf("\n=== Snake Unit Tests ===\n\n");

    RUN_TEST(snake_init);
    RUN_TEST(snake_init_null);
    RUN_TEST(snake_move);
    RUN_TEST(snake_set_direction);
    RUN_TEST(snake_opposite_direction);
    RUN_TEST(snake_grow);
    RUN_TEST(snake_boundary_collision);
    RUN_TEST(snake_self_collision);

    printf("\n========================\n");
    printf("Total: %d, Passed: %d, Failed: %d\n", total, passed, failed);

    if (failed == 0) {
        printf("All tests passed!\n");
        return 0;
    } else {
        printf("Some tests failed.\n");
        return 1;
    }
}
