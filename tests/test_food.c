#include <stdio.h>
#include <stdlib.h>
#include "food.h"
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

// 테스트: food_init
TEST(food_init) {
    Food food;
    food_init(&food);

    ASSERT(food.is_active == false);
    ASSERT(food.position.x == 0);
    ASSERT(food.position.y == 0);

    PASS();
}

// 테스트: food_init with NULL
TEST(food_init_null) {
    food_init(NULL);  // 크래시 없이 처리되어야 함
    PASS();
}

// 테스트: food_spawn
TEST(food_spawn_basic) {
    Snake snake;
    Food food;

    snake_init(&snake, 10, 10);
    food_init(&food);

    ASSERT(food_spawn(&food, &snake) == true);
    ASSERT(food.is_active == true);

    // 음식이 그리드 범위 내에 있는지 확인
    Position pos = food_get_position(&food);
    ASSERT(pos.x >= 0 && pos.x < GRID_WIDTH);
    ASSERT(pos.y >= 0 && pos.y < GRID_HEIGHT);

    snake_destroy(&snake);
    PASS();
}

// 테스트: food_spawn - 뱀과 겹치지 않는지 확인
TEST(food_spawn_not_on_snake) {
    Snake snake;
    Food food;

    snake_init(&snake, 10, 10);
    food_init(&food);

    // 음식 생성
    ASSERT(food_spawn(&food, &snake) == true);

    Position food_pos = food_get_position(&food);

    // 뱀의 모든 부분과 비교
    for (uint32_t i = 0; i < snake.length; i++) {
        Position snake_pos = snake_get_position_at(&snake, i);
        ASSERT(!(food_pos.x == snake_pos.x && food_pos.y == snake_pos.y));
    }

    snake_destroy(&snake);
    PASS();
}

// 테스트: food_spawn with NULL
TEST(food_spawn_null) {
    Snake snake;
    Food food;

    snake_init(&snake, 10, 10);
    food_init(&food);

    ASSERT(food_spawn(NULL, &snake) == false);
    ASSERT(food_spawn(&food, NULL) == false);

    snake_destroy(&snake);
    PASS();
}

// 테스트: food_get_position
TEST(food_get_position_test) {
    Food food;
    food_init(&food);

    food.position.x = 5;
    food.position.y = 7;
    food.is_active = true;

    Position pos = food_get_position(&food);
    ASSERT(pos.x == 5);
    ASSERT(pos.y == 7);

    PASS();
}

// 테스트: food_is_active
TEST(food_is_active_test) {
    Food food;
    food_init(&food);

    ASSERT(food_is_active(&food) == false);

    food.is_active = true;
    ASSERT(food_is_active(&food) == true);

    PASS();
}

// 테스트: food_is_active with NULL
TEST(food_is_active_null) {
    ASSERT(food_is_active(NULL) == false);
    PASS();
}

// 테스트: food_deactivate
TEST(food_deactivate_test) {
    Food food;
    food_init(&food);

    food.is_active = true;
    food_deactivate(&food);

    ASSERT(food.is_active == false);

    PASS();
}

// 테스트: food_deactivate with NULL
TEST(food_deactivate_null) {
    food_deactivate(NULL);  // 크래시 없이 처리되어야 함
    PASS();
}

// 테스트: 여러 번 spawn해도 항상 뱀과 겹치지 않는지
TEST(food_spawn_multiple) {
    Snake snake;
    Food food;

    snake_init(&snake, 10, 10);
    food_init(&food);

    // 10번 생성해보기
    for (int i = 0; i < 10; i++) {
        ASSERT(food_spawn(&food, &snake) == true);
        ASSERT(food.is_active == true);

        Position food_pos = food_get_position(&food);

        // 뱀과 겹치지 않는지 확인
        bool overlaps = false;
        for (uint32_t j = 0; j < snake.length; j++) {
            Position snake_pos = snake_get_position_at(&snake, j);
            if (food_pos.x == snake_pos.x && food_pos.y == snake_pos.y) {
                overlaps = true;
                break;
            }
        }
        ASSERT(overlaps == false);

        food_deactivate(&food);
    }

    snake_destroy(&snake);
    PASS();
}

int main(void) {
    int total = 0;
    int passed = 0;
    int failed = 0;

    printf("\n=== Food Unit Tests ===\n\n");

    RUN_TEST(food_init);
    RUN_TEST(food_init_null);
    RUN_TEST(food_spawn_basic);
    RUN_TEST(food_spawn_not_on_snake);
    RUN_TEST(food_spawn_null);
    RUN_TEST(food_get_position_test);
    RUN_TEST(food_is_active_test);
    RUN_TEST(food_is_active_null);
    RUN_TEST(food_deactivate_test);
    RUN_TEST(food_deactivate_null);
    RUN_TEST(food_spawn_multiple);

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
