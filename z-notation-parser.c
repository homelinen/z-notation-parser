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
    void* left;
    int leftType;
    void* right;
    int rightType;
} pair;

// Definitions
void print_set(set*, int);
void print_pair(pair*);
void print_type(void*, int);

void create_pair(pair** p, void* left, int lType, void* right, int rType) {
    *p = (pair*) malloc( sizeof( struct twin ) );
    (*p)->left = left;
    (*p)->leftType = lType;
    (*p)->right = right;
    (*p)->rightType = rType;
}

void print_pair(pair* p) {
    printf("(");

    print_type(p->left, p->leftType);
    printf(", ");
    print_type(p->right, p->rightType);
    printf(")");
}

void print_type(void* val, int type) {
    
        switch (type) {
            case INTEGER: 
                printf("%d", *((int*) val));
                break;
            case SET:
                print_set(val, 1);
                break;
            case PAIR:
                print_pair(val);
                break;
            default:
                printf("%p", val);
                break;
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

        void* val = el->key_value;
        int type = el->type;

        print_type(val, type);

        if (el->next != 0) {
            printf(", ");
        }

        print_set(el->next, 0);
    } else {
        printf("%c", RBR);
    }
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
    printf("\n");

    //---------------------------------
    // x2
    //---------------------------------
    pair* pair_x1 = 0;

    int one = 1;
    create_pair(&pair_x1, &one, INTEGER, x1, SET);

    set* x2 = 0;
    insert_el(x1, SET, &x2);
    insert_el(pair_x1, PAIR, &x2);

    printf("x2 = ");
    print_set(x2, 1);
    printf("\n");

    //---------------------------------
    // x3
    //---------------------------------
    
    pair* x3=0;
    create_pair(&x3, x2, SET, x1, SET);

    printf("x3 = ");
    print_pair(x3);
    printf("\n");

    // free things
    return 0;
}
