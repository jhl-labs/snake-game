#include <stdio.h>
#include "renderer.h"
#include "game_constants.h"
#include "snake.h"
#include "food.h"

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

// P2 색상 (파랑색)
static const Color COLOR_SNAKE2 = {
    COLOR_SNAKE2_R,
    COLOR_SNAKE2_G,
    COLOR_SNAKE2_B,
    COLOR_SNAKE2_A
};

static const Color COLOR_SNAKE2_HEAD = {
    COLOR_SNAKE2_R,
    COLOR_SNAKE2_G_HEAD,
    COLOR_SNAKE2_B,
    COLOR_SNAKE2_A
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

// 뱀 그리기 (색상 지정)
static void draw_snake_with_color(const Snake* p_snake, Color body_color, Color head_color) {
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
        Color color = (i == 0) ? head_color : body_color;

        DrawRectangle(screen_x + 1, screen_y + 1,
                      CELL_SIZE - 2, CELL_SIZE - 2, color);
    }
}

// P1 뱀 그리기
static void draw_snake(const Snake* p_snake) {
    draw_snake_with_color(p_snake, COLOR_SNAKE, COLOR_SNAKE_HEAD);
}

// P2 뱀 그리기
static void draw_snake_p2(const Snake* p_snake) {
    draw_snake_with_color(p_snake, COLOR_SNAKE2, COLOR_SNAKE2_HEAD);
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

// 점수 표시 (1인용)
static void draw_score(uint32_t score) {
    char score_text[32];
    snprintf(score_text, sizeof(score_text), "Score: %u", score);

    int text_width = MeasureText(score_text, 20);
    int pos_x = (SCREEN_WIDTH - text_width) / 2;
    int pos_y = 10;

    DrawText(score_text, pos_x, pos_y, 20, COLOR_TEXT);
}

// 점수 표시 (2인용)
static void draw_score_versus(uint32_t score1, uint32_t score2) {
    char p1_text[32];
    char p2_text[32];
    snprintf(p1_text, sizeof(p1_text), "P1: %u", score1);
    snprintf(p2_text, sizeof(p2_text), "P2: %u", score2);

    // P1 점수 (왼쪽, 초록색)
    DrawText(p1_text, 20, 10, 20, COLOR_SNAKE);

    // P2 점수 (오른쪽, 파랑색)
    int p2_width = MeasureText(p2_text, 20);
    DrawText(p2_text, SCREEN_WIDTH - p2_width - 20, 10, 20, COLOR_SNAKE2);
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

    // P1 뱀
    draw_snake(game_get_snake(p_game));

    // P2 뱀 (2인용 모드)
    if (game_get_mode(p_game) == GAME_MODE_VERSUS) {
        draw_snake_p2(game_get_snake_p2(p_game));
        draw_score_versus(game_get_score(p_game), game_get_score_p2(p_game));
    } else {
        draw_score(game_get_score(p_game));
    }
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
    int text_y = SCREEN_HEIGHT / 2 - 80;

    DrawText(game_over_text, text_x, text_y, text_size, COLOR_TEXT);

    if (game_get_mode(p_game) == GAME_MODE_VERSUS) {
        // 2인용 모드: 승패 결과 표시
        GameResult result = game_get_result(p_game);
        const char* result_text;
        Color result_color;

        switch (result) {
            case GAME_RESULT_P1_WIN:
                result_text = "Player 1 Wins!";
                result_color = COLOR_SNAKE;
                break;
            case GAME_RESULT_P2_WIN:
                result_text = "Player 2 Wins!";
                result_color = COLOR_SNAKE2;
                break;
            case GAME_RESULT_DRAW:
                result_text = "Draw!";
                result_color = COLOR_TEXT;
                break;
            default:
                result_text = "";
                result_color = COLOR_TEXT;
        }

        int result_width = MeasureText(result_text, 32);
        int result_x = (SCREEN_WIDTH - result_width) / 2;
        int result_y = text_y + 50;
        DrawText(result_text, result_x, result_y, 32, result_color);

        // 점수 표시
        char score_text[64];
        snprintf(score_text, sizeof(score_text), "P1: %u  -  P2: %u",
                 game_get_score(p_game), game_get_score_p2(p_game));
        int score_width = MeasureText(score_text, 24);
        int score_x = (SCREEN_WIDTH - score_width) / 2;
        int score_y = result_y + 50;
        DrawText(score_text, score_x, score_y, 24, COLOR_TEXT);

        // 재시작 안내
        const char* restart_text = "Press R to Restart or ESC to Quit";
        int restart_width = MeasureText(restart_text, 20);
        int restart_x = (SCREEN_WIDTH - restart_width) / 2;
        int restart_y = score_y + 50;
        DrawText(restart_text, restart_x, restart_y, 20, COLOR_LIGHTGRAY);
    } else {
        // 1인용 모드: 기존 점수 표시
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
}

