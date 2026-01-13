# Snake Game - Claude Code Project Guide

> 이 파일은 Claude Code가 프로젝트를 이해하고 효과적으로 작업하기 위한 가이드입니다.

## Project Overview

- **프로젝트명**: Snake Game
- **언어**: C (C11 표준)
- **그래픽 라이브러리**: raylib
- **빌드 시스템**: Make (MinGW-w64)
- **플랫폼**: Windows

## Directory Structure

```
snake-game/
├── src/                # 소스 코드 (.c 파일)
│   ├── main.c          # 진입점, 게임 루프
│   ├── game.c          # 게임 로직
│   ├── snake.c         # 뱀 모델
│   ├── food.c          # 음식 모델
│   └── renderer.c      # 렌더링 (raylib 사용)
├── include/            # 헤더 파일 (.h 파일)
│   ├── game.h
│   ├── snake.h
│   ├── food.h
│   ├── renderer.h
│   └── game_constants.h
├── lib/                # 라이브러리 파일
├── bin/                # 빌드 출력
├── external/           # 외부 라이브러리 (raylib)
└── Makefile
```

## Build Commands

```bash
# 빌드
make

# 정리
make clean

# 재빌드
make clean && make

# 빌드 후 실행
make && ./bin/snake.exe

# 구문 검사만
gcc -fsyntax-only -Iinclude src/*.c
```

## Architecture (MVC Pattern)

### Model Layer (raylib 의존성 없음)
- `snake.c` / `snake.h` - 뱀 데이터 및 조작
- `food.c` / `food.h` - 음식 데이터 및 조작

### Logic Layer (raylib 의존성 없음)
- `game.c` / `game.h` - 게임 규칙, 충돌 감지, 상태 관리

### View Layer (raylib 사용)
- `renderer.c` / `renderer.h` - 화면 렌더링

### Controller Layer (raylib 입력 사용)
- `main.c` - 입력 처리, 게임 루프

## Coding Style

### Naming Conventions
| 대상 | 규칙 | 예시 |
|------|------|------|
| 함수명 | snake_case | `snake_move()`, `game_update()` |
| 타입/구조체 | PascalCase | `Snake`, `GameState` |
| 상수 | UPPER_SNAKE_CASE | `GRID_WIDTH`, `MAX_SNAKE_LENGTH` |
| 전역 변수 | g_ 접두사 | `g_game_state` |
| 정적 변수 | s_ 접두사 | `s_frame_count` |
| 포인터 변수 | p_ 접두사 | `p_snake` |

### Function Comments (Doxygen Style)
```c
/**
 * 뱀을 지정된 방향으로 이동시킵니다.
 *
 * @param p_snake 뱀 객체 포인터
 * @param direction 이동 방향
 * @return 이동 성공 여부
 */
bool snake_move(Snake* p_snake, Direction direction);
```

### Header File Template
```c
#ifndef MODULE_H
#define MODULE_H

#include <stdbool.h>

typedef struct { /* ... */ } TypeName;

void function_name(void);

#endif // MODULE_H
```

## Key Rules

### DO
- NULL 포인터 체크는 함수 시작 부분에서 수행
- 동적 메모리 할당 후 반드시 해제
- 상수는 `#define` 또는 `const`로 정의
- 함수는 50줄 이내로 유지
- 매개변수는 4개 이하로 유지

### DON'T
- raylib을 Model/Logic 계층에서 사용하지 않음
- 전역 변수 남용 금지
- `goto` 문 사용 금지
- 매직 넘버 사용 금지
- 순환 의존성 생성 금지

## Error Handling Pattern

```c
bool function_name(Type* p_param) {
    // NULL 체크
    if (p_param == NULL) {
        fprintf(stderr, "Error: NULL pointer\n");
        return false;
    }

    // 로직 수행
    // ...

    return true;
}
```

## Testing

- 테스트 파일: `test_*.c` 또는 `*_test.c`
- 테스트 패턴: AAA (Arrange-Act-Assert)
- 테스트 실행: `make test`

## Memory Management

- 할당과 해제는 같은 레벨에서 관리
- 해제 후 포인터를 NULL로 설정
- 메모리 누수 검사: Dr. Memory (Windows)

## Common Tasks

### 새 모듈 추가
1. `include/module.h` 헤더 파일 생성
2. `src/module.c` 소스 파일 생성
3. `Makefile`에 소스 파일 추가

### 버그 수정 시
1. 관련 파일 먼저 읽기
2. 최소한의 변경으로 수정
3. 빌드 및 테스트 실행

### 코드 리뷰 요청 시
1. 변경된 파일 목록 확인
2. 코딩 스타일 준수 여부 확인
3. 메모리 누수 가능성 확인

## Useful Snippets

### Game State Check
```c
GameState state = game_get_state(&game);
if (state == GAME_STATE_GAME_OVER) {
    // 게임 오버 처리
}
```

### Direction Change
```c
game_set_direction(&game, DIRECTION_UP);
```

---

*이 프로젝트는 교육 목적으로 제작되었습니다.*
