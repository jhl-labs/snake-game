#include <stdio.h>
#include "renderer.h"
#include "game_constants.h"
#include "snake.h"
#include "food.h"
#include "enemy.h"

// raylib 헤더
// raylib이 external/raylib/src에 있다고 가정
// 또는 시스템에 설치된 경우 raylib/raylib.h 사용
#include "raylib.h"

// 색상 상수 정의
static const Color COLOR_BACKGROUND = {
    COLOR_BACKGROUND_R,
    COLOR_BACKGROUND_G,
    COLOR_BACKGROUND_B,
    COLOR_BACKGROUND_A
};

static const Color COLOR_SNAKE = {
    COLOR_SNAKE_R,
    COLOR_SNAKE_G,
    COLOR_SNAKE_B,
    COLOR_SNAKE_A
};

static const Color COLOR_SNAKE_HEAD = {
    COLOR_SNAKE_R,
    COLOR_SNAKE_G_HEAD,
    COLOR_SNAKE_B,
    COLOR_SNAKE_A
};

static const Color COLOR_FOOD = {
    COLOR_FOOD_R,
    COLOR_FOOD_G,
    COLOR_FOOD_B,
    COLOR_FOOD_A
};

static const Color COLOR_GRID = {
    COLOR_GRID_R,
    COLOR_GRID_G,
    COLOR_GRID_B,
    COLOR_GRID_A
};

static const Color COLOR_TEXT = {
    COLOR_TEXT_R,
    COLOR_TEXT_G,
    COLOR_TEXT_B,
    COLOR_TEXT_A
};

static const Color COLOR_LIGHTGRAY = {
    200, 200, 200, 255
};

static const Color COLOR_ENEMY = {
    COLOR_ENEMY_R,
    COLOR_ENEMY_G,
    COLOR_ENEMY_B,
    COLOR_ENEMY_A
};

static const Color COLOR_ENEMY_HEAD = {
    COLOR_ENEMY_HEAD_R,
    COLOR_ENEMY_HEAD_G,
    COLOR_ENEMY_HEAD_B,
    COLOR_ENEMY_HEAD_A
};

// 그리드 오프셋 계산 (화면 중앙에 배치)
static int calculate_grid_offset_x(void) {
    return (SCREEN_WIDTH - (GRID_WIDTH * CELL_SIZE)) / 2;
}

static int calculate_grid_offset_y(void) {
    return (SCREEN_HEIGHT - (GRID_HEIGHT * CELL_SIZE)) / 2;
}

bool renderer_init(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");
    SetTargetFPS(60);
    return true;
}

void renderer_close(void) {
    CloseWindow();
}

void renderer_begin(void) {
    BeginDrawing();
}

void renderer_end(void) {
    EndDrawing();
}

bool renderer_window_should_close(void) {
    return WindowShouldClose();
}

// 그리드 배경 그리기
static void draw_grid(void) {
    int offset_x = calculate_grid_offset_x();
    int offset_y = calculate_grid_offset_y();

    // 배경
    DrawRectangle(offset_x, offset_y, 
                  GRID_WIDTH * CELL_SIZE, 
                  GRID_HEIGHT * CELL_SIZE, 
                  COLOR_BACKGROUND);

    // 그리드 라인 (선택적)
    // 세로선
    for (int x = 0; x <= GRID_WIDTH; x++) {
        DrawRectangle(offset_x + x * CELL_SIZE, offset_y, 
                      1, GRID_HEIGHT * CELL_SIZE, COLOR_GRID);
    }
    // 가로선
    for (int y = 0; y <= GRID_HEIGHT; y++) {
        DrawRectangle(offset_x, offset_y + y * CELL_SIZE, 
                      GRID_WIDTH * CELL_SIZE, 1, COLOR_GRID);
    }
}

// 뱀 그리기
static void draw_snake(const Snake* p_snake) {
    if (p_snake == NULL || p_snake->p_body == NULL || p_snake->length == 0) {
        return;
    }

    int offset_x = calculate_grid_offset_x();
    int offset_y = calculate_grid_offset_y();

    for (uint32_t i = 0; i < p_snake->length; i++) {
        Position pos = snake_get_position_at(p_snake, i);
        int screen_x = offset_x + pos.x * CELL_SIZE;
        int screen_y = offset_y + pos.y * CELL_SIZE;

        // 머리는 다른 색상
        Color color = (i == 0) ? COLOR_SNAKE_HEAD : COLOR_SNAKE;

        DrawRectangle(screen_x + 1, screen_y + 1, 
                      CELL_SIZE - 2, CELL_SIZE - 2, color);
    }
}

// 음식 그리기
static void draw_food(const Food* p_food) {
    if (p_food == NULL || !food_is_active(p_food)) {
        return;
    }

    int offset_x = calculate_grid_offset_x();
    int offset_y = calculate_grid_offset_y();

    Position pos = food_get_position(p_food);
    int screen_x = offset_x + pos.x * CELL_SIZE;
    int screen_y = offset_y + pos.y * CELL_SIZE;

    DrawRectangle(screen_x + 2, screen_y + 2, 
                  CELL_SIZE - 4, CELL_SIZE - 4, COLOR_FOOD);
}

// 적 뱀 그리기
static void draw_enemy(const Enemy* p_enemy) {
    if (p_enemy == NULL || !enemy_is_active(p_enemy)) {
        return;
    }

    const Snake* p_snake = enemy_get_snake(p_enemy);
    if (p_snake == NULL || p_snake->p_body == NULL || p_snake->length == 0) {
        return;
    }

    int offset_x = calculate_grid_offset_x();
    int offset_y = calculate_grid_offset_y();

    for (uint32_t i = 0; i < p_snake->length; i++) {
        Position pos = snake_get_position_at(p_snake, i);
        int screen_x = offset_x + pos.x * CELL_SIZE;
        int screen_y = offset_y + pos.y * CELL_SIZE;

        // 머리는 다른 색상
        Color color = (i == 0) ? COLOR_ENEMY_HEAD : COLOR_ENEMY;

        DrawRectangle(screen_x + 1, screen_y + 1, 
                      CELL_SIZE - 2, CELL_SIZE - 2, color);
    }
}

// 점수 표시
static void draw_score(uint32_t score) {
    char score_text[32];
    snprintf(score_text, sizeof(score_text), "Score: %u", score);
    
    int text_width = MeasureText(score_text, 20);
    int pos_x = (SCREEN_WIDTH - text_width) / 2;
    int pos_y = 10;

    DrawText(score_text, pos_x, pos_y, 20, COLOR_TEXT);
}

void renderer_draw_game(const Game* p_game) {
    if (p_game == NULL) {
        return;
    }

    ClearBackground(BLACK);

    // 그리드 배경
    draw_grid();

    // 음식
    draw_food(game_get_food(p_game));

    // 적 뱀
    draw_enemy(game_get_enemy(p_game));

    // 플레이어 뱀
    draw_snake(game_get_snake(p_game));

    // 점수
    draw_score(game_get_score(p_game));
}

void renderer_draw_game_over(const Game* p_game) {
    if (p_game == NULL) {
        return;
    }

    // 게임 화면 먼저 그리기
    renderer_draw_game(p_game);

    // 반투명 오버레이
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 
                  (Color){0, 0, 0, 180});

    // 게임 오버 텍스트
    const char* game_over_text = "GAME OVER";
    int text_size = 40;
    int text_width = MeasureText(game_over_text, text_size);
    int text_x = (SCREEN_WIDTH - text_width) / 2;
    int text_y = SCREEN_HEIGHT / 2 - 60;

    DrawText(game_over_text, text_x, text_y, text_size, COLOR_TEXT);

    // 점수 표시
    char score_text[64];
    snprintf(score_text, sizeof(score_text), "Final Score: %u", 
             game_get_score(p_game));
    int score_text_size = 24;
    int score_text_width = MeasureText(score_text, score_text_size);
    int score_text_x = (SCREEN_WIDTH - score_text_width) / 2;
    int score_text_y = text_y + 60;

    DrawText(score_text, score_text_x, score_text_y, 
             score_text_size, COLOR_TEXT);

    // 재시작 안내
    const char* restart_text = "Press R to Restart or ESC to Quit";
    int restart_text_size = 20;
    int restart_text_width = MeasureText(restart_text, restart_text_size);
    int restart_text_x = (SCREEN_WIDTH - restart_text_width) / 2;
    int restart_text_y = score_text_y + 50;

    DrawText(restart_text, restart_text_x, restart_text_y, 
             restart_text_size, COLOR_LIGHTGRAY);
}

