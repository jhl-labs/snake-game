---
name: code-review
description: 코드 품질을 검토합니다. "코드 리뷰", "review", "검토" 요청 시 사용합니다.
allowed-tools: Read, Grep, Glob
---

# Code Review Skill

코드를 검토하고 품질 이슈를 분석합니다.

## 체크리스트

### 스타일 검사
- 네이밍 규칙 준수 (snake_case, PascalCase, UPPER_CASE)
- 포인터 변수 p_ 접두사
- Doxygen 스타일 주석
- 들여쓰기 일관성

### 안전성 검사
- NULL 포인터 체크
- 배열 경계 검사
- 메모리 누수 가능성
- 에러 처리

### 아키텍처 검사
- raylib 의존성 분리 (Model/Logic에서 사용 금지)
- 순환 의존성 없음
- 단일 책임 원칙

## 출력 형식

```markdown
## Code Review Result

### File: <filename>

**Overall**: [PASS/NEEDS WORK]

#### Issues Found
- Line X: [description]

#### Recommendations
1. [recommendation]
```
