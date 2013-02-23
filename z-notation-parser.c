/**
 * Foundations 2 Assignment: Part 1
 *
 * A solution for the first part of the assignment, building tree-like data structures.
 */

#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "all.h"

/** 
 * Print the answer for each task to spec
 *
 * num - The number to be printed beside x
 * val - The value to be printed
 *
 * Example
 *   x1 = {1,2,3,4,5,6,7,8};
 */
void print_answer(int num, Value* val, FILE* f) {
    printf("x%d = ", num);
    print_type(val, f);
    printf(";\n");
}

int main(int argc, char** argv) {
    int i;

    FILE* fpo = stdout;

    //---------------------------------
    // x0
    //---------------------------------

    Value* x0 = (Value*) malloc ( sizeof (Value) );
    x0->val.i = 8;
    x0->type = INTEGER;

    print_answer(0, x0, fpo);

    //---------------------------------
    // x1
    //---------------------------------
    Value x1 = create_empty_value( SET );
    create_set(&x1.val.s);

    //Insert values into the set 
    for (i = 1; i <= 7; i++) {
        Value *temp = (Value*) malloc( sizeof (Value) );
        temp->val.i = i;
        temp->type = INTEGER;
        insert_el(*temp, &(&x1)->val.s);
    }

    insert_el(*x0, &(&x1)->val.s);

    print_answer(1, &x1, fpo);

    //---------------------------------
    // x2
    //---------------------------------

    Value x1_1 = create_empty_value(INTEGER);
    (&x1_1)->val.i = 1;
    Value pair_x1 = create_pair(&x1_1, &x1);

    Value x2 = create_empty_value(SET);
    create_set(&x2.val.s);

    insert_el(x1, &(&x2)->val.s);
    insert_el(pair_x1, &(&x2)->val.s);

    print_answer(2, &x2, fpo);

    //---------------------------------
    // x3
    //---------------------------------
    
    // (x2, x1)
    Value x3 = create_pair(&x2, &x1);

    print_answer(3, &x3, fpo);

    //---------------------------------
    // x4
    //---------------------------------
    
    Value x4 = create_empty_value(SET);
    create_set(&x4.val.s);

    // Insert x3 into x4, so that x3 is enclosed by a set
    // { x3 }
    insert_el(x3, &(&x4)->val.s);

    // { x3 } U x2
    set_union((&x4)->val.s, (&x2)->val.s);

    print_answer(4, &x4, fpo);

    //---------------------------------
    // x5
    //---------------------------------
   
    Value x4_temp = create_empty_value(SET);
    x4_temp = x4;

    Value x1_set = create_empty_value(SET);
    insert_el(x1, &(&x1_set)->val.s);

    subtraction((&x4_temp)->val.s, (&x1_set)->val.s);

    print_answer(5, &x4_temp, fpo);

    //---------------------------------
    // x6
    //---------------------------------
   
    // Rebuild x4 to get around a pointer problem with x4_temp above
    x4_temp = create_empty_value(SET);
    create_set(&x4_temp.val.s);
    insert_el(x3, &(&x4_temp)->val.s);
    set_union((&x4_temp)->val.s, (&x2)->val.s);

    // Assign memory for intersected set
    Set* intersected = 0;
    create_set(&intersected);

    intersection((&x4_temp)->val.s, (&x1_set)->val.s, intersected);

    fprintf(fpo, "x6 = ");
    print_set(intersected, fpo);
    fprintf(fpo, ";\n");

    // free things

    return 0;
}
