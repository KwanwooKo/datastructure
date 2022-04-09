#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEZERO 3

FILE *fin;
FILE *fout;

typedef struct stack {
    int *key;   // 얘가 스택의 값 역할
    int top;    // 얘가 top의 인덱스 역할
    int max_stack_size; // 20으로 고정
} Stack;

Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);

void DeleteStack(Stack* S);
int IsEmpty(Stack* S);
int IsFull(Stack* S);

int main(int argc, char *argv[]) {

    // 파일 입출력 받기 -> 실행파일을 실행하면서 입력을 받을 txt 파일과 출력을 할 txt파일을 입력
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    Stack* stack;
    char input_str[101];    // 문자열의 길이가 100이 최대라 해서 #을 포함한 크기를 101로 선언
    int max = 20, i = 0, a, b, result, error_flag = 0;

    fgets(input_str, 101, fin); // input에서 한 줄로 데이터 입력받음
    stack = CreateStack(max);   // stack 생성

    fprintf(fout, "top numbers : ");    // fout 파일로 출력
    while (input_str[i] != '#') {       // # 만나면 반복문 종료

        if (input_str[i] == '+') {  // + 일 때
            a = Pop(stack);      // 일단 pop 진행 (무조건 1개는 남아있을 테니 여기서는 Empty검사를 할 필요 없음)
            if (IsEmpty(stack)) {   // 두번째 pop을 하기 전에는 스택이 비어있을 수도 있으니 검사
                error_flag = ISEMPTY;   // 스택이 비어있으면 error_flag 처리 해주고 반복문 탈출
                break;
            }
            b = Pop(stack);         // 스택이 비어있지 않으니 연산을 위해 남은 하나도 pop
            Push(stack, b + a); // 연산한 값을 스택에 push
            // 나머지 연산들도 이와 같다
        }
        else if (input_str[i] == '-') { // - 일 때
            a = Pop(stack);
            if (IsEmpty(stack)) {
                error_flag = ISEMPTY;
                break;
            }
            b = Pop(stack);
            Push(stack, b - a);
        }
        else if (input_str[i] == '*') { // * 일 때
            a = Pop(stack);
            if (IsEmpty(stack)) {
                error_flag = ISEMPTY;
                break;
            }
            b = Pop(stack);
            Push(stack, a * b);
        }
        else if (input_str[i] == '/') { // / 일 때
            a = Pop(stack);
            if (IsEmpty(stack)) {
                error_flag = ISEMPTY;
                break;
            }
            b = Pop(stack);
            if (a == 0) {
                error_flag = DIVIDEZERO;
                break;
            }
            Push(stack, b / a);
        }
        else if (input_str[i] == '%') { // % 일 때
            a = Pop(stack);
            if (IsEmpty(stack)) {
                error_flag = ISEMPTY;
                break;
            }
            b = Pop(stack);
            if (a == 0) {
                error_flag = DIVIDEZERO;
                break;
            }
            Push(stack, b % a);
        }
        else {                              // 숫자 일 때 (숫자를 else 로 처리한 이유는 숫자임을 표현할 수 있는 방법이 없어서)
            if (IsFull(stack)) {        // 숫자일 때에는 push만 하기 때문에 스택이 다 차있는지만 확인하면 된다
                error_flag = ISFULL;        // 다 차있으면 error_flag 처리를 해주고 반복문 탈출
                break;
            }
            Push(stack, input_str[i] - '0');
        }

        // 여기서부터 다시 스켈레톤
        int t = Top(stack);
        fprintf(fout, "%d ", t);
        result = t;

        i++;
    }

    if (error_flag == ISFULL) {
        fprintf(fout, "\nerror : invalid postfix expression, stack is full!\n");
    }
    else if (error_flag == ISEMPTY) {
        fprintf(fout, "\nerror : invalid postfix expression, stack is empty!\n");
    }
    else if (error_flag == DIVIDEZERO) {
        fprintf(fout, "\nerror : invalid postfix expression, divide by zero!\n");
    }
    else {
        if (stack->top+1 > 1) {
            fprintf(fout, "\nerror : invalid postfix expression, %d element are left!\n", stack->top+1);
        }
        else {
            fprintf(fout, "\nevaluation result : %d\n", result);
        }
    }

    fclose(fin);
    fclose(fout);
    DeleteStack(stack);

    return 0;
}
// 스택을 생성해주는 함수
Stack* CreateStack(int max) {
    Stack* tmp = (Stack*)malloc(sizeof(Stack)); // 스택을 만들어 주고
    tmp->key = (int *)malloc(sizeof(int) * max);    // key 는 값이 들어갈 배열 이므로 배열을 동적할당
    tmp->max_stack_size = max;                           // 사이즈는 20으로 고정
    tmp->top = -1;                                      // 초기에는 배열이 비어있으니 -1로 초기화
    return tmp;
}
void Push(Stack* S, int X) {
    S->key[++S->top] = X;                           // 값을 넣어주고 top의 인덱스를 변경
}
int Pop(Stack* S) {
    int ret = S->key[S->top];                       // return을 위해서 변수 하나를 이용해서 값을 미리 저장하고
    S->top--;                                       // top의 인덱스를 변경
    return ret;                                     // 미리 저장한 ret값 반환
}
int Top(Stack* S) {
    return S->key[S->top];                          // top의 값을 반환
}

void DeleteStack(Stack* S) {
    free(S->key);                                   // key 배열을 없애주고
    free(S);                                        // 스택을 없애줌
}
int IsEmpty(Stack* S) {
    int index = S->top;                             // 인덱스가 -1이면 비어있는 스택 아니면 비어있지 않음
    if (index == -1) {
        return 1;
    }
    else {
        return 0;
    }
}
int IsFull(Stack* S) {                              // 인덱스가 사이즈보다 1작으면 다 차있는 스택 (인덱스가 0부터 시작하기 때문)
    int index = S->top;
    if (index == S->max_stack_size - 1) {
        return 1;
    }
    else {
        return 0;
    }
}



