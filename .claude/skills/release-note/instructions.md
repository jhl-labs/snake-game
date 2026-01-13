# Release Note 생성 지시사항

## 입력
- 버전 번호 (예: v1.0.1)
- 버전이 없으면 사용자에게 물어볼 것

## 실행 단계

### 1. 이전 버전 확인
```bash
git describe --tags --abbrev=0 2>/dev/null || echo "v0.0.0"
```

### 2. 커밋 히스토리 분석
```bash
git log <이전태그>..HEAD --oneline --no-merges
```

### 3. 커밋 분류 규칙
| 접두사 | 카테고리 |
|--------|----------|
| `feat:` | Added |
| `fix:` | Fixed |
| `docs:` | Documentation |
| `refactor:`, `chore:` | Changed |
| `perf:` | Performance |
| `test:` | Tests |
| `BREAKING` | Breaking Changes |
| 기타 | Changed |

### 4. CHANGELOG.md 업데이트
1. CHANGELOG.md 파일 읽기
2. [Unreleased] 섹션의 내용을 새 버전 섹션으로 이동
3. 새 버전 헤더 추가: `## [vX.Y.Z] - YYYY-MM-DD`
4. 분류된 커밋들을 각 카테고리에 추가
5. [Unreleased] 섹션 초기화

### 5. 결과 출력
```
CHANGELOG.md가 업데이트되었습니다.

태그를 생성하고 릴리스하려면:
  git add CHANGELOG.md
  git commit -m "chore: release vX.Y.Z"
  git tag vX.Y.Z
  git push origin main --tags

GitHub에서 자동으로 Release가 생성됩니다.
```

## 주의사항
- 버전 형식은 Semantic Versioning 준수 (vMAJOR.MINOR.PATCH)
- 날짜 형식은 ISO 8601 (YYYY-MM-DD)
- 커밋 메시지에서 유의미한 내용만 추출
- 중복 제거
- 사용자 정보(username, IP 등)가 노출되지 않도록 주의
