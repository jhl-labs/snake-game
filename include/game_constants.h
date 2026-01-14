#ifndef SNAKE_GAME_CONSTANTS_H
#define SNAKE_GAME_CONSTANTS_H

// 화면 크기
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// 그리드 설정
#define GRID_WIDTH 40
#define GRID_HEIGHT 30
#define CELL_SIZE 20

// 게임 설정
#define INITIAL_SNAKE_LENGTH 3
#define MAX_SNAKE_LENGTH (GRID_WIDTH * GRID_HEIGHT)

// 색상 정의 (raylib Color 구조체와 호환)
#define COLOR_BACKGROUND_R 20
#define COLOR_BACKGROUND_G 20
#define COLOR_BACKGROUND_B 20
#define COLOR_BACKGROUND_A 255

#define COLOR_SNAKE_R 0
#define COLOR_SNAKE_G 255
#define COLOR_SNAKE_G_HEAD 100
#define COLOR_SNAKE_B 0
#define COLOR_SNAKE_A 255

#define COLOR_FOOD_R 255
#define COLOR_FOOD_G 0
#define COLOR_FOOD_B 0
#define COLOR_FOOD_A 255

#define COLOR_GRID_R 50
#define COLOR_GRID_G 50
#define COLOR_GRID_B 50
#define COLOR_GRID_A 255

#define COLOR_TEXT_R 255
#define COLOR_TEXT_G 255
#define COLOR_TEXT_B 255
#define COLOR_TEXT_A 255

// 게임 속도
#define GAME_SPEED 10  // 프레임당 업데이트 간격

// 방향 정의
typedef enum {
    DIRECTION_UP = 0,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_NONE
} Direction;

// 게임 상태
typedef enum {
    GAME_STATE_MENU = 0,
    GAME_STATE_PLAYING,
    GAME_STATE_GAME_OVER,
    GAME_STATE_PAUSED
} GameState;

// 게임 모드
typedef enum {
    GAME_MODE_SINGLE = 0,
    GAME_MODE_VERSUS
} GameMode;

// 게임 결과 (2인용 모드)
typedef enum {
    GAME_RESULT_NONE = 0,
    GAME_RESULT_P1_WIN,
    GAME_RESULT_P2_WIN,
    GAME_RESULT_DRAW
} GameResult;

// 2인용 모드 시작 위치
#define P1_START_X 10
#define P1_START_Y 15
#define P2_START_X 30
#define P2_START_Y 15

// P2 색상 정의 (파랑색)
#define COLOR_SNAKE2_R 0
#define COLOR_SNAKE2_G 100
#define COLOR_SNAKE2_G_HEAD 200
#define COLOR_SNAKE2_B 255
#define COLOR_SNAKE2_A 255

#endif // SNAKE_GAME_CONSTANTS_H

