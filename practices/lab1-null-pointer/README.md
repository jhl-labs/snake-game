# GDB 실습 1: NULL 포인터 역참조

## 개요

이 프로그램은 학생 성적 관리 시스템입니다. 실행하면 Segmentation fault가 발생합니다.
GDB를 사용하여 크래시 원인을 찾아보세요.

## 증상

```
$ ./buggy
학생 관리 시스템 시작
...
Segmentation fault (core dumped)
```

## 실습 목표

1. GDB로 크래시 지점 확인
2. 백트레이스로 호출 스택 분석
3. 변수 값 검사로 원인 파악
4. 수정 방안 도출

## 실습 방법

### 1단계: 빌드 및 GDB 실행

```bash
gcc -g -o buggy problem.c
gdb ./buggy
```

### 2단계: 프로그램 실행

```
(gdb) run
Starting program: /home/ubuntu/git/snake-game/practices/lab1-null-pointer/buggy
학생 관리 시스템 시작

=== 학생 목록 ===
ID: 1001, 이름: 김철수, 평균: 35.00
ID: 1002, 이름: 이영희, 평균: 15.60
ID: 1003, 이름: 박민수, 평균: 0.00
================

존재하지 않는 학생(ID: 9999)에게 점수 설정 시도...

Program received signal SIGSEGV, Segmentation fault.
0x0000555555555397 in set_score (head=0x55555555a2b0, student_id=9999,
    subject_index=0, score=100) at problem.c:74
74	    student->scores[subject_index] = score;
```

### 3단계: 백트레이스 확인

```
(gdb) bt
#0  0x0000555555555397 in set_score (head=0x55555555a2b0, student_id=9999,
    subject_index=0, score=100) at problem.c:74
#1  0x0000555555555596 in main () at problem.c:117
```

**분석**: `main()`에서 `set_score()`를 호출했고, `set_score()` 74번 줄에서 크래시 발생

### 4단계: 상세 백트레이스

```
(gdb) bt full
#0  0x0000555555555397 in set_score (head=0x55555555a2b0, student_id=9999,
    subject_index=0, score=100) at problem.c:74
        student = 0x0
#1  0x0000555555555596 in main () at problem.c:117
        student_list = 0x55555555a2b0
```

**발견**: `student` 변수가 `0x0` (NULL)입니다!

### 5단계: 변수 확인

```
(gdb) frame 0
#0  0x0000555555555397 in set_score (...) at problem.c:74
74	    student->scores[subject_index] = score;

(gdb) print student
$1 = (Student *) 0x0

(gdb) print student_id
$2 = 9999

(gdb) info locals
student = 0x0
```

### 6단계: 코드 확인

```
(gdb) list
69
70	// 특정 학생의 점수 설정
71	void set_score(Student* head, int student_id, int subject_index, int score) {
72	    Student* student = find_student(head, student_id);
73	    student->scores[subject_index] = score;
74	}
```

## 분석 결과

| 항목 | 값 |
|------|-----|
| 크래시 위치 | `problem.c:74` |
| 크래시 함수 | `set_score()` |
| 문제 변수 | `student = 0x0 (NULL)` |
| 원인 | `find_student()`가 ID 9999를 찾지 못해 NULL 반환, NULL 체크 없이 역참조 |

## 수정 방법

```c
void set_score(Student* head, int student_id, int subject_index, int score) {
    Student* student = find_student(head, student_id);
    if (student == NULL) {
        printf("Error: Student ID %d not found\n", student_id);
        return;
    }
    student->scores[subject_index] = score;
}
```

## 핵심 GDB 명령어 정리

| 명령어 | 설명 |
|--------|------|
| `run` | 프로그램 실행 |
| `bt` | 백트레이스 (호출 스택) |
| `bt full` | 지역 변수 포함 백트레이스 |
| `frame N` | N번 스택 프레임으로 이동 |
| `print VAR` | 변수 값 출력 |
| `info locals` | 현재 프레임의 지역 변수 |
| `list` | 현재 위치 소스 코드 |
