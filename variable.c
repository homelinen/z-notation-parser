/*
 * Simple Data structure for holding a variable
 */
#include <stdlib.h>
#include <stdio.h>

#include "all.h"
#include "variable.h"

/*
 * Overloaded method to create a variable without giving 
 * the name and value
 *
 * Returns a pointer to the new Variable
 */
Variable* create_empty_variable() {
    return create_variable(0, 0);
}

/*
 * Set up a new variable
 */
Variable* create_variable(char* name, Value* val) {
    Variable* new_var = (Variable*) malloc( sizeof (Variable) );
    new_var->name = name;
    new_var->val = (Value*) malloc( sizeof (Variable));
    if (val) {
        new_var->val = val;
    }

    return new_var;
}

/*
 * Destroy the old variable
 *
 * TODO: Implement destroy_value
 */
void destroy_variable(Variable* var) {
    free(var->name);
    //FIXME: This will still leave memory kicking about
    //destroy_value(val)
    free(var->val);

    free(var);
}

/*
 * Print the variable as:
 * {name} = {value}
 */
void print_variable(Variable* var, FILE* f) {
    fprintf(f, "%s = ", var->name);
    print_type(var->val, f);
    fprintf(f, ";\n");
}
