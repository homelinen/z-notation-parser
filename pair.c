/*
 * pair.c
 *
 * Defines methods for working with Pairs.
 */

#include <stdio.h>
#include <stdlib.h>

// For memcpy
#include <string.h>

#include "constants.h"
#include "all.h"

/**
 * Create a new Pair instance
 * 
 * left - The first value in the pair
 * right - The second value in the pair
 *
 * Returns a Value containing the pair or left and right
 */
Value create_pair(Value* left, Value* right) {
    Value value_pair = create_empty_value(PAIR);
    Pair* p = (Pair*) malloc( sizeof( Pair ) );

    // Copy values
    Value* left_copy = create_empty_val(UNDEFINED);
    memcpy(left_copy, left, sizeof(Value));
    Value* right_copy = create_empty_val(UNDEFINED);
    memcpy(right_copy, right, sizeof(Value));

    p->left = left_copy;
    p->right = right_copy;
    value_pair.val.p = p;

    return value_pair;
}

/*
 * Creates an empty pair, doens't require giving the branch values on init
 *
 * Returns a pointer to the new pair
 */
Value* create_empty_pair() {
    Value* value_pair = create_empty_val(PAIR);
    Pair* p = (Pair*) malloc( sizeof( Pair ) );
    p->left = 0;
    p->right = 0;
    value_pair->val.p = p;

    return value_pair;
}

/*
 * The beginnings of a destroy function
 */
void destroy_pair(Pair* p) {
    // Ensure p points to something
    if (p != 0) {
        destroy_value(p->left);
        destroy_value(p->right);
    }
}

/**
 * Print each element in the pair to stdout
 *
 * Prints in the form 
 *   (left, right)
 */
void print_pair(Pair* p, FILE* f) {
    fprintf(f, "(");

    print_type(p->left, f);
    fprintf(f, ", ");
    print_type(p->right, f);
    fprintf(f, ")");
}

void print_pair_address(Pair* p, FILE* f) {
    fprintf(f, "(");

    printf("%p", p->left);
    fprintf(f, ", ");
    printf("%p", p->right);
    fprintf(f, ")");
}

/**
 * Check if two pairs are equal
 *
 * first - The first pair
 * second - The pair to check against
 *
 * Returns true if each independant side of the pair are equal
 * i.e
 *   left1 == left2 && right1 == right2
 */
int pair_equality(Pair* first, Pair* second) {
    if (value_equality(first->left, second->left) &&
        value_equality(first->right, second->right)) {
        return 1;
    } else {
        return 0;
    }
}
