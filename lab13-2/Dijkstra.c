#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct Node {
    int vertex;         // current vertex
    int dist;           // distance
    int prev;           // previous vertex
} Node;

typedef struct Graph {
    int size;
    int** vertices;
    Node* nodes;
} Graph;

typedef struct Heap {
    int Capacity;
    int Size;
    Node* Element;
} Heap;

Graph* createGraph(int size);                                   // solved
void deleteGraph(Graph* g);                                     // solved
void printShortestPath(Graph* g);                               // solved
Heap* createMinHeap(int heapSize);                              // solved
void deleteMinHeap(Heap* minHeap);                              // solved
void insertToMinHeap(Heap* minHeap, int vertex, int distance);  // solved
Node deleteMin(Heap* minHeap);                                  // solved

int isEmpty(Heap* minHeap);                                     // solved

int main(int argc, char *argv[]) {

//    argv[1] = "input.txt";
//    argv[2] = "output.txt";

    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    Graph* g;
    int size;
    fscanf(fin, "%d\n", &size);
    g = createGraph(size + 1);

    char tmp = 0;
    while (tmp != '\n' && tmp != EOF) {
        int node1, node2, weight;
        fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
        g->vertices[node1][node2] = weight;
        tmp = fgetc(fin);
    }

    printShortestPath(g);

    deleteGraph(g);

    return 0;
}
Graph* createGraph(int size) {

    Graph* g = (Graph*)malloc(sizeof(Graph));

    g->size = size;
    g->nodes = (Node*)malloc(sizeof(Node) * size);
    g->vertices = (int**)malloc(sizeof(int*) * size);
    for (int i = 0; i < size; i++) {
        g->vertices[i] = (int*)malloc(sizeof(int) * size);
    }
    for (int i = 1; i < size; i++) {
        g->nodes[i].dist = INF;
        g->nodes[i].vertex = i;
    }
    for (int i = 1; i < size; i++) {
        for (int j = 1; j < size; j++) {
            g->vertices[i][j] = 0;
        }
    }
    return g;
}
void deleteGraph(Graph* g) {
    free(g->nodes);
    for (int i = 0; i < g->size; i++) {
        free(g->vertices[i]);
    }
    free(g->vertices);
}

void printShortestPath(Graph* g) {

    Heap* minHeap = createMinHeap(g->size);

    // first, insert the node which is adjacent to node 1
    for (int i = 1; i < g->size; i++) {
        if (g->vertices[1][i] != 0) {
            g->nodes[i].dist = g->vertices[1][i];   // store the distance from the first node
            g->nodes[i].prev = 1;                   // store the key of previous node
            // g->nodes[i].vertex = i;                 // store the key of node
            insertToMinHeap(minHeap, i, g->vertices[1][i]);
        }
    }

    while (!isEmpty(minHeap)) {
        Node node = deleteMin(minHeap);
        int from = node.vertex;
        for (int i = 1; i < g->size; i++) {
            int to = i;
            int weight = g->vertices[from][to];
            // if the edge were present
            if (g->vertices[from][to]) {
                // if the new dist is smaller than original dist
                if (g->nodes[to].dist > g->nodes[from].dist + weight) {
                    g->nodes[to].dist = g->nodes[from].dist + weight;
                    g->nodes[to].prev = from;
                    insertToMinHeap(minHeap, to, g->nodes[to].dist);
                }
            }
        }
    }

    for (int i = 2; i < g->size; i++) {
        // how to print the path by using previous node
        if (g->nodes[i].dist == INF) {
            fprintf(fout, "can not reach to node %d\n", g->nodes[i].vertex);
        }
        else {
            int* printList = (int*)malloc(sizeof(int) * g->size);
            int index = 0;
            int node = g->nodes[i].vertex;
            while (1) {
                if (g->nodes[node].vertex == 1) {
                    printList[index++] = node;
                    break;
                }
                printList[index++] = node;
                node = g->nodes[node].prev;
            }
            for (int j = index - 1; j >= 0; j--) {
                fprintf(fout, "%d", printList[j]);
                if (j != 0) {
                    fprintf(fout, "->");
                }
            }
            fprintf(fout, " (cost : %d)\n", g->nodes[i].dist);
            free(printList);
        }
    }
    deleteMinHeap(minHeap);
}

Heap* createMinHeap(int heapSize) {

    Heap* h = (Heap*)malloc(sizeof(Heap));

    h->Size = heapSize;
    h->Capacity = 0;
    h->Element = (Node*)malloc(sizeof(Node) * (heapSize));

    return h;
}

void deleteMinHeap(Heap* minHeap) {
    free(minHeap->Element);
    free(minHeap);
}

void insertToMinHeap(Heap* minHeap, int vertex, int distance) {

    Node tmp;
    // tmp.prev = -1;
    tmp.dist = distance;
    tmp.vertex = vertex;

    int index = ++minHeap->Capacity;
    if (minHeap->Capacity == 1) {
        // 여기서 에러남
        minHeap->Element[index].dist = distance;
        minHeap->Element[index].vertex = vertex;
        return;
    }
    while (1) {
        if (index == 1) {
            break;
        }
        if (distance < minHeap->Element[index / 2].dist) {
            minHeap->Element[index] = minHeap->Element[index / 2];
        }
        else {
            break;
        }
        index /= 2;
    }
    minHeap->Element[index] = tmp;
}

Node deleteMin(Heap* minHeap) {
    Node minElement = minHeap->Element[1];
    Node lastElement = minHeap->Element[minHeap->Capacity--];

    int i, child;
    for (i = 1; i * 2 <= minHeap->Capacity; i = child) {
        child = i * 2;
        if (child < minHeap->Capacity && minHeap->Element[child].dist > minHeap->Element[child + 1].dist) {
            child++;
        }
        if (lastElement.dist > minHeap->Element[child].dist) {
            minHeap->Element[i] = minHeap->Element[child];
        }
        else {
            break;
        }
    }

    minHeap->Element[i] = lastElement;

    return minElement;
}

int isEmpty(Heap* minHeap) {
    if (minHeap->Capacity == 0) {
        return 1;
    }
    return 0;
}

