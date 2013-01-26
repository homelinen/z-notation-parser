/**
 * Value.c
 *
 * Structure for holding three types:
 *   pair, set and integer
 */

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "all.h"

/**
 * Try to print the value stored within the container of val
 *
 * val - The val to print
 *
 * If type isn't defined, print address
 */
void print_type(Value* val) {
    
    switch (val->type) {
        case INTEGER: 
            printf("%d", val->val.i);
            break;
        case SET:
            print_set(val->val.s);
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
 * Create a new Value of type, with nothing in it
 *
 * type - The type_constant that will identify this Value
 *
 * Returns the new value
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
 * Check if two Values are equal
 *
 * first - a Value
 * second - the Value to compare first with
 *
 * Returns 1 for true and 0 for false
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