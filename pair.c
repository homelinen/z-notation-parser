#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "all.h"

Value create_pair(Value left, Value right) {
    Value value_pair = create_empty_value(PAIR);
    Pair* p = (Pair*) malloc( sizeof( Pair ) );
    p->left = left;
    p->right = right;
    value_pair.val.p = p;

    return value_pair;
}

void print_pair(Pair* p) {
    printf("(");

    print_type(&p->left);
    printf(", ");
    print_type(&p->right);
    printf(")");
}

int pair_equality(Pair* first, Pair* second) {
    if (value_equality(&(first->left), &(second->left)) &&
            value_equality(&(first->right), &(second->right))) {
        return 1;
    } else {
        return 0;
    }
}
