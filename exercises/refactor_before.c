/**
 * @file refactor_before.c
 * @brief 리팩토링 실습용 파일 (개선 전)
 *
 * 이 파일은 의도적으로 좋지 않은 코드 스타일로 작성되었습니다.
 * AI Agent를 활용하여 리팩토링하는 실습을 진행합니다.
 *
 * 개선 포인트: 10개 이상
 */

#include <stdio.h>
#include <stdlib.h>

// ========================================
// 문제 1: 매직 넘버 사용
// ========================================
int grid[20][20];  // 20이 무엇을 의미하는지 불명확

// ========================================
// 문제 2: 잘못된 네이밍 규칙
// ========================================
typedef struct {
    int X, Y;           // 대문자 사용 (규칙 위반)
    int len;            // 축약어 사용
    int dir;            // 축약어 사용
} snake_data;           // snake_case가 아닌 타입명

// ========================================
// 문제 3: 전역 변수 남용
// ========================================
snake_data s;           // 전역 변수, g_ 접두사 없음
int score;              // 전역 변수
int gameOver;           // 카멜케이스 사용 (규칙 위반)

// ========================================
// 문제 4: 함수가 너무 김 + 여러 책임
// ========================================
void game_loop() {
    // 초기화
    s.X = 10;
    s.Y = 10;
    s.len = 1;
    s.dir = 0;
    score = 0;
    gameOver = 0;

    // 음식 위치
    int fx = 5, fy = 5;

    while (!gameOver) {
        // 입력 처리
        char input;
        scanf("%c", &input);
        if (input == 'w') s.dir = 0;
        else if (input == 's') s.dir = 1;
        else if (input == 'a') s.dir = 2;
        else if (input == 'd') s.dir = 3;

        // 이동
        if (s.dir == 0) s.Y--;
        else if (s.dir == 1) s.Y++;
        else if (s.dir == 2) s.X--;
        else if (s.dir == 3) s.X++;

        // 벽 충돌 체크
        if (s.X < 0 || s.X >= 20 || s.Y < 0 || s.Y >= 20) {
            gameOver = 1;
        }

        // 음식 먹기
        if (s.X == fx && s.Y == fy) {
            score += 10;
            s.len++;
            fx = rand() % 20;
            fy = rand() % 20;
        }

        // 화면 출력
        system("cls");
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                if (i == s.Y && j == s.X) printf("O");
                else if (i == fy && j == fx) printf("*");
                else printf(".");
            }
            printf("\n");
        }
        printf("Score: %d\n", score);
    }

    printf("Game Over! Final Score: %d\n", score);
}

// ========================================
// 문제 5: 주석 없음
// ========================================
int calc(int a, int b, int c) {
    if (a < 0 || b < 0) return -1;
    int r = a * 20 + b;
    if (c == 1) r += 100;
    return r;
}

// ========================================
// 문제 6: 일관성 없는 에러 처리
// ========================================
void* allocate_data(int size) {
    void* p = malloc(size);
    // 에러 체크 없음!
    return p;
}

int process_data(int* data) {
    // 어떤 함수는 체크하고
    if (data == NULL) {
        printf("Error!\n");  // stderr 대신 stdout
        return 0;           // bool 대신 int
    }
    return 1;
}

// ========================================
// 문제 7: 하드코딩된 문자열
// ========================================
void print_status(int status) {
    if (status == 0) printf("Playing\n");
    else if (status == 1) printf("Paused\n");
    else if (status == 2) printf("Game Over\n");
    else printf("Unknown\n");
}

// ========================================
// 문제 8: 중복 코드
// ========================================
void move_up(int* y) { *y = *y - 1; }
void move_down(int* y) { *y = *y + 1; }
void move_left(int* x) { *x = *x - 1; }
void move_right(int* x) { *x = *x + 1; }

// ========================================
// 문제 9: 포인터에 p_ 접두사 없음
// ========================================
void update_position(int* x, int* y, int direction) {
    switch (direction) {
        case 0: (*y)--; break;
        case 1: (*y)++; break;
        case 2: (*x)--; break;
        case 3: (*x)++; break;
    }
}

// ========================================
// 문제 10: 함수 선언과 정의 혼재
// ========================================
int helper_func(int val);  // 선언

int main_logic(int input) {
    return helper_func(input) * 2;
}

int helper_func(int val) {  // 정의가 사용 후에 옴
    return val + 1;
}

// ========================================
// 메인 함수
// ========================================
int main() {
    printf("This file demonstrates poor coding practices.\n");
    printf("Use AI Agent to refactor this code!\n\n");

    printf("Issues to fix:\n");
    printf("1. Magic numbers -> Use constants\n");
    printf("2. Naming conventions -> snake_case, PascalCase\n");
    printf("3. Global variables -> g_ prefix or localize\n");
    printf("4. Long functions -> Split into smaller ones\n");
    printf("5. Missing comments -> Add Doxygen comments\n");
    printf("6. Inconsistent error handling -> Standardize\n");
    printf("7. Hardcoded strings -> Use enums/constants\n");
    printf("8. Duplicate code -> Extract common function\n");
    printf("9. Pointer naming -> Use p_ prefix\n");
    printf("10. Declaration order -> Proper structure\n");

    return 0;
}

/*
 * ========================================
 * 실습 가이드
 * ========================================
 *
 * AI Agent에게 다음과 같이 요청해보세요:
 *
 * 1. "이 파일의 코딩 스타일 문제를 찾아줘"
 * 2. "매직 넘버를 상수로 변경해줘"
 * 3. "네이밍 규칙을 프로젝트 규칙에 맞게 수정해줘"
 * 4. "game_loop 함수를 작은 함수들로 분리해줘"
 * 5. "전체 파일을 리팩토링해줘"
 *
 * 리팩토링 원칙:
 * - 기존 동작 유지
 * - 작은 단위로 변경
 * - 각 변경 후 컴파일 확인
 *
 * 참고 파일:
 * - .cursor/rules/coding-style.mdc
 * - .claude/rules/coding-style.md
 */
