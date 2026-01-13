/*
 * GDB 디버깅 연습용 예제 - Use-After-Free
 * 이 프로그램에는 의도적인 버그가 있습니다.
 *
 * 빌드: gcc -g -o buggy main.c
 * 실행: ./buggy
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int id;
    char data[32];
    struct Node* next;
} Node;

Node* create_node(int id, const char* data) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) return NULL;
    node->id = id;
    strncpy(node->data, data, 31);
    node->data[31] = '\0';
    node->next = NULL;
    return node;
}

void print_node(Node* node) {
    if (node == NULL) {
        printf("Node is NULL\n");
        return;
    }
    printf("Node ID: %d, Data: %s\n", node->id, node->data);
}

// 버그: 해제 후 포인터를 반환!
Node* remove_node(Node** head, int id) {
    if (*head == NULL) return NULL;

    Node* current = *head;
    Node* prev = NULL;

    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return current;  // 버그: 해제된 메모리 반환!
        }
        prev = current;
        current = current->next;
    }
    return NULL;
}

void print_all_nodes(Node* head) {
    printf("\n=== Node List ===\n");
    Node* current = head;
    while (current != NULL) {
        print_node(current);
        current = current->next;
    }
    printf("=================\n\n");
}

void free_all_nodes(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
}

int main() {
    Node* list = NULL;

    printf("노드 관리 시스템 시작\n\n");

    // 노드 추가
    Node* n1 = create_node(1, "First");
    Node* n2 = create_node(2, "Second");
    Node* n3 = create_node(3, "Third");

    n1->next = n2;
    n2->next = n3;
    list = n1;

    printf("초기 리스트:");
    print_all_nodes(list);

    // 노드 삭제 후 반환된 포인터 사용 시도
    printf("노드 2 삭제 후 반환된 포인터 사용 시도...\n");
    Node* removed = remove_node(&list, 2);

    // Use-after-free!
    printf("삭제된 노드 접근:\n");
    print_node(removed);

    // 삭제된 노드의 데이터 수정 시도
    printf("\n삭제된 노드 데이터 수정 시도...\n");
    strcpy(removed->data, "Modified!");
    print_node(removed);

    printf("\n현재 리스트:");
    print_all_nodes(list);

    free_all_nodes(list);
    return 0;
}
