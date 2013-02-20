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

/**
 * Initial test for the parser
 */
void test_parse(cJSON *item) {

    cJSON* operator = cJSON_GetObjectItem(item, "operator");
    if (operator && strncmp(operator->valuestring, "equal", 80)==0) {
        cJSON* args = cJSON_GetObjectItem(item, "arguments");

        if (args) {
            char* tempName;
            Value val;

            cJSON* subitem = args->child;

            switch (subitem->type) {
                case cJSON_Object:
                    tempName = subitem->child->valuestring;
                    break;
                case cJSON_Number:
                    val = create_empty_value(INTEGER);
                    val.val.i = subitem->valueint;
                    break;
            }
            subitem = subitem->next;
            switch (subitem->type) {
                case cJSON_Object:
                    tempName = subitem->child->valuestring;
                    break;
                case cJSON_Number:
                    val = create_empty_value(INTEGER);
                    val.val.i = subitem->valueint;
                    break;
            }

            Variable var = *(create_variable(tempName, &val));
            printf("%s = ", var.name);
            print_type(var.val);
            printf(";\n");

        }
    }
}

/*
 * Iterates through the JSON Tree
 */
void parse_item(cJSON *item) {

    cJSON* subitem = item->child;

    while (subitem != 0) {
        printf("Type: %s, ", subitem->string);
        printf("val: ");

        switch (subitem->type) {
            case cJSON_Object:
                break;
            case cJSON_String:
                printf("%s", subitem->valuestring);
                break;
            case cJSON_Number:
                printf("%d", subitem->valueint);
                break;
        }
        printf("\n");

        if (subitem->child) {
            parse_item(subitem);
        }

        subitem = subitem->next;
        /*subitem=0;*/
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
    test_parse(root->child->child->next);

    cJSON_Delete(root);
    fclose(f);
    return 0;
}
