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
Value create_empty_value(int);

Value create_pair(Value left, Value right) {
    Value value_pair;
    value_pair = create_empty_value(PAIR);
    Pair* p = (Pair*) malloc( sizeof( Pair ) );
    p->left = left;
    p->right = right;
    value_pair.val.p = p;

    return value_pair;
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
        (*el)->next = 0;
    } else {
        insert_el(key, &((*el)->next));
    }
}

/**
 * Create a new Value of type, with nothing in it
 */
Value create_empty_value(int type) {
    Value* new_val = (Value*) malloc ( sizeof (Value) );
    switch (type) {

        case SET:
            new_val->val.s = 0;
            new_val->type = SET;
            break;
        case INTEGER:
            new_val->val.i = 0;
            new_val->type = INTEGER;
            break;
        case PAIR:
            new_val->val.p = 0;
            new_val->type = PAIR;
            break;
    }

    return *new_val;
}

/**
 * Create the union between the second set and first set
 * The first set is used to store the second
 **/
void set_union(Set* first, Set* second) {

    if (second != 0) {
        insert_el(second->val, &first);
        set_union(first, second->next);
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
    Value *x1 = (Value*) malloc( sizeof (Value) );
    x1->val.s = 0;
    x1->type = SET;

    for (i = 1; i <= 7; i++) {
        Value *temp = (Value*) malloc( sizeof (Value) );
        temp->val.i = i;
        temp->type = INTEGER;
        insert_el(*temp, &x1->val.s);
    }

    insert_el(*x0, &x1->val.s);
    printf("x1 = ");
    print_type(x1);
    printf("\n");

    //---------------------------------
    // x2
    //---------------------------------

    Value* x1_1 = (Value*) malloc ( sizeof (Value) );
    x1_1->val.i = 1;
    x1_1->type = INTEGER;
    Value pair_x1 = create_pair(*x1_1, *x1);

    Value *x2 = (Value*) malloc( sizeof (Value) );
    x2->val.s = 0;
    x2->type = SET;

    insert_el(*x1, &x2->val.s);
    insert_el(pair_x1, &x2->val.s);

    printf("x2 = ");
    print_type(x2);
    printf("\n");

    //---------------------------------
    // x3
    //---------------------------------
    
    Value x3 = create_pair(*x2, *x1);

    printf("x3 = ");
    print_type(&x3);
    printf("\n");

    //---------------------------------
    // x4
    //---------------------------------
    
    Value x4 = create_empty_value(SET);
    insert_el(x3, &(&x4)->val.s);

    set_union((&x4)->val.s, x2->val.s);

    printf("x4 = ");
    print_type(&x4);
    printf("\n");

    // free things
    return 0;
}
