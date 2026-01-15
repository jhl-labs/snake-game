#ifndef SNAKE_GAME_GAME_H
#define SNAKE_GAME_GAME_H

#include <stdbool.h>
#include "snake.h"
#include "food.h"
#include "game_constants.h"

// 게임 상태 구조체
typedef struct {
    Snake snake;            // P1 뱀
    Snake snake2;           // P2 뱀 (2인용 모드)
    Food food;
    GameState state;
    GameMode mode;          // 게임 모드
    GameResult result;      // 게임 결과 (2인용 모드)
    uint32_t score;         // P1 점수
    uint32_t score2;        // P2 점수 (2인용 모드)
    uint32_t frame_counter;
    bool should_update;
} Game;

/**
 * 게임 객체를 초기화합니다.
 * 
 * @param p_game 게임 객체 포인터
 * @return 초기화 성공 여부
 */
bool game_init(Game* p_game);

/**
 * 게임 객체를 해제합니다.
 * 
 * @param p_game 게임 객체 포인터
 */
void game_destroy(Game* p_game);

/**
 * 게임을 재시작합니다.
 * 
 * @param p_game 게임 객체 포인터
 */
void game_restart(Game* p_game);

/**
 * 게임을 업데이트합니다 (한 프레임).
 * 
 * @param p_game 게임 객체 포인터
 */
void game_update(Game* p_game);

/**
 * 게임 상태를 반환합니다.
 * 
 * @param p_game 게임 객체 포인터
 * @return 게임 상태
 */
GameState game_get_state(const Game* p_game);

/**
 * 게임 점수를 반환합니다.
 * 
 * @param p_game 게임 객체 포인터
 * @return 점수
 */
uint32_t game_get_score(const Game* p_game);

/**
 * 뱀의 이동 방향을 설정합니다.
 * 
 * @param p_game 게임 객체 포인터
 * @param direction 이동 방향
 */
void game_set_direction(Game* p_game, Direction direction);

/**
 * 게임의 뱀 객체를 반환합니다 (읽기 전용).
 * 
 * @param p_game 게임 객체 포인터
 * @return 뱀 객체 포인터 (const)
 */
const Snake* game_get_snake(const Game* p_game);

/**
 * 게임의 음식 객체를 반환합니다 (읽기 전용).
 *
 * @param p_game 게임 객체 포인터
 * @return 음식 객체 포인터 (const)
 */
const Food* game_get_food(const Game* p_game);

/**
 * 게임 모드를 설정합니다.
 *
 * @param p_game 게임 객체 포인터
 * @param mode 게임 모드
 */
void game_set_mode(Game* p_game, GameMode mode);

/**
 * 게임 모드를 반환합니다.
 *
 * @param p_game 게임 객체 포인터
 * @return 게임 모드
 */
GameMode game_get_mode(const Game* p_game);

/**
 * P2 뱀의 이동 방향을 설정합니다.
 *
 * @param p_game 게임 객체 포인터
 * @param direction 이동 방향
 */
void game_set_direction_p2(Game* p_game, Direction direction);

/**
 * P2의 점수를 반환합니다.
 *
 * @param p_game 게임 객체 포인터
 * @return P2 점수
 */
uint32_t game_get_score_p2(const Game* p_game);

/**
 * P2 뱀 객체를 반환합니다 (읽기 전용).
 *
 * @param p_game 게임 객체 포인터
 * @return P2 뱀 객체 포인터 (const)
 */
const Snake* game_get_snake_p2(const Game* p_game);

/**
 * 게임 결과를 반환합니다.
 *
 * @param p_game 게임 객체 포인터
 * @return 게임 결과
 */
GameResult game_get_result(const Game* p_game);

#endif // SNAKE_GAME_GAME_H

