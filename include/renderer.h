#ifndef SNAKE_GAME_RENDERER_H
#define SNAKE_GAME_RENDERER_H

#include <stdbool.h>
#include "game.h"

/**
 * 렌더러를 초기화합니다.
 * raylib 창을 생성합니다.
 * 
 * @return 초기화 성공 여부
 */
bool renderer_init(void);

/**
 * 렌더러를 종료합니다.
 * raylib 창을 닫습니다.
 */
void renderer_close(void);

/**
 * 게임 화면을 렌더링합니다.
 * 
 * @param p_game 게임 객체 포인터
 */
void renderer_draw_game(const Game* p_game);

/**
 * 게임 오버 화면을 렌더링합니다.
 * 
 * @param p_game 게임 객체 포인터
 */
void renderer_draw_game_over(const Game* p_game);

/**
 * 렌더링을 시작합니다 (BeginDrawing 호출).
 */
void renderer_begin(void);

/**
 * 렌더링을 종료합니다 (EndDrawing 호출).
 */
void renderer_end(void);

/**
 * 창이 닫혔는지 확인합니다.
 * 
 * @return 창 닫힘 여부
 */
bool renderer_window_should_close(void);

#endif // SNAKE_GAME_RENDERER_H

