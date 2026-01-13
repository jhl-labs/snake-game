#ifndef SNAKE_GAME_INTRO_H
#define SNAKE_GAME_INTRO_H

#include <stdbool.h>

/**
 * 인트로 화면을 렌더링합니다.
 * JHL Snake Game 타이틀을 초록색으로 표시합니다.
 */
void intro_render(void);

/**
 * 인트로 화면에서 사용자 입력을 처리합니다.
 *
 * @return 게임 시작 여부 (true면 게임 시작)
 */
bool intro_should_start_game(void);

#endif // SNAKE_GAME_INTRO_H
