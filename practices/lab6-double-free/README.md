# GDB 실습 6: Double Free

## 개요

같은 메모리를 두 번 해제하는 버그를 GDB와 AddressSanitizer로 디버깅합니다.

## 증상

- `free(): double free detected`
- 힙 손상
- 프로그램 abort

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
# free 호출 추적
(gdb) break free
(gdb) run
(gdb) bt          # 첫 번째 free
(gdb) continue
(gdb) bt          # 두 번째 free (같은 주소!)

# 포인터 값 확인
(gdb) break clear_buffer
(gdb) run
(gdb) print buf->data
(gdb) next        # free 실행
(gdb) print buf->data  # 여전히 같은 주소!
```

## 버그 위치

`clear_buffer` 함수에서 `free()` 후 포인터를 NULL로 설정하지 않음

## 수정 방법

해제 후 NULL 설정:
```c
void clear_buffer(Buffer* buf) {
    if (buf == NULL) return;
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;  // 추가!
    }
    buf->size = 0;
}
```
