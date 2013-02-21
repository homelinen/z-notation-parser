/*
 * Struct for a simple variable type
 */

typedef struct {
    char* name;
    Value* val;
} Variable;

Variable* create_empty_variable();
Variable* create_variable(char*, Value*);
void destroy_variable(Variable*);
