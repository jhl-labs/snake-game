# Snake Game

raylib을 사용한 C 언어 스네이크 게임입니다.

## 빌드

### 요구사항

- Windows 10 이상
- MSYS2/MinGW-w64

### 빌드 방법

MSYS2 MinGW64 터미널에서:

```bash
cd /c/git/snake-game
mingw32-make
```

> 자세한 환경 설정은 [DEV.md](DEV.md)를 참고하세요.

## 실행

```bash
./bin/snake.exe
```

## 조작법

| 키 | 동작 |
|---|---|
| 화살표 키 | 뱀 이동 |
| R | 재시작 (게임 오버 시) |
| ESC | 종료 |

## 프로젝트 구조

```
snake-game/
├── src/           # 소스 코드
├── include/       # 헤더 파일
├── lib/           # 라이브러리 (raylib)
├── external/      # 외부 헤더 (raylib)
├── bin/           # 빌드 출력
├── Makefile
├── DEV.md         # 개발 환경 설정 가이드
└── README.md
```

## 라이선스

MIT License
