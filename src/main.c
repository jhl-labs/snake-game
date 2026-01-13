#include <stdio.h>
#include "game.h"
#include "renderer.h"
#include "game_constants.h"

// raylib 입력 헤더
#include "raylib.h"

/**
 * 입력을 처리하고 게임 상태를 업데이트합니다.
 * 
 * @param p_game 게임 객체 포인터
 */
static void process_input(Game* p_game) {
    if (p_game == NULL) {
        return;
    }

    GameState state = game_get_state(p_game);

    // ESC 키로 종료
    if (IsKeyPressed(KEY_ESCAPE)) {
        // 창 닫기 플래그 설정은 raylib이 자동으로 처리
    }

    // 게임 오버 상태에서 재시작
    if (state == GAME_STATE_GAME_OVER) {
        if (IsKeyPressed(KEY_R)) {
            game_restart(p_game);
        }
        return;
    }

    // 플레이 중일 때만 방향 입력 처리
    if (state == GAME_STATE_PLAYING) {
        if (IsKeyPressed(KEY_UP) || IsKeyDown(KEY_UP)) {
            game_set_direction(p_game, DIRECTION_UP);
        } else if (IsKeyPressed(KEY_DOWN) || IsKeyDown(KEY_DOWN)) {
            game_set_direction(p_game, DIRECTION_DOWN);
        } else if (IsKeyPressed(KEY_LEFT) || IsKeyDown(KEY_LEFT)) {
            game_set_direction(p_game, DIRECTION_LEFT);
        } else if (IsKeyPressed(KEY_RIGHT) || IsKeyDown(KEY_RIGHT)) {
            game_set_direction(p_game, DIRECTION_RIGHT);
        }
    }
}

/**
 * 게임 루프의 메인 함수입니다.
 * 
 * @return 프로그램 종료 코드
 */
int main(void) {
    // 렌더러 초기화
    if (!renderer_init()) {
        fprintf(stderr, "Failed to initialize renderer\n");
        return 1;
    }

    // 게임 초기화
    Game game;
    if (!game_init(&game)) {
        fprintf(stderr, "Failed to initialize game\n");
        renderer_close();
        return 1;
    }

    // 게임 루프
    while (!renderer_window_should_close()) {
        // 입력 처리
        process_input(&game);

        // 게임 업데이트
        game_update(&game);

        // 렌더링
        renderer_begin();

        GameState state = game_get_state(&game);
        if (state == GAME_STATE_GAME_OVER) {
            renderer_draw_game_over(&game);
        } else {
            renderer_draw_game(&game);
        }

        renderer_end();
    }

    // 정리
    game_destroy(&game);
    renderer_close();

    return 0;
}

