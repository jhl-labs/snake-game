# Changelog

이 프로젝트의 모든 주요 변경 사항은 이 파일에 기록됩니다.

형식은 [Keep a Changelog](https://keepachangelog.com/ko/1.0.0/)를 따르며,
버전은 [Semantic Versioning](https://semver.org/lang/ko/)을 준수합니다.

## [Unreleased]

### Added
- 향후 추가될 기능들이 여기에 기록됩니다

### Changed
- 향후 변경될 사항들이 여기에 기록됩니다

### Fixed
- 향후 수정될 버그들이 여기에 기록됩니다

---

## [v1.0.0] - 2026-01-14

### Added
- Snake Game 핵심 게임 로직 구현
  - 뱀 이동 및 방향 전환
  - 음식 생성 및 충돌 감지
  - 점수 시스템
- MVC 아키텍처 기반 코드 구조
  - Model: `snake.c`, `food.c`
  - Logic: `game.c`
  - View: `renderer.c` (raylib 사용)
  - Controller: `main.c`
- 유닛 테스트 프레임워크 구현 (`make test`)
- GitHub Actions CI/CD 파이프라인
  - Windows/Linux 빌드 자동화
  - GitHub Release 자동 배포
- `/release` 명령어를 통한 릴리즈 자동화
- SECURITY.md 보안 정책 문서
- Storybook 기반 교육 문서 사이트
- 코딩 스타일 가이드 (CLAUDE.md)

### Fixed
- MSYS2에 git 패키지 추가
- `snake_init` 함수에 NULL 포인터 체크 추가
- Windows 빌드에서 MSYS2 사용하도록 변경
- GitHub Actions workflow 오류 수정

### Changed
- `buggy_snake.c`에서 불필요한 주석 제거
- 미구현 command 파일 정리

### Technical Details
- C11 표준 사용
- raylib 5.0 그래픽 라이브러리
- MinGW-w64 (Windows) / GCC (Linux) 빌드 환경

---

<!--
## Release Note 작성 가이드 (Claude용)

새 버전 릴리스 시 아래 명령어로 Claude에게 요청하세요:

```
최근 커밋들을 분석해서 CHANGELOG.md의 [Unreleased] 섹션을
새 버전 [vX.Y.Z]로 업데이트해줘.

포함할 내용:
- Added: 새로 추가된 기능
- Changed: 변경된 기능
- Fixed: 수정된 버그
- Removed: 제거된 기능
- Security: 보안 관련 변경
```

### 버전 규칙
- Major (X): 호환되지 않는 API 변경
- Minor (Y): 하위 호환되는 기능 추가
- Patch (Z): 하위 호환되는 버그 수정

### 태그 생성
```bash
git tag v1.0.1
git push origin v1.0.1
```
-->
