#include <stdio.h>
#include <stdlib.h>

typedef struct Node* PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;
struct Node {
    ElementType element;
    Position next;
};

List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
void Insert( ElementType X, List L, Position P );
void PrintList(List L);
void Delete( ElementType X, List L );
Position Find(ElementType X, List L);
Position FindPrevious ( ElementType X, List L );
void DeleteList ( List L );

FILE *input; FILE *output;
int main(int argc, char *argv[]) {
    char command;
    int key1, key2;
    Position header = NULL;
    Position tmp = NULL;

    if (argc == 1) {
        input = fopen("input.txt", "r");
        output = fopen("output.txt", "w");
    }
    else {
        input = fopen(argv[1], "r");
        output = fopen(argv[2], "w");
    }
    header = MakeEmpty(header);
    while (1) {
        command = fgetc(input);
        if (feof(input))
            break;
        switch (command) {
            case 'i':
                fscanf(input, "%d %d", &key1, &key2);
                //duplication check
                tmp = Find(key1, header);   // NULL
                if (tmp != NULL) {
                    fprintf(output, "insertion(%d) failed : the key already exists in the list\n", key1);
                    break;
                }
                tmp = Find(key2, header);
                if (tmp == NULL) {
                    fprintf(output, "insertion(%d) failed : can not find the location to be inserted\n", key1);
                    break;
                }
                Insert(key1, header, tmp);
                break;
            case 'd':
                fscanf(input, "%d", &key1);
                Delete(key1, header);
                break;
            case 'f':
                fscanf(input, "%d", &key1);
                tmp = FindPrevious(key1, header);
                if (IsLast(tmp, header))
                    fprintf(output, "finding(%d) failed : element %dis not in the list\n", key1, key1);
                else {
                    if (tmp->element > 0)
                        fprintf(output, "key of the previous node of %d is %d\n", key1, tmp->element);
                    else {
                        fprintf(output, "key of the previous node of %d is head\n", key1);
                    }
                }
                break;
            case 'p':
                PrintList(header);
                break;
            case '\n':
                break;
            default:
                break;
        }

    }

    DeleteList(header);
    fclose(input);
    fclose(output);
    return 0;
}
// 노드를 탐색하면서 current 노드가 null이 될 때 까지 출력
void PrintList(List L) {
    List current = L->next;
    if (current == NULL) {
        fprintf(output, "empty List!\n");
        return;
    }
    while (1) {
        if (current == NULL) {
            break;
        }
        fprintf(output, "key:%d ", current->element);
        current = current->next;
    }
}
// 노드를 찾는 함수인데, 리스트 전체를 탐색하면서 결과 찾기
Position Find(ElementType X, List L) {
    // L이 헤더
    Position current = L;   // 현재 current는 헤더
    while (1) {
        if (current == NULL) {
            return NULL;
        }
        if (current->element == X) {
            break;
        }
        current = current->next;
    }
    return current;
}

// 노드 삽입 함수
void Insert(ElementType X, List L, Position P) {
    Position tmp = (Position)malloc(sizeof(Position));
    // tmp가 header 인 경우
    if (P == L) {
        // 헤더 중에서도 헤더만 존재하는 경우
        if (L->next == NULL) {
            L->next = tmp;
            tmp->next = NULL;
            tmp->element = X;
        }
        // 헤더 뒤에 노드가 있는 경우
        else {
            tmp->next = L->next;
            L->next = tmp;
            tmp->element = X;
        }
        return;
    }
    // tmp가 header가 아닌 경우
    tmp->next = P->next;   // P->next의 위치를 알고 있을 때 먼저 연결
    P->next = tmp;         // P->next를 node로 변경
    tmp->element = X;      // node에 값을 넣어줌
    return;
}

// header 를 만들어주는 함수
List MakeEmpty(List L) {
    L = (List)malloc(sizeof(List));
    L->element = -1;     // header->element를 그냥 -1로 초기화 했습니다. (insert에서 -1 뒤에 넣는게 바로 헤더라고 했기 떄문에)
    L->next = NULL;    // header 하나 밖에 없기 때문에 header 다음 노드는 NULL로 초기화
    return L;
}

int IsEmpty( List L ) {
    if (L == NULL) {    // Header가 비어있다면
        return 1;       // 참
    }
    else {              // Header가 비어있지 않다면
        return 0;       // 거짓
    }
}

// P가 이전노드 , L이 헤더
int IsLast(Position P, List L) {
    if (P->next == NULL) {
        return 1;
    }
    else {
        return 0;
    }
}


//삭제 함수, X = key1, L = header
void Delete(ElementType X, List L) {
    Position before = FindPrevious(X, L);  // 이전노드
    Position current = Find(X, L);         // 현재노드
    // current가 NULL인 경우 -> element 값이 x인 노드가 없는 경우
    if (current == NULL) {
        fprintf(output, "deletion(%d) failed : element[%d] is not in the list\n", X, X);
        return;
    }
    // 노드가 존재하는 경우
    else {
        before->next = current->next;
        current->next = NULL;
        current = NULL;
    }
    free(current);
}
// 이전 노드를 찾는 함수
Position FindPrevious(ElementType X, List L) {
    Position current = L;
    // 헤더 하나 밖에 없는경우
    if (current->next == NULL) {
        return L;       // 헤더 반환
    }
    while (1) {
        // 이게 next가 NULL인 이유는 current가 null인 경우에 break를 하게 되면
        if (current->next == NULL) {
            break;
        }
        // 이 if 문이 실행이 안되기 때문
        if (current->next->element == X) {
            return current;
        }
        current = current->next;
    }
    return current;
}
// 리스트를 제거
void DeleteList(List L) {
    Position current = L->next; // 헤더만 남기고 남은 노드들 삭제
    while (current != NULL) {
        Position node = current->next;  // 다음 주소로 옮기기 위해 임시 공간에 저장
        free(current);                  // 메모리 해제
        current = node;                 // current를 다시 node 로 설정
    }
}







