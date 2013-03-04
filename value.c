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
void print_type(Value* val, FILE* f) {
    
    if (val) {
        switch (val->type) {
            case INTEGER: 
                fprintf(f, "%d", val->val.i);
                break;
            case SET:
                print_set(val->val.s, f);
                break;
            case PAIR:
                print_pair(val->val.p, f);
                break;
            case UNDEFINED:
                fprintf(f, "undefined!");
                break;
            default:
                fprintf(f, "%p", val);
                break;
        }
    } else {
        printf("null");
    }
}

void print_type_address(Value* val, FILE* f) {
    
    if (val) {
        fprintf(f, "%p: ", val);
        switch (val->type) {
            case INTEGER: 
                fprintf(f, "%d", val->val.i);
                break;
            case SET:
                print_set_address(val->val.s, f);
                break;
            case PAIR:
                print_pair_address(val->val.p, f);
                break;
            case UNDEFINED:
                fprintf(f, "undefined!");
                break;
            default:
                fprintf(f, "%p", val);
                break;
        }
    } else {
        printf("null");
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
        case UNDEFINED:
            new_val->val.i = 0;
            new_val->type = UNDEFINED;
            break;
    }

    return *new_val;
}

/*
 * Return a pointer to the value rather than an object (as above)
 */
Value* create_empty_val(int type) {
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
        case UNDEFINED:
            new_val->val.i = 0;
            new_val->type = UNDEFINED;
            break;
        default:
            printf("Warning: undefined!\n");
            break;
    }

    return new_val;
}

/*
 * The beginnings of a destruction function
 *
 * FIXME: The pointers are not unique so causes some funky things to happen
 * NOTE: DO NOT USE, yet
 */
void destroy_value(Value* val) {
    if (val != 0 && val != NULL) {
        switch (val->type) {
            printf("Destroy!\n");
            case SET:
                destroy_set(&val->val.s);
                break;
            case PAIR:
                destroy_pair(val->val.p);
                break;
            //Ignore ints
        }
        
        free(val);
    }
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
