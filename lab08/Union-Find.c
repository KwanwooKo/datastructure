#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
    int size_maze;
    int *ptr_arr;
} DisjointSets;

void init(DisjointSets *sets, DisjointSets *maze_print, int num);   // 제공됨
void Union(DisjointSets *sets, int i, int j);                   // 해결
int find(DisjointSets *sets, int i);                            // 해결
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num);     // 해결
void printMaze(DisjointSets *maze_print, int num);                    // 해결
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);    // 해결

int abs(int num);                                               // 추가한 함수
int max(int a, int b);                                          // 추가한 함수

int main(int argc, char* argv[]) {

//    argv[1] = "input.txt";
//    argv[2] = "output.txt";

    srand((unsigned int)time(NULL));

    int num, i;

    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    DisjointSets *sets, *maze_print;
    fscanf(fin, "%d", &num);

    sets = (DisjointSets*)malloc(sizeof(DisjointSets));
    maze_print = (DisjointSets*)malloc(sizeof(DisjointSets));

    init(sets, maze_print, num);
    createMaze(sets, maze_print, num);
    printMaze(maze_print, num);

    freeMaze(sets, maze_print);

    fclose(fin);
    fclose(fout);

    return 0;
}


void init(DisjointSets *sets, DisjointSets *maze_print, int num) {

    sets->size_maze = num * num;
//    값이 음수인 노드가 루트 노드
    sets->ptr_arr = (int*)malloc(sizeof(int) * (sets->size_maze + 1));
    for (int i = 1; i <= sets->size_maze; i++) {
        sets->ptr_arr[i] = 0;   // rank를 사용하기 위해서 루트 노드는 경로 횟수에 - 붙여서 저장
    }

    maze_print->size_maze = num * num * 2;  // size를 왜 2배를 해줬지?
    maze_print->ptr_arr = (int*)malloc(sizeof(int) * (maze_print->size_maze + 1));
    for (int i = 1; i <= maze_print->size_maze; i++) {
        maze_print->ptr_arr[i] = 1; // 모두 벽을 설치
    }
//    maze_print->ptr_arr[(x-1)*2+1 ~ x*2] : two directions(right, down) walls of the number x
//    x 가 현재 노드 => 오른쪽 벽 = (x-1)*2+1, 아래 벽 = x*2
//    start and end position have not to have the wall
    maze_print->ptr_arr[maze_print->size_maze - 1] = 0; // -1 해주는게 맞아 (마지막 노드의 right wall 이라서)
}

void Union(DisjointSets *sets, int i, int j) {

    int iRoot = find(sets, i);
    int jRoot = find(sets, j);

    // 높이를 부호를 바꿔서 루트 index에 저장을 해서 절댓값을 씌워서 비교해줘야 함
    if (abs(sets->ptr_arr[iRoot]) < abs(sets->ptr_arr[jRoot])) {
        sets->ptr_arr[iRoot] = jRoot;
        sets->ptr_arr[jRoot] -= 1;
    }
    else {
        sets->ptr_arr[jRoot] = iRoot;
        sets->ptr_arr[iRoot] -= 1;
    }

}

// rank 쓰는 find
int find(DisjointSets *sets, int i) {
    if (sets->ptr_arr[i] < 1) {
        return i;
    }
    return find(sets, sets->ptr_arr[i]);
}

void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num) {
    int flag = 0;
    // sets 의 ptr_arr 을 이용해서 maze_print 의 간선 조정
    while (1) {
        int root = find(sets, 1);
        for (int i = 2; i <= num * num; i++) {      // 모든 노드의 부모가 다 같은 경우가 미로가 출구까지 연결돼 있는 경우
            if (find(sets, i) == root) {
                if (i == num * num) {
                    flag = 1;
                }
                continue;
            }
            else {
                break;
            }
        }
        if (flag == 1) {        // 모든 노드의 부모가 다 같으므로 break
            break;
        }

        int randI = rand() % (num * num) + 1;       // 1 ~ num * num 까지 랜덤으로 값을 받아옴
        int randJ = rand() % (num * num) + 1;       // 1 ~ num * num 까지 랜덤으로 값을 받아옴

        if (randI > randJ) {        // randI 와 randJ value swap => randI를 기준으로 보기 위해서
            int tmp = randI;
            randI = randJ;
            randJ = tmp;
        }
        if (find(sets, randI) == find(sets, randJ)) {
            continue;
        }
        if (randI + 1 == randJ) {       // randJ 가 randI의 right 일 경우
            if (randI % num == 0) {
                continue;
            }

            maze_print->ptr_arr[(randI-1)*2 + 1] = 0;
        }
        else if (randI + num == randJ) {    // randJ가 randI의 down 일 경우
            if (randJ > num * num) {
                continue;
            }
            maze_print->ptr_arr[randI * 2] = 0;
        }
        else {                              // 그 외의 케이스의 경우는 right 나 down 이 아니기 때문에 다시
            continue;
        }

        Union(sets, randI, randJ);      // maze 의 경로를 연결 한 뒤에 union
    }
}

void printMaze(DisjointSets *maze_print, int num) {

    int flag = 1;       // flag = 0 => horizontal , flag = 1 => vertical
    int count = 0;      // count = num 이 될때마다 \n
    // sets => maze_print
    for (int i = 1; i <= num * 2 + 1; i++) {    // 첫 줄 출력
        if (i % 2 == 1) {
            fprintf(fout, "+");
        }
        else {
            fprintf(fout, "---");
        }
    }
    fprintf(fout, "\n ");

    for (int i = 1; i <= num * num * 2 + 1; i += 2) {
        if (count == num) {
            if (flag == 1) {        // 이전 행이 vertical 이었다면
                fprintf(fout, "\n+");
                flag = 0;
                count = 0;
                i = i - (num * 2 - 1);
            }
            else {                  // 이전 행이 horizontal 이었다면
                fprintf(fout, "\n|");
                flag = 1;
                count = 0;
                i = i - 1;
            }
        }
        if (maze_print->ptr_arr[i] == 1 && flag == 1) {
            fprintf(fout, "   |");
        }
        else if (maze_print->ptr_arr[i] == 0 && flag == 1) {
            fprintf(fout, "    ");
        }
        else if (maze_print->ptr_arr[i] == 1 && flag == 0) {
            fprintf(fout, "---+");
        }
        else if (maze_print->ptr_arr[i] == 0 && flag == 0){
            fprintf(fout, "   +");
        }
        count++;

    }


}


void freeMaze(DisjointSets *sets, DisjointSets *maze_print) {
    free(sets->ptr_arr);
    free(sets);
    free(maze_print->ptr_arr);
    free(maze_print);
}

int abs(int num) {
    if (num < 0) {
        num = num * (-1);
    }
    return num;
}
int max(int a, int b) {
    if (a < b) {
        return b;
    }
    else {
        return a;
    }
}




