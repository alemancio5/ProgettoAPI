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

// Function to find the station with the minimum km in the cartree
car* cartree_min(car* node) {
    while (node->left != NULL)
        node = node->left;
    return node;
}

// Function to transplant a tree in the cartree
void cartree_transplant(car** root, car* u, car* v) {
    if (u->parent == NULL) {
        // u is the root node
        *root = v;
    } else if (u == u->parent->left) {
        // u is a left child
        u->parent->left = v;
    } else {
        // u is a right child
        u->parent->right = v;
    }

    if (v != NULL) {
        v->parent = u->parent;
    }
}

// Function to delete a node from the cartree
void cartree_delete(car ** root, car* z) {
    car* y = z;
    car* x = NULL;
    char original_color = y->color;

    // If the node has at most one child, splice it out directly
    if (z->left == NULL) {
        x = z->right;
        cartree_fix(root, z->right);
        cartree_transplant(root, z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        cartree_fix(root, z->left);
        cartree_transplant(root, z, z->left);
    } else {
        // If the node has two children, find the successor and splice it out
        y = cartree_min(z->right);
        original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            if (x != NULL)
                x->parent = y;
        } else {
            cartree_transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        cartree_transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    // If we removed a black node, we have to fix the stationtree
    if (original_color == 'B')
        cartree_fix(root, x);
}

// Structure to represent a station in the stationtree
struct stationnode {
    int km;
    char color;
    struct stationnode* left;
    struct stationnode* right;
    struct stationnode* parent;
    car* cartree;
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

// Function to find the station with the minimum km in the stationtree
station* stationtree_min(station* node) {
    while (node->left != NULL)
        node = node->left;
    return node;
}

// Function to transplant a tree in the stationtree
void stationtree_transplant(station** root, station* u, station* v) {
    if (u->parent == NULL) {
        // u is the root node
        *root = v;
    } else if (u == u->parent->left) {
        // u is a left child
        u->parent->left = v;
    } else {
        // u is a right child
        u->parent->right = v;
    }

    if (v != NULL) {
        v->parent = u->parent;
    }
}

// Function to delete a node from the stationtree
void stationtree_delete(station ** root, station* z) {
    station* y = z;
    station* x = NULL;
    char original_color = y->color;

    // If the node has at most one child, splice it out directly
    if (z->left == NULL) {
        x = z->right;
        stationtree_fix(root, z->right);
        stationtree_transplant(root, z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        stationtree_fix(root, z->left);
        stationtree_transplant(root, z, z->left);
    } else {
        // If the node has two children, find the successor and splice it out
        y = stationtree_min(z->right);
        original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            if (x != NULL)
                x->parent = y;
        } else {
            stationtree_transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        stationtree_transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    // If we removed a black node, we have to fix the stationtree
    if (original_color == 'B')
        stationtree_fix(root, x);
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
    station* stationtree = NULL; // The stationtree

    char input[50]; // Input buffer
    station* s = NULL; // Station pointer
    car* c = NULL; // Car pointer
    int s_km = 0; // Station km
    int c_km = 0; // Car km


    while (scanf("%s", input) != EOF) {
        if (input[0] == 'a') { // "aggiungi-"
            if (input[9] == 's') { // "stazione"
                scanf("%s", input);
                s_km = atoi(input);
                s = stationtree_search(stationtree, s_km);
                if (s == NULL) {
                    stationtree_insert(&stationtree, s_km);
                    s = stationtree_search(stationtree, s_km);
                    while (scanf("%s", input) != '\n') {
                        c_km = atoi(input);
                        cartree_insert(&(s->cartree), c_km);
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
                    printf("rottamata\n");
                } else {
                    printf("non rottamata\n");
                }
            } else {
                printf("non rottamata\n");
            }
        } else if (input[0] == 'p') { // "pianifica-percorso"

        }
    }
    return 0;
}
