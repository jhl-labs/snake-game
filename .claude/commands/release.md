---
description: 새 버전을 릴리스합니다. CHANGELOG 업데이트, 태그 생성, GitHub Release 배포까지 자동화합니다.
argument-hint: <version> (예: v1.0.1)
allowed-tools:
  - Bash
  - Read
  - Edit
---

# Release 명령어

버전 인자: $ARGUMENTS

## 실행 단계

### 1. 버전 확인
- 인자로 버전이 제공되지 않았다면 사용자에게 버전을 물어봐
- 버전 형식은 `vX.Y.Z` (예: v1.0.0, v1.2.3)

### 2. 이전 버전 태그 확인
```bash
git describe --tags --abbrev=0 2>/dev/null || echo "태그 없음"
```

### 3. 커밋 히스토리 분석
이전 태그 이후의 커밋들을 분석해서 카테고리별로 분류:

```bash
git log <이전태그>..HEAD --oneline --no-merges
```

분류 규칙:
| 커밋 접두사 | 카테고리 |
|-------------|----------|
| `feat:` | Added |
| `fix:` | Fixed |
| `docs:` | Documentation |
| `refactor:`, `chore:` | Changed |
| `perf:` | Performance |
| `test:` | Tests |
| `BREAKING` | Breaking Changes |

### 4. CHANGELOG.md 업데이트
1. CHANGELOG.md 파일 읽기
2. `[Unreleased]` 섹션 아래에 새 버전 섹션 추가
3. 형식:
```markdown
## [vX.Y.Z] - YYYY-MM-DD

### Added
- 새 기능들...

### Fixed
- 버그 수정들...

### Changed
- 변경 사항들...
```

### 5. 커밋 및 태그 생성
```bash
git add CHANGELOG.md
git commit -m "chore: release vX.Y.Z"
git tag vX.Y.Z
git push origin main --tags
```

### 6. 완료 메시지
```
✅ Release vX.Y.Z 완료!

GitHub Actions가 자동으로:
- GitHub Release 생성
- Windows/Linux 바이너리 빌드 및 첨부

확인: https://github.com/jhl-labs/snake-game/releases
```

## 주의사항
- Semantic Versioning 준수 (vMAJOR.MINOR.PATCH)
- 날짜는 ISO 8601 형식 (YYYY-MM-DD)
- 사용자 정보가 코드에 노출되지 않도록 주의
