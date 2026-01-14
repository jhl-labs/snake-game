/**
 * @file enemy.c
 * @brief 적 뱀 모델 구현
 *
 * 적 뱀의 초기화, 이동, 충돌 검사 등의 동작을 구현합니다.
 * raylib에 의존하지 않는 순수 모델 레이어입니다.
 */

#include <stdlib.h>
#include <time.h>
#include "enemy.h"

// 정적 변수: 난수 초기화 여부
static bool s_rand_initialized = false;

/**
 * 난수 생성기를 초기화합니다.
 */
static void init_random(void) {
    if (!s_rand_initialized) {
        srand((unsigned int)time(NULL));
        s_rand_initialized = true;
    }
}

/**
 * 주어진 범위 내의 랜덤 정수를 반환합니다.
 * 
 * @param min 최소값 (포함)
 * @param max 최대값 (포함)
 * @return 랜덤 정수
 */
static int32_t random_range(int32_t min, int32_t max) {
    if (min >= max) {
        return min;
    }
    return min + (rand() % (max - min + 1));
}

/**
 * 랜덤 방향을 반환합니다.
 * 
 * @return 랜덤 방향 (UP, DOWN, LEFT, RIGHT 중 하나)
 */
static Direction random_direction(void) {
    int dir = rand() % 4;
    switch (dir) {
        case 0: return DIRECTION_UP;
        case 1: return DIRECTION_DOWN;
        case 2: return DIRECTION_LEFT;
        case 3: return DIRECTION_RIGHT;
        default: return DIRECTION_RIGHT;
    }
}

/**
 * 위치가 플레이어 뱀과 겹치는지 확인합니다.
 * 
 * @param x X 좌표
 * @param y Y 좌표
 * @param p_player_snake 플레이어 뱀 포인터
 * @return 겹침 여부
 */
static bool is_position_on_player(int32_t x, int32_t y, const Snake* p_player_snake) {
    if (p_player_snake == NULL || p_player_snake->p_body == NULL) {
        return false;
    }

    for (uint32_t i = 0; i < p_player_snake->length; i++) {
        Position pos = snake_get_position_at(p_player_snake, i);
        if (pos.x == x && pos.y == y) {
            return true;
        }
    }
    return false;
}

/**
 * 적 뱀의 안전한 시작 위치를 찾습니다.
 * 
 * @param p_player_snake 플레이어 뱀 포인터
 * @param out_x 출력: X 좌표
 * @param out_y 출력: Y 좌표
 * @return 위치 찾기 성공 여부
 */
static bool find_safe_start_position(const Snake* p_player_snake, int32_t* out_x, int32_t* out_y) {
    // 최대 시도 횟수
    const int MAX_ATTEMPTS = 100;
    
    for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
        // 경계에서 충분히 떨어진 위치 선택 (적 뱀이 즉시 죽지 않도록)
        int32_t x = random_range(ENEMY_INITIAL_LENGTH, GRID_WIDTH - ENEMY_INITIAL_LENGTH - 1);
        int32_t y = random_range(ENEMY_INITIAL_LENGTH, GRID_HEIGHT - ENEMY_INITIAL_LENGTH - 1);
        
        // 플레이어와 겹치지 않는지 확인
        bool is_safe = true;
        for (int32_t i = 0; i < ENEMY_INITIAL_LENGTH && is_safe; i++) {
            if (is_position_on_player(x - i, y, p_player_snake)) {
                is_safe = false;
            }
        }
        
        if (is_safe) {
            *out_x = x;
            *out_y = y;
            return true;
        }
    }
    
    return false;
}

bool enemy_init(Enemy* p_enemy, const Snake* p_player_snake) {
    if (p_enemy == NULL) {
        return false;
    }

    init_random();

    // 안전한 시작 위치 찾기
    int32_t start_x, start_y;
    if (!find_safe_start_position(p_player_snake, &start_x, &start_y)) {
        // 안전한 위치를 찾지 못하면 기본 위치 사용
        start_x = GRID_WIDTH / 4;
        start_y = GRID_HEIGHT / 4;
    }

    // 내부 Snake 초기화
    if (!snake_init(&p_enemy->snake, start_x, start_y)) {
        return false;
    }

    // 랜덤 방향 설정
    Direction dir = random_direction();
    snake_set_direction(&p_enemy->snake, dir);

    // 적 뱀 속성 설정
    p_enemy->move_delay = ENEMY_MOVE_DELAY;
    p_enemy->move_counter = 0;
    p_enemy->is_active = true;

    return true;
}

void enemy_destroy(Enemy* p_enemy) {
    if (p_enemy != NULL) {
        snake_destroy(&p_enemy->snake);
        p_enemy->is_active = false;
    }
}

void enemy_update(Enemy* p_enemy) {
    if (p_enemy == NULL || !p_enemy->is_active) {
        return;
    }

    p_enemy->move_counter++;

    // 이동 지연에 따라 업데이트
    if (p_enemy->move_counter % p_enemy->move_delay != 0) {
        return;
    }

    // 일정 확률로 방향 변경 (약 20% 확률)
    if (rand() % 5 == 0) {
        Direction new_dir = random_direction();
        snake_set_direction(&p_enemy->snake, new_dir);
    }

    // 이동
    snake_move(&p_enemy->snake);

    // 경계 충돌 체크 - 충돌 시 비활성화 (나중에 리스폰)
    if (snake_check_boundary_collision(&p_enemy->snake)) {
        p_enemy->is_active = false;
    }

    // 자기 충돌 체크
    if (snake_check_self_collision(&p_enemy->snake)) {
        p_enemy->is_active = false;
    }
}

bool enemy_respawn(Enemy* p_enemy, const Snake* p_player_snake) {
    if (p_enemy == NULL) {
        return false;
    }

    // 기존 뱀 해제
    snake_destroy(&p_enemy->snake);

    // 새로운 위치에서 재초기화
    return enemy_init(p_enemy, p_player_snake);
}

const Snake* enemy_get_snake(const Enemy* p_enemy) {
    if (p_enemy == NULL) {
        return NULL;
    }
    return &p_enemy->snake;
}

bool enemy_is_active(const Enemy* p_enemy) {
    if (p_enemy == NULL) {
        return false;
    }
    return p_enemy->is_active;
}

bool enemy_check_collision_with_player(const Enemy* p_enemy, const Snake* p_player_snake) {
    if (p_enemy == NULL || !p_enemy->is_active) {
        return false;
    }
    if (p_player_snake == NULL || p_player_snake->p_body == NULL) {
        return false;
    }

    // 플레이어 머리 위치
    Position player_head = snake_get_head(p_player_snake);

    // 적 뱀의 모든 부분과 충돌 검사
    const Snake* p_enemy_snake = &p_enemy->snake;
    if (p_enemy_snake->p_body == NULL) {
        return false;
    }

    for (uint32_t i = 0; i < p_enemy_snake->length; i++) {
        Position enemy_pos = snake_get_position_at(p_enemy_snake, i);
        if (player_head.x == enemy_pos.x && player_head.y == enemy_pos.y) {
            return true;
        }
    }

    return false;
}
