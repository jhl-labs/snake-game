# AI Agent 교육 문서 사이트

Storybook을 사용한 AI Agent 교육 커리큘럼 문서 사이트입니다.

## 시작하기

### 설치

```bash
cd docs
npm install
```

### 개발 서버 실행

```bash
npm run storybook
```

브라우저에서 http://localhost:6006 으로 접속합니다.

### 프로덕션 빌드

```bash
npm run build-storybook
```

빌드 결과물은 `../docs-dist` 폴더에 생성됩니다.

## 구조

```
docs/
├── .storybook/           # Storybook 설정
│   ├── main.ts           # 메인 설정
│   ├── preview.ts        # 전역 설정
│   ├── manager.ts        # 사이드바 설정
│   └── theme.ts          # 커스텀 테마
├── stories/              # MDX 문서
│   ├── Introduction.mdx  # 메인 페이지
│   ├── chapters/         # 8개 챕터
│   ├── appendix/         # 부록
│   └── exercises/        # 실습
├── components/           # React 컴포넌트
│   ├── Placeholder.tsx   # 미디어 플레이스홀더
│   ├── ComparisonTable.tsx
│   ├── ExerciseCard.tsx
│   └── ChapterNav.tsx
├── public/               # 정적 파일
└── package.json
```

## 배포

GitHub Pages로 자동 배포됩니다.

- `docs/` 폴더에 변경사항을 push하면 자동으로 빌드 및 배포
- `.github/workflows/deploy-docs.yml` 참고

## 스크린샷/영상 추가

`Placeholder` 컴포넌트로 표시된 위치에 실제 미디어를 추가합니다:

1. `docs/public/images/` 또는 `docs/public/videos/`에 파일 추가
2. MDX에서 Placeholder를 실제 이미지/비디오로 교체

```mdx
// Before
<Placeholder type="image" alt="설명" />

// After
<img src="/images/screenshot.png" alt="설명" />
```

## 기여

1. 새 챕터나 섹션 추가 시 `stories/chapters/` 폴더에 MDX 파일 생성
2. 사이드바 순서는 `.storybook/preview.ts`의 `storySort`에서 설정
