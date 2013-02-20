/*
 * Simple Data structure for holding a variable
 */
#include <stdlib.h>

#include "all.h"
#include "variable.h"

/*
 * Set up a new variable
 */
Variable* create_variable(char* name, Value* val) {
    Variable* new_var = (Variable*) malloc( sizeof (Variable) );
    new_var->name = name;
    new_var->val = val;

    return new_var;
}

/*
 * Destroy the old variable
 *
 * TODO: Implement destroy_value
 */
void destroy_variable(Variable* var) {
    free(var->name);
    //destroy_value(val)

    free(var);
}
