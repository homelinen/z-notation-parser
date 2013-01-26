typedef union _Container Container;
typedef struct _Value Value;
typedef struct _Set Set;
typedef struct _Pair Pair;

union _Container {
    int i;
    Set* s;
    Pair* p;
};

struct _Value {
    Container val;
    int type;
};


struct _Pair {
    Value left;
    Value right;
};

struct _Set {
    Value val;
    Set *next;
    int length;
    int head;
};

/*
 * Sets
 */
void create_set(Set **set_new);
void insert_el(Value key, Set **el);
void print_set(Set*);

void set_union(Set*, Set*);
int set_contents_equality(Set*, Set*);
void intersection(Set*, Set*, Set*, int);
void subtraction(Set*, Set*, int);

/*
 * Value
 */
Value create_empty_value(int);
void print_type(Value*);
int value_equality(Value*, Value*);

/*
 * Pair
 */
void print_pair(Pair*);
Value create_pair(Value, Value);
int pair_equality(Pair*, Pair*);

