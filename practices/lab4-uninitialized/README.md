# GDB 실습 4: 초기화되지 않은 변수

## 개요

초기화되지 않은 변수로 인한 비결정적 동작을 GDB와 컴파일러 경고로 디버깅합니다.

## 증상

- 실행마다 다른 결과
- 예상과 다른 계산 결과
- 비결정적 동작

## 실습 방법

### 자동 실행
```bash
./debug.sh
```

### 수동 디버깅
```bash
# 경고 옵션으로 빌드
gcc -g -Wall -Wuninitialized -o buggy problem.c

# GDB 실행
gdb ./buggy
```

## 핵심 GDB 명령어

```gdb
# init_stats 후 구조체 확인
(gdb) break init_stats
(gdb) run
(gdb) finish
(gdb) print stats

# 특정 멤버 확인
(gdb) print stats.sum

# find_max에서 max 확인
(gdb) break find_max
(gdb) continue
(gdb) print max  # 쓰레기 값
```

## 버그 위치

1. `init_stats`: `sum`, `average` 초기화 누락
2. `find_max`: `max` 변수 초기화 누락

## 수정 방법

모든 변수 초기화:
```c
void init_stats(Statistics* stats) {
    stats->count = 0;
    stats->sum = 0;       // 추가
    stats->average = 0;   // 추가
    // ...
}

int find_max(Statistics* stats) {
    if (stats->count == 0) return 0;
    int max = stats->values[0];  // 첫 번째 값으로 초기화
    // ...
}
```
