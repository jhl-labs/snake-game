---
alwaysApply: true
description: 코드 작성 시 따라야 할 네이밍 규칙, 파일 구조, 그리고 헤더/소스 파일의 표준 구조를 정의합니다.
---

# 코딩 스타일

## 네이밍 규칙
- **함수명**: `snake_case` 사용 (예: `snake_move()`, `game_update()`)
- **타입명/구조체**: `PascalCase` 사용 (예: `Snake`, `GameState`)
- **상수**: `UPPER_SNAKE_CASE` 사용 (예: `GRID_WIDTH`, `MAX_SNAKE_LENGTH`)
- **전역 변수**: `g_` 접두사 사용 (예: `g_game_state`)
- **정적 변수**: `s_` 접두사 사용 (예: `s_frame_count`)
- **포인터 변수**: `p_` 접두사 사용 (예: `p_snake`)

## 파일 구조
- **소스 파일**: `src/` 디렉토리에 `.c` 파일
- **헤더 파일**: `include/` 디렉토리에 `.h` 파일
- **헤더 가드**: 모든 헤더 파일에 `#ifndef`, `#define`, `#endif` 사용
- **헤더 가드 형식**: `PROJECT_MODULE_H` (예: `SNAKE_GAME_SNAKE_H`)

## 코드 구조

### 헤더 파일 구조
```c
#ifndef MODULE_H
#define MODULE_H

// 시스템 헤더
#include <stdbool.h>
#include <stdint.h>

// 프로젝트 헤더
#include "other_module.h"

// 타입 정의
typedef struct {
    // ...
} TypeName;

// 함수 선언
void function_name(void);

#endif // MODULE_H
```

### 소스 파일 구조
```c
// 시스템 헤더
#include <stdlib.h>
#include <string.h>

// 프로젝트 헤더
#include "module.h"

// 정적 변수
static int s_counter = 0;

// 정적 함수 선언
static void helper_function(void);

// 공개 함수 구현
void function_name(void) {
    // 구현
}
```
