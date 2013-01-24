#include <stdlib.h>
#include <stdio.h>

#define LBR '{'
#define RBR '}'

#define INTEGER 1
#define SET 5
#define PAIR 6
#define TREE 7

/*
 * Type definitions
 */
typedef struct _Set Set;
typedef struct _Pair Pair;
typedef union _Container Container;
typedef struct _Value Value;

union _Container {
    int i;
    Set* s;
    Pair* p;
};

struct _Value {
    Container val;
    int type;
};

// The elements of a Set
struct _Set {
    Value val;
    Set *next;
};

struct _Pair {
    Value left;
    Value right;
};

// Definitions
void print_set(Set*, int);
void print_pair(Pair*);
void print_type(Value*);

void create_pair(Pair** p, Value left, Value right) {
    *p = (Pair*) malloc( sizeof( Pair ) );
    (*p)->left = left;
    (*p)->right = right;
}

void print_pair(Pair* p) {
    printf("(");

    print_type(&p->left);
    printf(", ");
    print_type(&p->right);
    printf(")");
}

void print_type(Value* val) {
    
    switch (val->type) {
        case INTEGER: 
            printf("%d", val->val.i);
            break;
        case SET:
            print_set(val->val.s, 1);
            break;
        case PAIR:
            print_pair(val->val.p);
            break;
        default:
            printf("%p", val);
            break;
    }
}

/**
 * Run through the Set and print the values
 **/
void print_set(Set *el, int front) {
    if (front) {
        printf("%c", LBR);
    } 

    if (el != 0) {

        print_type(&el->val);

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
 * from the end of the Set
 **/
void destroy_set(Set *el) {
    if (el == 0) {
        destroy_set(el->next);
        free(el);
    }
}

/** 
 * Insert the key onto the end of the Set
 **/
void insert_el(Value key, Set **el) {
    if (*el == 0) {
        *el = (Set*) malloc( sizeof ( Set ) );
        (*el)->val = *((Value*) malloc( sizeof ( Value )));
        (*el)->val = key;
    } else {
        insert_el(key, &((*el)->next));
    }
}

int main(int argc, char** argv) {
    int i;

    //---------------------------------
    // x0
    //---------------------------------
    //int x0 = 8;
    Value* x0 = (Value*) malloc ( sizeof (Value) );
    x0->val.i = 8;
    x0->type = INTEGER;

    printf("x0 = %d\n", x0->val.i);

    //---------------------------------
    // x1
    //---------------------------------
    //Set *x1 = 0;
    Value *x1 = (Value*) malloc( sizeof (Value) );
    x1->val.s = 0;
    x1->type = SET;

    for (i = 1; i <= 7; i++) {
        Value *temp = (Value*) malloc( sizeof (Value) );
        temp->val.i = i;
        temp->type = INTEGER;
        insert_el(*temp, &x1->val.s);
    }
    // Temporary, should be able to add trees to Set
    insert_el(*x0, &x1->val.s);
    printf("x1 = ");
    print_type(x1);
    printf("\n");

    //---------------------------------
    // x2
    //---------------------------------
    //Pair* pair_x1 = 0;
    Value* pair_x1 = (Value*) malloc ( sizeof (Value) );
    pair_x1->val.p = 0;
    pair_x1->type = PAIR;

    //int one = 1;
    Value* x1_1 = (Value*) malloc ( sizeof (Value) );
    x1_1->val.i = 1;
    x1_1->type = INTEGER;
    create_pair(&pair_x1->val.p, *x1_1, *x1);

    //Set* x2 = 0;
    Value *x2 = (Value*) malloc( sizeof (Value) );
    x2->val.s = 0;
    x2->type = SET;

    insert_el(*x1, &x2->val.s);
    insert_el(*pair_x1, &x2->val.s);

    printf("x2 = ");
    print_type(x2);
    printf("\n");

    //---------------------------------
    // x3
    //---------------------------------
    
    //Pair* x3=0;
    Value* x3 = (Value*) malloc ( sizeof (Value) );
    x3->val.p = 0;
    x3->type = PAIR;
    create_pair(&x3->val.p, *x2, *x1);

    printf("x3 = ");
    print_type(x3);
    printf("\n");

    // free things
    return 0;
}
