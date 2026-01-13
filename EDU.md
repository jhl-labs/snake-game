# AI Agent 교육 커리큘럼

> Cursor와 Claude Code를 활용한 AI 기반 개발 실습 가이드

**총 교육 시간**: 4시간
**대상**: 개발자, AI 도구 활용에 관심있는 분
**실습 프로젝트**: Snake Game (C + raylib)

---

## 목차

1. [Chapter 1: 소개 및 환경 설정](#chapter-1-소개-및-환경-설정-30분)
2. [Chapter 2: Agent 기본 개념](#chapter-2-agent-기본-개념-30분)
3. [Chapter 3: Rule 파일 작성](#chapter-3-rule-파일-작성-45분)
4. [Chapter 4: Command 파일 작성](#chapter-4-command-파일-작성-45분)
5. [Chapter 5: claude.md 및 agent.md](#chapter-5-claudemd-및-agentmd-30분)
6. [Chapter 6: Subagent와 Agent Skills](#chapter-6-subagent와-agent-skills-30분)
7. [Chapter 7: Test Case 작성 실습](#chapter-7-test-case-작성-실습-30분)
8. [Chapter 8: 버그 수정 실습](#chapter-8-버그-수정-실습-30분)

---

## Chapter 1: 소개 및 환경 설정 (30분)

### 1.1 AI Agent란?

AI Agent는 사용자의 의도를 이해하고 자율적으로 작업을 수행하는 AI 시스템입니다.

**핵심 특징:**
- 컨텍스트 이해: 프로젝트 구조, 코드 스타일을 파악
- 자율적 실행: 파일 읽기/쓰기, 명령어 실행
- 반복적 개선: 결과를 확인하고 수정

### 1.2 Cursor vs Claude Code 비교

| 특징 | Cursor | Claude Code |
|------|--------|-------------|
| 인터페이스 | GUI (VS Code 기반) | CLI (터미널) |
| 규칙 파일 | `.cursor/rules/*.mdc` | `CLAUDE.md`, `.claude/` |
| 커맨드 | `.cursor/commands/*.md` | `/` 슬래시 커맨드 |
| 서브에이전트 | Background Agent | Task tool + subagent |
| 모델 | 다양한 모델 선택 | Claude 모델 전용 |

### 1.3 환경 설정

#### Cursor 설치
```bash
# 1. https://cursor.com 에서 다운로드
# 2. VS Code 확장 및 설정 마이그레이션
# 3. API 키 설정 (필요시)
```

#### Claude Code 설치
```bash
# npm을 통한 설치
npm install -g @anthropic-ai/claude-code

# 또는 직접 실행
npx @anthropic-ai/claude-code
```

### 1.4 실습 프로젝트 구조 확인

```
snake-game/
├── .cursor/
│   ├── rules/          # Cursor 규칙 파일
│   │   ├── coding-style.mdc
│   │   ├── architecture.mdc
│   │   └── ...
│   └── commands/       # Cursor 커맨드 파일
│       ├── build.md
│       ├── run.md
│       └── ...
├── src/                # 소스 코드
├── include/            # 헤더 파일
├── CLAUDE.md           # Claude Code 규칙 (생성 예정)
└── Makefile
```

### 실습 1-1: 프로젝트 탐색

```bash
# Cursor에서
# 1. 프로젝트 폴더 열기
# 2. Cmd+K로 AI와 대화 시작
# 3. "이 프로젝트의 구조를 설명해줘" 입력

# Claude Code에서
cd snake-game
claude
# > "이 프로젝트의 구조를 설명해줘"
```

---

## Chapter 2: Agent 기본 개념 (30분)

### 2.1 Agent의 동작 원리

```
┌─────────────────────────────────────────────────────────┐
│                      Agent Loop                          │
│                                                          │
│   ┌──────────┐    ┌──────────┐    ┌──────────┐         │
│   │  사용자   │───▶│   LLM    │───▶│  도구    │         │
│   │  입력    │    │  추론    │    │  실행    │         │
│   └──────────┘    └──────────┘    └──────────┘         │
│        ▲                               │                │
│        └───────────────────────────────┘                │
│                  결과 피드백                             │
└─────────────────────────────────────────────────────────┘
```

### 2.2 Agent가 사용하는 도구들

| 도구 | Cursor | Claude Code | 설명 |
|------|--------|-------------|------|
| 파일 읽기 | Read | Read | 파일 내용 확인 |
| 파일 쓰기 | Write | Write | 새 파일 생성 |
| 파일 수정 | Edit | Edit | 기존 파일 수정 |
| 검색 | Search | Grep/Glob | 코드/파일 검색 |
| 터미널 | Terminal | Bash | 명령어 실행 |
| 웹 검색 | Web | WebSearch | 인터넷 검색 |

### 2.3 컨텍스트의 중요성

Agent의 성능은 **컨텍스트**에 크게 의존합니다:

1. **프로젝트 컨텍스트**: 디렉토리 구조, 파일 목록
2. **코드 컨텍스트**: 현재 열린 파일, 관련 파일
3. **규칙 컨텍스트**: Rule 파일에 정의된 지침
4. **대화 컨텍스트**: 이전 대화 히스토리

### 실습 2-1: Agent 동작 관찰

```bash
# Claude Code에서 verbose 모드로 실행
claude --verbose

# 다음 요청을 하고 Agent의 동작을 관찰하세요
> "snake.c 파일에서 뱀이 이동하는 함수를 찾아줘"

# 관찰 포인트:
# 1. 어떤 도구를 사용하는가?
# 2. 어떤 순서로 실행하는가?
# 3. 결과를 어떻게 해석하는가?
```

### 실습 2-2: 멀티턴 대화

```bash
# 연속적인 대화로 복잡한 작업 수행
> "game.c 파일을 읽어줘"
> "game_update 함수가 하는 일을 설명해줘"
> "이 함수에서 충돌 감지는 어떻게 처리되고 있어?"
```

---

## Chapter 3: Rule 파일 작성 (45분)

### 3.1 Cursor Rule 파일 (.mdc)

Rule 파일은 Agent에게 프로젝트별 지침을 제공합니다.

**파일 위치**: `.cursor/rules/*.mdc`

**기본 구조**:
```markdown
---
alwaysApply: true
description: 이 규칙의 설명
---

# 규칙 제목

## 섹션 1
- 규칙 내용

## 섹션 2
- 규칙 내용
```

### 3.2 Rule 파일 메타데이터

| 필드 | 설명 | 값 |
|------|------|-----|
| `alwaysApply` | 항상 적용 여부 | `true` / `false` |
| `description` | 규칙 설명 | 문자열 |
| `globs` | 적용 파일 패턴 | `["*.c", "*.h"]` |

### 3.3 기존 Rule 파일 분석

#### coding-style.mdc 분석
```markdown
---
alwaysApply: true
description: 코드 작성 시 따라야 할 네이밍 규칙...
---

# 코딩 스타일

## 네이밍 규칙
- **함수명**: `snake_case` 사용
- **타입명/구조체**: `PascalCase` 사용
- **상수**: `UPPER_SNAKE_CASE` 사용
```

**핵심 포인트**:
- 구체적인 예시 제공 (`snake_move()`, `GameState`)
- 일관된 규칙 적용
- 쉽게 따라할 수 있는 형식

### 3.4 효과적인 Rule 작성 팁

1. **구체적으로 작성하기**
   ```markdown
   # 나쁜 예
   - 좋은 코드를 작성하세요

   # 좋은 예
   - 함수는 20줄을 넘지 않도록 합니다
   - 매개변수는 최대 4개까지만 허용합니다
   ```

2. **예시 코드 포함하기**
   ```markdown
   ## 에러 처리 패턴
   ```c
   if (p_data == NULL) {
       fprintf(stderr, "Error: NULL pointer\n");
       return false;
   }
   ```
   ```

3. **컨텍스트 제공하기**
   ```markdown
   ## 프로젝트 개요
   - **언어**: C (C11 표준)
   - **라이브러리**: raylib
   - **플랫폼**: Windows
   ```

### 실습 3-1: 새로운 Rule 파일 생성

테스트 관련 규칙을 정의하는 새 Rule 파일을 생성합니다.

```markdown
# .cursor/rules/testing.mdc 파일 생성

---
alwaysApply: false
description: 테스트 코드 작성 규칙을 정의합니다.
globs: ["test_*.c", "*_test.c"]
---

# 테스트 규칙

## 테스트 함수 네이밍
- `test_` 접두사 사용
- 테스트 대상 함수명 포함
- 테스트 시나리오 설명

예시:
- `test_snake_move_up()` - 뱀이 위로 이동하는지 테스트
- `test_snake_collision_wall()` - 벽 충돌 테스트

## 테스트 구조
```c
void test_function_name(void) {
    // Arrange - 준비
    Snake snake;
    snake_init(&snake, 5, 5);

    // Act - 실행
    snake_move(&snake, DIRECTION_UP);

    // Assert - 검증
    assert(snake.head_y == 4);
}
```

## 테스트 커버리지
- 모든 public 함수는 최소 하나의 테스트를 가져야 함
- 경계 조건 테스트 필수
- 에러 케이스 테스트 필수
```

### 실습 3-2: Rule 적용 확인

```bash
# Cursor에서 새 규칙이 적용되는지 확인
# 1. test_snake.c 파일 생성 요청
# 2. Agent가 testing.mdc 규칙을 따르는지 확인

# Cursor Chat에서:
> "snake_move 함수에 대한 테스트 파일을 만들어줘"
```

---

## Chapter 4: Command 파일 작성 (45분)

### 4.1 Cursor Command 파일

Command 파일은 자주 사용하는 작업을 정의합니다.

**파일 위치**: `.cursor/commands/*.md`

**기본 구조**:
```markdown
---
name: 커맨드 이름
command: 실행할 명령어
description: 커맨드 설명
cwd: ${workspaceFolder}
---
```

### 4.2 기존 Command 분석

#### build.md
```markdown
---
name: 빌드 프로젝트
command: make
description: Makefile을 사용하여 프로젝트를 빌드합니다.
cwd: ${workspaceFolder}
---
```

#### build-and-run.md
```markdown
---
name: 빌드 후 실행
command: make && ./bin/snake.exe
description: 프로젝트를 빌드하고 실행합니다.
cwd: ${workspaceFolder}
---
```

### 4.3 Claude Code 슬래시 커맨드

Claude Code는 내장 슬래시 커맨드를 제공합니다:

| 커맨드 | 설명 |
|--------|------|
| `/help` | 도움말 표시 |
| `/clear` | 대화 초기화 |
| `/compact` | 컨텍스트 압축 |
| `/init` | CLAUDE.md 생성 |
| `/review` | 코드 리뷰 |
| `/pr` | PR 생성 |
| `/commit` | 커밋 생성 |

### 4.4 복합 Command 작성

여러 작업을 조합한 Command를 만들 수 있습니다.

### 실습 4-1: 새로운 Command 파일 생성

```markdown
# .cursor/commands/test.md 파일 생성

---
name: 테스트 실행
command: make test
description: 모든 유닛 테스트를 실행합니다.
cwd: ${workspaceFolder}
---
```

```markdown
# .cursor/commands/lint.md 파일 생성

---
name: 코드 검사
command: cppcheck --enable=all src/
description: 정적 분석으로 코드 품질을 검사합니다.
cwd: ${workspaceFolder}
---
```

### 실습 4-2: Agent에게 Command 생성 요청

```bash
# Cursor Chat에서:
> "메모리 누수를 체크하는 command 파일을 만들어줘.
   Dr. Memory를 사용하고, bin/snake.exe를 분석해야 해"

# 예상 결과 (.cursor/commands/memory-check.md):
---
name: 메모리 누수 검사
command: drmemory.exe -logdir ./logs -- ./bin/snake.exe
description: Dr. Memory로 메모리 누수를 검사합니다.
cwd: ${workspaceFolder}
---
```

### 실습 4-3: Claude Code에서 커스텀 커맨드 사용

```bash
# Claude Code 실행
claude

# 내장 커맨드 사용
> /init    # CLAUDE.md 생성
> /help    # 도움말 확인

# 프로젝트 빌드 요청
> "프로젝트를 빌드해줘"
```

---

## Chapter 5: claude.md 및 agent.md (30분)

### 5.1 CLAUDE.md 파일

`CLAUDE.md`는 Claude Code를 위한 프로젝트 규칙 파일입니다.

**파일 위치**: 프로젝트 루트 (`./CLAUDE.md`)

**역할**:
- 프로젝트 컨텍스트 제공
- 코딩 스타일 정의
- 자주 사용하는 명령어 안내
- 프로젝트별 특수 지침

### 5.2 CLAUDE.md 구조

```markdown
# Project: Snake Game

## Overview
raylib을 사용한 C 언어 스네이크 게임 프로젝트입니다.

## Build Commands
- `make` - 프로젝트 빌드
- `make clean` - 빌드 파일 정리
- `make run` - 빌드 후 실행

## Code Style
- 함수명: snake_case
- 타입명: PascalCase
- 상수: UPPER_SNAKE_CASE

## Architecture
- Model: snake.c, food.c
- View: renderer.c
- Controller: main.c
- Logic: game.c

## Testing
- 테스트 파일: test_*.c
- 테스트 실행: make test

## Important Notes
- raylib 의존성은 Model 계층에서 사용하지 않음
- 모든 포인터는 NULL 체크 필수
- 동적 메모리 할당 시 반드시 해제
```

### 5.3 agent.md 파일

`agent.md`는 Agent의 행동 지침을 정의합니다.

```markdown
# Agent Instructions

## 작업 방식
1. 코드 수정 전 반드시 관련 파일 읽기
2. 변경 사항은 최소화
3. 기존 코드 스타일 유지

## 금지 사항
- 불필요한 리팩토링 금지
- 주석 없이 복잡한 로직 추가 금지
- 테스트 없이 기능 추가 금지

## 선호 사항
- 간결하고 명확한 코드
- 의미 있는 변수명
- 작은 단위의 커밋
```

### 실습 5-1: CLAUDE.md 생성

```bash
# Claude Code에서
claude
> /init

# 또는 직접 요청
> "이 프로젝트에 맞는 CLAUDE.md 파일을 생성해줘"
```

### 실습 5-2: CLAUDE.md 커스터마이징

```bash
# 프로젝트 특성에 맞게 수정
> "CLAUDE.md에 raylib 사용 규칙을 추가해줘"
> "CLAUDE.md에 테스트 작성 가이드를 추가해줘"
```

---

## Chapter 6: Subagent와 Agent Skills (30분)

### 6.1 Subagent 개념

Subagent는 특정 작업에 특화된 보조 Agent입니다.

**Claude Code의 Subagent 유형**:

| 유형 | 설명 | 용도 |
|------|------|------|
| `Bash` | 명령어 실행 전문 | git, 빌드, 터미널 작업 |
| `Explore` | 코드베이스 탐색 | 파일 검색, 코드 분석 |
| `Plan` | 구현 계획 수립 | 아키텍처 설계 |
| `general-purpose` | 범용 작업 | 복잡한 멀티스텝 작업 |

### 6.2 Subagent 동작 방식

```
┌───────────────────────────────────────────────────────┐
│                    Main Agent                          │
│                                                        │
│   "snake.c에서 버그를 찾아서 수정해줘"                │
│                        │                               │
│                        ▼                               │
│   ┌──────────────────────────────────────┐            │
│   │  Task: Explore subagent               │            │
│   │  "버그가 있을 만한 코드 탐색"         │            │
│   └──────────────────────────────────────┘            │
│                        │                               │
│                        ▼                               │
│   ┌──────────────────────────────────────┐            │
│   │  Task: general-purpose subagent       │            │
│   │  "발견된 버그 분석 및 수정"           │            │
│   └──────────────────────────────────────┘            │
│                        │                               │
│                        ▼                               │
│   ┌──────────────────────────────────────┐            │
│   │  Task: Bash subagent                  │            │
│   │  "빌드 및 테스트 실행"                │            │
│   └──────────────────────────────────────┘            │
└───────────────────────────────────────────────────────┘
```

### 6.3 Agent Skills

Skills는 Agent가 특정 작업을 수행할 수 있는 능력입니다.

**주요 Skills**:
- **코드 리뷰**: 코드 품질 분석
- **버그 수정**: 문제 식별 및 해결
- **리팩토링**: 코드 구조 개선
- **테스트 작성**: 유닛 테스트 생성
- **문서화**: 주석 및 문서 작성

### 6.4 Cursor Background Agent

Cursor는 Background Agent를 통해 장기 실행 작업을 처리합니다:

1. **Composer**: 복잡한 코드 생성
2. **Agent Mode**: 자율적 작업 수행
3. **Multi-file Edit**: 여러 파일 동시 수정

### 실습 6-1: Subagent 활용

```bash
# Claude Code에서
> "프로젝트 전체에서 메모리 누수 가능성이 있는 코드를 찾아줘"

# Agent가 Explore subagent를 사용하여 탐색
# 결과를 분석하고 문제점 보고
```

### 실습 6-2: 복합 작업 요청

```bash
# 여러 Subagent가 협력하는 작업
> "snake.c 파일을 리팩토링하고, 테스트를 작성하고, 빌드해서 확인해줘"

# 예상 동작:
# 1. Explore: 현재 코드 분석
# 2. Plan: 리팩토링 계획 수립
# 3. Edit: 코드 수정
# 4. Write: 테스트 파일 생성
# 5. Bash: 빌드 및 테스트 실행
```

---

## Chapter 7: Test Case 작성 실습 (30분)

### 7.1 Agent를 활용한 TC 작성

Agent에게 테스트 케이스 작성을 요청하는 방법:

1. **대상 함수 지정**: 테스트할 함수 명시
2. **시나리오 제공**: 테스트하려는 상황 설명
3. **기대 결과 명시**: 예상되는 동작 설명

### 7.2 테스트 케이스 요청 템플릿

```
다음 함수에 대한 테스트 케이스를 작성해줘:
- 함수: snake_move()
- 파일: src/snake.c
- 테스트 시나리오:
  1. 정상적인 이동 (상하좌우)
  2. 벽 충돌
  3. 자기 자신과 충돌
  4. 경계 조건 (맵 끝에서 이동)
```

### 7.3 효과적인 TC 요청 방법

**좋은 요청**:
```
snake_move 함수에 대한 유닛 테스트를 작성해줘.
- AAA 패턴 (Arrange-Act-Assert) 사용
- 각 방향별 이동 테스트
- 벽 충돌 시 게임 오버 확인
- assert 매크로 사용
```

**나쁜 요청**:
```
테스트 만들어줘
```

### 실습 7-1: 기본 TC 작성

```bash
# Cursor 또는 Claude Code에서
> "snake.h에 정의된 snake_init 함수에 대한 테스트를 작성해줘.
   초기 위치가 올바르게 설정되는지,
   초기 길이가 1인지,
   초기 방향이 올바른지 테스트해야 해"
```

### 실습 7-2: 복잡한 시나리오 TC

```bash
> "game.c의 game_update 함수에 대한 통합 테스트를 작성해줘.
   다음 시나리오를 테스트해야 해:
   1. 음식을 먹으면 점수가 증가하는지
   2. 음식을 먹으면 뱀 길이가 증가하는지
   3. 벽에 충돌하면 게임 오버가 되는지"
```

### 실습 7-3: TC 리뷰 요청

```bash
# 작성된 TC 리뷰
> "test_snake.c 파일의 테스트 케이스를 리뷰해줘.
   - 누락된 테스트 시나리오가 있는지
   - 코드 품질은 어떤지
   - 개선할 점은 없는지 알려줘"
```

---

## Chapter 8: 버그 수정 실습 (30분)

### 8.1 Agent를 활용한 버그 수정 프로세스

```
┌─────────────────────────────────────────────────────┐
│              Bug Fix Workflow                        │
│                                                      │
│   1. 버그 리포트 → 2. 원인 분석 → 3. 수정 계획     │
│                                                      │
│   4. 코드 수정 → 5. 테스트 검증 → 6. 리뷰          │
└─────────────────────────────────────────────────────┘
```

### 8.2 버그 리포트 형식

Agent에게 버그를 효과적으로 설명하는 방법:

```markdown
## 버그 리포트

### 증상
게임 실행 중 음식을 먹으면 프로그램이 크래시됨

### 재현 방법
1. 게임 시작
2. 뱀을 음식 방향으로 이동
3. 음식을 먹는 순간 크래시

### 예상 동작
뱀 길이가 증가하고 새 음식이 생성되어야 함

### 실제 동작
Segmentation fault 발생

### 관련 파일
- src/snake.c
- src/food.c
```

### 8.3 디버깅 요청 방법

```bash
# 구체적인 증상 설명
> "snake.c의 snake_grow 함수에서 메모리 오류가 발생해.
   뱀의 길이가 10 이상이 되면 크래시가 발생하는데,
   원인을 찾아서 수정해줘"

# 에러 메시지 제공
> "다음 에러가 발생해:
   'Segmentation fault at snake.c:45'
   이 문제를 분석하고 수정해줘"
```

### 실습 8-1: 의도적 버그 삽입 및 수정

```c
// snake.c에 의도적으로 버그 삽입
// 원본 코드
void snake_grow(Snake* p_snake) {
    if (p_snake->length < MAX_SNAKE_LENGTH) {
        p_snake->length++;
    }
}

// 버그가 있는 코드 (경계 체크 누락)
void snake_grow(Snake* p_snake) {
    p_snake->length++;  // MAX_SNAKE_LENGTH 체크 없음!
}
```

```bash
# Agent에게 버그 수정 요청
> "snake_grow 함수에서 뱀 길이가 MAX_SNAKE_LENGTH를 초과할 때
   배열 오버플로우가 발생해. 이 버그를 수정해줘"
```

### 실습 8-2: 로직 버그 수정

```bash
# 충돌 감지 버그
> "뱀이 자기 몸통과 충돌해도 게임 오버가 되지 않는 버그가 있어.
   game.c의 check_collision 함수를 확인하고 수정해줘"
```

### 실습 8-3: 복합 버그 수정

```bash
# 여러 파일에 걸친 버그
> "게임 재시작 시 점수가 초기화되지 않는 버그가 있어.
   관련된 모든 파일을 확인하고 수정해줘.
   수정 후 테스트도 추가해줘"
```

---

## 부록 A: 유용한 프롬프트 모음

### 코드 분석
```
- "이 함수가 하는 일을 설명해줘"
- "이 코드의 시간 복잡도는?"
- "이 코드에서 개선할 점을 찾아줘"
```

### 코드 생성
```
- "~하는 함수를 만들어줘"
- "~패턴으로 리팩토링해줘"
- "~에 대한 테스트를 작성해줘"
```

### 버그 수정
```
- "이 에러의 원인을 찾아줘"
- "~가 동작하지 않는 이유를 분석해줘"
- "이 메모리 누수를 수정해줘"
```

### 문서화
```
- "이 함수에 주석을 추가해줘"
- "README를 업데이트해줘"
- "API 문서를 생성해줘"
```

---

## 부록 B: 트러블슈팅

### Agent가 잘못된 코드를 생성할 때

1. **더 구체적인 컨텍스트 제공**
   ```
   "src/snake.c 파일을 읽고, snake_move 함수를 수정해줘"
   ```

2. **기존 패턴 참조 요청**
   ```
   "기존 코딩 스타일을 따라서 작성해줘"
   ```

3. **단계별 진행**
   ```
   "먼저 현재 코드를 분석하고, 그 다음에 수정해줘"
   ```

### Agent가 지시를 따르지 않을 때

1. **Rule 파일 확인**
   - `.cursor/rules/` 파일이 올바르게 작성되었는지 확인
   - `alwaysApply: true` 설정 확인

2. **명시적 지시**
   ```
   "반드시 snake_case 네이밍 규칙을 따라줘"
   ```

3. **예시 제공**
   ```
   "다음 형식으로 작성해줘:
    void function_name(Type* p_param) {
        // 구현
    }"
   ```

---

## 부록 C: 실습 체크리스트

### Chapter 1-2 완료 체크
- [ ] Cursor 설치 및 프로젝트 열기
- [ ] Claude Code 설치 및 실행
- [ ] 프로젝트 구조 탐색
- [ ] Agent와 기본 대화 수행

### Chapter 3-4 완료 체크
- [ ] 기존 Rule 파일 분석
- [ ] 새로운 Rule 파일 생성
- [ ] 기존 Command 파일 분석
- [ ] 새로운 Command 파일 생성

### Chapter 5-6 완료 체크
- [ ] CLAUDE.md 파일 생성
- [ ] CLAUDE.md 커스터마이징
- [ ] Subagent 동작 이해
- [ ] 복합 작업 요청 실습

### Chapter 7-8 완료 체크
- [ ] 기본 TC 작성 실습
- [ ] 복잡한 시나리오 TC 작성
- [ ] 의도적 버그 수정 실습
- [ ] 복합 버그 수정 실습

---

## 참고 자료

### 공식 문서
- [Cursor 공식 문서](https://cursor.com/docs)
- [Claude Code GitHub](https://github.com/anthropics/claude-code)
- [Anthropic API 문서](https://docs.anthropic.com)

### 추가 학습
- raylib 공식 문서: https://www.raylib.com
- C 프로그래밍 가이드
- 유닛 테스팅 베스트 프랙티스

---

**작성일**: 2024
**버전**: 1.0
**교육 시간**: 4시간
