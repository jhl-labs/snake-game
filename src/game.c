#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "game.h"

bool game_init(Game* p_game) {
    if (p_game == NULL) {
        fprintf(stderr, "[GAME] game_init: NULL pointer\n");
        return false;
    }

    // 게임 상태를 메뉴로 초기화 (인트로 화면 표시)
    p_game->state = GAME_STATE_MENU;
    p_game->mode = GAME_MODE_SINGLE;
    p_game->result = GAME_RESULT_NONE;
    p_game->score = 0;
    p_game->score2 = 0;
    p_game->frame_counter = 0;
    p_game->should_update = false;

    printf("[GAME] Game initialized - state: MENU\n");
    // 뱀과 음식은 게임 시작 시 초기화됨
    return true;
}

void game_destroy(Game* p_game) {
    if (p_game != NULL && p_game->state != GAME_STATE_MENU) {
        printf("[GAME] Destroying game resources\n");
        snake_destroy(&p_game->snake);
        if (p_game->mode == GAME_MODE_VERSUS) {
            snake_destroy(&p_game->snake2);
        }
        // food는 동적 할당이 없으므로 별도 해제 불필요
    }
}

void game_restart(Game* p_game) {
    if (p_game == NULL) {
        return;
    }

    printf("[GAME] ===== Game Restart =====\n");

    // 기존 뱀 해제 (메뉴 상태가 아닌 경우에만)
    if (p_game->state != GAME_STATE_MENU) {
        snake_destroy(&p_game->snake);
        if (p_game->mode == GAME_MODE_VERSUS) {
            snake_destroy(&p_game->snake2);
        }
    }

    // 모드에 따라 시작 위치 결정
    int32_t start_x, start_y;
    if (p_game->mode == GAME_MODE_VERSUS) {
        start_x = P1_START_X;
        start_y = P1_START_Y;
    } else {
        start_x = GRID_WIDTH / 2;
        start_y = GRID_HEIGHT / 2;
    }

    // P1 뱀 초기화
    snake_init(&p_game->snake, start_x, start_y);
    printf("[GAME] P1 Snake spawned at (%d, %d)\n", start_x, start_y);

    // 2인용 모드일 때 P2 뱀 초기화
    if (p_game->mode == GAME_MODE_VERSUS) {
        snake_init(&p_game->snake2, P2_START_X, P2_START_Y);
        // P2는 왼쪽을 바라보도록 초기 방향 설정
        p_game->snake2.direction = DIRECTION_LEFT;
        p_game->snake2.next_direction = DIRECTION_LEFT;
        printf("[GAME] P2 Snake spawned at (%d, %d)\n", P2_START_X, P2_START_Y);
    }

    // 음식 재생성
    food_init(&p_game->food);
    if (p_game->mode == GAME_MODE_VERSUS) {
        food_spawn_versus(&p_game->food, &p_game->snake, &p_game->snake2);
    } else {
        food_spawn(&p_game->food, &p_game->snake);
    }

    // 게임 상태 재설정
    p_game->state = GAME_STATE_PLAYING;
    p_game->result = GAME_RESULT_NONE;
    p_game->score = 0;
    p_game->score2 = 0;
    p_game->frame_counter = 0;
    p_game->should_update = false;
    printf("[GAME] State changed to PLAYING\n");
}

void game_update(Game* p_game) {
    if (p_game == NULL || p_game->state != GAME_STATE_PLAYING) {
        return;
    }

    p_game->frame_counter++;

    // 게임 속도에 맞춰 업데이트
    if (p_game->frame_counter % GAME_SPEED != 0) {
        return;
    }

    // 뱀 이동
    snake_move(&p_game->snake);
    Position head1 = snake_get_head(&p_game->snake);
    printf("[GAME] P1 moved to (%d, %d) | Length: %u | Score: %u\n",
           head1.x, head1.y, p_game->snake.length, p_game->score);

    Position head2;
    if (p_game->mode == GAME_MODE_VERSUS) {
        snake_move(&p_game->snake2);
        head2 = snake_get_head(&p_game->snake2);
        printf("[GAME] P2 moved to (%d, %d) | Length: %u | Score: %u\n",
               head2.x, head2.y, p_game->snake2.length, p_game->score2);
    }

    // 충돌 체크
    bool p1_dead = false;
    bool p2_dead = false;

    // P1 경계 충돌
    if (snake_check_boundary_collision(&p_game->snake)) {
        printf("[GAME] !!! P1 BOUNDARY COLLISION at (%d, %d) !!!\n", head1.x, head1.y);
        p1_dead = true;
    }

    // P1 자기 충돌
    if (!p1_dead && snake_check_self_collision(&p_game->snake)) {
        printf("[GAME] !!! P1 SELF COLLISION at (%d, %d) !!!\n", head1.x, head1.y);
        p1_dead = true;
    }

    if (p_game->mode == GAME_MODE_VERSUS) {
        // P2 경계 충돌
        if (snake_check_boundary_collision(&p_game->snake2)) {
            printf("[GAME] !!! P2 BOUNDARY COLLISION at (%d, %d) !!!\n", head2.x, head2.y);
            p2_dead = true;
        }

        // P2 자기 충돌
        if (!p2_dead && snake_check_self_collision(&p_game->snake2)) {
            printf("[GAME] !!! P2 SELF COLLISION at (%d, %d) !!!\n", head2.x, head2.y);
            p2_dead = true;
        }

        // 머리끼리 충돌 (무승부)
        if (head1.x == head2.x && head1.y == head2.y) {
            printf("[GAME] !!! HEAD-TO-HEAD COLLISION - DRAW !!!\n");
            p1_dead = true;
            p2_dead = true;
        }

        // P1 머리가 P2 몸통과 충돌 (P1 패배)
        if (!p1_dead) {
            for (uint32_t i = 1; i < p_game->snake2.length; i++) {
                Position pos = snake_get_position_at(&p_game->snake2, i);
                if (head1.x == pos.x && head1.y == pos.y) {
                    printf("[GAME] !!! P1 HIT P2 BODY - P1 LOSES !!!\n");
                    p1_dead = true;
                    break;
                }
            }
        }

        // P2 머리가 P1 몸통과 충돌 (P2 패배)
        if (!p2_dead) {
            for (uint32_t i = 1; i < p_game->snake.length; i++) {
                Position pos = snake_get_position_at(&p_game->snake, i);
                if (head2.x == pos.x && head2.y == pos.y) {
                    printf("[GAME] !!! P2 HIT P1 BODY - P2 LOSES !!!\n");
                    p2_dead = true;
                    break;
                }
            }
        }

        // 결과 판정
        if (p1_dead || p2_dead) {
            p_game->state = GAME_STATE_GAME_OVER;
            if (p1_dead && p2_dead) {
                p_game->result = GAME_RESULT_DRAW;
                printf("[GAME] GAME OVER - DRAW!\n");
            } else if (p1_dead) {
                p_game->result = GAME_RESULT_P2_WIN;
                printf("[GAME] GAME OVER - P2 WINS!\n");
            } else {
                p_game->result = GAME_RESULT_P1_WIN;
                printf("[GAME] GAME OVER - P1 WINS!\n");
            }
            return;
        }
    } else {
        // 1인용 모드
        if (p1_dead) {
            printf("[GAME] State changed to GAME_OVER\n");
            p_game->state = GAME_STATE_GAME_OVER;
            return;
        }
    }

    // 음식과의 충돌 체크
    Position food_pos = food_get_position(&p_game->food);

    if (food_is_active(&p_game->food)) {
        bool food_eaten = false;

        // P1이 음식을 먹음
        if (head1.x == food_pos.x && head1.y == food_pos.y) {
            printf("[GAME] *** P1 FOOD EATEN at (%d, %d) ***\n", food_pos.x, food_pos.y);
            snake_grow(&p_game->snake);
            p_game->score += 10;
            printf("[GAME] P1 grew! New length: %u, New score: %u\n",
                   p_game->snake.length, p_game->score);
            food_eaten = true;
        }
        // P2가 음식을 먹음 (2인용 모드)
        else if (p_game->mode == GAME_MODE_VERSUS &&
                 head2.x == food_pos.x && head2.y == food_pos.y) {
            printf("[GAME] *** P2 FOOD EATEN at (%d, %d) ***\n", food_pos.x, food_pos.y);
            snake_grow(&p_game->snake2);
            p_game->score2 += 10;
            printf("[GAME] P2 grew! New length: %u, New score: %u\n",
                   p_game->snake2.length, p_game->score2);
            food_eaten = true;
        }

        if (food_eaten) {
            // 새 음식 생성
            food_deactivate(&p_game->food);
            bool spawn_success;
            if (p_game->mode == GAME_MODE_VERSUS) {
                spawn_success = food_spawn_versus(&p_game->food, &p_game->snake, &p_game->snake2);
            } else {
                spawn_success = food_spawn(&p_game->food, &p_game->snake);
            }

            if (!spawn_success) {
                printf("[GAME] Cannot spawn food - GAME WIN!\n");
                p_game->state = GAME_STATE_GAME_OVER;
            } else {
                Position new_food = food_get_position(&p_game->food);
                printf("[GAME] New food spawned at (%d, %d)\n", new_food.x, new_food.y);
            }
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

static const char* direction_to_string(Direction dir) {
    switch (dir) {
        case DIRECTION_UP: return "UP";
        case DIRECTION_DOWN: return "DOWN";
        case DIRECTION_LEFT: return "LEFT";
        case DIRECTION_RIGHT: return "RIGHT";
        default: return "NONE";
    }
}

void game_set_direction(Game* p_game, Direction direction) {
    if (p_game == NULL || p_game->state != GAME_STATE_PLAYING) {
        return;
    }
    Direction old_dir = p_game->snake.direction;
    snake_set_direction(&p_game->snake, direction);
    if (p_game->snake.next_direction != old_dir) {
        printf("[INPUT] Direction changed: %s -> %s\n",
               direction_to_string(old_dir), direction_to_string(direction));
    }
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

void game_set_mode(Game* p_game, GameMode mode) {
    if (p_game == NULL || p_game->state != GAME_STATE_MENU) {
        return;
    }
    p_game->mode = mode;
}

GameMode game_get_mode(const Game* p_game) {
    if (p_game == NULL) {
        return GAME_MODE_SINGLE;
    }
    return p_game->mode;
}

void game_set_direction_p2(Game* p_game, Direction direction) {
    if (p_game == NULL || p_game->state != GAME_STATE_PLAYING ||
        p_game->mode != GAME_MODE_VERSUS) {
        return;
    }
    Direction old_dir = p_game->snake2.direction;
    snake_set_direction(&p_game->snake2, direction);
    if (p_game->snake2.next_direction != old_dir) {
        printf("[INPUT] P2 Direction changed: %s -> %s\n",
               direction_to_string(old_dir), direction_to_string(direction));
    }
}

uint32_t game_get_score_p2(const Game* p_game) {
    if (p_game == NULL) {
        return 0;
    }
    return p_game->score2;
}

const Snake* game_get_snake_p2(const Game* p_game) {
    if (p_game == NULL || p_game->mode != GAME_MODE_VERSUS) {
        return NULL;
    }
    return &p_game->snake2;
}

GameResult game_get_result(const Game* p_game) {
    if (p_game == NULL) {
        return GAME_RESULT_NONE;
    }
    return p_game->result;
}

