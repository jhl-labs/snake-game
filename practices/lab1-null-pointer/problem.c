/*
 * GDB 디버깅 연습용 예제
 * 이 프로그램에는 의도적인 버그가 있습니다.
 *
 * 빌드: gcc -g -o buggy main.c
 * 실행: ./buggy
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 학생 정보를 저장하는 구조체
typedef struct Student {
    int id;
    char name[50];
    int scores[5];  // 5과목 점수
    struct Student* next;
} Student;

// 학생 노드 생성
Student* create_student(int id, const char* name) {
    Student* student = (Student*)malloc(sizeof(Student));
    student->id = id;
    strcpy(student->name, name);
    student->next = NULL;

    // 점수 초기화
    for (int i = 0; i < 5; i++) {
        student->scores[i] = 0;
    }

    return student;
}

// 학생 리스트에 추가
void add_student(Student** head, Student* new_student) {
    if (*head == NULL) {
        *head = new_student;
        return;
    }

    Student* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_student;
}

// 학생 ID로 검색
Student* find_student(Student* head, int id) {
    Student* current = head;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;  // 찾지 못함
}

// 학생의 평균 점수 계산
double calculate_average(Student* student) {
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += student->scores[i];
    }
    return sum / 5.0;
}

// 특정 학생의 점수 설정
void set_score(Student* head, int student_id, int subject_index, int score) {
    Student* student = find_student(head, student_id);
    student->scores[subject_index] = score;
}

// 모든 학생 정보 출력
void print_all_students(Student* head) {
    printf("\n=== 학생 목록 ===\n");
    Student* current = head;
    while (current != NULL) {
        printf("ID: %d, 이름: %s, 평균: %.2f\n",
               current->id, current->name, calculate_average(current));
        current = current->next;
    }
    printf("================\n\n");
}

// 메모리 해제
void free_students(Student* head) {
    Student* current = head;
    while (current != NULL) {
        Student* next = current->next;
        free(current);
        current = next;
    }
}

int main() {
    Student* student_list = NULL;

    printf("학생 관리 시스템 시작\n");

    // 학생 추가
    add_student(&student_list, create_student(1001, "김철수"));
    add_student(&student_list, create_student(1002, "이영희"));
    add_student(&student_list, create_student(1003, "박민수"));

    // 점수 설정
    set_score(student_list, 1001, 0, 85);  // 김철수 과목1
    set_score(student_list, 1001, 1, 90);  // 김철수 과목2
    set_score(student_list, 1002, 0, 78);  // 이영희 과목1

    print_all_students(student_list);

    printf("존재하지 않는 학생(ID: 9999)에게 점수 설정 시도...\n");
    set_score(student_list, 9999, 0, 100);

    printf("프로그램 정상 종료\n");

    free_students(student_list);
    return 0;
}
