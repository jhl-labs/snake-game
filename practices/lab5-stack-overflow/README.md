# GDB 실습 5: 무한 재귀 (Stack Overflow)

## 개요

무한 재귀로 인한 스택 오버플로우를 GDB의 백트레이스로 디버깅합니다.

## 증상

- Segmentation fault
- 스택 메모리 소진
- 프로그램 응답 없음 후 크래시

## 실습 방법

### 자동 실행
```bash
./debug.sh
```

### 수동 디버깅
```bash
# 빌드
gcc -g -o buggy problem.c

# GDB 실행
gdb ./buggy
```

## 핵심 GDB 명령어

```gdb
# 실행 후 크래시
(gdb) run

# 백트레이스 확인
(gdb) bt
(gdb) bt 20     # 최근 20개 프레임
(gdb) bt -20    # 가장 오래된 20개 프레임

# 특정 프레임으로 이동
(gdb) frame 0
(gdb) frame 100
(gdb) print node->value

# 프레임 간 이동
(gdb) up
(gdb) down
```

## 버그 위치

`count_nodes` 함수에서 `parent`도 순회하여 순환 참조 발생

## 수정 방법

parent 순회 제거:
```c
int count_nodes(TreeNode* node) {
    if (node == NULL) {
        return 0;
    }
    return 1 + count_nodes(node->left)
             + count_nodes(node->right);
    // parent는 순회하지 않음!
}
```
