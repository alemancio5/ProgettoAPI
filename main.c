#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_INPUT 50

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
    printf("%d ", root->km, root->color);
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

// Function to find a car in the cartree
car* cartree_search(car* root, int km) {
    while (root != NULL) {
        if (km == root->km)
            return root;
        else if (km < root->km)
            root = root->left;
        else
            root = root->right;
    }
    return NULL;
}

// Function to find the station with the maximum km in the cartree
car* cartree_max(car* node) {
    while (node->right != NULL)
        node = node->right;
    return node;
}

// Function to delete a node from the cartree
void cartree_delete(car ** root, car* z) {
    car* y = z;
    car* x = NULL;
    char original_color = y->color;

    if (z->left == NULL) {
        x = z->right;
        if (z->parent == NULL)
            *root = z->right;
        else if (z == z->parent->left)
            z->parent->left = z->right;
        else
            z->parent->right = z->right;
    } else if (z->right == NULL) {
        x = z->left;
        if (z->parent == NULL)
            *root = z->left;
        else if (z == z->parent->left)
            z->parent->left = z->left;
        else
            z->parent->right = z->left;
    } else {
        y = z->right;
        while (y->left != NULL)
            y = y->left;
        original_color = y->color;
        x = y->right;
        if (y->parent == z)
            x->parent = y;
        else {
            if (y->parent == NULL)
                *root = y->right;
            else if (y == y->parent->left)
                y->parent->left = y->right;
            else
                y->parent->right = y->right;
            y->right->parent = y->parent;
            y->right = z->right;
            y->right->parent = y;
        }
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (original_color == 'B')
        cartree_fix(root, x);

    free(z);
}

// Structure to represent a station in the stationtree
struct stationnode {
    int km;
    char color;
    struct stationnode* left;
    struct stationnode* right;
    struct stationnode* parent;
    car* cartree;
    int carmax;
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
    t->cartree = NULL;
    t->carmax = 0;
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
    printf("%d [ ", root->km, root->color);
    cartree_print(root->cartree);
    printf("]\n");
    stationtree_print(root->right);
}

// Function to find a station in the stationtree
station* stationtree_search(station* root, int km) {
    while (root != NULL) {
        if (km == root->km)
            return root;
        else if (km < root->km)
            root = root->left;
        else
            root = root->right;
    }
    return NULL;
}

// Function to delete a node from the stationtree
void stationtree_delete(station ** root, station* z) {
    station* y = z;
    station* x = NULL;
    char original_color = y->color;

    if (z->left == NULL) {
        x = z->right;
        if (z->parent == NULL)
            *root = z->right;
        else if (z == z->parent->left)
            z->parent->left = z->right;
        else
            z->parent->right = z->right;
    } else if (z->right == NULL) {
        x = z->left;
        if (z->parent == NULL)
            *root = z->left;
        else if (z == z->parent->left)
            z->parent->left = z->left;
        else
            z->parent->right = z->left;
    } else {
        y = z->right;
        while (y->left != NULL)
            y = y->left;
        original_color = y->color;
        x = y->right;
        if (y->parent == z)
            x->parent = y;
        else {
            if (y->parent == NULL)
                *root = y->right;
            else if (y == y->parent->left)
                y->parent->left = y->right;
            else
                y->parent->right = y->right;
            y->right->parent = y->parent;
            y->right = z->right;
            y->right->parent = y;
        }
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (original_color == 'B')
        stationtree_fix(root, x);

    cartree_free(z->cartree);
    free(z);
}

// Function to free the memory used by stationtree
void stationtree_free(station* root) {
    if (root == NULL)
        return;

    stationtree_free(root->left);
    stationtree_free(root->right);
    cartree_free(root->cartree);
    free(root);
}

// Function for front paths
void path_front (station* root, int begin, int end) {
    if (begin == end) { // If there is no movement
        printf("%d\n", begin);
        return;
    }

}

// Function for back paths
void path_back (station* root, int start, int end) {

}

int main() {
    station* stationtree = NULL; // The stationtree

    char input[MAX_INPUT]; // Input buffer for strings

    station* s = NULL; // Station pointer
    car* c = NULL; // Car pointer

    int s_km; // Station km
    int c_num; // Car number
    int c_km; // Car km

    int begin; // Start of the path
    int end; // End of the path

    while (scanf("%s", input) != EOF) {
        if (input[0] == 'a') { // "aggiungi-"
            if (input[9] == 's') { // "stazione"
                scanf("%s", input);
                s_km = atoi(input);
                s = stationtree_search(stationtree, s_km);
                if (s == NULL) {
                    stationtree_insert(&stationtree, s_km);
                    s = stationtree_search(stationtree, s_km);
                    scanf("%s", input);
                    c_num = atoi(input);
                    for (int i = 0; i < c_num; i++) {
                        scanf("%s", input);
                        c_km = atoi(input);
                        cartree_insert(&(s->cartree), c_km);
                        if (c_km > s->carmax) {
                            s->carmax = c_km;
                        }
                    }
                    printf("aggiunta\n");
                } else {
                    printf("non aggiunta\n");
                }
            } else if (input[9] == 'a') { // "auto"
                scanf("%s", input);
                s_km = atoi(input);
                scanf("%s", input);
                c_km = atoi(input);
                s = stationtree_search(stationtree, s_km);
                if (s != NULL) {
                    cartree_insert(&(s->cartree), c_km);
                    if (c_km > s->carmax) {
                        s->carmax = c_km;
                    }
                    printf("aggiunta\n");
                } else {
                    printf("non aggiunta\n");
                }
            }
        } else if (input[0] == 'd') { // "demolisci-stazione"
            scanf("%s", input);
            s_km = atoi(input);
            s = stationtree_search(stationtree, s_km);
            if (s != NULL) {
                stationtree_delete(&stationtree, s);
                printf("demolita\n");
            } else {
                printf("non demolita\n");
            }
        } else if (input[0] == 'r') { // "rottama-auto"
            scanf("%s", input);
            s_km = atoi(input);
            s = stationtree_search(stationtree, s_km);
            if (s != NULL) {
                scanf("%s", input);
                c_km = atoi(input);
                c = cartree_search(s->cartree, c_km);
                if (c != NULL) {
                    cartree_delete(&(s->cartree), c);
                    if (c_km == s->carmax) {
                        s->carmax = cartree_max(s->cartree)->km;
                    }
                    printf("rottamata\n");
                } else {
                    printf("non rottamata\n");
                }
            } else {
                printf("non rottamata\n");
            }
        } else if (input[0] == 'p') { // "pianifica-percorso"
            scanf("%s", input);
            begin = atoi(input);
            scanf("%s", input);
            end = atoi(input);
            if (begin <= end) {
                path_front(stationtree, begin, end);
            } else {
                path_back(stationtree, begin, end);
            }
        }
    }

    return 0;
}
