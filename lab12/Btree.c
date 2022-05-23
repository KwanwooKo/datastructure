#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0

FILE *fin;
FILE *fout;

typedef struct BNode* BNodePtr;

struct BNode {
    int order;          // size < order
    int size;           // number of key
    BNodePtr *child;    // children pointers , array
    int *key;           // keys
    int is_leaf;
} BNode;

BNodePtr CreateTree(int order);             // 해결
void Insert(BNodePtr* root, int key);       // 해결
int Find(BNodePtr root, int key);           // 해결
void PrintTree(BNodePtr root);              // 해결
void DeleteTree(BNodePtr root);

// 추가한 함수
// 부모 노드를 알 수 있는 매개변수를 같이 넣어주면 좋지 않나?
// 그럼 nodeSplit을 할 때 return 값을 넣어주는게 좋지 않을까?
int isFull(BNodePtr root);
int main(int argc, char* argv[]) {

//    argv[1] = "input.txt";
//    argv[2] = "output.txt";

    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int order;
    fscanf(fin, "%d", &order);
    BNodePtr root = CreateTree(order);

    char cv;
    int key;
    while (!feof(fin)) {
        fscanf(fin, "%c", &cv);
        switch(cv) {
            case 'i':
                fscanf(fin, "%d", &key);
                if (Find(root, key))
                    fprintf(fout, "insert error : key %d is already in the tree!\n", key);
                else
                    Insert(&root, key); // root가 바뀌는 시점이 오니까?
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if (Find(root, key))
                    fprintf(fout, "key %d found\n", key);
                else
                    fprintf(fout, "finding error : key %d is not in the tree!\n", key);
                break;
            case 'p':
                if (root->size == 0)
                    fprintf(fout, "print error : tree is empty!");
                else
                    PrintTree(root);
                fprintf(fout, "\n");
                break;
        }
    }

    DeleteTree(root);
    fclose(fin);
    fclose(fout);

    return 0;
}

BNodePtr CreateTree(int order) {
    BNodePtr root = (BNodePtr)malloc(sizeof(BNode));
    root->order = order;
    root->size = 0;
    root->child = (BNodePtr*)malloc(sizeof(BNodePtr) * (order + 1));
    for (int i = 0; i < order; i++) {
        root->child[i] = NULL;
    }
    root->key = (int *)malloc(sizeof(int) * (order));
    root->is_leaf = true;
    return root;
}

// node split 이거를 함수로 구현하는게 좋나? ㄴㄴ
// 아 이게 (*root)->is_leaf 가 아니라 (*root)->child[i]->is_leaf로 해야되네;
// 옮겨 심는 걸 함수로 짜면? ㄴㄴ
void Insert(BNodePtr* root, int key) {
    int i, index, childIndex;
    BNodePtr child = NULL;
    if ((*root)->is_leaf == false) {
        if ((*root)->child[0]->is_leaf) {
            for (childIndex = 0; childIndex < (*root)->size; childIndex++) {
                if ((*root)->key[childIndex] > key)
                    break;
            }
//            childIndex를 받아왔네
            child = (*root)->child[childIndex];
            // 여기서 child에 key를 넣어주자
            for (index = 0; index < child->size; index++) {
                if (child->key[index] > key)
                    break;
            }
            for (i = child->size; i > index; i--) {
                child->key[i] = child->key[i - 1];
            }
            child->key[index] = key;
            child->size++;

            // split을 해야하는 경우 => root 에 값 추가 => root 에 way 하나 더 추가
            if (isFull(child)) {
//                nodeSplit
                child->child[0] = CreateTree((*root)->order);
                child->child[1] = CreateTree((*root)->order);
                int mid = (*root)->order / 2;
                int insertKey = child->key[mid];
                // child[0], child[1] 에 값 넣어줌
                for (i = 0; i < mid; i++) {
                    child->child[0]->key[i] = child->key[i];
                    child->child[0]->size++;
                }

                // 여기서 바로 바꾸면 안돼
                // 10이 들어갈 위치 찾기
                for (i = 0; i < (*root)->size; i++) {
                    if (insertKey < (*root)->key[i])
                        break;
                }
                for (index = (*root)->size; index > i; index--) {
                    (*root)->key[index] = (*root)->key[index - 1];
                }
                (*root)->key[i] = insertKey;
                (*root)->size++;
//                (*root)->key[childIndex] = child->key[mid];

                for (i = mid + 1; i < child->size; i++) {
                    child->child[1]->key[i - (mid + 1)] = child->key[i];
                    child->child[1]->size++;
                }
                for (i = (*root)->size; i > childIndex + 1; i--) {
                    (*root)->child[i] = (*root)->child[i - 1];
                }

                // 근데 이렇게 마구잡이로 해도 괜찮은건가?
                (*root)->child[childIndex] = child->child[0];
                (*root)->child[childIndex + 1] = child->child[1];
//                root 의 key value 를 바꿔야돼

                if (isFull(*root)) { // 이번엔 root 를 쪼개야 돼
//                    1. root 의 key 값 이사
//                    2. child 포인터 이사
//                    3. 이사 끝냈으니 이제 집어넣어줘야지
                    mid = (*root)->order / 2;
                    BNodePtr newRoot = CreateTree((*root)->order);
                    newRoot->key[0] = (*root)->key[mid];
                    newRoot->size++;
                    // child 2개 생성?
                    BNodePtr firstChild = CreateTree((*root)->order);
                    BNodePtr secondChild = CreateTree((*root)->order);
                    newRoot->child[0] = firstChild;
                    newRoot->child[1] = secondChild;
                    firstChild = (*root)->child[mid - 1];
                    secondChild = (*root)->child[mid + 1];
                    root = &newRoot;
                }
            }
        }
        else {  // root 의 child가 leaf 가 아닌 경우
            // 그럼 child 를 내려보내도 되니깐
            for (i = 0; i < (*root)->size; i++) {
                if (key > (*root)->key[i])
                    break;
            }
            Insert(&(*root)->child[i], key);
        }
    }
    else {  // root 가 leaf 인 경우
        // 그러면 root에 값을 추가해야 하니까
        for (i = 0; i < (*root)->size; i++) {
            if (key < (*root)->key[i])
                break;
        }
        for (index = (*root)->size; index > i; index--) {
            (*root)->key[index] = (*root)->key[index - 1];
        }
        (*root)->key[i] = key;
        (*root)->size++;
        if (isFull(*root)) {
            // nodeSplit
            (*root)->child[0] = CreateTree((*root)->order);
            (*root)->child[1] = CreateTree((*root)->order);
            int mid = (*root)->order / 2;
            for (i = 0; i < mid; i++) {
                (*root)->child[0]->key[i] = (*root)->key[i];
                (*root)->child[0]->size++;
            }
            for (i = mid + 1; i < (*root)->size; i++) {
                (*root)->child[1]->key[i - (mid + 1)] = (*root)->key[i];
                (*root)->child[1]->size++;
            }
            (*root)->key[0] = (*root)->key[mid];
            (*root)->size = 1;
            (*root)->is_leaf = false;
        }
    }
}


int Find(BNodePtr root, int key) {
    int i;
    if (root->is_leaf) {
        for (i = 0; i < root->size; i++) {
            if (root->key[i] == key) {
                return true;
            }
        }
        return false;
    }
    else {      // if root is not leaf node => go down to the child node
        for (i = 0; i < root->size; i++) {
            if (key == root->key[i]) {
                return true;
            }
            if (key < root->key[i]) {
                return Find(root->child[i], key);
            }
        }
        return Find(root->child[i], key);
    }
}

void PrintTree(BNodePtr root) {
    int i;
    for (i = 0; i < root->size; i++) {
        if (!(root->is_leaf)) {
            PrintTree(root->child[i]);
        }
        fprintf(fout, "%d ", root->key[i]);
    }
    if (!(root->is_leaf))
        PrintTree(root->child[i]);
}

int isFull(BNodePtr root) {
    if (root->size == root->order) {
        return true;
    }
    return false;
}

void DeleteTree(BNodePtr root){
    int i;
    if (root->is_leaf) {
        for (i = 0; i < root->size; i++) {
            free(root->child[i]);
        }
    }
    for (i = 0; i <= root->size; i++) {
        if (!root->is_leaf) {
            DeleteTree(root->child[i]);
        }
    }
    free(root);
}
