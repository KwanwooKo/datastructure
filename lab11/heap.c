#include <stdio.h>
#include <stdlib.h>
#define INF 1e9
#define true 1
#define false 0
FILE *fin;
FILE *fout;

typedef struct HeapStruct {
    int Capacity;
    int Size;
    int *Elements;
} Heap;

Heap* CreateHeap(int heapSize);             // 해결
void Insert(Heap *heap, int value);         // 해결
int Find(Heap *heap, int value);            // 해결
int DeleteMax(Heap* heap);                  // 해결
void PrintHeap(Heap* heap);                 // 해결
int IsFull(Heap *heap);                     // 해결

void swap(int *a, int *b);                  // 새로 추가한 함수

int main(int argc, char* argv[]) {

//    argv[1] = "input.txt";
//    argv[2] = "output.txt";

    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char cv;
    Heap* maxHeap;
    int heapSize, key, max_element;

    while (!feof(fin)) {
        fscanf(fin, "%c", &cv);
        switch (cv) {
            case 'n':               // 첫째 줄에만 존재
                fscanf(fin, "%d", &heapSize);
                maxHeap = CreateHeap(heapSize);
                break;
            case 'i':
                fscanf(fin, "%d", &key);
                Insert(maxHeap, key);
                break;
            case 'd':
                max_element = DeleteMax(maxHeap);
                if (max_element != -INF) {
                    fprintf(fout, "max element : %d deleted\n", max_element);
                }
                break;
            case 'p':
                PrintHeap(maxHeap);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if (Find(maxHeap, key)) {
                    fprintf(fout, "%d is in the heap\n", key);
                }
                else {
                    fprintf(fout, "finding error : %d is not in the heap\n", key);
                }
                break;
        }
    }

    return 0;
}
Heap* CreateHeap(int heapSize) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->Elements = (int *)malloc(sizeof(int) * (heapSize + 1));   // 0번째 index 는 사용 안하기 때문
    for (int i = 1; i <= heap->Size; i++) {
        heap->Elements[i] = -1;
    }
    heap->Capacity = 0;
    heap->Size = heapSize;

    return heap;
}

void Insert(Heap *heap, int value) {
    if (IsFull(heap) == true) {
        fprintf(fout, "insert error : heap is full\n");
        return;
    }
    if (Find(heap, value) == true) {
        fprintf(fout, "insert error : %d is already in the heap\n", value);
        return;
    }

    int index = ++heap->Capacity;   // 현재 Capacity 값을 늘려줌
    heap->Elements[index] = value;
    if (index / 2 == 0) {
        fprintf(fout, "insert %d\n", heap->Elements[index]);
        return;
    }
    while (1) {
        if (index / 2 == 0) {
            break;
        }
        if (heap->Elements[index] > heap->Elements[index / 2]) {
            swap(&heap->Elements[index], &heap->Elements[index / 2]);
        }
        else {
            break;
        }
        index /= 2;
    }
    fprintf(fout, "insert %d\n", heap->Elements[index]);
}

int Find(Heap *heap, int value) {
    int isFind = false;
    for (int i = 1; i <= heap->Capacity; i++) {
        if (heap->Elements[i] == value) {
            isFind = true;
        }
    }
    return isFind;
}
int DeleteMax(Heap* heap) {
    if (heap->Capacity == 0) {
        fprintf(fout, "delete error : heap is empty\n");
        return -INF;
    }
    int index = 1;
    int child = 2;
    int max_element = heap->Elements[1];
    int last = heap->Elements[heap->Capacity--];
    while (1) {
        if (index > heap->Capacity) {
            break;
        }
        // child 의 index 설정
        child = index * 2;
        // 더 큰 child로 percolate 하기 위해서 큰 child 설정
        if (child < heap->Capacity && heap->Elements[child] < heap->Elements[child + 1]) {
            child++;
        }
        // 더이상 child가 없을 경우 => child 에 last 를 넣어주기 위해 if 문으로 확인
        if (last < heap->Elements[child]) {
            heap->Elements[index] = heap->Elements[child];
        }
        else {      // 만약 last가 더 큰 경우 => 더이상 child를 찾을 이유가 없으므로 break
            break;
        }
        index = child;
    }
    heap->Elements[index] = last;
    return max_element;
}

void PrintHeap(Heap* heap) {
    if (heap->Capacity == 0) {
        fprintf(fout, "print error : heap is empty\n");
        return;
    }
    for (int i = 1; i <= heap->Capacity; i++) {
        fprintf(fout, "%d ", heap->Elements[i]);
    }
    fprintf(fout, "\n");
}
int IsFull(Heap *heap) {
    int isFull = false;
    if (heap->Capacity == heap->Size) {
        isFull = true;
    }
    return isFull;
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
