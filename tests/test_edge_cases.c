#include <stdio.h>
#include <stdlib.h>
#include "snake.h"
#include "food.h"
#include "game.h"

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

// Edge Case 1: snake_grow - capacity 확장 (realloc)
TEST(snake_grow_capacity_expansion) {
    Snake snake;
    snake_init(&snake, 10, 10);

    uint32_t initial_capacity = snake.capacity;

    // 초기 용량까지 성장 (INITIAL_SNAKE_LENGTH + 10)
    uint32_t grow_count = initial_capacity - snake.length;
    for (uint32_t i = 0; i < grow_count; i++) {
        snake_grow(&snake);
    }

    ASSERT(snake.length == initial_capacity);
    ASSERT(snake.capacity == initial_capacity);

    // 한 번 더 성장 → capacity가 2배로 확장되어야 함
    snake_grow(&snake);

    ASSERT(snake.length == initial_capacity + 1);
    ASSERT(snake.capacity == initial_capacity * 2);
    ASSERT(snake.p_body != NULL);

    snake_destroy(&snake);
    PASS();
}

// Edge Case 2: snake_grow - MAX_SNAKE_LENGTH 제한
TEST(snake_grow_max_length) {
    Snake snake;
    snake_init(&snake, 10, 10);

    // MAX_SNAKE_LENGTH까지 성장
    while (snake.length < MAX_SNAKE_LENGTH) {
        snake_grow(&snake);
    }

    ASSERT(snake.length == MAX_SNAKE_LENGTH);

    uint32_t final_length = snake.length;

    // 더 이상 성장하지 않아야 함
    snake_grow(&snake);
    ASSERT(snake.length == final_length);

    snake_grow(&snake);
    ASSERT(snake.length == final_length);

    snake_destroy(&snake);
    PASS();
}

// Edge Case 3: food_spawn - 그리드가 거의 꽉 찬 경우
TEST(food_spawn_nearly_full_grid) {
    Snake snake;
    Food food;

    snake_init(&snake, 10, 10);
    food_init(&food);

    // 뱀을 매우 길게 만들기 (전체 그리드의 90%)
    uint32_t target_length = (GRID_WIDTH * GRID_HEIGHT) * 9 / 10;
    while (snake.length < target_length && snake.length < MAX_SNAKE_LENGTH) {
        snake_grow(&snake);
    }

    // 음식 생성이 여전히 가능해야 함 (빈 공간이 있음)
    bool success = food_spawn(&food, &snake);
    ASSERT(success == true);

    // 음식이 뱀과 겹치지 않는지 확인
    Position food_pos = food_get_position(&food);
    bool overlaps = false;
    for (uint32_t i = 0; i < snake.length; i++) {
        Position snake_pos = snake_get_position_at(&snake, i);
        if (food_pos.x == snake_pos.x && food_pos.y == snake_pos.y) {
            overlaps = true;
            break;
        }
    }
    ASSERT(overlaps == false);

    snake_destroy(&snake);
    PASS();
}

// Edge Case 4: food_spawn - 그리드 완전히 꽉 참 (실패)
TEST(food_spawn_full_grid_failure) {
    Snake snake;
    Food food;

    snake_init(&snake, 10, 10);
    food_init(&food);

    // 뱀을 그리드 전체 크기로 만들기
    while (snake.length < MAX_SNAKE_LENGTH) {
        snake_grow(&snake);
    }

    ASSERT(snake.length == MAX_SNAKE_LENGTH);
    ASSERT(snake.length == GRID_WIDTH * GRID_HEIGHT);

    // 음식 생성 실패해야 함 (빈 공간 없음)
    bool success = food_spawn(&food, &snake);
    ASSERT(success == false);

    snake_destroy(&snake);
    PASS();
}

// Edge Case 5: game_update - 음식 생성 실패 시 게임 오버
TEST(game_update_food_spawn_failure_game_over) {
    Game game;
    game_init(&game);
    game_restart(&game);

    // 뱀을 거의 최대 길이로 만들기
    while (game.snake.length < MAX_SNAKE_LENGTH - 2) {
        snake_grow(&game.snake);
    }

    // 음식을 뱀 앞에 배치하고 먹기
    Position head = snake_get_head(&game.snake);
    game.food.position.x = head.x + 1;
    game.food.position.y = head.y;
    game.food.is_active = true;

    // 음식 먹기 (이제 뱀이 거의 가득 참)
    for (int i = 0; i < GAME_SPEED; i++) {
        game_update(&game);
    }

    // 한 번 더 먹으면 그리드가 완전히 꽉 차서 게임 오버
    head = snake_get_head(&game.snake);
    game.food.position.x = head.x + 1;
    game.food.position.y = head.y;
    game.food.is_active = true;

    for (int i = 0; i < GAME_SPEED; i++) {
        game_update(&game);
    }

    // 음식을 생성할 수 없어서 게임 오버되어야 함
    ASSERT(game_get_state(&game) == GAME_STATE_GAME_OVER);

    game_destroy(&game);
    PASS();
}

// Edge Case 6: snake_get_position_at - 유효한 인덱스
TEST(snake_get_position_at_valid_index) {
    Snake snake;
    snake_init(&snake, 10, 10);

    // 모든 유효한 인덱스 테스트
    for (uint32_t i = 0; i < snake.length; i++) {
        Position pos = snake_get_position_at(&snake, i);
        ASSERT(pos.x >= 0 && pos.x < GRID_WIDTH * 10);  // 합리적인 범위
        ASSERT(pos.y >= 0 && pos.y < GRID_HEIGHT * 10);
    }

    snake_destroy(&snake);
    PASS();
}

// Edge Case 7: snake_get_position_at - 무효한 인덱스
TEST(snake_get_position_at_invalid_index) {
    Snake snake;
    snake_init(&snake, 10, 10);

    // 범위를 벗어난 인덱스 (안전하게 {0,0} 반환해야 함)
    Position pos = snake_get_position_at(&snake, snake.length + 100);
    ASSERT(pos.x == 0);
    ASSERT(pos.y == 0);

    snake_destroy(&snake);
    PASS();
}

// Edge Case 8: 빠른 방향 전환 (입력 버퍼링)
TEST(snake_rapid_direction_changes) {
    Snake snake;
    snake_init(&snake, 10, 10);

    // 초기 방향: RIGHT
    ASSERT(snake.direction == DIRECTION_RIGHT);

    // UP으로 변경 (아직 이동 안함)
    snake_set_direction(&snake, DIRECTION_UP);
    ASSERT(snake.next_direction == DIRECTION_UP);
    ASSERT(snake.direction == DIRECTION_RIGHT);  // 아직 적용 안됨

    // LEFT로 다시 변경 (UP이 적용되기 전)
    snake_set_direction(&snake, DIRECTION_LEFT);
    // UP → LEFT는 반대 방향이 아니므로 허용됨
    ASSERT(snake.next_direction == DIRECTION_LEFT);

    // 이동 → 마지막으로 설정한 방향(LEFT) 적용
    snake_move(&snake);
    ASSERT(snake.direction == DIRECTION_LEFT);

    snake_destroy(&snake);
    PASS();
}

// Edge Case 9: snake_destroy - 이미 파괴된 뱀
TEST(snake_destroy_twice) {
    Snake snake;
    snake_init(&snake, 10, 10);

    snake_destroy(&snake);
    ASSERT(snake.p_body == NULL);

    // 다시 호출해도 안전해야 함
    snake_destroy(&snake);

    PASS();
}

// Edge Case 10: game_restart - 여러 번 호출
TEST(game_restart_multiple_times) {
    Game game;
    game_init(&game);

    for (int i = 0; i < 5; i++) {
        game_restart(&game);

        ASSERT(game_get_state(&game) == GAME_STATE_PLAYING);
        ASSERT(game_get_score(&game) == 0);
        ASSERT(game.snake.length == INITIAL_SNAKE_LENGTH);
    }

    game_destroy(&game);
    PASS();
}

// Edge Case 11: 뱀이 한 칸에서 자기 자신과 충돌 (불가능하지만 테스트)
TEST(snake_self_collision_minimum_length) {
    Snake snake;
    snake_init(&snake, 10, 10);

    // 길이 3 (초기 상태)에서는 자기 충돌 불가능
    ASSERT(snake.length == 3);
    ASSERT(snake_check_self_collision(&snake) == false);

    // 길이 4에서도 불가능 (코드에서 < 4 체크)
    snake_grow(&snake);
    ASSERT(snake.length == 4);
    ASSERT(snake_check_self_collision(&snake) == false);

    snake_destroy(&snake);
    PASS();
}

// Edge Case 12: 모든 경계에서의 충돌
TEST(snake_boundary_all_edges) {
    Snake snake;

    // 위쪽 경계
    snake_init(&snake, 10, 0);
    snake_set_direction(&snake, DIRECTION_UP);
    snake_move(&snake);
    ASSERT(snake_check_boundary_collision(&snake) == true);
    snake_destroy(&snake);

    // 아래쪽 경계
    snake_init(&snake, 10, GRID_HEIGHT - 1);
    snake_set_direction(&snake, DIRECTION_DOWN);
    snake_move(&snake);
    ASSERT(snake_check_boundary_collision(&snake) == true);
    snake_destroy(&snake);

    // 왼쪽 경계
    snake_init(&snake, 0, 10);
    snake_set_direction(&snake, DIRECTION_LEFT);
    snake_move(&snake);
    ASSERT(snake_check_boundary_collision(&snake) == true);
    snake_destroy(&snake);

    // 오른쪽 경계
    snake_init(&snake, GRID_WIDTH - 1, 10);
    snake_set_direction(&snake, DIRECTION_RIGHT);
    snake_move(&snake);
    ASSERT(snake_check_boundary_collision(&snake) == true);
    snake_destroy(&snake);

    PASS();
}

int main(void) {
    int total = 0;
    int passed = 0;
    int failed = 0;

    printf("\n=== Edge Cases Tests ===\n\n");

    RUN_TEST(snake_grow_capacity_expansion);
    RUN_TEST(snake_grow_max_length);
    RUN_TEST(food_spawn_nearly_full_grid);
    RUN_TEST(food_spawn_full_grid_failure);
    RUN_TEST(game_update_food_spawn_failure_game_over);
    RUN_TEST(snake_get_position_at_valid_index);
    RUN_TEST(snake_get_position_at_invalid_index);
    RUN_TEST(snake_rapid_direction_changes);
    RUN_TEST(snake_destroy_twice);
    RUN_TEST(game_restart_multiple_times);
    RUN_TEST(snake_self_collision_minimum_length);
    RUN_TEST(snake_boundary_all_edges);

    printf("\n========================\n");
    printf("Total: %d, Passed: %d, Failed: %d\n", total, passed, failed);

    if (failed == 0) {
        printf("All edge case tests passed!\n");
        return 0;
    } else {
        printf("Some tests failed.\n");
        return 1;
    }
}
