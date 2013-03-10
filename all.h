/*
 * all.h
 *
 * Contains all the defintions for Set, Pair and Value
 *
 * The definitions are not seperate due to some recursive dependency 
 * problems.
 */

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
    //Type for the container
    int type;
};

struct _Pair {
    Value* left;
    Value* right;
};

struct _Set {
    Value val;
    Set *next;
    //Sub elements in the tree
    int length;
    //Is this the head or not
    int head;
};

/*
 * Set functions
 */
void create_set(Set **set_new);
void destroy_set(Set*); 
void insert_el(Value key, Set **el);
void print_set(Set*, FILE*);
void set_union(Set*, Set*, Set*);
int set_membership(Value element, Set set);
int isFunction(Value*);
Set* func_dom(Value*);
Set* func_ran(x2);
Value* func_inverse(Value*);
int func_is_injective(Value*);
Value* apply_func(Value*, Value*);
int set_contents_equality(Set*, Set*);
void intersection(Set*, Set*, Set*);
void subtraction(Set*, Set*, Set*);

/*
 * Value functions
 */
Value create_empty_value(int);
Value* create_empty_val(int);
void destroy_value(Value*); 
void print_type(Value*, FILE*);
int value_equality(Value*, Value*);

/*
 * Pair functions
 */
void print_pair(Pair*, FILE*);
Value create_pair(Value*, Value*);
Value* create_empty_pair(); 
void destroy_pair(Pair*); 
void pair_invert(Pair*);
int pair_equality(Pair*, Pair*);


