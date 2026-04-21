#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>

// Folosim structura de baza
struct node {
    int data;
    struct node *left;
    struct node *right;
};

// Functie pentru crearea unui nod nou
struct node* createNode(int data) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    if (newNode) {
        newNode->data = data;
        newNode->left = NULL;
        newNode->right = NULL;
    }
    return newNode;
}

// --- Tehnici de parcurgere

void inOrder(struct node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->data);
        inOrder(root->right);
    }
}

void preOrder(struct node* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void postOrder(struct node* root) {
    if (root != NULL) {
        postOrder(root->left);
        postOrder(root->right);
        printf("%d ", root->data);
    }
}

// --- Ex 1: Echilibrat
int height(struct node* n) {
    if (n == NULL) return 0;
    int lh = height(n->left);
    int rh = height(n->right);
    return 1 + (lh > rh ? lh : rh);
}

bool isBalanced(struct node* root) {
    if (root == NULL) return true;
    int lh = height(root->left);
    int rh = height(root->right);
    return (abs(lh - rh) <= 1) && isBalanced(root->left) && isBalanced(root->right);
}

// --- Ex 2: Verificare BST
bool isBSTUtil(struct node* n, int min, int max) {
    if (n == NULL) return true;
    if (n->data <= min || n->data >= max) return false;
    return isBSTUtil(n->left, min, n->data) && isBSTUtil(n->right, n->data, max);
}

// --- Ex 3: Strămoș comun (LCA)
struct node* findLCA(struct node* root, int n1, int n2) {
    if (root == NULL) return NULL;
    if (root->data == n1 || root->data == n2) return root;
    struct node* L = findLCA(root->left, n1, n2);
    struct node* R = findLCA(root->right, n1, n2);
    if (L && R) return root;
    return L ? L : R;
}

// Ex 4 Serializar (Arbore -> String -> Arbore)
void serialize(struct node* root, char* str) {
    if (root == NULL) {
        strcat(str, "#,");
        return;
    }
    char buf[20];
    sprintf(buf, "%d,", root->data);
    strcat(str, buf);
    serialize(root->left, str);
    serialize(root->right, str);
}

struct node* deserializeUtil(char** str) {
    if (**str == '#' || **str == '\0') {
        if (**str == '#') *str += 2;
        return NULL;
    }
    char* end;
    int val = strtol(*str, &end, 10);
    *str = end + 1;
    struct node* n = createNode(val);
    n->left = deserializeUtil(str);
    n->right = deserializeUtil(str);
    return n;
}

// --- Ex 5: Verișori
int getLevel(struct node* root, int val, int level) {
    if (root == NULL) return 0;
    if (root->data == val) return level;
    int l = getLevel(root->left, val, level + 1);
    return l ? l : getLevel(root->right, val, level + 1);
}

bool isSibling(struct node* root, int a, int b) {
    if (root == NULL) return false;
    bool direct = false;
    if (root->left && root->right) {
        direct = (root->left->data == a && root->right->data == b) ||
                 (root->left->data == b && root->right->data == a);
    }
    return direct || isSibling(root->left, a, b) || isSibling(root->right, a, b);
}
//ex 6
//Un Arbore Avl este un arbore perfect, in care nici un nod nu are mai mult de 2 copii(in cazul meu frunze). Un B-tree , se refera la un arbore in care un nod poate avea zeci sau sute de copii(mic si gras).

// --- Extra: ZigZag
int globalMaxZZ = 0;
void findZZ(struct node* n, bool goLeft, int len) {
    if (!n) return;
    if (len > globalMaxZZ) globalMaxZZ = len;
    if (goLeft) {
        findZZ(n->left, false, len + 1);
        findZZ(n->right, true, 1);
    } else {
        findZZ(n->right, true, len + 1);
        findZZ(n->left, false, 1);
    }
}

int main() {
    // Test Ex 1 & 2
    struct node* r = createNode(10);
    r->left = createNode(5);
    r->right = createNode(15);

    printf("--- Test Laborator 6 ---\n");
    printf("In-Order: "); inOrder(r); printf("\n");
    printf("Este echilibrat? %s\n", isBalanced(r) ? "Da" : "Nu");
    printf("Este BST? %s\n", isBSTUtil(r, INT_MIN, INT_MAX) ? "Da" : "Nu");

    // Test Ex 4
    char buffer[100] = "";
    serialize(r, buffer);
    printf("Serializat: %s\n", buffer);

    // Test Extra
    findZZ(r->left, false, 1);
    findZZ(r->right, true, 1);
    printf("Lungime maxima ZigZag: %d\n", globalMaxZZ);

    return 0;
}
