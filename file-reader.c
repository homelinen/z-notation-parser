#include <stdio.h>
#include <stdlib.h>
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

/*
 * Iterates through the JSON Tree
 */
void parse_item(cJSON *item) {

    cJSON* subitem = item->child;

    while (subitem != 0) {

        //parse
            printf("Name: %s", subitem->string);
            switch (subitem->type) {
                case cJSON_String:
                    printf(", value: %s\n", subitem->valuestring);
                    break;
                case cJSON_Number:
                    printf(", value: %d\n", subitem->valueint);
                    break;
                default:
                    printf("\n");
            }

        if (subitem->child) {
            parse_item(subitem);
        }

        subitem = subitem->next;
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
    parse_item(root);

    cJSON_Delete(root);
    fclose(f);
    return 0;
}
