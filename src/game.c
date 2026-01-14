#include <assert.h>
#include <stddef.h>
#include "game.h"

bool game_init(Game* p_game) {
    if (p_game == NULL) {
        return false;
    }

    // 게임 상태를 메뉴로 초기화 (인트로 화면 표시)
    p_game->state = GAME_STATE_MENU;
    p_game->score = 0;
    p_game->frame_counter = 0;
    p_game->should_update = false;

    // 뱀과 음식은 게임 시작 시 초기화됨
    return true;
}

void game_destroy(Game* p_game) {
    if (p_game != NULL && p_game->state != GAME_STATE_MENU) {
        snake_destroy(&p_game->snake);
        enemy_destroy(&p_game->enemy);
        // food는 동적 할당이 없으므로 별도 해제 불필요
    }
}

void game_restart(Game* p_game) {
    if (p_game == NULL) {
        return;
    }

    // 기존 객체 해제 (메뉴 상태가 아닌 경우에만)
    if (p_game->state != GAME_STATE_MENU) {
        snake_destroy(&p_game->snake);
        enemy_destroy(&p_game->enemy);
    }

    // 뱀 재초기화
    int32_t start_x = GRID_WIDTH / 2;
    int32_t start_y = GRID_HEIGHT / 2;
    snake_init(&p_game->snake, start_x, start_y);

    // 음식 재생성
    food_init(&p_game->food);
    food_spawn(&p_game->food, &p_game->snake);

    // 적 뱀 초기화
    enemy_init(&p_game->enemy, &p_game->snake);

    // 게임 상태 재설정
    p_game->state = GAME_STATE_PLAYING;
    p_game->score = 0;
    p_game->frame_counter = 0;
    p_game->should_update = false;
}

void game_update(Game* p_game) {
    if (p_game == NULL || p_game->state != GAME_STATE_PLAYING) {
        return;
    }

    p_game->frame_counter++;

    // 적 뱀 업데이트 (매 프레임 - 내부적으로 지연 처리)
    enemy_update(&p_game->enemy);

    // 적 뱀이 비활성화되면 리스폰
    if (!enemy_is_active(&p_game->enemy)) {
        enemy_respawn(&p_game->enemy, &p_game->snake);
    }

    // 게임 속도에 맞춰 플레이어 뱀 업데이트
    if (p_game->frame_counter % GAME_SPEED != 0) {
        return;
    }

    // 뱀 이동
    snake_move(&p_game->snake);

    // 경계 충돌 체크
    if (snake_check_boundary_collision(&p_game->snake)) {
        p_game->state = GAME_STATE_GAME_OVER;
        return;
    }

    // 자기 자신과의 충돌 체크
    if (snake_check_self_collision(&p_game->snake)) {
        p_game->state = GAME_STATE_GAME_OVER;
        return;
    }

    // 적 뱀과의 충돌 체크
    if (enemy_check_collision_with_player(&p_game->enemy, &p_game->snake)) {
        p_game->state = GAME_STATE_GAME_OVER;
        return;
    }

    // 음식과의 충돌 체크
    Position head = snake_get_head(&p_game->snake);
    Position food_pos = food_get_position(&p_game->food);

    if (food_is_active(&p_game->food) &&
        head.x == food_pos.x && head.y == food_pos.y) {
        // 음식을 먹음
        snake_grow(&p_game->snake);
        p_game->score += 10;

        // 새 음식 생성
        food_deactivate(&p_game->food);
        if (!food_spawn(&p_game->food, &p_game->snake)) {
            // 음식을 생성할 수 없으면 게임 승리 (모든 공간을 채움)
            p_game->state = GAME_STATE_GAME_OVER;
        }
    }
}

GameState game_get_state(const Game* p_game) {
    if (p_game == NULL) {
        return GAME_STATE_MENU;
    }
    return p_game->state;
}

uint32_t game_get_score(const Game* p_game) {
    if (p_game == NULL) {
        return 0;
    }
    return p_game->score;
}

void game_set_direction(Game* p_game, Direction direction) {
    if (p_game == NULL || p_game->state != GAME_STATE_PLAYING) {
        return;
    }
    snake_set_direction(&p_game->snake, direction);
}

const Snake* game_get_snake(const Game* p_game) {
    if (p_game == NULL) {
        return NULL;
    }
    return &p_game->snake;
}

const Food* game_get_food(const Game* p_game) {
    if (p_game == NULL) {
        return NULL;
    }
    return &p_game->food;
}

const Enemy* game_get_enemy(const Game* p_game) {
    if (p_game == NULL) {
        return NULL;
    }
    return &p_game->enemy;
}

