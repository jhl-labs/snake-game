---
name: refactoring
description: 코드를 리팩토링합니다. "리팩토링", "코드 개선", "refactor" 요청 시 사용합니다.
allowed-tools: Read, Edit, Bash(make:*)
---

# Refactoring Skill

코드를 리팩토링합니다.

## 원칙

### 리팩토링 대상
- 중복 코드
- 50줄 초과 함수
- 높은 복잡도
- 불명확한 네이밍

### 금지 사항
- 동작하는 코드 불필요 변경
- 요청 없는 대규모 변경
- 테스트 없는 변경

## 프로세스

1. **분석**: 현재 코드 구조 파악
2. **계획**: 변경 목록 및 영향 범위
3. **실행**: 작은 단위로 변경, 빌드 확인

## 패턴

### Extract Function
```c
// Before: 긴 함수
void big_function() { /* 100 lines */ }

// After: 분리된 함수
static void helper_a() { }
static void helper_b() { }
void big_function() {
    helper_a();
    helper_b();
}
```
