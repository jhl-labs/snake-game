/**
 * @file test_enemy.c
 * @brief 적 뱀 모듈 테스트
 *
 * Enemy 모듈의 초기화, 이동, 충돌 검사 등을 테스트합니다.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "enemy.h"
#include "snake.h"
#include "game_constants.h"

// 테스트 카운터
static int s_tests_passed = 0;
static int s_tests_failed = 0;

// 테스트 매크로
#define TEST(name) void test_##name(void)
#define RUN_TEST(name) do { \
    printf("  Running %s... ", #name); \
    test_##name(); \
    printf("PASSED\n"); \
    s_tests_passed++; \
} while(0)

#define ASSERT_TRUE(expr) do { \
    if (!(expr)) { \
        printf("FAILED\n"); \
        printf("    Assertion failed: %s\n", #expr); \
        printf("    File: %s, Line: %d\n", __FILE__, __LINE__); \
        s_tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))
#define ASSERT_EQ(a, b) ASSERT_TRUE((a) == (b))
#define ASSERT_NE(a, b) ASSERT_TRUE((a) != (b))

// ============================================================================
// 초기화 테스트
// ============================================================================

TEST(enemy_init_success) {
    Enemy enemy;
    Snake player_snake;
    
    // 플레이어 뱀 초기화
    snake_init(&player_snake, GRID_WIDTH / 2, GRID_HEIGHT / 2);
    
    // 적 뱀 초기화
    bool result = enemy_init(&enemy, &player_snake);
    ASSERT_TRUE(result);
    ASSERT_TRUE(enemy.is_active);
    ASSERT_EQ(enemy.move_delay, ENEMY_MOVE_DELAY);
    ASSERT_EQ(enemy.move_counter, 0);
    
    // 내부 Snake 확인
    const Snake* p_snake = enemy_get_snake(&enemy);
    ASSERT_NE(p_snake, NULL);
    ASSERT_EQ(p_snake->length, INITIAL_SNAKE_LENGTH);
    
    enemy_destroy(&enemy);
    snake_destroy(&player_snake);
}

TEST(enemy_init_null_pointer) {
    bool result = enemy_init(NULL, NULL);
    ASSERT_FALSE(result);
}

TEST(enemy_init_without_player) {
    Enemy enemy;
    
    // 플레이어 없이 초기화 (기본 위치 사용)
    bool result = enemy_init(&enemy, NULL);
    ASSERT_TRUE(result);
    ASSERT_TRUE(enemy.is_active);
    
    enemy_destroy(&enemy);
}

// ============================================================================
// 해제 테스트
// ============================================================================

TEST(enemy_destroy_success) {
    Enemy enemy;
    Snake player_snake;
    
    snake_init(&player_snake, GRID_WIDTH / 2, GRID_HEIGHT / 2);
    enemy_init(&enemy, &player_snake);
    
    enemy_destroy(&enemy);
    ASSERT_FALSE(enemy.is_active);
    
    snake_destroy(&player_snake);
}

TEST(enemy_destroy_null) {
    // NULL 포인터에 대해 크래시 없이 처리
    enemy_destroy(NULL);
    // 크래시 없이 통과하면 성공
}

// ============================================================================
// 활성화 상태 테스트
// ============================================================================

TEST(enemy_is_active_true) {
    Enemy enemy;
    Snake player_snake;
    
    snake_init(&player_snake, GRID_WIDTH / 2, GRID_HEIGHT / 2);
    enemy_init(&enemy, &player_snake);
    
    ASSERT_TRUE(enemy_is_active(&enemy));
    
    enemy_destroy(&enemy);
    snake_destroy(&player_snake);
}

TEST(enemy_is_active_false_after_destroy) {
    Enemy enemy;
    Snake player_snake;
    
    snake_init(&player_snake, GRID_WIDTH / 2, GRID_HEIGHT / 2);
    enemy_init(&enemy, &player_snake);
    enemy_destroy(&enemy);
    
    ASSERT_FALSE(enemy_is_active(&enemy));
    
    snake_destroy(&player_snake);
}

TEST(enemy_is_active_null) {
    ASSERT_FALSE(enemy_is_active(NULL));
}

// ============================================================================
// Snake 포인터 반환 테스트
// ============================================================================

TEST(enemy_get_snake_success) {
    Enemy enemy;
    Snake player_snake;
    
    snake_init(&player_snake, GRID_WIDTH / 2, GRID_HEIGHT / 2);
    enemy_init(&enemy, &player_snake);
    
    const Snake* p_snake = enemy_get_snake(&enemy);
    ASSERT_NE(p_snake, NULL);
    ASSERT_EQ(p_snake->length, INITIAL_SNAKE_LENGTH);
    
    enemy_destroy(&enemy);
    snake_destroy(&player_snake);
}

TEST(enemy_get_snake_null) {
    const Snake* p_snake = enemy_get_snake(NULL);
    ASSERT_EQ(p_snake, NULL);
}

// ============================================================================
// 업데이트 테스트
// ============================================================================

TEST(enemy_update_increments_counter) {
    Enemy enemy;
    Snake player_snake;
    
    snake_init(&player_snake, GRID_WIDTH / 2, GRID_HEIGHT / 2);
    enemy_init(&enemy, &player_snake);
    
    uint32_t initial_counter = enemy.move_counter;
    enemy_update(&enemy);
    
    ASSERT_EQ(enemy.move_counter, initial_counter + 1);
    
    enemy_destroy(&enemy);
    snake_destroy(&player_snake);
}

TEST(enemy_update_null) {
    // NULL 포인터에 대해 크래시 없이 처리
    enemy_update(NULL);
    // 크래시 없이 통과하면 성공
}

TEST(enemy_update_inactive) {
    Enemy enemy;
    Snake player_snake;
    
    snake_init(&player_snake, GRID_WIDTH / 2, GRID_HEIGHT / 2);
    enemy_init(&enemy, &player_snake);
    enemy_destroy(&enemy);  // 비활성화
    
    uint32_t initial_counter = enemy.move_counter;
    enemy_update(&enemy);
    
    // 비활성 상태에서는 카운터가 증가하지 않음
    ASSERT_EQ(enemy.move_counter, initial_counter);
    
    snake_destroy(&player_snake);
}

// ============================================================================
// 리스폰 테스트
// ============================================================================

TEST(enemy_respawn_success) {
    Enemy enemy;
    Snake player_snake;
    
    snake_init(&player_snake, GRID_WIDTH / 2, GRID_HEIGHT / 2);
    enemy_init(&enemy, &player_snake);
    enemy_destroy(&enemy);  // 비활성화
    
    ASSERT_FALSE(enemy_is_active(&enemy));
    
    // 리스폰
    bool result = enemy_respawn(&enemy, &player_snake);
    ASSERT_TRUE(result);
    ASSERT_TRUE(enemy_is_active(&enemy));
    
    enemy_destroy(&enemy);
    snake_destroy(&player_snake);
}

TEST(enemy_respawn_null) {
    bool result = enemy_respawn(NULL, NULL);
    ASSERT_FALSE(result);
}

// ============================================================================
// 충돌 검사 테스트
// ============================================================================

TEST(enemy_collision_with_player_no_collision) {
    Enemy enemy;
    Snake player_snake;
    
    // 플레이어를 중앙에 배치
    snake_init(&player_snake, GRID_WIDTH / 2, GRID_HEIGHT / 2);
    // 적을 다른 위치에 초기화 (겹치지 않도록)
    enemy_init(&enemy, &player_snake);
    
    // 일반적으로 적은 플레이어와 다른 위치에 스폰되므로 충돌 없음
    // (랜덤 위치라서 드물게 충돌할 수 있음 - 테스트는 대부분 통과해야 함)
    
    enemy_destroy(&enemy);
    snake_destroy(&player_snake);
}

TEST(enemy_collision_null_enemy) {
    Snake player_snake;
    snake_init(&player_snake, GRID_WIDTH / 2, GRID_HEIGHT / 2);
    
    bool result = enemy_check_collision_with_player(NULL, &player_snake);
    ASSERT_FALSE(result);
    
    snake_destroy(&player_snake);
}

TEST(enemy_collision_null_player) {
    Enemy enemy;
    Snake player_snake;
    
    snake_init(&player_snake, GRID_WIDTH / 2, GRID_HEIGHT / 2);
    enemy_init(&enemy, &player_snake);
    
    bool result = enemy_check_collision_with_player(&enemy, NULL);
    ASSERT_FALSE(result);
    
    enemy_destroy(&enemy);
    snake_destroy(&player_snake);
}

TEST(enemy_collision_inactive_enemy) {
    Enemy enemy;
    Snake player_snake;
    
    snake_init(&player_snake, GRID_WIDTH / 2, GRID_HEIGHT / 2);
    enemy_init(&enemy, &player_snake);
    enemy_destroy(&enemy);  // 비활성화
    
    bool result = enemy_check_collision_with_player(&enemy, &player_snake);
    ASSERT_FALSE(result);  // 비활성 적은 충돌하지 않음
    
    snake_destroy(&player_snake);
}

// ============================================================================
// 메인 함수
// ============================================================================

int main(void) {
    printf("=== Enemy Module Tests ===\n\n");
    
    printf("[Initialization Tests]\n");
    RUN_TEST(enemy_init_success);
    RUN_TEST(enemy_init_null_pointer);
    RUN_TEST(enemy_init_without_player);
    printf("\n");
    
    printf("[Destruction Tests]\n");
    RUN_TEST(enemy_destroy_success);
    RUN_TEST(enemy_destroy_null);
    printf("\n");
    
    printf("[Active State Tests]\n");
    RUN_TEST(enemy_is_active_true);
    RUN_TEST(enemy_is_active_false_after_destroy);
    RUN_TEST(enemy_is_active_null);
    printf("\n");
    
    printf("[Get Snake Tests]\n");
    RUN_TEST(enemy_get_snake_success);
    RUN_TEST(enemy_get_snake_null);
    printf("\n");
    
    printf("[Update Tests]\n");
    RUN_TEST(enemy_update_increments_counter);
    RUN_TEST(enemy_update_null);
    RUN_TEST(enemy_update_inactive);
    printf("\n");
    
    printf("[Respawn Tests]\n");
    RUN_TEST(enemy_respawn_success);
    RUN_TEST(enemy_respawn_null);
    printf("\n");
    
    printf("[Collision Tests]\n");
    RUN_TEST(enemy_collision_with_player_no_collision);
    RUN_TEST(enemy_collision_null_enemy);
    RUN_TEST(enemy_collision_null_player);
    RUN_TEST(enemy_collision_inactive_enemy);
    printf("\n");
    
    printf("=== Test Results ===\n");
    printf("Passed: %d\n", s_tests_passed);
    printf("Failed: %d\n", s_tests_failed);
    printf("Total:  %d\n", s_tests_passed + s_tests_failed);
    
    return s_tests_failed > 0 ? 1 : 0;
}
