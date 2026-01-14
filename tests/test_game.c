#include <stdio.h>
#include <stdlib.h>
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

// 테스트: game_init
TEST(game_init) {
    Game game;
    ASSERT(game_init(&game) == true);

    ASSERT(game_get_state(&game) == GAME_STATE_MENU);
    ASSERT(game_get_score(&game) == 0);
    ASSERT(game.frame_counter == 0);
    ASSERT(game.should_update == false);

    game_destroy(&game);
    PASS();
}

// 테스트: game_init with NULL
TEST(game_init_null) {
    ASSERT(game_init(NULL) == false);
    PASS();
}

// 테스트: game_restart
TEST(game_restart) {
    Game game;
    game_init(&game);

    game_restart(&game);

    // 게임 상태가 PLAYING으로 변경되어야 함
    ASSERT(game_get_state(&game) == GAME_STATE_PLAYING);
    ASSERT(game_get_score(&game) == 0);
    ASSERT(game.frame_counter == 0);

    // 뱀이 초기화되어야 함
    const Snake* p_snake = game_get_snake(&game);
    ASSERT(p_snake != NULL);
    ASSERT(p_snake->length == INITIAL_SNAKE_LENGTH);

    // 음식이 생성되어야 함
    const Food* p_food = game_get_food(&game);
    ASSERT(p_food != NULL);
    ASSERT(food_is_active(p_food) == true);

    game_destroy(&game);
    PASS();
}

// 테스트: game_restart with NULL
TEST(game_restart_null) {
    game_restart(NULL);  // 크래시 없이 처리되어야 함
    PASS();
}

// 테스트: game_update - 프레임 카운터
TEST(game_update_frame_counter) {
    Game game;
    game_init(&game);
    game_restart(&game);

    uint32_t initial_frame = game.frame_counter;

    // 여러 번 업데이트
    for (int i = 0; i < 5; i++) {
        game_update(&game);
    }

    ASSERT(game.frame_counter == initial_frame + 5);

    game_destroy(&game);
    PASS();
}

// 테스트: game_update - 뱀이 GAME_SPEED마다 이동
TEST(game_update_snake_movement) {
    Game game;
    game_init(&game);
    game_restart(&game);

    Position initial_head = snake_get_head(game_get_snake(&game));

    // GAME_SPEED-1번 업데이트 (아직 이동 안함)
    for (int i = 0; i < GAME_SPEED - 1; i++) {
        game_update(&game);
    }

    Position head_before = snake_get_head(game_get_snake(&game));
    ASSERT(head_before.x == initial_head.x);
    ASSERT(head_before.y == initial_head.y);

    // 한 번 더 업데이트 (이제 이동함)
    game_update(&game);

    Position head_after = snake_get_head(game_get_snake(&game));
    ASSERT(head_after.x != initial_head.x || head_after.y != initial_head.y);

    game_destroy(&game);
    PASS();
}

// 테스트: game_update - 경계 충돌로 게임 오버
TEST(game_update_boundary_collision) {
    Game game;
    game_init(&game);
    game_restart(&game);

    // 뱀을 위쪽 경계로 이동
    game.snake.p_body[0].x = 10;
    game.snake.p_body[0].y = 0;
    game_set_direction(&game, DIRECTION_UP);

    // GAME_SPEED번 업데이트하여 이동
    for (int i = 0; i < GAME_SPEED; i++) {
        game_update(&game);
    }

    ASSERT(game_get_state(&game) == GAME_STATE_GAME_OVER);

    game_destroy(&game);
    PASS();
}

// 테스트: game_update - 자기 충돌로 게임 오버
TEST(game_update_self_collision) {
    Game game;
    game_init(&game);
    game_restart(&game);

    // 뱀을 충분히 길게 만들기
    for (int i = 0; i < 10; i++) {
        snake_grow(&game.snake);
    }

    // 자기 몸에 부딪히도록 이동 (사각형 그리기)
    game_set_direction(&game, DIRECTION_UP);
    for (int i = 0; i < GAME_SPEED; i++) game_update(&game);

    game_set_direction(&game, DIRECTION_LEFT);
    for (int i = 0; i < GAME_SPEED; i++) game_update(&game);

    game_set_direction(&game, DIRECTION_DOWN);
    for (int i = 0; i < GAME_SPEED; i++) game_update(&game);

    game_set_direction(&game, DIRECTION_RIGHT);
    for (int i = 0; i < GAME_SPEED * 2; i++) game_update(&game);

    ASSERT(game_get_state(&game) == GAME_STATE_GAME_OVER);

    game_destroy(&game);
    PASS();
}

// 테스트: game_update - 음식 먹기
TEST(game_update_eat_food) {
    Game game;
    game_init(&game);
    game_restart(&game);

    uint32_t initial_score = game_get_score(&game);
    uint32_t initial_length = game.snake.length;

    // 음식을 뱀 머리 앞에 배치
    Position head = snake_get_head(&game.snake);
    game.food.position.x = head.x + 1;
    game.food.position.y = head.y;
    game.food.is_active = true;

    // 뱀을 오른쪽으로 이동 (기본 방향)
    for (int i = 0; i < GAME_SPEED; i++) {
        game_update(&game);
    }

    // 점수가 증가했는지 확인
    ASSERT(game_get_score(&game) == initial_score + 10);

    // 뱀이 성장했는지 확인
    ASSERT(game.snake.length == initial_length + 1);

    // 새 음식이 생성되었는지 확인
    ASSERT(food_is_active(&game.food) == true);

    game_destroy(&game);
    PASS();
}

// 테스트: game_set_direction
TEST(game_set_direction_test) {
    Game game;
    game_init(&game);
    game_restart(&game);

    // 초기 방향은 RIGHT
    ASSERT(game.snake.direction == DIRECTION_RIGHT);

    // UP으로 변경
    game_set_direction(&game, DIRECTION_UP);
    ASSERT(game.snake.next_direction == DIRECTION_UP);

    game_destroy(&game);
    PASS();
}

// 테스트: game_set_direction - PLAYING 상태가 아닐 때
TEST(game_set_direction_not_playing) {
    Game game;
    game_init(&game);

    // MENU 상태에서는 방향 변경 안됨
    game_set_direction(&game, DIRECTION_UP);
    // 크래시 없이 처리되어야 함

    game_destroy(&game);
    PASS();
}

// 테스트: game_set_direction with NULL
TEST(game_set_direction_null) {
    game_set_direction(NULL, DIRECTION_UP);  // 크래시 없이 처리되어야 함
    PASS();
}

// 테스트: game_get_state
TEST(game_get_state_test) {
    Game game;
    game_init(&game);

    ASSERT(game_get_state(&game) == GAME_STATE_MENU);

    game_restart(&game);
    ASSERT(game_get_state(&game) == GAME_STATE_PLAYING);

    game_destroy(&game);
    PASS();
}

// 테스트: game_get_state with NULL
TEST(game_get_state_null) {
    ASSERT(game_get_state(NULL) == GAME_STATE_MENU);
    PASS();
}

// 테스트: game_get_score
TEST(game_get_score_test) {
    Game game;
    game_init(&game);
    game_restart(&game);

    ASSERT(game_get_score(&game) == 0);

    game.score = 100;
    ASSERT(game_get_score(&game) == 100);

    game_destroy(&game);
    PASS();
}

// 테스트: game_get_score with NULL
TEST(game_get_score_null) {
    ASSERT(game_get_score(NULL) == 0);
    PASS();
}

// 테스트: game_get_snake
TEST(game_get_snake_test) {
    Game game;
    game_init(&game);
    game_restart(&game);

    const Snake* p_snake = game_get_snake(&game);
    ASSERT(p_snake != NULL);
    ASSERT(p_snake->length == INITIAL_SNAKE_LENGTH);

    game_destroy(&game);
    PASS();
}

// 테스트: game_get_snake with NULL
TEST(game_get_snake_null) {
    ASSERT(game_get_snake(NULL) == NULL);
    PASS();
}

// 테스트: game_get_food
TEST(game_get_food_test) {
    Game game;
    game_init(&game);
    game_restart(&game);

    const Food* p_food = game_get_food(&game);
    ASSERT(p_food != NULL);
    ASSERT(food_is_active(p_food) == true);

    game_destroy(&game);
    PASS();
}

// 테스트: game_get_food with NULL
TEST(game_get_food_null) {
    ASSERT(game_get_food(NULL) == NULL);
    PASS();
}

// 테스트: game_update with NULL
TEST(game_update_null) {
    game_update(NULL);  // 크래시 없이 처리되어야 함
    PASS();
}

// 테스트: game_update - PLAYING 상태가 아닐 때
TEST(game_update_not_playing) {
    Game game;
    game_init(&game);

    // MENU 상태에서는 업데이트 안됨
    uint32_t initial_frame = game.frame_counter;
    game_update(&game);

    ASSERT(game.frame_counter == initial_frame);

    game_destroy(&game);
    PASS();
}

// 테스트: game_destroy with NULL
TEST(game_destroy_null) {
    game_destroy(NULL);  // 크래시 없이 처리되어야 함
    PASS();
}

// 통합 테스트: 전체 게임 플레이 시나리오
TEST(game_integration_full_gameplay) {
    Game game;
    game_init(&game);
    game_restart(&game);

    ASSERT(game_get_state(&game) == GAME_STATE_PLAYING);

    // 음식을 먹고 점수 올리기
    for (int i = 0; i < 3; i++) {
        // 음식을 뱀 앞에 배치
        Position head = snake_get_head(&game.snake);
        game.food.position.x = head.x + 1;
        game.food.position.y = head.y;
        game.food.is_active = true;

        // 음식까지 이동
        for (int j = 0; j < GAME_SPEED; j++) {
            game_update(&game);
        }

        ASSERT(game_get_score(&game) == (i + 1) * 10);
    }

    // 뱀이 성장했는지 확인
    ASSERT(game.snake.length == INITIAL_SNAKE_LENGTH + 3);

    game_destroy(&game);
    PASS();
}

int main(void) {
    int total = 0;
    int passed = 0;
    int failed = 0;

    printf("\n=== Game Unit Tests ===\n\n");

    RUN_TEST(game_init);
    RUN_TEST(game_init_null);
    RUN_TEST(game_restart);
    RUN_TEST(game_restart_null);
    RUN_TEST(game_update_frame_counter);
    RUN_TEST(game_update_snake_movement);
    RUN_TEST(game_update_boundary_collision);
    RUN_TEST(game_update_self_collision);
    RUN_TEST(game_update_eat_food);
    RUN_TEST(game_set_direction_test);
    RUN_TEST(game_set_direction_not_playing);
    RUN_TEST(game_set_direction_null);
    RUN_TEST(game_get_state_test);
    RUN_TEST(game_get_state_null);
    RUN_TEST(game_get_score_test);
    RUN_TEST(game_get_score_null);
    RUN_TEST(game_get_snake_test);
    RUN_TEST(game_get_snake_null);
    RUN_TEST(game_get_food_test);
    RUN_TEST(game_get_food_null);
    RUN_TEST(game_update_null);
    RUN_TEST(game_update_not_playing);
    RUN_TEST(game_destroy_null);
    RUN_TEST(game_integration_full_gameplay);

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
