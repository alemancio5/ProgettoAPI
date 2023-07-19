#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1000
#define MAX_INTER 1000

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

// Function to find the minimum node in the cartree
car* cartree_min(car* node) {
    if (node == NULL)
        return NULL;
    while (node->left != NULL)
        node = node->left;
    return node;
}

// Function to find the maximum node in the cartree
car* cartree_max(car* node) {
    if (node == NULL)
        return NULL;
    while (node->right != NULL)
        node = node->right;
    return node;
}

// Function to find the successor of a node in the cartree
car* cartree_suc(car* node) {
    if (node->right != NULL) {
        return cartree_min(node->right);
    } else {
        car* parent = node->parent;
        while (parent != NULL && node == parent->right) {
            node = parent;
            parent = node->parent;
        }
        return parent;
    }
}

// Function to find the predecessor of a node in the cartree
car* cartree_pre(car* node) {
    if (node->left != NULL) {
        return cartree_max(node->left);
    } else {
        car* parent = node->parent;
        while (parent != NULL && node == parent->left) {
            node = parent;
            parent = node->parent;
        }
        return parent;
    }
}

// Function to fix the properties of the cartree, after insertion
void cartree_insert_fix(car** root, car* z) {
    car* x = NULL;
    car* y = NULL;

    if (z == *root) {
        (*root)->color = 'B';
    } else {
        x = z->parent;
        if (x->color == 'R') {
            if (x == x->parent->left) {
                y = x->parent->right;
                if (y != NULL && y->color == 'R') {
                    x->color = 'B';
                    y->color = 'B';
                    x->parent->color = 'R';
                    cartree_insert_fix(root, x->parent);
                } else if (z == x->right) {
                    z = x;
                    cartree_left(root, z);
                    x = z->parent;
                }
                x->color = 'B';
                x->parent->color = 'R';
                cartree_right(root, x->parent);
            } else {
                y = x->parent->left;
                if (y != NULL && y->color == 'R') {
                    x->color = 'B';
                    y->color = 'B';
                    x->parent->color = 'R';
                    cartree_insert_fix(root, x->parent);
                } else if (z == x->left) {
                    z = x;
                    cartree_right(root, z);
                    x = z->parent;
                }
                x->color = 'B';
                x->parent->color = 'R';
                cartree_left(root, x->parent);
            }
        }
    }
}

// Function to insert a new station into the cartree
void cartree_insert(car** root, int km) {
    car* t = cartree_create(km);
    car* y = NULL;
    car* x = *root;

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

    cartree_insert_fix(root, t);
}

// Function to fix up the cartree after deletion
void cartree_delete_fix(car** root, car* x) {
    car* w = NULL;

    if (x->color == 'R' || x->parent == NULL) {
        x->color = 'B';
    } else if (x == x->parent->left) {
        w = x->parent->right;
        if (w->color == 'R') {
            w->color = 'B';
            x->parent->color = 'R';
            cartree_left(root, x->parent);
            w = x->parent->right;
        }
        if (w->left->color == 'B' && w->right->color == 'B') {
            w->color = 'R';
            cartree_delete_fix(root, x->parent);
        } else if (w->right->color == 'B') {
            w->left->color = 'B';
            w->color = 'R';
            cartree_right(root, w);
            w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = 'B';
        w->right->color = 'B';
        cartree_left(root, x->parent);
    } else {
        w = x->parent->left;
        if (w->color == 'R') {
            w->color = 'B';
            x->parent->color = 'R';
            cartree_right(root, x->parent);
            w = x->parent->left;
        }
        if (w->right->color == 'B' && w->left->color == 'B') {
            w->color = 'R';
            cartree_delete_fix(root, x->parent);
        } else if (w->left->color == 'B') {
            w->right->color = 'B';
            w->color = 'R';
            cartree_left(root, w);
            w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = 'B';
        w->left->color = 'B';
        cartree_right(root, x->parent);
    }
}

// Function to delete a node from the cartree
void cartree_delete(car** root, car* z) {
    car* y = NULL;
    car* x = NULL;

    if (z->left == NULL || z->right == NULL) {
        y = z;
    } else {
        y = cartree_suc(z);
    }

    if (y->left != NULL) {
        x = y->left;
    } else {
        x = y->right;
    }

    if (x != NULL) {
        x->parent = y->parent;
    }

    if (y->parent == NULL) {
        *root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    if (y != z) {
        z->km = y->km;
    }

    if (y->color == 'B' && x != NULL) {
        cartree_delete_fix(root, x);
    }

    free(y);
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

// Function to traverse and print the cartree in-order
void cartree_print(car* root) {
    if (root == NULL)
        return;

    cartree_print(root->left);
    printf("%d ", root->km);
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

// Function to find the minimum node in the stationtree
station* stationtree_min(station* node) {
    if (node == NULL)
        return NULL;
    while (node->left != NULL)
        node = node->left;
    return node;
}

// Function to find the maximum node in the stationtree
station* stationtree_max(station* node) {
    if (node == NULL)
        return NULL;
    while (node->right != NULL)
        node = node->right;
    return node;
}

// Function to find the successor of a node in the stationtree
station* stationtree_suc(station* node) {
    if (node->right != NULL) {
        return stationtree_min(node->right);
    } else {
        station* parent = node->parent;
        while (parent != NULL && node == parent->right) {
            node = parent;
            parent = node->parent;
        }
        return parent;
    }
}

// Function to find the predecessor of a node in the stationtree
station* stationtree_pre(station* node) {
    if (node->left != NULL) {
        return stationtree_max(node->left);
    } else {
        station* parent = node->parent;
        while (parent != NULL && node == parent->left) {
            node = parent;
            parent = node->parent;
        }
        return parent;
    }
}

// Function to fix the properties of the stationtree, after insertion
void stationtree_insert_fix(station** root, station* z) {
    station* x = NULL;
    station* y = NULL;

    if (z == *root) {
        (*root)->color = 'B';
    } else {
        x = z->parent;
        if (x->color == 'R') {
            if (x == x->parent->left) {
                y = x->parent->right;
                if (y != NULL && y->color == 'R') {
                    x->color = 'B';
                    y->color = 'B';
                    x->parent->color = 'R';
                    stationtree_insert_fix(root, x->parent);
                } else if (z == x->right) {
                    z = x;
                    stationtree_left(root, z);
                    x = z->parent;
                }
                x->color = 'B';
                x->parent->color = 'R';
                stationtree_right(root, x->parent);
            } else {
                y = x->parent->left;
                if (y != NULL && y->color == 'R') {
                    x->color = 'B';
                    y->color = 'B';
                    x->parent->color = 'R';
                    stationtree_insert_fix(root, x->parent);
                } else if (z == x->left) {
                    z = x;
                    stationtree_right(root, z);
                    x = z->parent;
                }
                x->color = 'B';
                x->parent->color = 'R';
                stationtree_left(root, x->parent);
            }
        }
    }
}

// Function to insert a new station into the stationtree
void stationtree_insert(station** root, int km) {
    station* t = stationtree_create(km);
    station* y = NULL;
    station* x = *root;

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

    stationtree_insert_fix(root, t);
}

// Function to fix up the stationtree after deletion
void stationtree_delete_fix(station** root, station* x) {
    station* w = NULL;

    if (x->color == 'R' || x->parent == NULL) {
        x->color = 'B';
    } else if (x == x->parent->left) {
        w = x->parent->right;
        if (w->color == 'R') {
            w->color = 'B';
            x->parent->color = 'R';
            stationtree_left(root, x->parent);
            w = x->parent->right;
        }
        if (w->left->color == 'B' && w->right->color == 'B') {
            w->color = 'R';
            stationtree_delete_fix(root, x->parent);
        } else if (w->right->color == 'B') {
            w->left->color = 'B';
            w->color = 'R';
            stationtree_right(root, w);
            w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = 'B';
        w->right->color = 'B';
        stationtree_left(root, x->parent);
    } else {
        w = x->parent->left;
        if (w->color == 'R') {
            w->color = 'B';
            x->parent->color = 'R';
            stationtree_right(root, x->parent);
            w = x->parent->left;
        }
        if (w->right->color == 'B' && w->left->color == 'B') {
            w->color = 'R';
            stationtree_delete_fix(root, x->parent);
        } else if (w->left->color == 'B') {
            w->right->color = 'B';
            w->color = 'R';
            stationtree_left(root, w);
            w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = 'B';
        w->left->color = 'B';
        stationtree_right(root, x->parent);
    }
}

// Function to delete a node from the stationtree
void stationtree_delete(station ** root, station* z) {
    station* y = NULL;
    station* x = NULL;

    if (z->left == NULL || z->right == NULL) {
        y = z;
    } else {
        y = stationtree_suc(z);
    }

    if (y->left != NULL) {
        x = y->left;
    } else {
        x = y->right;
    }

    if (x != NULL) {
        x->parent = y->parent;
    }

    if (y->parent == NULL) {
        *root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    if (y != z) {
        z->km = y->km;
    }

    if (y->color == 'B' && x != NULL) {
        stationtree_delete_fix(root, x);
    }

    cartree_free(y->cartree);
    free(y);
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

// Function to traverse and print the stationtree in-order
void stationtree_print(station* root) {
    if (root == NULL)
        return;

    stationtree_print(root->left);
    printf("%d [ ", root->km);
    cartree_print(root->cartree);
    printf("]\n");
    stationtree_print(root->right);
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

// Function to calculate the number of stations seen from a step
int forward_view(station* root, int end) {
    int result = 0;
    station* i = root;
    while (i->km < end) {
        if (i->km < end && (stationtree_suc(i)->km - root->km) <= root->carmax) {
            result++;
            i = stationtree_suc(i);
        } else {
            break;
        }
    }
    return result;
}

// Function for forward paths
void forward_path(station* root, int begin, int end) {
    if (begin == end) { // If there is no movement
        printf("%d\n", begin);
        return;
    }

    station* start = stationtree_search(root, begin); // Find the starting station
    if (end - begin <= start->carmax) { // If the trip can be direct
        printf("%d %d\n", begin, end);
        return;
    }

    // If are needed intermediate stations
    station* step = stationtree_suc(start); // Set the first step station
    int count_step; // Variable for the number of steps
    station* base = start; // Set the base station
    int view; // Variable for the view that a step has
    int max_view; // Variable for the maximum view that the steps can have
    station* best_view = NULL; // Variable for the best step from the current base
    int inter[MAX_INTER]; // Array for the steps
    int num_inter = 0; // Variable for the number of steps

    while (step->km < end) { // Algorithm to catch intermediate stations
        count_step = 0;
        max_view = 0;
        best_view = NULL;
        while (step->km < end && (step->km - base->km) <= base->carmax) {
            view = forward_view(step, end) + count_step;
            if (max_view < view) {
                max_view = view;
                best_view = step;
            }
            step = stationtree_suc(step);
            count_step++;
        }
        if (best_view != NULL) { // If the best step is found
            base = best_view;
            step = stationtree_suc(best_view);
            inter[num_inter] = best_view->km;
            num_inter++;
        } else { // If there are no steps to reach the end
            printf("nessun percorso\n");
            return;
        }
        if (end - base->km <= base->carmax) { // If the trip can be direct now
            break;
        }
    }

    if (num_inter != 0) {
        printf("%d ", begin); // Print the starting station
        for (int i = 0; i < num_inter; i++) { // Print the intermediate stations
            printf("%d ", inter[i]);
        }
        printf("%d\n", end); // Print the ending station
    } else {
        printf("nessun percorso\n");
    }
}

// Function to calculate the number of stations seen from a step
int back_view(station* root, int end) {
    int result = 0;
    station* i = root;
    while (i->km > end) {
        if (i->km > end && (root->km - stationtree_pre(i)->km) <= root->carmax) {
            result++;
            i = stationtree_pre(i);
        } else {
            break;
        }
    }
    return result;
}

// Function for back paths
void back_path(station* root, int begin, int end) {
    station* start = stationtree_search(root, begin); // Find the starting station
    if (begin - end <= start->carmax) { // If the trip can be direct
        printf("%d %d\n", begin, end);
        return;
    }

    // If are needed intermediate stations
    station* step = stationtree_pre(start); // Set the first step station
    int count_step; // Variable for the number of steps
    station* base = start; // Set the base station
    int view; // Variable for the view that a step has
    int max_view; // Variable for the maximum view that the steps can have
    station* best_view = NULL; // Variable for the best step from the current base
    int inter[MAX_INTER]; // Array for the steps
    int num_inter = 0; // Variable for the number of steps

    while (step->km > end) { // Algorithm to catch intermediate stations
        count_step = 0;
        max_view = 0;
        best_view = NULL;
        while (step->km > end && (base->km - step->km) <= base->carmax) {
            view = back_view(step, end) + count_step;
            if (max_view <= view) {
                max_view = view;
                best_view = step;
            }
            step = stationtree_pre(step);
            count_step++;
        }
        if (best_view != NULL) { // If the best step is found
            base = best_view;
            step = stationtree_pre(best_view);
            inter[num_inter] = best_view->km;
            num_inter++;
        } else { // If there are no steps to reach the end
            printf("nessun percorso\n");
            return;
        }
        if (base->km - end <= base->carmax) { // If the trip can be direct now
            break;
        }
    }

    if (num_inter != 0) {
        printf("%d ", begin); // Print the starting station
        for (int i = 0; i < num_inter; i++) { // Print the intermediate stations
            printf("%d ", inter[i]);
        }
        printf("%d\n", end); // Print the ending station
    } else {
        printf("nessun percorso\n");
    }
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
                if (scanf("%s", input) == EOF) {
                    return 0;
                }
                s_km = atoi(input);
                s = stationtree_search(stationtree, s_km);
                if (s == NULL) {
                    stationtree_insert(&stationtree, s_km);
                    s = stationtree_search(stationtree, s_km);
                    if (scanf("%s", input) == EOF) {
                        return 0;
                    }
                    c_num = atoi(input);
                    for (int i = 0; i < c_num; i++) {
                        if (scanf("%s", input) == EOF) {
                            return 0;
                        }
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
                if (scanf("%s", input) == EOF) {
                    return 0;
                }
                s_km = atoi(input);
                if (scanf("%s", input) == EOF) {
                    return 0;
                }
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
            if (scanf("%s", input) == EOF) {
                return 0;
            }
            s_km = atoi(input);
            s = stationtree_search(stationtree, s_km);
            if (s != NULL) {
                stationtree_delete(&stationtree, s);
                printf("demolita\n");
            } else {
                printf("non demolita\n");
            }
        } else if (input[0] == 'r') { // "rottama-auto"
            if (scanf("%s", input) == EOF) {
                return 0;
            }
            s_km = atoi(input);
            s = stationtree_search(stationtree, s_km);
            if (s != NULL) {
                if (scanf("%s", input) == EOF) {
                    return 0;
                }
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
            if (scanf("%s", input) == EOF) {
                return 0;
            }
            begin = atoi(input);
            if (scanf("%s", input) == EOF) {
                return 0;
            }
            end = atoi(input);
            if (begin <= end) {
                forward_path(stationtree, begin, end);
            } else {
                back_path(stationtree, begin, end);
            }
        }
    }
    return 0;
}
