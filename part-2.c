#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/cJSON.h"

#include "constants.h"
#include "all.h"
#include "variable.h"

/*
 * The operators used in the input file, and the struct equivalents are:
 *   Equal: create_variable
 *   set: create_value()->set
 *   tuple: create_value()->pair
 *   //TODO: Set membership
 */

// Global array of variables (Messy)
Variable* vars[100];
FILE* output_file;

/* Signitures */
Value* parse_base_type(cJSON*);
Value* parse_equality_op(cJSON*);
Variable* parse_equal_op(cJSON*);
Value* parse_is_func_op(cJSON*);
Value* parse_dom_op(cJSON*);
Value* parse_ran_op(cJSON*);
Value* parse_union_op(cJSON*);
Value* parse_subtraction_op(cJSON*);
Value* parse_intersect_op(cJSON*);
Value* parse_member_op(cJSON*);
Value* parse_tuple_op(cJSON*);
Variable* find_variable(char*);
Value* find_var_val(char*);

/**
 * Helper method to get the value of a variable
 *
 * Returns the value of the variable, or a 0 pointer if no such variable exists
 */
Value* find_var_val(char* var) {

    Variable* t_var = 0;
    t_var = find_variable(var);

    if (t_var) {
        return(t_var->val);
    } else {
        return(0); 
    }
}

/*
 * Loop through the global variables array searching for the given variable
 *
 * Returns a zero pointer if the variable doesn't exist
 */
Variable* find_variable(char* var) {
    int i;
    for (i = 0; i < 100; i++) {
        if (vars[i] && strncmp(vars[i]->name, var, 30) == 0) {
            return vars[i];
        }
    }

    return 0;
}

/**
 * Given arguments for a set operation build a value containing a set
 */
Value* parse_set_op(cJSON* arguments) {
    
    /* Traverse into the first child of the array */
    cJSON* argument = arguments->child;

    Value* val = create_empty_val(SET);
    create_set(&val->val.s);

    Value* val_temp = 0;

    while (argument) {

        // Get the base type of the argument
        val_temp = parse_base_type(argument);

        // Add the new value to the set
        insert_el(*val_temp, &val->val.s);

        // Goto the next object
        argument = argument->next;
    }

    return val;
}

Value* parse_is_func_op(cJSON* arguments) {
    
    /* Traverse into the first child of the array */
    cJSON* argument = arguments->child;

    Value* val = create_empty_val(INTEGER);
    Value* val_temp = 0;

    if (argument) {

        // Get the base type of the argument
        val_temp = parse_base_type(argument);

        val->val.i = isFunction(val_temp);
    }

    return val;
}

Value* parse_apply_func_op(cJSON* arguments) {
    
    /* Traverse into the first child of the array */
    cJSON* argument = arguments->child;

    Value* val = create_empty_val(INTEGER);
    Value* val_temp = 0;
    Value* set_temp = 0;

    if (argument) {

        /* Get the first element to compare */
        val_temp = parse_base_type(argument);
        argument = argument->next;

        /* Get the set to check if val_temp is contained with */
        set_temp = parse_base_type(argument);

        /* FIXME: Check argument is a set */

        val = apply_func(val_temp, set_temp);
    }

    return val;
}

Value* parse_dom_op(cJSON* arguments) {
    
    /* Traverse into the first child of the array */
    cJSON* argument = arguments->child;

    Value* val = create_empty_val(SET);
    create_set(&val->val.s);

    Value* val_temp = 0;

    if (argument) {

        // Get the base type of the argument
        val_temp = parse_base_type(argument);

        val->val.s = func_dom(val_temp);
    }

    return val;
}

Value* parse_ran_op(cJSON* arguments) {
    
    /* Traverse into the first child of the array */
    cJSON* argument = arguments->child;

    Value* val = create_empty_val(SET);
    create_set(&val->val.s);

    Value* val_temp = 0;

    if (argument) {

        // Get the base type of the argument
        val_temp = parse_base_type(argument);

        val->val.s = func_ran(val_temp);
    }

    return val;
}

/**
 * Look through the different types in an abstracted recursive way
 */
Value* parse_base_type(cJSON* argument) {

    Value* val_temp = 0;
    if (argument->type == cJSON_Object) {
        if (strncmp(argument->child->string, "variable", 30) == 0) {
            /* 
             * Given a variable, find it in the vars array 
             * FIXME: If the variable is undefined, handle it 
             */
            val_temp = find_var_val(argument->child->valuestring);
            if (!val_temp) {
                val_temp = create_empty_val(UNDEFINED);
            }
        } else if (strncmp(argument->child->string, "operator", 30) == 0) {
            if (strncmp(argument->child->valuestring, "set", 30) == 0) {

                val_temp = parse_set_op(argument->child->next);
            } else if (strncmp(argument->child->valuestring, "tuple", 30) == 0) {

                val_temp = parse_tuple_op(argument->child->next);
            } else if (strncmp(argument->child->valuestring, "member", 30) == 0) {

                val_temp = parse_member_op(argument->child->next);
            } else if (strncmp(argument->child->valuestring, "equal", 30) == 0) {

                // Equals in an equals is an equality operation
                val_temp = parse_equality_op(argument->child->next);

            } else if (strncmp(argument->child->valuestring, "set-difference", 30) == 0) {
                val_temp = parse_subtraction_op(argument->child->next);
            } else if (strncmp(argument->child->valuestring, "union", 30) == 0) {
                val_temp = parse_union_op(argument->child->next);
            } else if (strncmp(argument->child->valuestring, "intersection", 30) == 0) {
                val_temp = parse_intersect_op(argument->child->next);
            } else if (strncmp(argument->child->valuestring, "is-function", 30) == 0) {
                val_temp = parse_is_func_op(argument->child->next);
            } else if (strncmp(argument->child->valuestring, "apply-function", 30) == 0) {
                val_temp = parse_apply_func_op(argument->child->next);
            } else if (strncmp(argument->child->valuestring, "domain", 30) == 0) {
                val_temp = parse_dom_op(argument->child->next);
            } else if (strncmp(argument->child->valuestring, "range", 30) == 0) {
                val_temp = parse_ran_op(argument->child->next);
            } else {
                fprintf(stderr, "BAD INPUT: Undefined operator: %s\n", argument->child->valuestring);
                fprintf(output_file, "BAD INPUT\n");
              //  exit(EXIT_FAILURE);
            }
        }
    } else if (argument->type == cJSON_Number) {
        val_temp = create_empty_val(INTEGER);
        val_temp->val.i = argument->valueint;
    } else {
        fprintf(stderr, "BAD INPUT: Unexpected JSON Type.");
        fprintf(output_file, "BAD INPUT\n");
        exit(EXIT_FAILURE);
    }

    if (!val_temp) {
        fprintf(stderr, "BAD INPUT: Value null\n");
        fprintf(output_file, "BAD INPUT\n");
        exit(EXIT_FAILURE);
    }

    return val_temp;
}

/** 
 * Given a tuple operators argument, build a tuple
 *
 * Returns a value containing a tuple (Pair type).
 */
Value* parse_tuple_op(cJSON* arguments) {
    
    /* Traverse into the first child of the array */
    cJSON* argument = arguments->child;

    Value* val = create_empty_pair();
    Value* val_temp = 0;

    int arg_count = 0;

    while (argument) {

        val_temp = parse_base_type(argument);

        if(val->val.p->left == 0) {
            val->val.p->left = val_temp;
        } else if (val->val.p->right == 0) {
            val->val.p->right = val_temp;
        } else {
            printf("Tuple can only have two values\n");
        }

        arg_count++;
        argument = argument->next;
    }

    return val;
}

/**
 * Check if the arguments in a member operation are contained in one another
 *
 * first memberof second
 *
 * Returns true or false dependant on outcome
 */
Value* parse_member_op(cJSON* args) {

    /* Traverse into the first child of the array */
    cJSON* argument = args->child;

    Value* set_temp = 0;
    Value* value = create_empty_val(INTEGER);

    Value* val_temp = 0;

    if (argument && argument->next) {

        /* Get the first element to compare */
        val_temp = parse_base_type(argument);
        argument = argument->next;

        /* Get the set to check if val_temp is contained with */
        set_temp = parse_base_type(argument);

        value->val.i = set_membership(*val_temp, *set_temp->val.s);
    } else {
        /* If there aren't enough argument, set value to false */
        value->val.i = 0;
    }
    
    return value;
}

Value* parse_union_op(cJSON* args) {

    /* Traverse into the first child of the array */
    cJSON* argument = args->child;

    Value* set_temp = 0;
    Value* value = create_empty_val(SET);
    create_set(&value->val.s);

    Value* val_temp = 0;

    if (argument && argument->next) {

        /* Get the first element to compare */
        val_temp = parse_base_type(argument);
        argument = argument->next;

        /* Get the set to check if val_temp is contained with */
        set_temp = parse_base_type(argument);

        /* FIXME: Check argument is a set */

        set_union(val_temp->val.s, set_temp->val.s, value->val.s);
    } else {
        /* If there aren't enough argument, set value to false */
        value->val.i = 0;
    }
    
    return value;
}

Value* parse_intersect_op(cJSON* args) {

    /* Traverse into the first child of the array */
    cJSON* argument = args->child;

    Value* set_temp = 0;
    Value* value = create_empty_val(SET);
    create_set(&value->val.s);

    Value* val_temp = 0;

    if (argument && argument->next) {

        /* Get the first element to compare */
        val_temp = parse_base_type(argument);

        /* Get second element */
        argument = argument->next;
        set_temp = parse_base_type(argument);

        /* FIXME: Check argument is a set */

        intersection(val_temp->val.s, set_temp->val.s, value->val.s);
    } else {
        /* If there aren't enough argument, set value to false */
        value->val.i = 0;
    }
    
    return value;
}

Value* parse_subtraction_op(cJSON* args) {

    /* Traverse into the first child of the array */
    cJSON* argument = args->child;

    Value* set_temp = 0;
    Value* value = create_empty_val(SET);
    create_set(&value->val.s);

    Value* val_temp = 0;

    if (argument && argument->next) {

        /* Get the first element to compare */
        val_temp = parse_base_type(argument);

        /* Get second element */
        argument = argument->next;
        set_temp = parse_base_type(argument);

        subtraction(val_temp->val.s, set_temp->val.s, value->val.s);
    } else {
        /* If there aren't enough argument, set value to false */
        value->val.i = 0;
    }
    
    return value;
}

/*
 * Checks if two arguments of an equality operation are equal to one another
 * Returns an int Value of 1 or 0, which are True, False
 */
Value* parse_equality_op(cJSON* arguments) {

    cJSON* argument = arguments->child;

    Value* val_temp = 0;

    while (argument) {
        if (val_temp) {
            /* Re-use the value created in the else and set it to be of integer type
             * to store true/false */
            val_temp->val.i = 
                value_equality(
                    val_temp, parse_base_type(argument)
                );
            val_temp->type = INTEGER;
        } else {
            /* For the first argument, build a new value to build on */
            val_temp = create_empty_val(SET);
            *val_temp = *parse_base_type(argument);
        }
        
        argument = argument->next;
    }

    return val_temp;
}

/*
 * Allocation opertation. Sets the arguments to the given variable string.
 *
 * FIXME: Re-Use the base_type parsing function to reduce code.
 *
 * Returns a variable with the name of the allocated variable and the type in reference.
 */
Variable* parse_equal_op(cJSON* arguments) {
    cJSON* argument = arguments->child;

    Variable* var = create_empty_variable();

    while (argument) {
        if (argument->type == cJSON_Object) {
            if (strncmp(argument->child->string, "variable", 30) == 0) {

                if (!var->name) {
                    if (!find_var_val(argument->child->valuestring)) {

                        /* Set Variable name */
                        var->name = argument->child->valuestring;
                    } else {
                        //Variable has already been set
                        // FIXME: Store this somewhere, to reduce O(n) lookups
                        var = find_variable(argument->child->valuestring);
                    }
                } else {
                    *var->val = *parse_base_type(argument);
                }
            } else if (strncmp(argument->child->string, "operator", 30) == 0) {

                *var->val = *parse_base_type(argument);
            }
        } else if (argument->type == cJSON_Number) {

            /* Build a number set */
            Value value = create_empty_value(INTEGER);
            value.val.i = argument->valueint;
            *var->val = value;
        }
        
        argument = argument->next;
    }
    print_variable(var, output_file);

    return var;
}

/*
 * Iterates through the JSON Tree and builds the variable array
 */
void parse_operator(cJSON *item) {

    int i = 0;
    while(item) {
        /* Find the operator, then find it's arguments */
        cJSON* operator = cJSON_GetObjectItem(item, "operator");

        /* Get the arguments array */
        cJSON* arguments = cJSON_GetObjectItem(item, "arguments");

        if (strncmp(operator->valuestring, "equal", 30) == 0) {
            /* Given an allocation add the variable to the vars array */ 
            // FIXME: Deal with duplicate variable names
            vars[i] = parse_equal_op(arguments);
        } else {
            fprintf(stderr, "ERROR: Undefined op\n");
            fprintf(output_file, "BAD INPUT\n");
            exit(EXIT_FAILURE);
        }

        // Try the next item in the array
        item = item->next;
        i++;
    }
}

// TODO: Move this to a shared file
void print_answer(int num, Value* val, FILE* f) {
    fprintf(f, "x%d = ", num);
    print_type(val, f);
    fprintf(f, ";\n");
}

int main (int argc, char** args) {

    FILE* pFile = fopen("input.json", "r");

    // Check file
    if (pFile == 0) {
        printf("File does not exist\n");
        exit(1);
    }

    int fileSize = 0;

    fseek(pFile, 0, SEEK_END);
    fileSize = ftell(pFile);
    rewind(pFile);
    char *data = (char*) calloc(sizeof(char), fileSize + 20);
    fread(data, 1, fileSize, pFile);

    if(ferror(pFile)){
        printf("fread issue\n");
        exit(1);
    }

    // Initialise Global Vars;
    int i = 0;
    for (i = 0; i < 100; i++) {
        vars[i] = 0;
    }


    /* The output file
     * TODO: Make a command line arg
     */
    output_file = fopen("output.txt", "w");

    /* Use if want to print to stdout */
    /*FILE* fpo = stdout;*/

    /* Get the JSON Tree */
    cJSON *root = cJSON_Parse(data);

    // Main parsing op
    parse_operator(root->child->child);

    /* Clean up */
    cJSON_Delete(root);
    fclose(pFile);
    fclose(output_file);

    return EXIT_SUCCESS;
}
