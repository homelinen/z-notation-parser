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
    int length;
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

int value_equality(Value*, Value*);

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
        (*el)->length = 1;
    } else {
        insert_el(key, &((*el)->next));
        (*el)->length += 1;
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

/**
 * Remove the elements from set 2 that match those of set 1
 */
void subtraction(Set* first, Set* second, int head) {

    if (first != 0 && second != 0) {

        // Check the current node for equality only on the head of the set, otherwise weird things may happen.
        if (head) {
            if (value_equality(&(first->val), &(second->val))) {
                if (first->next != 0) {
                    first = first->next;
                }
            }
        }

        if (first->next != 0) {

            // Copy the second set
            Set* sec_temp = (Set*) (malloc ( sizeof (Set) ));
            *sec_temp = *second;

            // Iterate through all of the second sets children
            do {

                // Check if the children are equal
                if (value_equality(&(first->next->val), &(sec_temp->val))) {
                    Set* next = first->next;
                    // Change the current next pointer to the child's child
                    // Remove the child node from the list
                    first->next = first->next->next;

                    // Remove next from memory
                    //
                    // TODO: Pointers are all messed up, so maybe don't do this
                    //free(next);
                }

                if (sec_temp->next != 0) {
                    *sec_temp = *(sec_temp->next);
                } else {
                    sec_temp = 0;
                }
            } while (sec_temp != 0);
            // Subtract the next two elements
            subtraction(first->next, second, 0);
        }
    }
}

/**
 * Create a new set containing the values of first and second that intersect
 */
void intersection(Set* first, Set* second, Set* intersected, int head) {

    if (first != 0 && second != 0) {

        // Check the current node for equality only on the head of the set, otherwise weird things may happen.
        if (head) {
            if (value_equality(&(first->val), &(second->val))) {
                // Only checking the heads of the set, need children as well

                printf("\n");

                insert_el(first->val, &intersected);
            }
        }

        if (first->next != 0) {

            // Copy the second set
            Set* sec_temp = (Set*) (malloc ( sizeof (Set) ));
            *sec_temp = *second;

            // Iterate through all of the second sets children
            do {
                // Check if the children are equal
                if (value_equality(&(first->next->val), &(sec_temp->val))) {
                    // Insert the intersected value
                    insert_el(first->next->val, &intersected);
                }

                if (sec_temp->next != 0) {
                    *sec_temp = *(sec_temp->next);
                } else {
                    sec_temp = 0;
                }
            } while (sec_temp != 0);

            // Find intersection for next
            intersection(first->next, second, intersected, 0);
        }
    }
}
/**
 * Check if two sets are equal
 *
 * Two sets are equal if their elements are equal
 */
int set_contents_equality(Set* first, Set* second) {

    if (first != 0 && second != 0) {

        // If first value false, return 0
        if (!(value_equality(&(first->val), &(second->val)))) {
           return 0; 
        }

        // If reached the end of the sets without error, they're the same
        if (first->next == 0 && second->next == 0) {
            return 1;
        }

        // Compare first set with the second
        if (value_equality(&(first->next->val), &(second->next->val))) {
            // Check the next elements in the set for equality
            return set_contents_equality(first->next, second->next);
        }
    }
        
    return 0;
}

int pair_equality(Pair* first, Pair* second) {
    if (value_equality(&(first->left), &(second->left)) &&
            value_equality(&(first->right), &(second->right))) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Check if two values are equal
 */
int value_equality(Value* first, Value* second) {
    
    // If not the same type, not the same
    if (first->type != second->type) {
        return 0;
    }

    switch (first->type) {
        case INTEGER: 
            if (first->val.i == second->val.i) {
                return 1;
            }
            break;
        case SET:
            // Check set length
            if (first->val.s->length == second->val.s->length) {
                return 1;
            } else if (set_contents_equality(first->val.s, second->val.s)) {
                return 1;
            }
            break;
        case PAIR:
            if (pair_equality(first->val.p, second->val.p)) {
                return 1;
            }
            break;
    }

    // If no tests passed, it has to be false
    return 0;
}

/** 
 * Print the answer for each task to spec
 *
 * Example
 *   x1 = {1,2,3,4,5,6,7,8};
 */
void print_answer(int num, Value* val) {
    printf("x%d = ", num);
    print_type(val);
    printf(";\n");
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

    print_answer(0, x0);

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

    print_answer(1, x1);

    //---------------------------------
    // x2
    //---------------------------------

    Value x1_1 = create_empty_value(INTEGER);
    (&x1_1)->val.i = 1;
    Value pair_x1 = create_pair(x1_1, *x1);

    Value x2 = create_empty_value(SET);

    insert_el(*x1, &(&x2)->val.s);
    insert_el(pair_x1, &(&x2)->val.s);

    print_answer(2, &x2);

    //---------------------------------
    // x3
    //---------------------------------
    
    Value x3 = create_pair(x2, *x1);

    print_answer(3, &x3);

    //---------------------------------
    // x4
    //---------------------------------
    
    Value x4 = create_empty_value(SET);

    // Insert x3 into x4
    insert_el(x3, &(&x4)->val.s);

    // { x3 } U x2
    set_union((&x4)->val.s, (&x2)->val.s);

    print_answer(4, &x4);

    //---------------------------------
    // x5
    //---------------------------------
   
    Value x4_temp = create_empty_value(SET);
    x4_temp = x4;

    Value x1_set = create_empty_value(SET);
    insert_el(*x1, &(&x1_set)->val.s);

    subtraction((&x4_temp)->val.s, (&x1_set)->val.s, 1);

    print_answer(5, &x4_temp);

    //---------------------------------
    // x6
    //---------------------------------
   
    // Rebuild x4 to get around a pointer problem with x4_temp above
    x4_temp = create_empty_value(SET);
    insert_el(x3, &(&x4_temp)->val.s);
    set_union((&x4_temp)->val.s, (&x2)->val.s);

    // Assign memory for intersected set
    Set* intersected = (Set*) malloc ( sizeof(Set) );

    intersection((&x4_temp)->val.s, (&x1_set)->val.s, intersected, 1);
    //Drop the head of the set (Horrific code)
    intersected = intersected->next;

    printf("x6 = ");
    print_set(intersected, 1);
    printf(";\n");

    // free things
    return 0;
}
