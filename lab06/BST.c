#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
typedef struct BST {
    int value;
    struct BST* left;
    struct BST* right;
}BST;

Tree insertNode(Tree root, int key);
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(Tree root);
void deleteTree(Tree root);

int main(int argc, char* argv[]) {

    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char cv;
    int key;

    Tree root = NULL;

    while (!feof(fin)) {
        fscanf(fin, "%c", &cv);
        switch(cv) {
            case 'i':
                fscanf(fin, "%d", &key);
                root = insertNode(root, key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if (findNode(root, key))
                    fprintf(fout, "%d is in the tree\n", key);
                else
                    fprintf(fout, "finding error: %d is not in the tree\n", key);
                break;
            case 'd':
                fscanf(fin, "%d", &key);
                if (findNode(root, key)) {
                    root = deleteNode(root, key);
                    fprintf(fout, "delete %d\n", key);
                }
                else {
                    fprintf(fout, "deletion error: %d is not in the tree\n", key);
                }
                break;
            case 'p':
                fscanf(fin, "%c", &cv);
                if (cv == 'i') {
                    if (root == NULL) {
                        fprintf(fout, "tree is empty");
                    }
                    else {
                        printInorder(root);
                    }
                }
                fprintf(fout, "\n");
                break;
        }
    }
    deleteTree(root);
    return 0;
}
Tree insertNode(Tree root, int key) {
    Tree current = root;
    Tree tmp = (Tree)malloc(sizeof(BST));

    tmp->value = key;
    while (1) {
        if (current == NULL) {
            fprintf(fout, "insert %d\n", key);
            return tmp;
        }
        if (current->left && current->right) {  // ????????? ??? ??? ????????? ?????? ??????
            if (key > current->value) {
                current = current->right;
            }
            else if (key < current->value) {
                current = current->left;
            }
        }
        else {      // current->left ?????? current->right ??? ????????????
            if (key > current->value) {
                if (current->right == NULL) {
                    current->right = tmp;
                    break;
                }
                current = current->right;
            }
            else if (key < current->value) {
                if (current->left == NULL) {
                    current->left = tmp;
                    break;
                }
                current = current->left;
            }
        }
    }
    fprintf(fout, "insert %d\n", key);
    return root;
}



Tree deleteNode(Tree root, int key) {   // ????????? ????????? ???????????? ?????????
    Tree current = root;
    Tree parent = root;
    Tree node = root;
    if (root->value == key) {   // ???????????? ????????? root ??? ??????, root??? no parent
        if (root->left && root->right) {        // ????????? ??? ??? ?????? ??????
            current = current->right;
            while (1) {
                if (current->left == NULL) {
                    break;
                }
                parent = current;
                current = current->left;
            }

            int tmp = current->value;       // value swap
            current->value = root->value;
            root->value = tmp;

            if (current->right == NULL) {
                if (root == parent) {
                    parent->right = NULL;
                }
                else {
                    parent->left = NULL;
                }
            }
            else {  // current->left == NULL
                parent->right = current->right;
            }
            current = NULL;
            free(current);
            return root;
        }
        else {  // ?????? ????????? 1?????? ?????? ??????
            if (root->left == NULL) {
                current = root->right;
            }
            else {
                current = root->left;
            }
            root = NULL;
            free(root);
            return current;
        }
    }
    if (key > current->value) {     // ?????? ??????????????? ?????????
        current = current->right;
    }
    else if (key < current->value) {    // ????????? ????????? ??????
        current = current->left;
    }

    while (1) {
        if (current->value == key) {    // ????????? ?????? ??????
            // ?????? ?????? ??? ?????? ????????? 2??? ??? ?????? ?????? -> ????????? ?????????????????? ?????? ??? ??????
            if (current->left && current->right) {
                parent = current;
                current = current->right;   // ??????????????? ????????????
                node = current;
                while (1) {
                    if (current->left == NULL) {
                        break;
                    }
                    node = current;
                    current = current->left;
                }
                int tmp = current->value;       // value swap
                current->value = parent->value;
                parent->value = tmp;

                if (current->right) {   // right ??? ???????????????
                    if (node->value < current->right->value) {
                        parent->right = current->right;
                    }
                    else {
                        node->left = current->right;
                    }
                }
                else {
                    node->left = NULL;
                }
                break;
            }
            // ??????????????? 1??? ?????? ?????? ??????
            else {
                if (current->left == NULL && current->right == NULL) {  // ??????????????? ????????????
                    if (parent->left == current) {
                        parent->left = NULL;
                    }
                    else {
                        parent->right = NULL;
                    }
                    break;
                }
                else if (current->left == NULL && current->right) {    // ?????? ??????????????? ?????? ???????????? ?????? ??????
                    if (parent->value > current->right->value) {
                        parent->left = current->right;
                    }
                    else {
                        current->right->left = current->left;
                        parent->right = current->right;
                    }
                    break;
                }
                else if (current->right == NULL && current->left) {  // ????????? ??????????????? ?????? ????????? ?????? ??????
                    if (parent->value > current->left->value) {
                        parent->left = current->left;
                    }
                    else {
                        parent->right = current->left;
                    }
                    break;
                }
            }

        }
        if (key > current->value) {
            parent = current;
            current = current->right;
        }
        else if (key < current->value) {
            parent = current;
            current = current->left;
        }
    }
    current = NULL;
    free(current);
    return root;
}
int findNode(Tree root, int key) {  // return true, false
    Tree current = root;
    while (1) {
        if (current == NULL) {
            return false;
        }
        if (current->value == key) {    // ????????? 1 ??????
            return true;
        }
        else if (current->value < key) {
            current = current->right;
        }
        else if (current->value > key) {
            current = current->left;
        }
    }
}
void printInorder(Tree root) {
    if (root) {
        printInorder(root->left);
        fprintf(fout, "%d ", root->value);
        printInorder(root->right);
    }
}
void deleteTree(Tree root) {
    if (root) {
        deleteTree(root->left);
        deleteTree(root->right);
        free(root);
    }
}


















