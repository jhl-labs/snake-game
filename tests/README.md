# Snake Game - Test Suite

이 디렉토리에는 Snake Game의 유닛 테스트가 포함되어 있습니다.

## 테스트 구조

```
tests/
├── test_snake.c    # Snake 모듈 테스트 (8개)
├── test_food.c     # Food 모듈 테스트 (11개)
├── test_game.c     # Game 로직 테스트 (24개)
└── README.md       # 이 파일
```

## 테스트 실행 방법

### 개별 테스트 실행

```bash
# Snake 테스트
clang -Wall -Wextra -std=c11 -Iinclude tests/test_snake.c src/snake.c -o bin/test_snake.exe
./bin/test_snake.exe

# Food 테스트
clang -Wall -Wextra -std=c11 -Iinclude tests/test_food.c src/food.c src/snake.c -o bin/test_food.exe
./bin/test_food.exe

# Game 테스트
clang -Wall -Wextra -std=c11 -Iinclude tests/test_game.c src/game.c src/snake.c src/food.c -o bin/test_game.exe
./bin/test_game.exe
```

### 전체 테스트 실행

```bash
# 루트 디렉토리에서
bash run_tests.sh
```

## 테스트 커버리지

### 전체 커버리지: 70% (23/33 함수)

| 모듈 | 함수 수 | 테스트된 함수 | 커버리지 |
|------|---------|--------------|---------|
| **snake.c** | 9 | 9 | ✅ 100% |
| **food.c** | 5 | 5 | ✅ 100% |
| **game.c** | 9 | 9 | ✅ 100% |
| **renderer.c** | 7 | 0 | ⚠️ 0% (raylib 의존) |
| **intro.c** | 2 | 0 | ⚠️ 0% (raylib 의존) |
| **main.c** | 1 | 0 | ⚠️ 0% (통합 테스트) |

### 실질적 커버리지: 100% (23/23)
*raylib 의존 함수를 제외한 테스트 가능한 모든 함수가 테스트됨*

## test_snake.c (8개 테스트)

| 테스트 케이스 | 설명 |
|-------------|------|
| `test_snake_init` | 뱀 초기화 검증 |
| `test_snake_init_null` | NULL 포인터 안전성 |
| `test_snake_move` | 기본 이동 로직 |
| `test_snake_set_direction` | 방향 전환 |
| `test_snake_opposite_direction` | 반대 방향 무시 |
| `test_snake_grow` | 성장 로직 |
| `test_snake_boundary_collision` | 경계 충돌 감지 |
| `test_snake_self_collision` | 자기 충돌 감지 |

**커버리지:** 9/9 함수 (100%)

## test_food.c (11개 테스트)

| 테스트 케이스 | 설명 |
|-------------|------|
| `test_food_init` | 음식 초기화 |
| `test_food_init_null` | NULL 안전성 |
| `test_food_spawn_basic` | 기본 생성 로직 |
| `test_food_spawn_not_on_snake` | 뱀과 겹침 방지 |
| `test_food_spawn_null` | NULL 안전성 |
| `test_food_get_position_test` | 위치 반환 |
| `test_food_is_active_test` | 활성화 상태 확인 |
| `test_food_is_active_null` | NULL 안전성 |
| `test_food_deactivate_test` | 비활성화 |
| `test_food_deactivate_null` | NULL 안전성 |
| `test_food_spawn_multiple` | 반복 생성 검증 |

**커버리지:** 5/5 함수 (100%)

## test_game.c (24개 테스트)

### 초기화/정리 (4개)
- `test_game_init` - 게임 초기화
- `test_game_init_null` - NULL 안전성
- `test_game_restart` - 재시작 로직
- `test_game_restart_null` - NULL 안전성

### 게임 업데이트 로직 (7개)
- `test_game_update_frame_counter` - 프레임 카운터
- `test_game_update_snake_movement` - GAME_SPEED 기반 이동
- `test_game_update_boundary_collision` - 경계 충돌 → 게임 오버
- `test_game_update_self_collision` - 자기 충돌 → 게임 오버
- `test_game_update_eat_food` - 음식 먹기 (점수, 성장)
- `test_game_update_null` - NULL 안전성
- `test_game_update_not_playing` - 비활성 상태 처리

### 방향 제어 (3개)
- `test_game_set_direction_test` - 방향 설정
- `test_game_set_direction_not_playing` - 상태 체크
- `test_game_set_direction_null` - NULL 안전성

### Getter 함수 (9개)
- `test_game_get_state_test` + NULL
- `test_game_get_score_test` + NULL
- `test_game_get_snake_test` + NULL
- `test_game_get_food_test` + NULL

### 정리 (1개)
- `test_game_destroy_null` - NULL 안전성

### 통합 테스트 (1개)
- `test_game_integration_full_gameplay` - 전체 게임 플레이 시나리오

**커버리지:** 9/9 함수 (100%)

## 테스트 패턴

### AAA 패턴 (Arrange-Act-Assert)
```c
TEST(snake_move) {
    // Arrange: 테스트 데이터 준비
    Snake snake;
    snake_init(&snake, 10, 10);

    // Act: 실행
    snake_move(&snake);

    // Assert: 검증
    Position head = snake_get_head(&snake);
    ASSERT(head.x == 11);

    // Cleanup: 정리
    snake_destroy(&snake);
    PASS();
}
```

### NULL 안전성 테스트
모든 public 함수에 대해 NULL 포인터 테스트가 포함되어 있습니다.

### 경계 조건 테스트
- 최소값/최대값
- 빈 상태/가득 찬 상태
- 유효/무효 입력

## 테스트 결과

```
=== Test Summary ===
Total Test Suites: 3
Total Tests: 43
├─ Snake: 8/8 PASS ✓
├─ Food:  11/11 PASS ✓
└─ Game:  24/24 PASS ✓

Status: ALL PASSED ✓
```

## 커버리지 상세

### 테스트된 주요 시나리오:

#### ✅ 게임 로직 (Core)
- 뱀 이동 및 방향 전환
- 음식 생성 및 충돌
- 점수 계산
- 게임 오버 조건
- 상태 전환 (MENU → PLAYING → GAME_OVER)

#### ✅ 충돌 감지
- 경계 충돌 (4방향)
- 자기 충돌
- 음식과의 충돌

#### ✅ 메모리 관리
- 동적 할당/해제
- NULL 포인터 안전성
- 재할당 (뱀 성장 시)

#### ✅ Edge Cases
- NULL 입력
- 반대 방향 입력
- 비활성 상태에서의 입력
- 그리드 경계 케이스

#### ⚠️ 테스트 안 된 부분
- Renderer (raylib 의존)
- Intro 화면 (raylib 의존)
- Main 게임 루프 (통합 테스트 필요)

## 테스트 프레임워크

자체 제작 매크로 기반 프레임워크 사용:
```c
#define TEST(name)           // 테스트 정의
#define ASSERT(cond)         // 조건 검증
#define PASS()               // 테스트 성공
#define RUN_TEST(name)       // 테스트 실행
```

**장점:**
- 외부 라이브러리 불필요
- 간단하고 명확한 문법
- 교육 목적에 적합

## 메모리 누수 검사

### Windows (Dr. Memory)
```bash
drmemory -brief -- ./bin/test_snake.exe
drmemory -brief -- ./bin/test_food.exe
drmemory -brief -- ./bin/test_game.exe
```

### Linux (Valgrind)
```bash
valgrind --leak-check=full ./bin/test_snake.exe
```

## 기여 가이드

새로운 테스트 추가 시:
1. 테스트 함수명: `test_<module>_<scenario>`
2. NULL 안전성 테스트 포함
3. 메모리 정리 (`destroy` 호출)
4. ASSERT 조건을 명확하게 작성
5. README 업데이트

---

**마지막 업데이트:** 2026-01-14
**총 테스트 수:** 43
**전체 통과율:** 100%
