# Exercises - 실습 과제

AI Agent 교육을 위한 실습 파일들을 포함합니다.

## 폴더 구조

```
exercises/
├── basics/              # 기본 실습 (버그 수정, TC 작성, 리팩토링)
│   ├── buggy_snake.c
│   ├── test_example.c
│   └── refactor_before.c
├── challenges/          # 도전 과제 (게임 기능 추가)
│   ├── beginner.md      # 초급 (3개)
│   ├── intermediate.md  # 중급 (4개)
│   └── advanced.md      # 고급 (5개)
└── solutions/           # 모범 답안
```

## 실습 목록

### 1. 기본 실습 (basics/)

| 파일 | 용도 | 난이도 |
|------|------|--------|
| `buggy_snake.c` | 버그 수정 실습 (6개 버그) | 초급~중급 |
| `test_example.c` | 테스트 케이스 작성 실습 | 초급 |
| `refactor_before.c` | 코드 리팩토링 실습 | 중급 |

### 2. 도전 과제 (challenges/)

**초급** (beginner.md):
1. 점수 시스템 추가
2. 게임 속도 조절
3. 색상 변경

**중급** (intermediate.md):
4. 벽 통과 모드
5. 장애물 추가
6. 일시정지 기능
7. 유닛 테스트 작성

**고급** (advanced.md):
8. 저장/불러오기 기능
9. AI 자동 플레이
10. 멀티플레이어
11. 성능 최적화
12. 전체 리팩토링

## 사용 방법

### 버그 수정 실습

```bash
# Cursor 또는 Claude Code에서
> "exercises/basics/buggy_snake.c 파일의 버그를 찾아서 수정해줘"
```

### TC 작성 실습

```bash
> "exercises/basics/test_example.c를 참고해서
>  snake_grow 함수의 테스트를 작성해줘"
```

### 리팩토링 실습

```bash
> "exercises/basics/refactor_before.c를
>  프로젝트 코딩 스타일에 맞게 리팩토링해줘"
```

### 도전 과제

```bash
> "exercises/challenges/beginner.md의 Challenge 1을 구현해줘"
```

## 정답 확인

각 실습의 모범 답안은 `solutions/` 디렉토리에 있습니다.

> **주의**: 먼저 직접 해보고, 정답은 막힐 때만 확인하세요!

---

## 관련 실습

- **GDB 디버깅 실습**: `practices/` 폴더 참조 (메모리 버그 디버깅)
- **문서**: `docs/stories/exercises/` 폴더에 자세한 설명 포함
