#include "intro.h"
#include "game_constants.h"
#include "raylib.h"

/**
 * 인트로 화면을 렌더링합니다.
 * JHL Snake Game 타이틀을 초록색으로 표시합니다.
 */
void intro_render(void) {
    // 배경 색상
    Color background = {
        COLOR_BACKGROUND_R,
        COLOR_BACKGROUND_G,
        COLOR_BACKGROUND_B,
        COLOR_BACKGROUND_A
    };
    ClearBackground(background);

    // 타이틀 텍스트
    const char* title = "JHL Snake Game";

    // 초록색 정의
    Color green = {
        COLOR_SNAKE_R,
        COLOR_SNAKE_G,
        COLOR_SNAKE_B,
        COLOR_SNAKE_A
    };

    // 타이틀 폰트 크기
    int font_size = 60;

    // 타이틀 텍스트 크기 측정
    int text_width = MeasureText(title, font_size);

    // 화면 중앙에 타이틀 배치
    int pos_x = (SCREEN_WIDTH - text_width) / 2;
    int pos_y = SCREEN_HEIGHT / 2 - font_size;

    // 타이틀 그리기
    DrawText(title, pos_x, pos_y, font_size, green);

    // 시작 안내 메시지
    const char* start_msg = "Press SPACE to start";
    int start_font_size = 20;
    int start_text_width = MeasureText(start_msg, start_font_size);
    int start_pos_x = (SCREEN_WIDTH - start_text_width) / 2;
    int start_pos_y = SCREEN_HEIGHT / 2 + 40;

    Color text_color = {
        COLOR_TEXT_R,
        COLOR_TEXT_G,
        COLOR_TEXT_B,
        COLOR_TEXT_A
    };

    DrawText(start_msg, start_pos_x, start_pos_y, start_font_size, text_color);
}

/**
 * 인트로 화면에서 사용자 입력을 처리합니다.
 *
 * @return 게임 시작 여부 (true면 게임 시작)
 */
bool intro_should_start_game(void) {
    return IsKeyPressed(KEY_SPACE);
}
