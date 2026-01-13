# GDB 디버깅 실습 모음

C 프로그램의 일반적인 버그를 GDB로 디버깅하는 방법을 학습하는 실습입니다.

## 실습 목록

| 폴더 | 버그 유형 | 핵심 학습 |
|------|-----------|-----------|
| `lab1-null-pointer` | NULL 포인터 역참조 | `bt`, `print` - GDB 기초 |
| `lab2-buffer-overflow` | 배열 범위 초과 | `watch`, `x/` - 메모리 감시 |
| `lab3-use-after-free` | Use-After-Free | ASan 활용 |
| `lab4-uninitialized` | 초기화되지 않은 변수 | `-Wall`, `info locals` |
| `lab5-stack-overflow` | 무한 재귀 | `bt 20`, `frame` - 콜스택 분석 |
| `lab6-double-free` | Double Free | `break free` - 힙 문제 추적 |

## 사용법

각 폴더에서 `debug.sh`를 실행하면 Claude Code가 자동으로 버그를 분석하고 수정합니다.

```bash
cd lab1-null-pointer
./debug.sh
```

## 폴더 구조

각 실습 폴더는 동일한 구조를 가집니다:

```
lab-name/
├── problem.c    # 버그가 있는 소스 코드
├── debug.sh     # 자동 디버깅 스크립트
└── README.md    # 실습 가이드
```

## 권장 학습 순서

1. `lab1-null-pointer` - GDB 기본 사용법
2. `lab4-uninitialized` - 컴파일러 경고 활용
3. `lab2-buffer-overflow` - watchpoint 사용
4. `lab5-stack-overflow` - 콜 스택 분석
5. `lab3-use-after-free` - AddressSanitizer
6. `lab6-double-free` - 복합적인 메모리 문제

## GDB 명령어 요약

| 명령어 | 설명 |
|--------|------|
| `run` | 프로그램 실행 |
| `bt` | 백트레이스 (콜 스택) |
| `print var` | 변수 값 출력 |
| `break func` | 함수에 브레이크포인트 |
| `next` / `step` | 한 줄 실행 |
| `watch var` | 변수 변경 감시 |
| `frame N` | N번 프레임으로 이동 |
