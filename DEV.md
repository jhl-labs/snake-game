# 개발 환경 설정 가이드

Windows에서 MSYS2/MinGW를 사용하여 Snake Game을 빌드하는 방법입니다.

## 1. MSYS2 설치

### 1.1 다운로드 및 설치

1. [MSYS2 공식 사이트](https://www.msys2.org/)에서 설치 파일 다운로드
2. 설치 경로: `C:\msys64` (기본값 권장)
3. 설치 완료 후 MSYS2 터미널이 자동으로 열림

### 1.2 패키지 업데이트

MSYS2 터미널에서 실행:

```bash
pacman -Syu
```

터미널이 닫히면 다시 열고 한 번 더 실행:

```bash
pacman -Su
```

### 1.3 개발 도구 설치

MSYS2 터미널에서 MinGW 64비트 도구체인 설치:

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make mingw-w64-x86_64-cmake git
```

## 2. 환경 변수 설정

Windows 환경 변수 PATH에 다음 경로 추가:

```
C:\msys64\mingw64\bin
C:\msys64\usr\bin
```

설정 방법:
1. Windows 검색 → "환경 변수" → "시스템 환경 변수 편집"
2. "환경 변수" 버튼 클릭
3. "Path" 선택 → "편집" → "새로 만들기"
4. 위 경로들 추가

## 3. 프로젝트 빌드

### 3.1 raylib 라이브러리 다운로드

프로젝트 폴더에서 PowerShell 또는 MSYS2 터미널 실행:

```bash
# 폴더 생성
mkdir -p lib external/raylib/src

# raylib 미리 컴파일된 버전 다운로드
curl -L -o raylib-mingw.zip "https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_win64_mingw-w64.zip"

# 압축 해제
unzip raylib-mingw.zip

# 파일 복사
cp raylib-5.5_win64_mingw-w64/lib/libraylib.a lib/
cp raylib-5.5_win64_mingw-w64/include/*.h external/raylib/src/

# 임시 파일 정리
rm -rf raylib-mingw.zip raylib-5.5_win64_mingw-w64
```

### 3.2 빌드

**방법 1: MSYS2 MinGW64 터미널 사용 (권장)**

시작 메뉴에서 "MSYS2 MinGW64" 터미널 실행:

```bash
cd /c/git/snake-game
mingw32-make
```

**방법 2: PowerShell에서 MSYS2 셸 호출**

```powershell
C:\msys64\usr\bin\bash.exe -lc "cd /c/git/snake-game && mingw32-make"
```

> **주의**: PowerShell에서 직접 `mingw32-make`를 실행하면 환경 변수 문제로 컴파일이 실패할 수 있습니다. 반드시 MSYS2 로그인 셸(`-l` 플래그)을 사용하세요.

### 3.3 빌드 결과

빌드 성공 시 `bin/snake.exe` 파일이 생성됩니다.

## 4. 실행

```powershell
.\bin\snake.exe
```

또는 탐색기에서 `bin\snake.exe` 더블클릭

## 5. Makefile 명령어

```bash
mingw32-make          # 빌드
mingw32-make clean    # 빌드 파일 정리
mingw32-make rebuild  # 클린 후 재빌드
mingw32-make run      # 빌드 후 실행
```

## 6. 문제 해결

### GCC 컴파일 실패 (에러 메시지 없이 종료)

**증상**: `mingw32-make` 실행 시 에러 메시지 없이 `Error 1`로 종료

**원인**: PowerShell에서 직접 실행 시 MSYS2 환경 변수가 제대로 설정되지 않음

**해결**: MSYS2 로그인 셸 사용

```powershell
C:\msys64\usr\bin\bash.exe -lc "cd /c/git/snake-game && mingw32-make"
```

### raylib.h를 찾을 수 없음

**증상**: `fatal error: raylib.h: No such file or directory`

**해결**: raylib 헤더 파일이 `external/raylib/src/`에 있는지 확인

```bash
ls external/raylib/src/
# raylib.h, raymath.h, rlgl.h 파일이 있어야 함
```

### libraylib.a를 찾을 수 없음

**증상**: `cannot find -lraylib`

**해결**: raylib 라이브러리가 `lib/`에 있는지 확인

```bash
ls lib/
# libraylib.a 파일이 있어야 함
```

## 7. 프로젝트 구조

```
snake-game/
├── bin/                  # 빌드 출력 (snake.exe)
├── external/
│   └── raylib/
│       └── src/          # raylib 헤더 파일
├── include/              # 프로젝트 헤더 파일
│   ├── game_constants.h
│   ├── game.h
│   ├── snake.h
│   ├── food.h
│   └── renderer.h
├── lib/                  # 라이브러리 (libraylib.a)
├── src/                  # 소스 파일
│   ├── main.c
│   ├── game.c
│   ├── snake.c
│   ├── food.c
│   └── renderer.c
├── .gitignore
├── DEV.md                # 이 문서
├── Makefile
└── README.md
```

## 8. 게임 조작법

- **화살표 키**: 뱀 이동
- **R**: 게임 오버 시 재시작
- **ESC**: 종료
