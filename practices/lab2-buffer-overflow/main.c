/*
 * GDB 디버깅 연습용 예제 - 배열 범위 초과 (Buffer Overflow)
 * 이 프로그램에는 의도적인 버그가 있습니다.
 *
 * 빌드: gcc -g -o buggy main.c
 * 실행: ./buggy
 */

#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 5

typedef struct {
    char name[20];
    int prices[MAX_ITEMS];
    int total;
} Cart;

void init_cart(Cart* cart, const char* name) {
    strcpy(cart->name, name);
    cart->total = 0;
    for (int i = 0; i < MAX_ITEMS; i++) {
        cart->prices[i] = 0;
    }
}

// 수정: 배열 범위 검사 추가
void add_item(Cart* cart, int index, int price) {
    if (index < 0 || index >= MAX_ITEMS) {
        printf("오류: 인덱스 %d는 유효 범위(0-%d)를 벗어났습니다.\n", index, MAX_ITEMS - 1);
        return;
    }
    cart->prices[index] = price;
    cart->total += price;
}

int calculate_total(Cart* cart) {
    int sum = 0;
    for (int i = 0; i < MAX_ITEMS; i++) {
        sum += cart->prices[i];
    }
    return sum;
}

void print_cart(Cart* cart) {
    printf("장바구니: %s\n", cart->name);
    printf("총액: %d원\n", cart->total);
    printf("계산된 총액: %d원\n", calculate_total(cart));
}

int main() {
    Cart cart;
    init_cart(&cart, "MyCart");

    printf("장바구니 시스템 시작\n\n");

    printf("정상적인 아이템 추가...\n");
    add_item(&cart, 0, 1000);
    add_item(&cart, 1, 2000);
    add_item(&cart, 2, 3000);

    print_cart(&cart);

    printf("\n잘못된 인덱스로 아이템 추가 시도...\n");
    // 버그: 배열 범위(0-4)를 벗어난 인덱스 사용
    add_item(&cart, 5, 5000);
    add_item(&cart, 6, 6000);
    add_item(&cart, 10, 9999);

    print_cart(&cart);

    printf("\n프로그램 종료\n");
    return 0;
}
