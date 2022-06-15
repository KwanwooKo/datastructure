#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fin;
FILE *fout;

typedef int ElementType;
typedef ElementType List;
typedef struct HashTbl* HashTable;

typedef struct HashTbl {
    int TableSize;
    List *TheLists;
}HashTbl;

HashTable createTable(int TableSize);                           // solved
void Insert(HashTable H, ElementType Key, int solution);        // solved
void Delete(HashTable H, ElementType Key, int solution);        // solved
int Find(HashTable H, ElementType Key, int solution);           // solved
void printTable(HashTable H);                                   // solved
void deleteTable(HashTable H);                                  // solved

int isFull(HashTable H);
int main(int argc, char *argv[]) {

    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char solution_str[20];
    int solution, TableSize;

    fscanf(fin, "%s", solution_str);
    if (!strcmp(solution_str, "linear"))
        solution = 1;
    else if (!strcmp(solution_str, "quadratic"))
        solution = 2;
    else {
        fprintf(fout, "error: invalid hashing solution!");
        return 0;
    }

    fscanf(fin, "%d", &TableSize);

    HashTable H = createTable(TableSize);

    char cv;
    int key;
    while (!feof(fin)) {
        fscanf(fin, "%c", &cv);
        switch(cv) {
            case 'i':
                fscanf(fin, "%d", &key);
                Insert(H, key, solution);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                int result = Find(H, key, solution);
                if (result == -1)
                    fprintf(fout, "%d is not in the table\n", key);
                else
                    fprintf(fout, "%d is in the table\n", key);
                break;
            case 'd':
                fscanf(fin, "%d", &key);
                Delete(H, key, solution);
                break;
            case 'p':
                printTable(H);
                break;
        }
    }

    deleteTable(H);

    return 0;
}

HashTable createTable(int TableSize) {
    HashTable hash = (HashTable)malloc(sizeof(HashTbl));
    hash->TableSize = TableSize;
    hash->TheLists = (ElementType*)malloc(sizeof(ElementType) * TableSize);
    for (int i = 0; i < TableSize; i++) {
        hash->TheLists[i] = 0;
    }
    return hash;
}

void Insert(HashTable H, ElementType Key, int solution) {
    if (isFull(H)) {
        fprintf(fout, "insertion error : table is full\n");
        return;
    }
    int index = Key % H->TableSize;
    int cmpIndex = Find(H, Key, solution);
    int count = 1;
    if (cmpIndex != -1 && H->TheLists[cmpIndex] == Key) {
        fprintf(fout, "insertion error : %d already exists at address %d\n", Key, cmpIndex);
        return;
    }
    // linear case
    if (solution == 1) {
        while (1) {
            // found place to insert the key
            if (H->TheLists[index % H->TableSize] == 0) {
                fprintf(fout, "insert %d into address %d\n", Key, index % H->TableSize);
                H->TheLists[index % H->TableSize] = Key;
                break;
            }
            index++;
        }
    }
    // quadratic case
    if (solution == 2) {
        while (1) {
            // found place to insert the key
            if (H->TheLists[index % H->TableSize] == 0) {
                fprintf(fout, "insert %d into address %d\n", Key, index % H->TableSize);
                H->TheLists[index % H->TableSize] = Key;
                break;
            }
            else {
                index = Key % H->TableSize;
            }
            index += count * count;
            count++;
        }
    }
}

void Delete(HashTable H, ElementType Key, int solution) {
    int index = Find(H, Key, solution);
    if (index == -1) {
        fprintf(fout, "deletion error : %d is not in the table\n", Key);
    }
    else {
        fprintf(fout, "delete %d\n", Key);
        // erase the value, but not 0 because Find method returns false when the value is 0
        H->TheLists[index] = -1;
    }

}

int Find(HashTable H, ElementType Key, int solution) {
    int count = 1;
    int isFound = 0;
    int index = Key % H->TableSize;
    // linear case
    if (solution == 1) {
        while (1) {
            if (H->TheLists[index % H->TableSize] == Key) {
                isFound = 1;
                break;
            }
            else if (H->TheLists[index % H->TableSize] == 0) {
                break;
            }
            index++;
        }
    }
    // quadratic case
    if (solution == 2) {
        while (1) {
            if (H->TheLists[index % H->TableSize] == Key) {
                isFound = 1;
                break;
            }
            else if (H->TheLists[index % H->TableSize] == 0) {
                break;
            }
            else {
                index = Key % H->TableSize;
            }
            index += count*count;
            count++;
        }
    }
    if (isFound) {
        return index % H->TableSize;
    }
    else {
        return -1;
    }
}

void printTable(HashTable H) {
    for (int i = 0; i < H->TableSize; i++) {
        if (H->TheLists[i] == -1) {
            fprintf(fout, "0 ");
        }
        else {
            fprintf(fout, "%d ", H->TheLists[i]);
        }
    }
    fprintf(fout, "\n");
}

void deleteTable(HashTable H) {
    free(H->TheLists);
    free(H);
}

int isFull(HashTable H) {
    int ret = 1;
    for (int i = 0; i < H->TableSize; i++) {
        if (H->TheLists[i] == 0) {
            ret = 0;
            break;
        }
    }
    return ret;
}



