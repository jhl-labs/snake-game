#include "intro.h"
#include "game_constants.h"
#include "raylib.h"

// 메뉴 선택 상태 (0: 1인용, 1: 2인용)
static int s_selected_option = 0;

void intro_init(void) {
    s_selected_option = 0;
}

void intro_render(void) {
    // 배경 색상
    Color background = {
        COLOR_BACKGROUND_R,
        COLOR_BACKGROUND_G,
        COLOR_BACKGROUND_B,
        COLOR_BACKGROUND_A
    };
    ClearBackground(background);

    // 초록색 정의
    Color green = {
        COLOR_SNAKE_R,
        COLOR_SNAKE_G,
        COLOR_SNAKE_B,
        COLOR_SNAKE_A
    };

    // 텍스트 색상
    Color text_color = {
        COLOR_TEXT_R,
        COLOR_TEXT_G,
        COLOR_TEXT_B,
        COLOR_TEXT_A
    };

    // 비활성 옵션 색상
    Color dim_color = { 150, 150, 150, 255 };

    // 타이틀 텍스트
    const char* title = "JHL Snake Game";
    int font_size = 60;
    int text_width = MeasureText(title, font_size);
    int pos_x = (SCREEN_WIDTH - text_width) / 2;
    int pos_y = SCREEN_HEIGHT / 2 - 100;
    DrawText(title, pos_x, pos_y, font_size, green);

    // 메뉴 옵션
    const char* option1 = "1 Player";
    const char* option2 = "2 Players";
    int option_font_size = 30;
    int option_spacing = 50;
    int menu_start_y = SCREEN_HEIGHT / 2;

    // 옵션 1 (1인용)
    int option1_width = MeasureText(option1, option_font_size);
    int option1_x = (SCREEN_WIDTH - option1_width) / 2;
    Color option1_color = (s_selected_option == 0) ? text_color : dim_color;
    if (s_selected_option == 0) {
        DrawText("> ", option1_x - 30, menu_start_y, option_font_size, green);
    }
    DrawText(option1, option1_x, menu_start_y, option_font_size, option1_color);

    // 옵션 2 (2인용)
    int option2_width = MeasureText(option2, option_font_size);
    int option2_x = (SCREEN_WIDTH - option2_width) / 2;
    Color option2_color = (s_selected_option == 1) ? text_color : dim_color;
    if (s_selected_option == 1) {
        DrawText("> ", option2_x - 30, menu_start_y + option_spacing, option_font_size, green);
    }
    DrawText(option2, option2_x, menu_start_y + option_spacing, option_font_size, option2_color);

    // 조작 안내
    const char* control_msg = "UP/DOWN to select, SPACE to start";
    int control_font_size = 18;
    int control_width = MeasureText(control_msg, control_font_size);
    int control_x = (SCREEN_WIDTH - control_width) / 2;
    int control_y = SCREEN_HEIGHT / 2 + 130;
    DrawText(control_msg, control_x, control_y, control_font_size, dim_color);

    // 2인용 조작 안내
    const char* p2_control_msg = "P1: Arrow Keys  |  P2: WASD";
    int p2_control_width = MeasureText(p2_control_msg, control_font_size);
    int p2_control_x = (SCREEN_WIDTH - p2_control_width) / 2;
    int p2_control_y = control_y + 25;
    DrawText(p2_control_msg, p2_control_x, p2_control_y, control_font_size, dim_color);
}

bool intro_should_start_game(void) {
    // 위/아래 키로 메뉴 선택
    if (IsKeyPressed(KEY_UP)) {
        s_selected_option = 0;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        s_selected_option = 1;
    }

    return IsKeyPressed(KEY_SPACE);
}

GameMode intro_get_selected_mode(void) {
    return (s_selected_option == 0) ? GAME_MODE_SINGLE : GAME_MODE_VERSUS;
}
