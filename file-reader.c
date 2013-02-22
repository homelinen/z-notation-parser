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

/* Signitures */
void parse_item(cJSON *item);

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

    Variable* vars[20];
    int i = 0;
    for (i = 0; i < 20; i++) {
        vars[i] = 0;
    }

    i = 0;
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

    cJSON *root = cJSON_Parse(line);

    /*parse_item(root->child);*/
    /*test_parse(root->child->child);*/
    parse_operator(root->child->child);

    cJSON_Delete(root);
    fclose(f);
    return 0;
}
