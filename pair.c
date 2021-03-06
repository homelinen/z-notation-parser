/*
 * pair.c
 *
 * Defines methods for working with Pairs.
 */

#include <stdio.h>
#include <stdlib.h>
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
    p->left = left;
    p->right = right;
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

/**
 * Perform a deep copy on a pair, make a new pair with copies of the branches
 */
Pair* copy_pair(Pair* pair) {
   Pair* new_pair = (Pair*) malloc(sizeof( Pair ));
   memcpy(new_pair, pair, sizeof (Pair));

   new_pair->left = copy_value(*pair->left);
   new_pair->right = copy_value(*pair->right);

   return new_pair;
}

/*
 * The beginnings of a destroy function
 */
void destroy_pair(Pair* p) {
    // Ensure p points to something
    if (p != 0) {
        destroy_value(p->left);
        destroy_value(p->right);
        free(p);
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

/**
 * Inverts a pair
 * Uses the XOR swapping algorithm
 */
void pair_invert(Pair* tuple) {

    // If the same, no need to swap
    if (!value_equality(tuple->left, tuple->right)) { 
        // Store a temporary pointer
        Value* temp_left = tuple->left;
        tuple->left = tuple->right;
        tuple->right = temp_left;
    } 
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
