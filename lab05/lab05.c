#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

FILE *fin;
FILE *fout;

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree {
    int left_thread;    // flag if ptr is thread
    ThreadedPtr left_child;
    ElementType data;
    ThreadedPtr right_child;
    int right_thread;   // flag if ptr is thread
} ThreadedTree;



ThreadedPtr CreateTree();
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx);
void printInorder(ThreadedPtr root);
void DeleteTree(ThreadedPtr root);

int main(int argc, char * argv[]) {

    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    ThreadedPtr root = CreateTree();

    if (root == NULL) {
        exit(1);
    }

    int NUM_NODES;
    fscanf(fin, "%d", &NUM_NODES);      // 입력받을 노드의 갯수

    int root_idx = 0, idx = 0;

    while (++idx <= NUM_NODES) {
        ElementType data;
        fscanf(fin, "%d", &data);       // 데이터 하나 입력받고

        if (Insert(root, root_idx, data, idx) == 0) {   // Insert가 실패하면 함수 종료
            return 0;
        }
    }

    printInorder(root);

    DeleteTree(root);

    fclose(fin);
    fclose(fout);

    return 0;
}

ThreadedPtr CreateTree() {
    ThreadedPtr tree = NULL;
    tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));
    if (tree == NULL) {
        fprintf(fout, "Out of Space!\n");
        return NULL;
    }
    tree->data = -1;
    tree->left_thread = 1;
    tree->left_child = tree;
    tree->right_thread = 1;
    tree->right_child = tree;
    return tree;
}
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx) {
    int count = root_idx + 1;   // 시작할 때 1번 노드에서 탐색하려고

    if (data < 0) {
        return false;
    }

    ThreadedPtr current = root; // 처음에 시작할 노드를 다 root 로 설정
    ThreadedPtr parent = root;
    ThreadedPtr tmp = (ThreadedPtr)malloc(sizeof(struct ThreadedTree));

    if (tmp == NULL) {
        fprintf(fout, "Out of Space!\n");
        return false;
    }
    tmp->data = data;
    tmp->left_thread = true;    // tmp 는 어차피 항상 리프노드 이므로 쓰레드 무조건 존재
    tmp->right_thread = true;

    if (count == idx) { // 첫번째로 삽입하는 노드이면
        tmp->right_child = root;
        tmp->left_child = root->left_child;
        root->right_child = tmp;
        root->right_thread = false;
        return true;
    }

    current = current->right_child; // 1번 노드부터 시작

    // 노드를 직접 찾아가면서 삽입하기
    while (1) {
        // left_child 노드
        if (count * 2 == idx) {
            tmp->left_child = current->left_child;  // 쓰레드 연결 먼저
            tmp->right_child = current;
            current->left_thread = false;
            current->left_child = tmp;
            break;
        }
        // right_child 노드
        if (count == idx) {
            tmp->left_child = current;
            tmp->right_child = current->right_child;
            current->right_thread = false;
            current->right_child = tmp;
            break;
        }

        if (count * 2 > idx) {  // depth 가 더 높은 노드가 아니면
            count++;        // 이게 sibling 노드로 넘어가니까
            if (current->right_child == parent) {   // 부모 노드가 같으면 반대편 서브트리로 이동하지 않으니까
                if (parent->right_thread == true) {     // 근데 parent의 right_child 가 존재하지 않는다면
                    current = parent;                   // NULL 값이니까 segmentation fault 방지
                }
                else {
                    current = parent->right_child;  // parent는 동일
                }

            }
            else {      // current->right_child != parent
                if (current->right_child == root->right_child) {
                    count = 3;
                    parent = current->right_child;
                    current = parent->right_child;
                    continue;
                }
                if (current->right_child == root->right_child->left_child) {
                    count = 5;
                    parent = current->right_child;
                    current = parent->right_child;
                    continue;
                }
                count = count / 2;              // 인덱스를 2로 나눠서 반대편 서브트리로 보내야돼
                parent = current->right_child;
                current = parent->right_child;
            }

            continue;
        }

        count *= 2;

        parent = current;
        current = current->left_child;
    }

    return true;

}

void printInorder(ThreadedPtr root) {
    ThreadedPtr before = root;
    ThreadedPtr current;
    fprintf(fout, "inorder traversal : ");
    while (1) {
        current = before->right_child;
        if (before->right_thread == false) {
            while (current->left_thread == false) {
                current = current->left_child;
            }
        }
        before = current;
        if (current == root) {
            break;
        }
        fprintf(fout, "%d ", current->data);
    }
    fprintf(fout, "\n");
}

void DeleteTree(ThreadedPtr root) {
    ThreadedPtr before = root;
    ThreadedPtr current;

    while (1) {
        current = before->right_child;
        free(before);
        if (before->right_thread == false) {
            while (current->left_thread == false) {
                current = current->left_child;
            }
        }
        before = current;
        if (current == root) {
            break;
        }
    }
}


