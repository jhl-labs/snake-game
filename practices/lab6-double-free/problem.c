/*
 * GDB 디버깅 연습용 예제 - Double Free
 * 이 프로그램에는 의도적인 버그가 있습니다.
 *
 * 빌드: gcc -g -o buggy main.c
 * 실행: ./buggy
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Buffer {
    char* data;
    int size;
    int capacity;
} Buffer;

Buffer* create_buffer(int capacity) {
    Buffer* buf = (Buffer*)malloc(sizeof(Buffer));
    if (buf == NULL) return NULL;

    buf->data = (char*)malloc(capacity);
    if (buf->data == NULL) {
        free(buf);
        return NULL;
    }

    buf->size = 0;
    buf->capacity = capacity;
    memset(buf->data, 0, capacity);

    return buf;
}

void append_data(Buffer* buf, const char* str) {
    if (buf == NULL || buf->data == NULL) return;

    int len = strlen(str);
    if (buf->size + len >= buf->capacity) {
        printf("Buffer full!\n");
        return;
    }

    strcpy(buf->data + buf->size, str);
    buf->size += len;
}

// 버그: 해제 후 포인터를 NULL로 설정하지 않음!
void clear_buffer(Buffer* buf) {
    if (buf == NULL) return;
    if (buf->data != NULL) {
        free(buf->data);
        // 버그: buf->data = NULL; 누락!
    }
    buf->size = 0;
}

void destroy_buffer(Buffer* buf) {
    if (buf == NULL) return;
    if (buf->data != NULL) {
        free(buf->data);  // clear_buffer 후면 double free!
    }
    free(buf);
}

void print_buffer(Buffer* buf) {
    if (buf == NULL) {
        printf("Buffer is NULL\n");
        return;
    }
    printf("Buffer: size=%d, capacity=%d\n", buf->size, buf->capacity);
    if (buf->data != NULL && buf->size > 0) {
        printf("Data: '%s'\n", buf->data);
    }
}

int main() {
    printf("버퍼 관리 시스템 시작\n\n");

    // 버퍼 생성
    Buffer* buf = create_buffer(100);
    append_data(buf, "Hello World!");
    print_buffer(buf);

    // 버퍼 클리어
    printf("\nclear_buffer 호출...\n");
    clear_buffer(buf);
    printf("클리어 완료\n");

    // 버퍼 파괴 시도 (double free 발생!)
    printf("\ndestroy_buffer 호출...\n");
    destroy_buffer(buf);

    printf("\n프로그램 종료\n");
    return 0;
}
