#include <stdlib.h>
#include <stdio.h>

// A node in the tree
struct node {
    int key_value;
    struct node *left;
    struct node *right;
};

// The elements of a set
struct element {
    int key_value;
    struct element *next;
};

/**
 * Recurse through tree, destroying all children up to leaves
 **/
void destroy_tree(struct node *leaf) {
    if (leaf != 0) {
        destroy_tree(leaf->right);
        destroy_tree(leaf->left);
        free(leaf);
    }
}

/**
 * Insert the key into the tree
 **/
void insert(int key, struct node **leaf) {
    if( *leaf == 0 ) {
        *leaf = (struct node*) malloc( sizeof( struct node ) );
        (*leaf)->key_value = key;
        // initialize the children to null
        (*leaf)->left = 0;    
        (*leaf)->right = 0;  
    } else if(key < (*leaf)->key_value) {
        insert( key, &(*leaf)->left );
    } else if(key > (*leaf)->key_value) {
        insert( key, &(*leaf)->right );
    }
}

int main(int argc, char** argv) {
    struct node *root = 0;
    insert(8, &root );

    printf("Root: %d", root->key_value);
    return 0;
}
