#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
typedef struct BST {
    int value;
    struct BST* left;
    struct BST* right;
}BST;

Tree insertNode(Tree root, int key);
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(Tree root);
void deleteTree(Tree root);

int main(int argc, char* argv[]) {

    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char cv;
    int key;

    Tree root = NULL;

    while (!feof(fin)) {
        fscanf(fin, "%c", &cv);
        switch(cv) {
            case 'i':
                fscanf(fin, "%d", &key);
                root = insertNode(root, key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if (findNode(root, key))
                    fprintf(fout, "%d is in the tree\n", key);
                else
                    fprintf(fout, "finding error: %d is not in the tree\n", key);
                break;
            case 'd':
                fscanf(fin, "%d", &key);
                if (findNode(root, key)) {
                    root = deleteNode(root, key);
                    fprintf(fout, "delete %d\n", key);
                }
                else {
                    fprintf(fout, "deletion error: %d is not in the tree\n", key);
                }
                break;
            case 'p':
                fscanf(fin, "%c", &cv);
                if (cv == 'i') {
                    if (root == NULL) {
                        fprintf(fout, "tree is empty");
                    }
                    else {
                        printInorder(root);
                    }
                }
                fprintf(fout, "\n");
                break;
        }
    }
    deleteTree(root);
    return 0;
}
Tree insertNode(Tree root, int key) {
    Tree current = root;
    Tree tmp = (Tree)malloc(sizeof(BST));

    tmp->value = key;
    while (1) {
        if (current == NULL) {
            fprintf(fout, "insert %d\n", key);
            return tmp;
        }
        if (current->left && current->right) {  // 노드가 둘 다 자식이 있는 경우
            if (key > current->value) {
                current = current->right;
            }
            else if (key < current->value) {
                current = current->left;
            }
        }
        else {      // current->left 또는 current->right 가 없는경우
            if (key > current->value) {
                if (current->right == NULL) {
                    current->right = tmp;
                    break;
                }
                current = current->right;
            }
            else if (key < current->value) {
                if (current->left == NULL) {
                    current->left = tmp;
                    break;
                }
                current = current->left;
            }
        }
    }
    fprintf(fout, "insert %d\n", key);
    return root;
}



Tree deleteNode(Tree root, int key) {   // 노드가 있다고 가정하고 짜야돼
    Tree current = root;
    Tree parent = root;
    Tree node = root;
    if (root->value == key) {   // 지워야할 노드가 root 인 경우, root는 no parent
        if (root->left && root->right) {        // 자식이 둘 다 있는 경우
            current = current->right;
            while (1) {
                if (current->left == NULL) {
                    break;
                }
                parent = current;
                current = current->left;
            }

            int tmp = current->value;       // value swap
            current->value = root->value;
            root->value = tmp;

            if (current->right == NULL) {
                if (root == parent) {
                    parent->right = NULL;
                }
                else {
                    parent->left = NULL;
                }
            }
            else {  // current->left == NULL
                parent->right = current->right;
            }
            current = NULL;
            free(current);
            return root;
        }
        else {  // 자식 노드가 1개만 있는 경우
            if (root->left == NULL) {
                current = root->right;
            }
            else {
                current = root->left;
            }
            root = NULL;
            free(root);
            return current;
        }
    }
    if (key > current->value) {     // 크면 오른쪽으로 넘기고
        current = current->right;
    }
    else if (key < current->value) {    // 작으면 왼쪽을 넘겨
        current = current->left;
    }

    while (1) {
        if (current->value == key) {    // 삭제할 노드 발견
            // 발견 했을 때 자식 노드가 2개 다 있는 경우 -> 오른쪽 서브트리에서 최소 값 찾기
            if (current->left && current->right) {
                parent = current;
                current = current->right;   // 오른쪽으로 넘겨주기
                node = current;
                while (1) {
                    if (current->left == NULL) {
                        break;
                    }
                    node = current;
                    current = current->left;
                }
                int tmp = current->value;       // value swap
                current->value = parent->value;
                parent->value = tmp;

                if (current->right) {   // right 가 존재한다면
                    if (node->value < current->right->value) {
                        parent->right = current->right;
                    }
                    else {
                        node->left = current->right;
                    }
                }
                else {
                    node->left = NULL;
                }
                break;
            }
            // 자식노드가 1개 또는 없는 경우
            else {
                if (current->left == NULL && current->right == NULL) {  // 자식노드가 없는경우
                    if (parent->left == current) {
                        parent->left = NULL;
                    }
                    else {
                        parent->right = NULL;
                    }
                    break;
                }
                else if (current->left == NULL && current->right) {    // 왼쪽 자식노드는 없고 오른쪽은 있는 경우
                    if (parent->value > current->right->value) {
                        parent->left = current->right;
                    }
                    else {
                        current->right->left = current->left;
                        parent->right = current->right;
                    }
                    break;
                }
                else if (current->right == NULL && current->left) {  // 오른쪽 자식노드는 없고 왼쪽은 있는 경우
                    if (parent->value > current->left->value) {
                        parent->left = current->left;
                    }
                    else {
                        parent->right = current->left;
                    }
                    break;
                }
            }

        }
        if (key > current->value) {
            parent = current;
            current = current->right;
        }
        else if (key < current->value) {
            parent = current;
            current = current->left;
        }
    }
    current = NULL;
    free(current);
    return root;
}
int findNode(Tree root, int key) {  // return true, false
    Tree current = root;
    while (1) {
        if (current == NULL) {
            return false;
        }
        if (current->value == key) {    // 찾으면 1 반환
            return true;
        }
        else if (current->value < key) {
            current = current->right;
        }
        else if (current->value > key) {
            current = current->left;
        }
    }
}
void printInorder(Tree root) {
    if (root) {
        printInorder(root->left);
        fprintf(fout, "%d ", root->value);
        printInorder(root->right);
    }
}
void deleteTree(Tree root) {
    if (root) {
        deleteTree(root->left);
        deleteTree(root->right);
        free(root);
    }
}


















