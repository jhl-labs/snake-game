#include <stdio.h>
#include "game.h"
#include "renderer.h"
#include "intro.h"
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
        // P1 입력 (방향키)
        if (IsKeyPressed(KEY_UP) || IsKeyDown(KEY_UP)) {
            game_set_direction(p_game, DIRECTION_UP);
        } else if (IsKeyPressed(KEY_DOWN) || IsKeyDown(KEY_DOWN)) {
            game_set_direction(p_game, DIRECTION_DOWN);
        } else if (IsKeyPressed(KEY_LEFT) || IsKeyDown(KEY_LEFT)) {
            game_set_direction(p_game, DIRECTION_LEFT);
        } else if (IsKeyPressed(KEY_RIGHT) || IsKeyDown(KEY_RIGHT)) {
            game_set_direction(p_game, DIRECTION_RIGHT);
        }

        // P2 입력 (WASD) - 2인용 모드에서만
        if (game_get_mode(p_game) == GAME_MODE_VERSUS) {
            if (IsKeyPressed(KEY_W) || IsKeyDown(KEY_W)) {
                game_set_direction_p2(p_game, DIRECTION_UP);
            } else if (IsKeyPressed(KEY_S) || IsKeyDown(KEY_S)) {
                game_set_direction_p2(p_game, DIRECTION_DOWN);
            } else if (IsKeyPressed(KEY_A) || IsKeyDown(KEY_A)) {
                game_set_direction_p2(p_game, DIRECTION_LEFT);
            } else if (IsKeyPressed(KEY_D) || IsKeyDown(KEY_D)) {
                game_set_direction_p2(p_game, DIRECTION_RIGHT);
            }
        }
    }
}

/**
 * 게임 루프의 메인 함수입니다.
 *
 * @return 프로그램 종료 코드
 */
int main(void) {
    printf("[MAIN] ===== Snake Game Starting =====\n");

    // 렌더러 초기화
    printf("[MAIN] Initializing renderer...\n");
    if (!renderer_init()) {
        fprintf(stderr, "Failed to initialize renderer\n");
        return 1;
    }
    printf("[MAIN] Renderer initialized successfully\n");

    // 게임 초기화
    printf("[MAIN] Initializing game...\n");
    Game game;
    if (!game_init(&game)) {
        fprintf(stderr, "Failed to initialize game\n");
        renderer_close();
        return 1;
    }
    printf("[MAIN] Game initialized successfully\n");

    // 인트로 초기화
    intro_init();

    printf("[MAIN] Entering main game loop\n");

    GameState prev_state = GAME_STATE_MENU;

    // 게임 루프
    while (!renderer_window_should_close()) {
        GameState state = game_get_state(&game);

        // 상태 변경 로그
        if (state != prev_state) {
            const char* state_names[] = {"MENU", "PLAYING", "PAUSED", "GAME_OVER"};
            printf("[MAIN] Game state changed: %s -> %s\n",
                   state_names[prev_state], state_names[state]);
            prev_state = state;
        }

        // 인트로 화면에서 게임 시작 확인
        if (state == GAME_STATE_MENU) {
            if (intro_should_start_game()) {
                // 선택된 모드 설정
                GameMode mode = intro_get_selected_mode();
                game_set_mode(&game, mode);
                printf("[MAIN] Starting game - Mode: %s\n",
                       mode == GAME_MODE_SINGLE ? "SINGLE" : "VERSUS");
                game_restart(&game);  // 게임 시작
            }
        } else {
            // 입력 처리
            process_input(&game);

            // 게임 업데이트
            game_update(&game);
        }

        // 렌더링
        renderer_begin();

        if (state == GAME_STATE_MENU) {
            intro_render();
        } else if (state == GAME_STATE_GAME_OVER) {
            renderer_draw_game_over(&game);
        } else {
            renderer_draw_game(&game);
        }

        renderer_end();
    }

    // 정리
    printf("[MAIN] ===== Game Shutting Down =====\n");
    printf("[MAIN] Final Score - P1: %u", game_get_score(&game));
    if (game_get_mode(&game) == GAME_MODE_VERSUS) {
        printf(", P2: %u", game_get_score_p2(&game));
    }
    printf("\n");
    game_destroy(&game);
    printf("[MAIN] Game resources freed\n");
    renderer_close();
    printf("[MAIN] Renderer closed\n");
    printf("[MAIN] ===== Goodbye! =====\n");

    return 0;
}

