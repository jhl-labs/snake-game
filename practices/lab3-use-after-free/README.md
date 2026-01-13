# GDB 실습 3: Use-After-Free

## 개요

`free()`로 해제한 메모리에 접근하는 버그를 GDB와 AddressSanitizer로 디버깅합니다.

## 증상

- 가비지 데이터 출력
- 간헐적 크래시
- 데이터 손상

## 실습 방법

### 자동 실행
```bash
./debug.sh
```

### 수동 디버깅
```bash
# 일반 빌드
gcc -g -o buggy problem.c
gdb ./buggy

# AddressSanitizer로 빌드 (권장)
gcc -g -fsanitize=address -o buggy_asan problem.c
./buggy_asan
```

## 핵심 GDB 명령어

```gdb
# remove_node 함수 추적
(gdb) break remove_node
(gdb) run

# free 전후 메모리 확인
(gdb) print *current
(gdb) next  # free 실행
(gdb) print *current  # 해제된 메모리

# free 호출 추적
(gdb) break free
(gdb) bt
```

## 버그 위치

`remove_node` 함수에서 `free()` 후 해제된 포인터 반환

## 수정 방법

성공/실패만 반환하도록 수정:
```c
int remove_node(Node** head, int id) {
    // ... 검색 로직 ...
    if (current->id == id) {
        // ... 연결 해제 ...
        free(current);
        return 1;  // 성공
    }
    return 0;  // 실패
}
```
