# GDB 실습 2: 배열 범위 초과 (Buffer Overflow)

## 개요

이 프로그램은 장바구니 시스템입니다. 실행하면 "stack smashing detected" 오류가 발생합니다.
GDB를 사용하여 메모리 손상 원인을 찾아보세요.

## 증상

```
$ ./buggy
장바구니 시스템 시작
...
*** stack smashing detected ***: terminated
Aborted (core dumped)
```

## 실습 목표

1. 메모리 레이아웃 이해
2. breakpoint로 단계별 실행
3. 메모리 덤프로 손상 확인
4. 배열 경계 검사의 중요성 이해

## 실습 방법

### 1단계: 빌드 및 GDB 실행

```bash
gcc -g -o buggy problem.c
gdb ./buggy
```

### 2단계: 구조체 메모리 레이아웃 확인

```
(gdb) break main
Breakpoint 1 at 0x12fe: file problem.c, line 48.

(gdb) run
Breakpoint 1, main () at problem.c:48

(gdb) next
(gdb) next
(gdb) next

(gdb) print cart
$1 = {name = "MyCart", '\000' <repeats 13 times>, prices = {0, 0, 0, 0, 0}, total = 0}

(gdb) print &cart.name
$2 = (char (*)[20]) 0x7fffffffdaa0

(gdb) print &cart.prices
$3 = (int (*)[5]) 0x7fffffffdab4

(gdb) print &cart.total
$4 = (int *) 0x7fffffffdac8

(gdb) print sizeof(cart.prices)
$5 = 20
```

**메모리 레이아웃**:
```
주소              필드           크기
0x7fffffffdaa0    name[20]       20 bytes
0x7fffffffdab4    prices[5]      20 bytes (int * 5)
0x7fffffffdac8    total          4 bytes
```

### 3단계: breakpoint로 배열 접근 추적

```
(gdb) break add_item
Breakpoint 2 at 0x55555555521a: file problem.c, line 29.

(gdb) continue
Breakpoint 2, add_item (cart=0x7fffffffdaa0, index=0, price=1000) at problem.c:29
29	    cart->prices[index] = price;

(gdb) continue
Breakpoint 2, add_item (cart=0x7fffffffdaa0, index=1, price=2000) at problem.c:29

(gdb) continue
Breakpoint 2, add_item (cart=0x7fffffffdaa0, index=2, price=3000) at problem.c:29

(gdb) continue
Breakpoint 2, add_item (cart=0x7fffffffdaa0, index=5, price=5000) at problem.c:29
```

**발견**: `index=5`가 전달됨! `prices[5]`는 배열 범위(0-4)를 벗어남

### 4단계: 범위 초과 전후 메모리 비교

**정상 상태 (index=2 처리 후)**:
```
(gdb) print *cart
$3 = {name = "MyCart", '\000' <repeats 13 times>, prices = {1000, 2000, 3000, 0, 0}, total = 6000}
```

**범위 초과 후 (index=5 처리 후)**:
```
(gdb) next
30	    cart->total += price;

(gdb) print *cart
$4 = {name = "MyCart", '\000' <repeats 13 times>, prices = {1000, 2000, 3000, 0, 0}, total = 10000}
```

### 5단계: 메모리 직접 확인

```
(gdb) x/20x cart
0x7fffffffdaa0:	0x6143794d	0x00007472	0x00000000	0x00000000
0x7fffffffdab0:	0x00000000	0x000003e8	0x000007d0	0x00000bb8
0x7fffffffdac0:	0x00000000	0x00000000	0x00002710	0x00007fff
                              ↑ prices[5] 위치 - total 뒤 영역 침범!
```

### 6단계: 크래시 원인

```
(gdb) continue
Breakpoint 2, add_item (cart=0x7fffffffdaa0, index=6, price=6000) at problem.c:29

(gdb) continue
Breakpoint 2, add_item (cart=0x7fffffffdaa0, index=10, price=9999) at problem.c:29

(gdb) continue
*** stack smashing detected ***: terminated

Program received signal SIGABRT, Aborted.
```

**원인**: `prices[10]`이 스택 카나리(stack canary)를 덮어써서 보안 검사 실패

## 분석 결과

| 항목 | 값 |
|------|-----|
| 문제 함수 | `add_item()` |
| 문제 코드 | `cart->prices[index] = price;` |
| 원인 | 배열 범위(0-4) 검사 없이 index 사용 |
| 결과 | `prices[5]` 이상 접근 시 스택 메모리 손상 |

**메모리 손상 범위**:
- `prices[5]`: total 바로 뒤 (4바이트 손상)
- `prices[6]`: 스택 프레임 영역
- `prices[10]`: 스택 카나리 영역 → 크래시

## 수정 방법

```c
void add_item(Cart* cart, int index, int price) {
    if (index < 0 || index >= MAX_ITEMS) {
        printf("Error: Invalid index %d (valid: 0-%d)\n", index, MAX_ITEMS - 1);
        return;
    }
    cart->prices[index] = price;
    cart->total += price;
}
```

## 핵심 GDB 명령어 정리

| 명령어 | 설명 |
|--------|------|
| `break FUNC` | 함수에 breakpoint 설정 |
| `next` | 한 줄 실행 (함수 안으로 들어가지 않음) |
| `print EXPR` | 표현식 값 출력 |
| `print &VAR` | 변수 주소 출력 |
| `print sizeof(X)` | 크기 출력 |
| `x/Nx ADDR` | 주소에서 N개 16진수 출력 |
| `continue` | 다음 breakpoint까지 실행 |
