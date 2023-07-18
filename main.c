#include <stdio.h>
#include <stdlib.h>

// Structure to represent a station in the cartree
struct carnode {
    int km;
    char color;
    struct carnode* left;
    struct carnode* right;
    struct carnode* parent;
};
typedef struct carnode car;

// Function to create a new station for the cartree
car* cartree_create(int km) {
    car* t = (car*)malloc(sizeof(car));
    t->km = km;
    t->color = 'R'; // New stations are always inserted as red
    t->left = NULL;
    t->right = NULL;
    t->parent = NULL;
    return t;
}

// Function to perform left rotation in the cartree
void cartree_left(car** root, car* x) {
    car* y = x->right;
    x->right = y->left;

    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

// Function to perform right rotation in the cartree
void cartree_right(car** root, car* y) {
    car* x = y->left;
    y->left = x->right;

    if (x->right != NULL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

// Function to fix the properties of the cartree, after insertion
void cartree_fix(car** root, car* z) {
    while (z != *root && z->parent->color == 'R') {
        car* grandparent = z->parent->parent;

        if (z->parent == grandparent->left) {
            car* uncle = grandparent->right;

            // Case 1: Uncle is red
            if (uncle != NULL && uncle->color == 'R') {
                z->parent->color = 'B';
                uncle->color = 'B';
                grandparent->color = 'R';
                z = grandparent;
            } else {
                // Case 2: Uncle is black, and z is a right child
                if (z == z->parent->right) {
                    z = z->parent;
                    cartree_left(root, z);
                }

                // Case 3: Uncle is black, and z is a left child
                z->parent->color = 'B';
                grandparent->color = 'R';
                cartree_right(root, grandparent);
            }
        } else {
            car* uncle = grandparent->left;

            // Case 1: Uncle is red
            if (uncle != NULL && uncle->color == 'R') {
                z->parent->color = 'B';
                uncle->color = 'B';
                grandparent->color = 'R';
                z = grandparent;
            } else {
                // Case 2: Uncle is black, and z is a left child
                if (z == z->parent->left) {
                    z = z->parent;
                    cartree_right(root, z);
                }

                // Case 3: Uncle is black, and z is a right child
                z->parent->color = 'B';
                grandparent->color = 'R';
                cartree_left(root, grandparent);
            }
        }
    }

    (*root)->color = 'B'; // Ensure the root is always black
}

// Function to insert a new station into the cartree
void cartree_insert(car** root, int km) {
    car* t = cartree_create(km);
    car* y = NULL;
    car* x = *root;

    // Find the position to insert the new car
    while (x != NULL) {
        y = x;
        if (km < x->km)
            x = x->left;
        else
            x = x->right;
    }

    t->parent = y;

    if (y == NULL)
        *root = t;
    else if (km < y->km)
        y->left = t;
    else
        y->right = t;

    // Fix the properties of the cartree, if they are violated
    cartree_fix(root, t);
}

// Function to traverse and print the cartree in-order
void cartree_print(car* root) {
    if (root == NULL)
        return;

    cartree_print(root->left);
    printf("%d (%c) ", root->km, root->color);
    cartree_print(root->right);
}

// Function to free the memory used by cartree
void cartree_free(car* root) {
    if (root == NULL)
        return;

    cartree_free(root->left);
    cartree_free(root->right);
    free(root);
}







// Structure to represent a station in the stationtree
struct stationnode {
    int km;
    char color;
    struct stationnode* left;
    struct stationnode* right;
    struct stationnode* parent;
};
typedef struct stationnode station;

// Function to create a new station for the stationtree
station* stationtree_create(int km) {
    station* t = (station*)malloc(sizeof(station));
    t->km = km;
    t->color = 'R'; // New stations are always inserted as red
    t->left = NULL;
    t->right = NULL;
    t->parent = NULL;
    return t;
}

// Function to perform left rotation in the stationtree
void stationtree_left(station** root, station* x) {
    station* y = x->right;
    x->right = y->left;

    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

// Function to perform right rotation in the stationtree
void stationtree_right(station** root, station* y) {
    station* x = y->left;
    y->left = x->right;

    if (x->right != NULL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

// Function to fix the properties of the stationtree, after insertion
void stationtree_fix(station** root, station* z) {
    while (z != *root && z->parent->color == 'R') {
        station* grandparent = z->parent->parent;

        if (z->parent == grandparent->left) {
            station* uncle = grandparent->right;

            // Case 1: Uncle is red
            if (uncle != NULL && uncle->color == 'R') {
                z->parent->color = 'B';
                uncle->color = 'B';
                grandparent->color = 'R';
                z = grandparent;
            } else {
                // Case 2: Uncle is black, and z is a right child
                if (z == z->parent->right) {
                    z = z->parent;
                    stationtree_left(root, z);
                }

                // Case 3: Uncle is black, and z is a left child
                z->parent->color = 'B';
                grandparent->color = 'R';
                stationtree_right(root, grandparent);
            }
        } else {
            station* uncle = grandparent->left;

            // Case 1: Uncle is red
            if (uncle != NULL && uncle->color == 'R') {
                z->parent->color = 'B';
                uncle->color = 'B';
                grandparent->color = 'R';
                z = grandparent;
            } else {
                // Case 2: Uncle is black, and z is a left child
                if (z == z->parent->left) {
                    z = z->parent;
                    stationtree_right(root, z);
                }

                // Case 3: Uncle is black, and z is a right child
                z->parent->color = 'B';
                grandparent->color = 'R';
                stationtree_left(root, grandparent);
            }
        }
    }

    (*root)->color = 'B'; // Ensure the root is always black
}

// Function to insert a new station into the stationtree
void stationtree_insert(station** root, int km) {
    station* t = stationtree_create(km);
    station* y = NULL;
    station* x = *root;

    // Find the position to insert the new station
    while (x != NULL) {
        y = x;
        if (km < x->km)
            x = x->left;
        else
            x = x->right;
    }

    t->parent = y;

    if (y == NULL)
        *root = t;
    else if (km < y->km)
        y->left = t;
    else
        y->right = t;

    // Fix the properties of the stationtree, if they are violated
    stationtree_fix(root, t);
}

// Function to traverse and print the stationtree in-order
void stationtree_print(station* root) {
    if (root == NULL)
        return;

    stationtree_print(root->left);
    printf("%d (%c) ", root->km, root->color);
    stationtree_print(root->right);
}

// Function to free the memory used by stationtree
void stationtree_free(station* root) {
    if (root == NULL)
        return;

    stationtree_free(root->left);
    stationtree_free(root->right);
    free(root);
}

int main() {
    station* root = NULL;

    stationtree_insert(&root, 10);
    stationtree_insert(&root, 20);
    stationtree_insert(&root, 30);
    stationtree_insert(&root, 15);
    stationtree_insert(&root, 25);


    printf("Print: ");
    stationtree_print(root);
    printf("\n");

    stationtree_free(root);

    return 0;
}
