/*
 * pair.c
 *
 * Defines methods for working with Pairs.
 */

#include <stdio.h>
#include <stdlib.h>

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
Value create_pair(Value left, Value right) {
    Value value_pair = create_empty_value(PAIR);
    Pair* p = (Pair*) malloc( sizeof( Pair ) );
    p->left = left;
    p->right = right;
    value_pair.val.p = p;

    return value_pair;
}

void destroy_pair(Pair* p) {
    // Ensure p points to something
    if (p != 0) {
        destroy_value(&p->left);
        destroy_value(&p->right);
        free(p);

    }
}

/**
 * Print each element in the pair to stdout
 *
 * Prints in the form 
 *   (left, right)
 */
void print_pair(Pair* p) {
    printf("(");

    print_type(&p->left);
    printf(", ");
    print_type(&p->right);
    printf(")");
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
    if (value_equality(&(first->left), &(second->left)) &&
        value_equality(&(first->right), &(second->right))) {
        return 1;
    } else {
        return 0;
    }
}
