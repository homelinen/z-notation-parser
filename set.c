#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "all.h"

/**
 * Recurse through the Set freeing memory
 * from the end of the Set
 **/
void destroy_set(Set *el) {
    if (el == 0) {
        destroy_set(el->next);
        free(el);
    }
}

/**
 * Create the head of an empty set
 */
void create_set(Set **set_new) {
    *set_new = (Set*) malloc( sizeof ( Set ) );
    (*set_new)->val = *((Value*) malloc( sizeof ( Value )));
    (*set_new)->next = 0;
    (*set_new)->length = 1;
    (*set_new)->head = 1;
}

/** 
 * Insert the key onto the end of the Set
 **/
void insert_el(Value key, Set **el) {
    if (*el == 0) {
        create_set (el);
        (*el)->val = key;
        (*el)->head = 0;
    } else if (*el != 0) {
        insert_el(key, &((*el)->next));
        (*el)->length += 1;
    }
}
/**
 * Run through the Set and print the values
 **/
void print_set(Set *el) {
    if (el != 0) {
        if (el->head) {
            printf("{");
        } 


        if (!el->head) {
            print_type(&el->val);

            if (el->next != 0) {
                printf(", ");
            }
        }

        print_set(el->next);
    } else {
        printf("}");
    }
}

/**
 * Create the union between the second set and first set
 * The first set is used to store the second
 **/
void set_union(Set* first, Set* second) {

    if (second->next != 0) {
        insert_el(second->next->val, &first);

        set_union(first, second->next);
    }
}

/**
 * Remove the elements from set 2 that match those of set 1
 */
void subtraction(Set* first, Set* second, int head) {

    if (first != 0 && second != 0) {

        // Check the current node for equality only on the head of the set, otherwise weird things may happen.
        if (head) {
            if (value_equality(&(first->val), &(second->val))) {
                if (first->next != 0) {
                    first = first->next;
                }
            }
        }

        if (first->next != 0) {

            // Copy the second set
            Set* sec_temp = (Set*) (malloc ( sizeof (Set) ));
            *sec_temp = *second;

            // Iterate through all of the second sets children
            do {

                // Check if the children are equal
                if (value_equality(&(first->next->val), &(sec_temp->val))) {
                    //Set* next = first->next;
                    // Change the current next pointer to the child's child
                    // Remove the child node from the list
                    first->next = first->next->next;

                    // Remove next from memory
                    //
                    // TODO: Pointers are all messed up, so maybe don't do this
                    //free(next);
                }

                if (sec_temp->next != 0) {
                    *sec_temp = *(sec_temp->next);
                } else {
                    sec_temp = 0;
                }
            } while (sec_temp != 0);
            // Subtract the next two elements
            subtraction(first->next, second, 0);
        }
    }
}

/**
 * Create a new set containing the values of first and second that intersect
 */
void intersection(Set* first, Set* second, Set* intersected, int head) {

    if (first != 0 && second != 0) {

        // Check the current node for equality only on the head of the set, otherwise weird things may happen.
        if (head) {
            if (value_equality(&(first->val), &(second->val))) {
                // Only checking the heads of the set, need children as well

                printf("\n");

                insert_el(first->val, &intersected);
            }
        }

        if (first->next != 0) {

            // Copy the second set
            Set* sec_temp = (Set*) (malloc ( sizeof (Set) ));
            *sec_temp = *second;

            // Iterate through all of the second sets children
            do {
                // Check if the children are equal
                if (value_equality(&(first->next->val), &(sec_temp->val))) {
                    // Insert the intersected value
                    insert_el(first->next->val, &intersected);
                }

                if (sec_temp->next != 0) {
                    *sec_temp = *(sec_temp->next);
                } else {
                    sec_temp = 0;
                }
            } while (sec_temp != 0);

            // Find intersection for next
            intersection(first->next, second, intersected, 0);
        }
    }
}
/**
 * Check if two sets are equal
 *
 * Two sets are equal if their elements are equal
 */
int set_contents_equality(Set* first, Set* second) {

    if (first != 0 && second != 0) {

        // If reached the end of the sets without error, they're the same
        if (first->next == 0 && second->next == 0) {
            return 1;
        }

        // Compare first set with the second
        if (value_equality(&(first->next->val), &(second->next->val))) {
            // Check the next elements in the set for equality
            return set_contents_equality(first->next, second->next);
        }
    }
        
    return 0;
}
