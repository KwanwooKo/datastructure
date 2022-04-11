#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct StackNode {
    int data;
    struct StackNode* next;
}StackNode;

typedef struct Node {
    int data;
    struct Node* nextNode;
}Node;

typedef struct LinkedList {
    int curCount;
    Node headNode;
}LinkedList;


void pushLinkedStack(StackNode** top, int data);
StackNode* topLinkedStack(StackNode* top);
void deleteLinkedStack(StackNode** top);
void addNode(LinkedList* pList, int pos, int data);
void reverseList(LinkedList* pList, StackNode** top);
void showNode(LinkedList* pList);
int isEmpty(StackNode* top);
int Pop(StackNode* top);


int main() {
    int pos;
    LinkedList* linkedList = (LinkedList*)malloc(sizeof(LinkedList));
    linkedList->curCount = 0;
    linkedList->headNode.nextNode = NULL;

    StackNode* top = NULL;
    StackNode* pNode = NULL;

    addNode(linkedList, 0, 10);
    addNode(linkedList, 5, 100);
    addNode(linkedList, 1, 20);
    addNode(linkedList, 2, 30);
    addNode(linkedList, 1, 50);
    addNode(linkedList, 1, 70);
    addNode(linkedList, 1, 40);

    showNode(linkedList);

    reverseList(linkedList, &top);

    /*makeEmpty(linkedList);*/
    showNode(linkedList);
    return 0;
}



void addNode(LinkedList* pList, int pos, int data) {
    int i = 0;
    Node* pNewNode = NULL;   // 새로 추가할 노드
    Node* current = &(pList->headNode);
    if (pList == NULL) {
        printf("addNode() error1\n");
        return;
    }
    if (pos < 0 || pos > pList->curCount) {
        printf("addNode() error2: 추가 범위 초과\n");
        return;
    }
    pNewNode = (Node*)malloc(sizeof(Node));
    pNewNode->data = data;
    pNewNode->nextNode = NULL;
    pList->curCount++;
    if (!pNewNode) {
        printf("addNode() error3 \n");
        return;
    }
    for (int i = 0; i < pos; i++) {
        current = current->nextNode;
    }
    if (current->nextNode == NULL) {
        current->nextNode = pNewNode;
    }
    else {
        pNewNode->nextNode = current->nextNode;
        current->nextNode = pNewNode;
    }

}

void reverseList(LinkedList* pList, StackNode** top) {
    Node* current = pList->headNode.nextNode;

    StackNode* sNode = NULL;

    printf("Reverse Linked List!\n");

    while (current != NULL) {
        pushLinkedStack(top, current->data);    // 스택노드 생성 하면서 push
        current = current->nextNode;
    }

    current = pList->headNode.nextNode;

    while (1) {
        if ((*top) == NULL) {
            break;
        }
        current->data = Pop((*top));
        (*top) = (*top)->next;
        current = current->nextNode;
    }

}
int Pop(StackNode* top) {
    StackNode* ret = top;
    top = top->next;
    return ret->data;
}
void pushLinkedStack(StackNode** top, int data) {

    StackNode* pNode = NULL;

    pNode = (StackNode*)malloc(sizeof(StackNode));
    pNode->data = data;
    pNode->next = NULL;

    if (isEmpty(*top)) {
        *top = pNode;
    }
    else {
        pNode->next = (*top);
        *top = pNode;
    }
}

int isEmpty(StackNode* top) {
    if (!top)
        return TRUE;
    else
        return FALSE;
}

void showNode(LinkedList* pList) {
    int i = 0;
    Node* pNode = NULL;

    if (pList == NULL) {
        printf("showNode() error\n");
        return;
    }

    printf("현재 Node 개수: %d \n", pList->curCount);
    pNode = pList->headNode.nextNode;

    while (1) {
        if (pNode == NULL) {
            break;
        }
        printf("[%d]\n", pNode->data);
        pNode = pNode->nextNode;
    }
    printf("----------------------\n");
}


