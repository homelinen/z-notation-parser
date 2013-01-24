#include <stdlib.h>
#include <stdio.h>

#define LBR '{'
#define RBR '}'

#define INTEGER 1
#define SET 5
#define PAIR 6
#define TREE 7

// A node in the tree
typedef struct node {
    int key_value;
    struct node *left;
    struct node *right;
} tree;

// The elements of a set
typedef struct element {
    void* key_value;
    int type;
    struct element *next;
} set;

typedef struct twin {
    int left;
    int right;
} pair;

void create_pair(pair** p, int left, int right) {
    *p = (pair*) malloc( sizeof( struct twin ) );
    (*p)->left = left;
    (*p)->right = right;
}

/**
 * Recurse through the Set freeing memory
 * from the end of the set
 **/
void destroy_set(set *el) {
    if (el == 0) {
        destroy_set(el->next);
        free(el);
    }
}

/**
 * Run through the set and print the values
 **/
void print_set(set *el, int front) {
    if (front) {
        printf("%c", LBR);
    } 

    if (el != 0) {
        switch (el->type) {
            case INTEGER: 
                printf("%d", *((int*) el->key_value));
                break;
            case SET:
                print_set(el->key_value, 0);
                break;
            default:
                printf("%p", el->key_value);
        }

        if (el->next != 0) {
            printf(", ");
        }

        print_set(el->next, 0);
    } else {
        printf("%c\n", RBR);
    }
}

/** 
 * Insert the key onto the end of the set
 **/
void insert_el(void * key, int type, set **el) {
    if (*el == 0) {
        *el = (set*) malloc( sizeof ( set ) );
        (*el)->key_value = malloc( sizeof (&key) );
        (*el)->key_value = key;
        (*el)->type = type;
    } else {
        insert_el(key, type, &((*el)->next));
    }
}

/**
 * Recurse through tree, destroying all children up to leaves
 **/
void destroy_tree(tree *leaf) {
    if (leaf != 0) {
        destroy_tree(leaf->right);
        destroy_tree(leaf->left);
        free(leaf);
    }
}

/**
 * Insert the key into the tree
 **/
void insert(int key, tree **leaf) {
    if( *leaf == 0 ) {
        *leaf = (tree*) malloc( sizeof( tree ) );
        (*leaf)->key_value = key;
        // initialize the children to null
        (*leaf)->left = 0;    
        (*leaf)->right = 0;  
    } else if(key < (*leaf)->key_value) {
        insert( key, &(*leaf)->left );
    } else if(key > (*leaf)->key_value) {
        insert( key, &(*leaf)->right );
    }
}

int main(int argc, char** argv) {
    int i;

    //---------------------------------
    // x0
    //---------------------------------
    int x0 = 8;

    printf("x0 = %d\n", x0);

    //---------------------------------
    // x1
    //---------------------------------
    set *x1 = 0;
    for (i = 1; i <= 7; i++) {
        int *temp = malloc( sizeof (int) );
        *temp = i;
        insert_el(temp, INTEGER, &x1);
    }
    // Temporary, should be able to add trees to set
    insert_el(&x0, INTEGER,  &x1);
    printf("x1 = ");
    print_set(x1, 1);

    //---------------------------------
    // x2
    //---------------------------------
    pair* pair_x1 = 0;

    create_pair(&pair_x1, 1, 20);
    printf("Pair ( %d, %d )\n", pair_x1->left, pair_x1->right);

    free(pair_x1);
    return 0;
}