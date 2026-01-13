/**
 * @file test_example.c
 * @brief TC 작성 실습용 예제 파일
 *
 * 이 파일은 테스트 케이스 작성 방법을 보여주는 예제입니다.
 * AI Agent를 활용하여 추가 테스트를 작성하는 실습을 진행합니다.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// ========================================
// 테스트 대상 구조체 및 함수 (간략화)
// ========================================

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

// 테스트 대상 함수들
bool snake_init(Snake* p_snake, int start_x, int start_y) {
    if (p_snake == NULL) return false;
    if (start_x < 0 || start_x >= GRID_WIDTH) return false;
    if (start_y < 0 || start_y >= GRID_HEIGHT) return false;

    p_snake->body[0].x = start_x;
    p_snake->body[0].y = start_y;
    p_snake->length = 1;
    p_snake->direction = DIRECTION_RIGHT;
    return true;
}

bool snake_move(Snake* p_snake) {
    if (p_snake == NULL) return false;

    for (int i = p_snake->length - 1; i > 0; i--) {
        p_snake->body[i] = p_snake->body[i - 1];
    }

    switch (p_snake->direction) {
        case DIRECTION_UP:    p_snake->body[0].y--; break;
        case DIRECTION_DOWN:  p_snake->body[0].y++; break;
        case DIRECTION_LEFT:  p_snake->body[0].x--; break;
        case DIRECTION_RIGHT: p_snake->body[0].x++; break;
    }
    return true;
}

bool snake_grow(Snake* p_snake) {
    if (p_snake == NULL) return false;
    if (p_snake->length >= MAX_SNAKE_LENGTH) return false;

    int last_idx = p_snake->length;
    p_snake->body[last_idx] = p_snake->body[last_idx - 1];
    p_snake->length++;
    return true;
}

// ========================================
// 테스트 유틸리티
// ========================================

static int s_tests_run = 0;
static int s_tests_passed = 0;

#define TEST_START(name) \
    printf("  [TEST] %s... ", name); \
    s_tests_run++;

#define TEST_PASS() \
    printf("PASS\n"); \
    s_tests_passed++;

#define TEST_FAIL(msg) \
    printf("FAIL: %s\n", msg); \

#define ASSERT_TRUE(cond, msg) \
    if (!(cond)) { TEST_FAIL(msg); return; }

#define ASSERT_EQ(expected, actual, msg) \
    if ((expected) != (actual)) { TEST_FAIL(msg); return; }

// ========================================
// 테스트 케이스: snake_init
// ========================================

/**
 * 테스트: snake_init - 정상 초기화
 * 시나리오: 유효한 좌표로 뱀 초기화
 * 기대결과: 초기화 성공, 위치/길이/방향 올바름
 */
void test_snake_init_normal(void) {
    TEST_START("snake_init - normal initialization");

    // Arrange
    Snake snake;
    int start_x = 5;
    int start_y = 10;

    // Act
    bool result = snake_init(&snake, start_x, start_y);

    // Assert
    ASSERT_TRUE(result, "init should return true");
    ASSERT_EQ(start_x, snake.body[0].x, "x position mismatch");
    ASSERT_EQ(start_y, snake.body[0].y, "y position mismatch");
    ASSERT_EQ(1, snake.length, "initial length should be 1");
    ASSERT_EQ(DIRECTION_RIGHT, snake.direction, "initial direction should be RIGHT");

    TEST_PASS();
}

/**
 * 테스트: snake_init - NULL 포인터
 * 시나리오: NULL 포인터로 초기화 시도
 * 기대결과: false 반환, 크래시 없음
 */
void test_snake_init_null_pointer(void) {
    TEST_START("snake_init - null pointer");

    // Act
    bool result = snake_init(NULL, 5, 5);

    // Assert
    ASSERT_TRUE(!result, "init with NULL should return false");

    TEST_PASS();
}

/**
 * 테스트: snake_init - 경계 좌표 (0, 0)
 * 시나리오: 최소 경계 좌표로 초기화
 * 기대결과: 초기화 성공
 */
void test_snake_init_boundary_min(void) {
    TEST_START("snake_init - boundary (0, 0)");

    // Arrange
    Snake snake;

    // Act
    bool result = snake_init(&snake, 0, 0);

    // Assert
    ASSERT_TRUE(result, "init at (0,0) should succeed");
    ASSERT_EQ(0, snake.body[0].x, "x should be 0");
    ASSERT_EQ(0, snake.body[0].y, "y should be 0");

    TEST_PASS();
}

/**
 * 테스트: snake_init - 경계 좌표 (MAX-1, MAX-1)
 * 시나리오: 최대 경계 좌표로 초기화
 * 기대결과: 초기화 성공
 */
void test_snake_init_boundary_max(void) {
    TEST_START("snake_init - boundary (MAX-1, MAX-1)");

    // Arrange
    Snake snake;

    // Act
    bool result = snake_init(&snake, GRID_WIDTH - 1, GRID_HEIGHT - 1);

    // Assert
    ASSERT_TRUE(result, "init at max boundary should succeed");

    TEST_PASS();
}

/**
 * 테스트: snake_init - 범위 초과 좌표
 * 시나리오: 그리드 범위를 벗어난 좌표로 초기화
 * 기대결과: false 반환
 */
void test_snake_init_out_of_bounds(void) {
    TEST_START("snake_init - out of bounds");

    // Arrange
    Snake snake;

    // Act & Assert
    ASSERT_TRUE(!snake_init(&snake, -1, 5), "negative x should fail");
    ASSERT_TRUE(!snake_init(&snake, 5, -1), "negative y should fail");
    ASSERT_TRUE(!snake_init(&snake, GRID_WIDTH, 5), "x >= WIDTH should fail");
    ASSERT_TRUE(!snake_init(&snake, 5, GRID_HEIGHT), "y >= HEIGHT should fail");

    TEST_PASS();
}

// ========================================
// 테스트 케이스: snake_move
// ========================================

/**
 * 테스트: snake_move - 오른쪽 이동
 * 시나리오: 기본 방향(오른쪽)으로 이동
 * 기대결과: x 좌표 1 증가
 */
void test_snake_move_right(void) {
    TEST_START("snake_move - move right");

    // Arrange
    Snake snake;
    snake_init(&snake, 5, 5);
    int initial_x = snake.body[0].x;

    // Act
    snake_move(&snake);

    // Assert
    ASSERT_EQ(initial_x + 1, snake.body[0].x, "x should increase by 1");
    ASSERT_EQ(5, snake.body[0].y, "y should not change");

    TEST_PASS();
}

/**
 * 테스트: snake_move - 위로 이동
 * 시나리오: 방향을 위로 변경 후 이동
 * 기대결과: y 좌표 1 감소
 */
void test_snake_move_up(void) {
    TEST_START("snake_move - move up");

    // Arrange
    Snake snake;
    snake_init(&snake, 5, 5);
    snake.direction = DIRECTION_UP;
    int initial_y = snake.body[0].y;

    // Act
    snake_move(&snake);

    // Assert
    ASSERT_EQ(5, snake.body[0].x, "x should not change");
    ASSERT_EQ(initial_y - 1, snake.body[0].y, "y should decrease by 1");

    TEST_PASS();
}

// ========================================
// 실습 과제: 아래 테스트를 작성해보세요!
// ========================================

/*
 * TODO: test_snake_move_down
 * 시나리오: 아래로 이동
 * 기대결과: y 좌표 1 증가
 */

/*
 * TODO: test_snake_move_left
 * 시나리오: 왼쪽으로 이동
 * 기대결과: x 좌표 1 감소
 */

/*
 * TODO: test_snake_grow_normal
 * 시나리오: 정상적인 성장
 * 기대결과: 길이 1 증가
 */

/*
 * TODO: test_snake_grow_max_length
 * 시나리오: 최대 길이에서 성장 시도
 * 기대결과: false 반환, 길이 변화 없음
 */

/*
 * TODO: test_snake_body_follows_head
 * 시나리오: 길이 3인 뱀이 이동할 때
 * 기대결과: 몸통이 머리를 따라감
 */

// ========================================
// 테스트 실행
// ========================================

void run_all_tests(void) {
    printf("\n========================================\n");
    printf("Running Snake Unit Tests\n");
    printf("========================================\n\n");

    printf("[snake_init tests]\n");
    test_snake_init_normal();
    test_snake_init_null_pointer();
    test_snake_init_boundary_min();
    test_snake_init_boundary_max();
    test_snake_init_out_of_bounds();

    printf("\n[snake_move tests]\n");
    test_snake_move_right();
    test_snake_move_up();

    printf("\n========================================\n");
    printf("Results: %d/%d tests passed\n", s_tests_passed, s_tests_run);
    printf("========================================\n");

    if (s_tests_passed == s_tests_run) {
        printf("All tests PASSED!\n");
    } else {
        printf("Some tests FAILED!\n");
    }
}

int main(void) {
    run_all_tests();
    return (s_tests_passed == s_tests_run) ? 0 : 1;
}

/*
 * ========================================
 * 실습 가이드
 * ========================================
 *
 * AI Agent에게 다음과 같이 요청해보세요:
 *
 * 1. "test_snake_move_down 테스트를 작성해줘"
 * 2. "snake_grow 함수에 대한 테스트를 추가해줘"
 * 3. "경계 조건 테스트를 더 추가해줘"
 * 4. "이 테스트 파일의 커버리지를 분석해줘"
 * 5. "누락된 테스트 시나리오를 찾아줘"
 *
 * 학습 포인트:
 * - AAA 패턴 (Arrange-Act-Assert)
 * - 경계 조건 테스트
 * - 에러 케이스 테스트
 * - 테스트 네이밍 규칙
 */
