typedef struct _Pair Pair;

struct _Pair {
    Value left;
    Value right;
};

void print_pair(Pair*);
Value create_pair(Value, Value);
int pair_equality(Pair*, Pair*);
