#ifndef SNAKE_GAME_INTRO_H
#define SNAKE_GAME_INTRO_H

#include <stdbool.h>
#include "game_constants.h"

/**
 * 인트로 화면을 초기화합니다.
 */
void intro_init(void);

/**
 * 인트로 화면을 렌더링합니다.
 * 게임 모드 선택 옵션을 표시합니다.
 */
void intro_render(void);

/**
 * 인트로 화면에서 사용자 입력을 처리합니다.
 *
 * @return 게임 시작 여부 (true면 게임 시작)
 */
bool intro_should_start_game(void);

/**
 * 선택된 게임 모드를 반환합니다.
 *
 * @return 선택된 게임 모드
 */
GameMode intro_get_selected_mode(void);

#endif // SNAKE_GAME_INTRO_H
