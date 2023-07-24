#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 30

// Structure to represent a station in the cartree
struct carnode {
    int km;
    struct carnode* next;
};
typedef struct carnode car;

// Function to create a new station for the cartree
car* cartree_create(int km) {
    car* t = (car*)malloc(sizeof(car));
    t->km = km;
    t->next = NULL;
    return t;
}

// Function to find the maximum node in the cartree
car* cartree_max(car* node) {
    int max = -1;
    car* saved = NULL;

    while (node != NULL) {
        if (max < node->km) {
            max = node->km;
            saved = node;
        }
        node = node->next;
    }

    return saved;
}

// Function to insert a node into the cartree
void cartree_insert(car** head, int km) {
    car* t = cartree_create(km);
    t->next = *head;
    *head = t;
}

// Function to delete a node from the cartree
void cartree_delete(car** head, int km) {
    car* q = *head;
    car* p = NULL;

    while (q != NULL) {
        if (q->km == km) {
            break;
        }
        p = q;
        q = q->next;
    }

    if (q != NULL) {
        if (p == NULL) {
            *head = q->next;
        } else {
            p->next = q->next;
        }
        free(q);
        printf("rottamata\n");
    } else {
        printf("non rottamata\n");
    }
}

// Function to print cartree in-order
void cartree_print(car* head) {
    car* i = head;

    while (i != NULL) {
        printf("%d ", i->km);
        i = i->next;
    }
}

// Function to free the memory of a cartree
void cartree_free(car* head) {
    car* t = NULL;

    while (head != NULL) {
        t = head;
        head = head->next;
        free(t);
    }
}

// Structure to represent a station in the stationtree
struct stationnode {
    int km;
    car* cartree;
    int carmax;
    struct stationnode* left;
    struct stationnode* right;
    struct stationnode* parent;
};
typedef struct stationnode station;

// Function to create a new station for the stationtree
station* stationtree_create(int km) {
    station* t = (station*)malloc(sizeof(station));
    t->km = km;
    t->cartree = NULL;
    t->carmax = 0;
    t->left = NULL;
    t->right = NULL;
    t->parent = NULL;
    return t;
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
    station* parent = NULL;

    if (node->right != NULL) {
        return stationtree_min(node->right);
    }
    parent = node->parent;
    while (parent != NULL && node == parent->right) {
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

// Function to find the predecessor of a node in the stationtree
station* stationtree_pre(station* node) {
    station* parent = NULL;

    if (node->left != NULL) {
        return stationtree_max(node->left);
    }
    parent = node->parent;
    while (parent != NULL && node == parent->left) {
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

// Function to insert a node into the stationtree
void stationtree_insert(station** root, int km) {
    station* z = stationtree_create(km);
    station* y = NULL;
    station* x = *root;

    while (x != NULL) {
        y = x;
        if (z->km < x->km) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == NULL) {
        *root = z;
    } else if (z->km < y->km) {
        y->left = z;
    } else {
        y->right = z;
    }
}

// Function to delete a node from the stationtree
void stationtree_delete(station** root, station* z) {
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
        cartree_free(z->cartree);
        z->cartree = y->cartree;
        z->carmax = y->carmax;
    } else {
        cartree_free(y->cartree);
    }
    free(y);
}

// Function to search for a node in the stationtree
station* stationtree_search(station* root, int km) {
    while (root != NULL && root->km != km) {
        if (km < root->km) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return root;
}

// Function to print stationtree in-order
void stationtree_print(station* root) {
    if (root != NULL) {
        stationtree_print(root->left);
        printf("%d [ ", root->km);
        cartree_print(root->cartree);
        printf("] %d\n", root->carmax);
        stationtree_print(root->right);
    }
}

// Function to free the memory of a stationtree
void stationtree_free(station* root) {
    if (root != NULL) {
        stationtree_free(root->left);
        stationtree_free(root->right);
        cartree_free(root->cartree);
        free(root);
    }
}

// structure to represent a pocket
struct pocketnode {
    int km;
    struct pocketnode* next;
};
typedef struct pocketnode pocket;

// Function to create a new pocket
pocket* pocket_create(int km) {
    pocket* t = (pocket*)malloc(sizeof(pocket));
    t->km = km;
    t->next = NULL;
    return t;
}

// Function to insert a new poket
void pocket_insert(pocket** root, int km) {
    pocket* t = pocket_create(km);
    t->next = *root;
    *root = t;
}

// Function to print a poket
void pocket_print(pocket* root) {
    pocket* i = root;
    while (i != NULL) {
        printf("%d ", i->km);
        i = i->next;
    }
}

// Function to free the memory of a poket
void pocket_free(pocket* root) {
    if (root != NULL) {
        pocket_free(root->next);
        free(root);
    }
}

// Function for forward paths
void front_path(station* root, int begin, int end) {
    // If there is no movement
    if (begin == end) {
        printf("%d\n", begin);
        return;
    }

    // If the trip can be direct
    station* begin_station = stationtree_search(root, begin);
    if (end - begin <= begin_station->carmax) {
        printf("%d %d\n", begin, end);
        return;
    }

    // If are needed intermediate stations
    station* chosen = stationtree_search(root, end);
    station* step = stationtree_pre(chosen);
    station* saved = NULL;
    pocket* pocket = NULL;
    while (begin + begin_station->carmax < chosen->km) {
        while (step->km > begin) {
            if (step->km + step->carmax >= chosen->km) {
                saved = step;
            }
            step = stationtree_pre(step);
        }
        if (saved == NULL) {
            printf("nessun percorso\n");
            return;
        } else {
            pocket_insert(&pocket, saved->km);
            chosen = saved;
            step = stationtree_pre(saved);
            saved = NULL;
        }
    }

    // Print the path
    printf("%d ", begin);
    pocket_print(pocket);
    printf("%d\n", end);

    pocket_free(pocket);
}

// Function for back paths
void back_path(station* root, int begin, int end) {
    // If the trip can be direct
    station* start = stationtree_search(root, begin);
    if (begin - end <= start->carmax) {
        printf("%d %d\n", begin, end);
        return;
    }

    // If are needed intermediate stations
    station* base = start;
    station* step = stationtree_pre(start);
    station* test = NULL;
    station* view = NULL;
    station* max_view = NULL;
    station* selected = NULL;
    station* saved[100];
    int length = 0;

    saved[0] = start;
    length++;

    while (base->km - base->carmax > end) {
        while (step->km > end && base->km - base->carmax <= step->km) {
            test = step;
            while (test->km > end && step->km - step->carmax <= stationtree_pre(test)->km) {
                test = stationtree_pre(test);
            }
            if (test->km > end) {
                view = stationtree_suc(test);
            } else {
                view = test;
            }
            if (max_view == NULL || max_view->km >= view->km) {
                max_view = view;
                selected = step;
            }
            step = stationtree_pre(step);
        }
        if (selected != NULL) {
            base = selected;
            step = stationtree_pre(selected);
            saved[length] = selected;
            length++;
        } else { // If there are no steps to reach the end
            printf("nessun percorso\n");
            return;
        }
        max_view = NULL;
        selected = NULL;
    }

    saved[length] = stationtree_search(root, end);
    length++;

    // Correction of the path from end
    for (int i = length - 2; i > 0; i--) {
        selected = NULL;
        step = stationtree_pre(saved[i]);
        while (saved[i-1]->km - saved[i-1]->carmax <= step->km) {
            if (step->km - step->carmax <= saved[i+1]->km) {
                selected = step;
            }
            step = stationtree_pre(step);
        }
        if (selected != NULL) {
            saved[i] = selected;
        }
    }

    // Print the path
    for (int i = 0; i < length - 1; i++) {
        printf("%d ", saved[i]->km);
    }
    printf("%d\n", end);
}

int main() {
    station* stationtree = NULL; // The stationtree

    char input[MAX_INPUT]; // Input buffer for strings

    station* s = NULL; // Station pointer
    car* c = NULL; // Car pointer

    int s_km = 0; // Station km
    int c_num = 0; // Car number
    int c_km = 0; // Car km

    int begin = 0; // Start of the path
    int end = 0; // End of the path

    while (scanf("%s", input) != EOF) {
        if (input[0] == 'a') {
            if (input[9] == 's') { // "aggiungi-stazione"
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
            } else if (input[9] == 'a') { // "aggiungi-auto"
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
                cartree_delete(&(s->cartree), c_km);
                if (c_km == s->carmax) {
                    c = cartree_max(s->cartree);
                    if (c != NULL) {
                        s->carmax = c->km;
                    } else {
                        s->carmax = 0;
                    }
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
                front_path(stationtree, begin, end);
            } else {
                back_path(stationtree, begin, end);
            }
        }
    }
    stationtree_free(stationtree);
    return 0;
}
