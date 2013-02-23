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
Variable* vars[20];

/* Signitures */
//

Value* find_variable(char* var) {
    int i;
    for (i = 0; i < 20; i++) {
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

    while (argument) {

        if (argument->type == cJSON_Object) {
            if (strncmp(argument->child->string, "variable", 30) == 0) {
                /* 
                 * Parse the variable into an expression 
                 * Add the variable into the set
                 */
                insert_el(*find_variable(argument->child->valuestring), &val->val.s);
            }
        }

        argument = argument->next;
    }

    return val;
}

Value* parse_tuple_op(cJSON* arguments) {
    
    /* Traverse into the first child of the array */
    cJSON* argument = arguments->child;

    Value* val = create_empty_pair();
    Value* val_temp = 0;

    while (argument) {

        if (argument->type == cJSON_Object) {
            if (strncmp(argument->child->string, "variable", 30) == 0) {
                val_temp = find_variable(argument->child->valuestring);
            }
        } else if (argument->type == cJSON_Number) {
            val_temp = create_empty_val(INTEGER);
            val_temp->val.i = argument->valueint;

        }

        if(val->val.p->left == 0) {
            printf("Left\n");
            val->val.p->left = val_temp;
        } else if (val->val.p->right == 0) {
            printf("Right\n");
            val->val.p->right = val_temp;
        } else {
            printf("Tuple can only have two values\n");
        }


        argument = argument->next;
    }

    return val;
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

    int j = 0;
    for (j = 0; j < i; j++) {
        if (vars[j]) {
            print_variable(vars[j]);
        } else {
            // If the position is empty, don't bother
            break;
        }
    }
}

// TODO: Move this to a shared file
void print_answer(int num, Value* val) {
    printf("x%d = ", num);
    print_type(val);
    printf(";\n");
}

int main (int argc, char** args) {

    FILE* f = fopen("simple-input.json", "r");

    // Check file
    if (f == 0) {
        printf("File does not exist\n");
        exit(1);
    }

    char ch;

    ch = fgetc(f);
    char line[5000];
    int i = 0;

    while (ch != EOF) {
        
        if (ch != '\n') {
            line[i] = ch;
        } else {
            i = 0;
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
    for (i = 0; i < 20; i++) {
        vars[i] = 0;
    }

    cJSON *root = cJSON_Parse(line);

    /*parse_item(root->child);*/
    /*test_parse(root->child->child);*/
    parse_operator(root->child->child);

    cJSON_Delete(root);
    fclose(f);
    return 0;
}
