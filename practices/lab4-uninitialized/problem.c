/*
 * GDB 디버깅 연습용 예제 - 초기화되지 않은 변수
 * 이 프로그램에는 의도적인 버그가 있습니다.
 *
 * 빌드: gcc -g -o buggy main.c
 * 실행: ./buggy
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int count;
    int values[10];
    int sum;
    int average;
} Statistics;

// 버그: sum과 average가 초기화되지 않음!
void init_stats(Statistics* stats) {
    stats->count = 0;
    // stats->sum = 0;       // 초기화 누락!
    // stats->average = 0;   // 초기화 누락!
    for (int i = 0; i < 10; i++) {
        stats->values[i] = 0;
    }
}

void add_value(Statistics* stats, int value) {
    if (stats->count >= 10) {
        printf("Error: Statistics full\n");
        return;
    }
    stats->values[stats->count] = value;
    stats->count++;
    stats->sum += value;  // 버그: 초기화되지 않은 sum에 +=
}

void calculate_average(Statistics* stats) {
    if (stats->count == 0) {
        stats->average = 0;
        return;
    }
    stats->average = stats->sum / stats->count;
}

void print_stats(Statistics* stats) {
    printf("\n=== Statistics ===\n");
    printf("Count: %d\n", stats->count);
    printf("Values: ");
    for (int i = 0; i < stats->count; i++) {
        printf("%d ", stats->values[i]);
    }
    printf("\n");
    printf("Sum: %d\n", stats->sum);
    printf("Average: %d\n", stats->average);
    printf("==================\n\n");
}

// 버그: max 초기화되지 않음!
int find_max(Statistics* stats) {
    int max;  // 초기화 누락!
    for (int i = 0; i < stats->count; i++) {
        if (stats->values[i] > max) {
            max = stats->values[i];
        }
    }
    return max;
}

int main() {
    Statistics stats;

    printf("통계 시스템 시작\n");

    init_stats(&stats);

    printf("\n값 추가: 10, 20, 30, 40, 50\n");
    add_value(&stats, 10);
    add_value(&stats, 20);
    add_value(&stats, 30);
    add_value(&stats, 40);
    add_value(&stats, 50);

    calculate_average(&stats);
    print_stats(&stats);

    printf("예상 합계: 150\n");
    printf("예상 평균: 30\n");

    if (stats.sum != 150) {
        printf("\n[오류] 합계가 예상과 다릅니다! (실제: %d)\n", stats.sum);
    }

    printf("\n최댓값 찾기...\n");
    int max = find_max(&stats);
    printf("최댓값: %d (예상: 50)\n", max);

    if (max != 50) {
        printf("[오류] 최댓값이 예상과 다릅니다!\n");
    }

    return 0;
}
