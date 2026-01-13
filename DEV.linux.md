# Linux 개발 환경 설정 가이드

Linux에서 Snake Game을 빌드하는 방법입니다.

## 1. 의존성 설치

### 1.1 필수 패키지 설치

Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install -y build-essential git
sudo apt-get install -y libgl1-mesa-dev libx11-dev libxrandr-dev libxi-dev libxcursor-dev libxinerama-dev
```

Fedora:

```bash
sudo dnf install gcc make git
sudo dnf install mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel
```

Arch Linux:

```bash
sudo pacman -S base-devel git
sudo pacman -S mesa libx11 libxrandr libxi libxcursor libxinerama
```

## 2. raylib 라이브러리 빌드

프로젝트에 이미 `lib-linux/libraylib.a`가 포함되어 있다면 이 단계를 건너뛰세요.

### 2.1 raylib 소스 다운로드 및 빌드

```bash
# 프로젝트 디렉토리에서 실행
git clone --depth 1 https://github.com/raysan5/raylib.git raylib-src
cd raylib-src/src
make PLATFORM=PLATFORM_DESKTOP
```

### 2.2 라이브러리 복사

```bash
# 프로젝트 루트 디렉토리에서 실행
mkdir -p lib/linux
cp raylib-src/src/libraylib.a lib/linux/
```

### 2.3 정리 (선택사항)

```bash
rm -rf raylib-src
```

## 3. 프로젝트 빌드

### 3.1 빌드

```bash
make -f Makefile.linux
```

### 3.2 빌드 결과

빌드 성공 시 `bin/snake` 파일이 생성됩니다.

## 4. 실행

```bash
./bin/snake
```

또는:

```bash
make -f Makefile.linux run
```

## 5. Makefile.linux 명령어

```bash
make -f Makefile.linux          # 빌드
make -f Makefile.linux clean    # 빌드 파일 정리
make -f Makefile.linux rebuild  # 클린 후 재빌드
make -f Makefile.linux run      # 빌드 후 실행
```

## 6. 문제 해결

### X11/GL 관련 헤더를 찾을 수 없음

**증상**: `fatal error: X11/extensions/Xrandr.h: No such file or directory`

**해결**: X11 개발 라이브러리 설치

```bash
# Ubuntu/Debian
sudo apt-get install -y libxrandr-dev libxi-dev libxcursor-dev libxinerama-dev

# Fedora
sudo dnf install libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel
```

### OpenGL 라이브러리를 찾을 수 없음

**증상**: `cannot find -lGL`

**해결**: Mesa OpenGL 개발 라이브러리 설치

```bash
# Ubuntu/Debian
sudo apt-get install -y libgl1-mesa-dev

# Fedora
sudo dnf install mesa-libGL-devel
```

### libraylib.a를 찾을 수 없음

**증상**: `cannot find -lraylib`

**해결**: raylib 라이브러리가 `lib/linux/`에 있는지 확인

```bash
ls lib/linux/
# libraylib.a 파일이 있어야 함
```

없다면 위의 "2. raylib 라이브러리 빌드" 섹션을 따라 빌드하세요.

### NULL 관련 컴파일 오류

**증상**: `error: 'NULL' undeclared`

**해결**: 해당 소스 파일에 `#include <stddef.h>` 또는 `#include <stdlib.h>` 추가

## 7. 프로젝트 구조

```
snake-game/
├── bin/                  # 빌드 출력 (snake)
├── external/
│   └── raylib/
│       └── src/          # raylib 헤더 파일
├── include/              # 프로젝트 헤더 파일
│   ├── game_constants.h
│   ├── game.h
│   ├── snake.h
│   ├── food.h
│   └── renderer.h
├── lib/
│   ├── mingw/            # Windows용 라이브러리 (libraylib.a)
│   └── linux/            # Linux용 라이브러리 (libraylib.a)
├── src/                  # 소스 파일
│   ├── main.c
│   ├── game.c
│   ├── snake.c
│   ├── food.c
│   └── renderer.c
├── .gitignore
├── DEV.md                # Windows 개발 가이드
├── DEV.linux.md          # 이 문서
├── Makefile              # Windows용 Makefile
├── Makefile.linux        # Linux용 Makefile
└── README.md
```

## 8. 게임 조작법

- **화살표 키**: 뱀 이동
- **R**: 게임 오버 시 재시작
- **ESC**: 종료
