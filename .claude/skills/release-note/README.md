---
description: Release note를 생성하고 CHANGELOG.md를 업데이트합니다. "릴리스 노트", "release", "버전 출시" 요청 시 사용합니다.
---

# Release Note Skill

## 사용법

```
/release-note v1.0.1
```

## 동작

1. 이전 버전 태그 이후의 커밋들을 분석합니다
2. 커밋 메시지를 카테고리별로 분류합니다:
   - `feat:` → Added
   - `fix:` → Fixed
   - `chore:`, `refactor:` → Changed
   - `docs:` → Documentation
   - `BREAKING CHANGE` → Breaking Changes
3. CHANGELOG.md의 [Unreleased] 섹션을 새 버전으로 업데이트합니다
4. 태그 생성 및 푸시 명령어를 안내합니다

## 출력 예시

```markdown
## [v1.0.1] - 2026-01-14

### Added
- 새로운 기능 설명

### Fixed
- 버그 수정 내용

### Changed
- 변경된 사항
```
