/*
 * GDB 디버깅 연습용 예제 - 무한 재귀 (Stack Overflow)
 * 이 프로그램에는 의도적인 버그가 있습니다.
 *
 * 빌드: gcc -g -o buggy main.c
 * 실행: ./buggy
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int value;
    struct TreeNode* left;
    struct TreeNode* right;
    struct TreeNode* parent;
} TreeNode;

TreeNode* create_node(int value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

void insert_node(TreeNode* parent, TreeNode* child, int is_left) {
    if (is_left) {
        parent->left = child;
    } else {
        parent->right = child;
    }
    child->parent = parent;
}

// 버그: parent도 순회하여 무한 재귀!
int count_nodes(TreeNode* node) {
    if (node == NULL) {
        return 0;
    }
    return 1 + count_nodes(node->left)
             + count_nodes(node->right)
             + count_nodes(node->parent);  // 무한 재귀!
}

// 정상적인 함수 (비교용)
int count_nodes_correct(TreeNode* node) {
    if (node == NULL) {
        return 0;
    }
    return 1 + count_nodes_correct(node->left)
             + count_nodes_correct(node->right);
}

void print_tree(TreeNode* node, int depth) {
    if (node == NULL || depth > 5) return;
    for (int i = 0; i < depth; i++) printf("  ");
    printf("%d\n", node->value);
    print_tree(node->left, depth + 1);
    print_tree(node->right, depth + 1);
}

void free_tree(TreeNode* node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

int main() {
    printf("트리 노드 카운트 테스트\n\n");

    // 트리 생성
    TreeNode* root = create_node(1);
    TreeNode* left = create_node(2);
    TreeNode* right = create_node(3);
    TreeNode* left_left = create_node(4);

    insert_node(root, left, 1);
    insert_node(root, right, 0);
    insert_node(left, left_left, 1);

    printf("트리 구조:\n");
    print_tree(root, 0);

    printf("\n정상적인 노드 카운트: %d\n", count_nodes_correct(root));

    printf("\n버그가 있는 노드 카운트 실행...\n");
    printf("노드 수: %d\n", count_nodes(root));  // 스택 오버플로우!

    free_tree(root);
    return 0;
}
