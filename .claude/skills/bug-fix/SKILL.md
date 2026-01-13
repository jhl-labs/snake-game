---
name: bug-fix
description: 버그를 분석하고 수정합니다. "버그 수정", "fix", "에러 고쳐" 요청 시 사용합니다.
allowed-tools: Read, Edit, Bash(make:*, gcc:*)
---

# Bug Fix Skill

버그를 분석하고 수정합니다.

## 프로세스

### 1. 정보 수집
- 버그 증상
- 재현 방법
- 에러 메시지
- 관련 파일/함수

### 2. 원인 분석
- 코드 흐름 추적
- 데이터 상태 확인
- 조건 검토

### 3. 수정
- 최소 변경 원칙
- 부작용 방지
- 기존 동작 유지

### 4. 검증
- 빌드 확인
- 문제 해결 확인

## 출력 형식

```markdown
## Bug Fix Report

### Problem
[버그 설명]

### Root Cause
[원인 분석]

### Solution
[수정 내용]

### Verification
- [ ] Build passes
- [ ] Bug resolved
```
