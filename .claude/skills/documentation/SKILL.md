---
name: documentation
description: 코드를 문서화합니다. "문서 작성", "주석 추가", "document" 요청 시 사용합니다.
allowed-tools: Read, Edit
---

# Documentation Skill

코드를 문서화합니다.

## 문서화 유형

### 함수 문서 (Doxygen)
```c
/**
 * Brief description.
 *
 * @param p_snake Pointer to Snake structure
 * @param direction Movement direction
 * @return true if successful, false otherwise
 */
bool snake_move(Snake* p_snake, Direction direction);
```

### 파일 헤더
```c
/**
 * @file snake.c
 * @brief Snake model implementation
 */
```

### 인라인 주석
- 복잡한 알고리즘 설명
- 비직관적인 로직 설명
- 중요한 결정 사항

## 규칙

### 문서화 필요
- 모든 공개 함수
- 복잡한 알고리즘
- 비직관적 로직

### 문서화 불필요
```c
// BAD - 자명한 코드
i++;  // i를 1 증가

// GOOD - WHY 설명
i++;  // 헤더 행 건너뛰기
```
