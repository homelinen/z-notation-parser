#include <stdlib.h>
#include <stdio.h>

const char LBR = '{';
const char RBR = '}';

// A node in the tree
struct node {
    int key_value;
    struct node *left;
    struct node *right;
};

// The elements of a set
struct element {
    int key_value;
    struct element *next;
};

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
void destroy_set(struct element *el) {
    if (el == 0) {
        destroy_set(el->next);
        free(el);
    }
}

/**
 * Run through the set and print the values
 **/
void print_set(struct element *el, int front) {
    if (front) {
        printf("%c", LBR);
    } 

    if (el != 0) {
        printf("%d", el->key_value);

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
void insert_el(int key, struct element **el) {
    if (*el == 0) {
        *el = (struct element*) malloc( sizeof ( struct element ) );
        (*el)->key_value = key;
    } else {
        insert_el(key, &((*el)->next));
    }
}

/**
 * Recurse through tree, destroying all children up to leaves
 **/
void destroy_tree(struct node *leaf) {
    if (leaf != 0) {
        destroy_tree(leaf->right);
        destroy_tree(leaf->left);
        free(leaf);
    }
}

/**
 * Insert the key into the tree
 **/
void insert(int key, struct node **leaf) {
    if( *leaf == 0 ) {
        *leaf = (struct node*) malloc( sizeof( struct node ) );
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
    struct node *x0 = 0;
    insert(8, &x0 );

    printf("x0 = %d\n", x0->key_value);

    //---------------------------------
    // x1
    //---------------------------------
    struct element *x1 = 0;
    for (i = 1; i <= 7; i++) {
      insert_el(i, &x1);
    }
    // Temporary, should be able to add trees to set
    insert_el(x0->key_value, &x1);
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
