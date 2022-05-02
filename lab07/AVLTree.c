#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
typedef int ElementType;

typedef struct AVLNode {
    ElementType element;
    AVLTree left, right;
    int height;
} AVLNode;


AVLTree Insert(ElementType x, AVLTree tree);
AVLTree Delete(ElementType x, AVLTree tree);
AVLTree FindMin(AVLTree tree);
// rotate 방향으로 하냐 아니면 함께 옮기는 서브 트리로 하냐 난 전자로
Position SingleRotateWithLeft(Position node);   // LL (right rotate)
Position SingleRotateWithRight(Position node);  // RR (left rotate)
Position DoubleRotateWithLeft(Position node);   // RL
Position DoubleRotateWithRight(Position node);  // LR


void PrintInorder(AVLTree node);
void DeleteTree(AVLTree node);

int Height(Position node);
int max(int a, int b);
int main(int argc, char *argv[]) {

//    argv[1] = "input.txt";
//    argv[2] = "output.txt";

    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    AVLTree Tree = NULL;
    char cv;
    int key;

    while (!feof(fin)) {
        fscanf(fin, "%c", &cv);
        switch (cv) {
            case 'i':
                fscanf(fin, "%d\n", &key);
                Tree = Insert(key, Tree);
                break;
            case 'd':
                fscanf(fin, "%d\n", &key);
                Tree = Delete(key, Tree);
                break;
        }
        PrintInorder(Tree);
        fprintf(fout, "\n");
    }

    DeleteTree(Tree);
    fclose(fin);
    fclose(fout);

    return 0;
}

AVLTree Insert(ElementType x, AVLTree tree) {
    if (tree == NULL) {                // tree의 위치를 찾았을 때
        tree = (Position)malloc(sizeof(AVLNode));
        tree->left = NULL;
        tree->right = NULL;
        tree->height = 0;
        tree->element = x;
        return tree;
    }
    else if (tree->element == x) {      // 삽입하려 하는 노드가 이미 있는 경우
        fprintf(fout, "insertion error : %d is already in the tree!\n", x);
        return tree;
    }
    else if (x < tree->element) {

        tree->left = Insert(x, tree->left);

        // 높이 차이가 1 보다 클 때 (left가 더 큰 경우)
        if (Height(tree->left) - Height(tree->right) > 1) {

            if (x < tree->left->element) {
                tree = SingleRotateWithLeft(tree);    // LL   right rotate
            }
            else {
                tree = DoubleRotateWithRight(tree);    // LR  -> LL(left rotate) -> right rotate
            }

        }
    }
    else if (x > tree->element) {

        tree->right = Insert(x, tree->right);

        // 높이 차이가 1보다 클 때 (right가 더 큰 경우)
        if (Height(tree->right) - Height(tree->left) > 1) {

            if (x > tree->right->element) {
                tree = SingleRotateWithRight(tree);   // RR   left rotate
            }
            else {
                tree = DoubleRotateWithLeft(tree);   // RL -> RR (right rotate) -> left rotate
            }

        }
    }
    if (Height(tree->left) < Height(tree->right)) {
        tree->height = Height(tree->right) + 1;
    }
    else {
        tree->height = Height(tree->left) + 1;
    }

    return tree;
}

Position SingleRotateWithLeft(Position node) {      // LL (right rotate)

    Position left = node->left;

    node->left = left->right;
    left->right = node;

    node->height = max(Height(node->left), Height(node->right)) + 1;
    left->height = max(Height(left->left), Height(node)) + 1;

    return left;
}

Position SingleRotateWithRight(Position node) {     // RR (left rotate)

    Position right = node->right;

    node->right = right->left;
    right->left = node;

    node->height = max(Height(node->left), Height(node->right)) + 1;
    right->height = max(Height(right->right), Height(node)) + 1;

    return right;
}

void PrintInorder(AVLTree node) {
    if (node) {
        PrintInorder(node->left);
        fprintf(fout, "%d(%d) ", node->element, node->height);
        PrintInorder(node->right);
    }
}

int Height(Position node) {
    if (node == NULL) {
        return -1;
    }
    return node->height;
}

Position DoubleRotateWithRight(Position node) {      // LR

    node->left = SingleRotateWithRight(node->left);

    return SingleRotateWithLeft(node);
}

Position DoubleRotateWithLeft(Position node) {  // RL

    node->right = SingleRotateWithLeft(node->right);

    return SingleRotateWithRight(node);
}

void DeleteTree(AVLTree node) {
    if (node) {
        DeleteTree(node->left);
        DeleteTree(node->right);
        free(node);
    }
}

AVLTree Delete(ElementType x, AVLTree tree) {
    Position tmp;
    if (tree == NULL) {
        fprintf(fout, "deletion error : %d is not in the tree!\n", x);
        return tree;
    }
    else if (x < tree->element) {

        tree->left = Delete(x, tree->left);

        if (Height(tree->right) - Height(tree->left) > 1) {

            if (x > tree->right->element) {
                tree = SingleRotateWithRight(tree);
            }
            else {
                tree = DoubleRotateWithLeft(tree);
            }
        }
        tree->height = max(Height(tree->left), Height(tree->right)) + 1;
    }
    else if (x > tree->element) {

        tree->right = Delete(x, tree->right);

        if (Height(tree->left) - Height(tree->right) > 1) {

            if (x < tree->left->height) {
                tree = SingleRotateWithLeft(tree);
            }
            else {
                tree = DoubleRotateWithRight(tree);
            }
        }
        tree->height = max(Height(tree->left), Height(tree->right)) + 1;
    }
    else if (tree->left && tree->right) {   // 값 찾음
        tmp = FindMin(tree->right);
        tree->element = tmp->element;
        tree->right = Delete(tmp->element, tree->right);

        tree->height = max(Height(tree->left), Height(tree->right)) + 1;

    }
    else {          // 1 or 0 child
        tmp = tree;

        if (tree->left == NULL) {
            tree = tree->right;
        }
        else {
            tree = tree->left;
        }
        free(tmp);
    }

    return tree;
}

AVLTree FindMin(AVLTree tree) {
    if (tree->left == NULL) {
        return tree;
    }
    return FindMin(tree->left);
}
int max(int a, int b) {
    if (a < b) {
        return b;
    }
    return a;
}
