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
Variable* vars[25];

/* Signitures */
Value* parse_base_type(cJSON*);
Value* parse_equality_op(cJSON*);
Variable* parse_equal_op(cJSON*);
Value* parse_member_op(cJSON*);
Value* parse_tuple_op(cJSON*);
Value* find_variable(char*);

Value* find_variable(char* var) {
    int i;
    for (i = 0; i < 25; i++) {
        if (vars[i] && strncmp(vars[i]->name, var, 30) == 0) {
            return vars[i]->val;
        }
    }

    return 0;
}

Value* parse_set_op(cJSON* arguments) {
    
    /* Traverse into the first child of the array */
    cJSON* argument = arguments->child;

    Value* val = create_empty_val(SET);
    create_set(&val->val.s);

    Value* val_temp = 0;

    while (argument) {

        val_temp = parse_base_type(argument);

        insert_el(*val_temp, &val->val.s);
        argument = argument->next;
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
            val_temp = find_variable(argument->child->valuestring);
        } else if (strncmp(argument->child->string, "operator", 30) == 0) {
            /* Parse new operator, to something */
            if (strncmp(argument->child->valuestring, "set", 30) == 0) {
                val_temp = parse_set_op(argument->child->next);
            } else if (strncmp(argument->child->valuestring, "tuple", 30) == 0) {
                val_temp = parse_tuple_op(argument->child->next);
            } else if (strncmp(argument->child->valuestring, "member", 30) == 0) {
                val_temp = parse_member_op(argument->child->next);
            } else if (strncmp(argument->child->valuestring, "equal", 30) == 0) {
                // Equals in an equals is an equality operation
                val_temp = parse_equality_op(argument->child->next);
            }
        }
    } else if (argument->type == cJSON_Number) {
        val_temp = create_empty_val(INTEGER);
        val_temp->val.i = argument->valueint;
    }

    return val_temp;
}

Value* parse_tuple_op(cJSON* arguments) {
    
    /* Traverse into the first child of the array */
    cJSON* argument = arguments->child;

    Value* val = create_empty_pair();
    Value* val_temp = 0;

    while (argument) {

        val_temp = parse_base_type(argument);

        if(val->val.p->left == 0) {
            val->val.p->left = val_temp;
        } else if (val->val.p->right == 0) {
            val->val.p->right = val_temp;
        } else {
            printf("Tuple can only have two values\n");
        }

        argument = argument->next;
    }

    return val;
}

Value* parse_member_op(cJSON* args) {

    /* Traverse into the first child of the array */
    cJSON* argument = args->child;

    Value* set_temp = 0;
    Value* value = create_empty_val(INTEGER);

    Value* val_temp = 0;

    if (argument && argument->next) {

        val_temp = parse_base_type(argument);
        argument = argument->next;

        set_temp = parse_base_type(argument);

        value->val.i = set_membership(*val_temp, *set_temp->val.s);
    } else {
        value->val.i = 0;
    }
    
    return value;
}

Value* parse_equality_op(cJSON* arguments) {

    cJSON* argument = arguments->child;

    Value* val_temp = 0;

    while (argument) {
        if (val_temp) {
            val_temp->val.i = 
                value_equality(
                    val_temp, parse_base_type(argument)
                );
            val_temp->type = INTEGER;
        } else {
            val_temp = create_empty_val(SET);
            *val_temp = *parse_base_type(argument);
        }
        
        argument = argument->next;
    }

    return val_temp;
}

Variable* parse_equal_op(cJSON* arguments) {
    cJSON* argument = arguments->child;

    Variable* var = create_empty_variable();

    while (argument) {
        if (argument->type == cJSON_Object) {
            if (strncmp(argument->child->string, "variable", 30) == 0) {
                /* Set Variable Name */

                var->name = argument->child->valuestring;

            } else if (strncmp(argument->child->string, "operator", 30) == 0) {
                /* Parse new operator, to something */
                if (strncmp(argument->child->valuestring, "set", 30) == 0) {
                    *var->val = *parse_set_op(argument->child->next);
                } else if (strncmp(argument->child->valuestring, "tuple", 30) == 0) {
                    *var->val = *parse_tuple_op(argument->child->next);
                } else if (strncmp(argument->child->valuestring, "member", 30) == 0) {
                    *var->val = *parse_member_op(argument->child->next);
                } else if (strncmp(argument->child->valuestring, "equal", 30) == 0) {
                    // Equals in an equals is an equality operation
                    *var->val = *parse_equality_op(argument->child->next);
                }
            }
        } else if (argument->type == cJSON_Number) {
            /* If there is more than one number, probably a set */

            Value value = create_empty_value(INTEGER);
            value.val.i = argument->valueint;
            *var->val = value;
        }
        
        argument = argument->next;
    }

    return var;
}

/*
 * Iterates through the JSON Tree
 */
void parse_operator(cJSON *item) {

    int i = 0;
    while(item) {
        /* Find the operator, then find it's arguments */
        cJSON* operator = cJSON_GetObjectItem(item, "operator");

        /* Get the arguments array */
        cJSON* arguments = cJSON_GetObjectItem(item, "arguments");

        if (strncmp(operator->valuestring, "equal", 30) == 0) {
            /* Apply the second argument to the first */
            vars[i] = parse_equal_op(arguments);
        } else {
            printf("Undefined op\n");
            break;
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

    FILE* f = fopen("input.json", "r");

    // Check file
    if (f == 0) {
        printf("File does not exist\n");
        exit(1);
    }

    char ch;

    ch = fgetc(f);
    char line[5000];
    int i = 0;


    /* Parse file into a line */
    while (ch != EOF) {
        
        if (ch != '\n') {
            line[i] = ch;
        }

        ch = fgetc(f);
        i++;
        if (i > 5000) {
            printf("Line too long\n");
            fclose(f);
            exit(2);
        }
    }

    // Initialise Global Vars;
    i = 0;
    for (i = 0; i < 25; i++) {
        vars[i] = 0;
    }

    /*FILE* fpo = fopen("simple-output.txt", "w");*/
    FILE* fpo = stdout;

    cJSON *root = cJSON_Parse(line);

    // Main parsing op
    parse_operator(root->child->child);

    /* Print the answers */
    int j = 0;
    for (j = 0; j < i; j++) {
        if (vars[j]) {
            print_variable(vars[j], fpo);
        } else {
            // If the position is empty, don't bother
            break;
        }
    }

    cJSON_Delete(root);
    fclose(f);
    return 0;
}
