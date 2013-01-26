typedef struct _Set Set;

struct _Set {
    Value val;
    Set *next;
    int length;
    int head;
};

// Functions
void create_set(Set **set_new);
void insert_el(Value key, Set **el);
void print_set(Set*);

void set_union(Set*, Set*);
int set_contents_equality(Set*, Set*);
void intersection(Set*, Set*, Set*, int);
void subtraction(Set*, Set*, int);
