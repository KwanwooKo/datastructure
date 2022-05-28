#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0

FILE *fin;
FILE *fout;

typedef struct _Queue {
    int* key;
    int front, rear, qsize, max_queue_size;
} Queue;

typedef struct _Graph {
    int size;
    int* node;
    int** matrix;
} Graph;

Graph* CreateGraph(int* nodes, int n);      // solved
void InsertEdge(Graph* G, int a, int b);    // solved
void PrintGraph(Graph* G);                  // solved
void DeleteGraph(Graph* G);                 // solved
void Topsort(Graph* G);                     // solved

Queue* MakeNewQueue(int X);                 // solved
int IsEmpty(Queue* Q);                      // solved
void Enqueue(Queue* Q, int X);              // solved
int Dequeue(Queue* Q);                      // solved
void DeleteQueue(Queue* Q);                 // solved

// function which is given
void countInput(int* n, char* str);
void parseInput(int* arr, char* str, int n);

int main(int argc, char* argv[]) {

//    argv[1] = "input.txt";
//    argv[2] = "output.txt";

    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char numbers[100], inputs[150];
    fgets(numbers, 100, fin);                                 // read the first line of input == numbers
    int n = 0;                                                // the number of nodes
    countInput(&n, numbers);
    int* nodes = (int *)malloc(sizeof(int) * n);        // make the nodes array
    parseInput(nodes, numbers, n);                  // input the value of nodes into nodes array

    Graph* G = CreateGraph(nodes, n);                       // make a graph

    fgets(inputs, 100, fin);                                // read the second line of input == inputs
    int len = strlen(inputs), i, j;
    for (i = 0; i < len; i += 4) {
        int a = inputs[i] - '0', b = inputs[i + 2] - '0';
        InsertEdge(G, a, b);
    }

    PrintGraph(G);
    Topsort(G);
    DeleteGraph(G);

    fclose(fin);
    fclose(fout);

    return 0;
}

Graph* CreateGraph(int* nodes, int n) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->size = n;            // number of nodes
    for (int i = 0; i < n; i++) {       // sort the array of nodes
        for (int j = i; j < n; j++) {
            if (nodes[j] < nodes[i]) {
                int tmp = nodes[j];
                nodes[j] = nodes[i];
                nodes[i] = tmp;
            }
        }
    }
    graph->node = nodes;        // array of nodes
    graph->matrix = (int **)malloc(sizeof(int *) * n);      // matrix of nodes
    for (int i = 0; i < n; i++) {
        graph->matrix[i] = (int *)malloc(sizeof(int) * n);
    }
    return graph;
}

void InsertEdge(Graph* G, int a, int b) {
    // find the index of a and b
    int aIndex, bIndex;
    for (int i = 0; i < G->size; i++) {
        if (G->node[i] == a) aIndex = i;
        if (G->node[i] == b) bIndex = i;
    }
    G->matrix[aIndex][bIndex] = 1;
}
void PrintGraph(Graph* G) {
    fprintf(fout, " ");
    for (int j = 0; j < G->size; j++) {
        fprintf(fout, "%3d", G->node[j]);
    }
    fprintf(fout, "\n");
    for (int i = 0; i < G->size; i++) {
        fprintf(fout, "%d", G->node[i]);
        for (int j = 0; j < G->size; j++) {
            fprintf(fout, "%3d", G->matrix[i][j]);
        }
        fprintf(fout, "\n");
    }
    fprintf(fout, "\n");
}
void DeleteGraph(Graph* G) {
    free(G->node);
    for (int i = 0; i < G->size; i++) {
        free(G->matrix[i]);
    }
    free(G);
}
void Topsort(Graph* G) {
    int* indegree = (int*)malloc(sizeof(int) * G->size);    // make the array of indegree
    for (int i = 0; i < G->size; i++) {
        indegree[i] = 0;                                // initialize the value of indegree
    }

    for (int i = 0; i < G->size; i++) {
        for (int j = 0; j < G->size; j++) {
            if (G->matrix[i][j] == 1) {
                indegree[j]++;
            }
        }
    }
    Queue* q = MakeNewQueue(G->size);
    for (int i = 0; i < G->size; i++) {         // find the indegree index which value is zero
        if (indegree[i] == 0) {
            Enqueue(q, i);
        }
    }
    if (IsEmpty(q)) {                        // if there is no zero value then print the error message
        fprintf(fout, "sorting error : cycle!\n");
        return;
    }
    while (!IsEmpty(q)) {
        int front = Dequeue(q);
        fprintf(fout, "%d ", G->node[front]);
        for (int i = 0; i < G->size; i++) {
            if (G->matrix[front][i] == 1) {
                indegree[i]--;
                if (indegree[i] == 0)
                    Enqueue(q, i);
            }
        }
    }
    fprintf(fout, "\n");
    DeleteQueue(q);
}
Queue* MakeNewQueue(int X) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->key = (int *)malloc(sizeof(int) * X);
    q->max_queue_size = X;
    q->front = 0;
    q->rear = -1;
    q->qsize = 0;
    return q;
}
int IsEmpty(Queue* Q) {
    if (Q->qsize == 0) {
        return true;
    }
    return false;
}
void Enqueue(Queue* Q, int X) {
    Q->qsize++;
    Q->key[++Q->rear] = X;
}
int Dequeue(Queue* Q) {
    Q->qsize--;
    return Q->key[Q->front++];
}
void DeleteQueue(Queue* Q) {
    free(Q->key);
    free(Q);
}

void countInput(int* n, char* str) {
    int len = strlen(str), i;
    for (i = 0; i < len; i++)
        if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}

void parseInput(int* arr, char* str, int n) {
    int len = strlen(str), i;
    int cnt = 0;
    for (i = 0; i < len; i++)
        if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0';
}



