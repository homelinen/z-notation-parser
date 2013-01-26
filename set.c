/*
 * set.c
 *
 * A Set structure and it's operations.
 *
 * A set has a possibly infinite number of elements and elements can always be added to them
 */

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "all.h"

/**
 * Recurse through the Set freeing memory
 * from the end of the Set
 *
 * TODO: Make this not segfault
 **/
void destroy_set(Set *el) {
    if (el == 0) {
        destroy_set(el->next);
        free(el);
    }
}

/**
 * Create the head of an empty set
 *
 * set_new - The new memory address is stored here
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
        //If empty, at end of tree so create new element
        create_set (el);
        (*el)->val = key;
        (*el)->head = 0;
    } else if (*el != 0) {
        // If not at the end, add one to the length and go to next
        insert_el(key, &((*el)->next));
        (*el)->length += 1;
    }
}

/**
 * Print out all the values stored in the set
 *
 * el - The set to be printed
 *
 * Prints the set in the form { el1, el2, ... }
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
 *
 * Operation is done in-place on first, so first is overwritten
 *
 * first - The initial set and the set that second is appended to
 *
 * Returns the union in first, through pointers
 **/
void set_union(Set* first, Set* second) {

    //Assuming the head of the set is always non-zero

    if (second->next != 0) {
        //Recursively add the next val of second to first
        insert_el(second->next->val, &first);

        set_union(first, second->next);
    }
}

/**
 * Remove elements from first that appear in second.
 *
 * Find the elements in second that are equal to those in first and remove them from first.
 *
 * first - The initial set and modified set, as first is modified in place
 * second - The set to compare against
 * head - bool value to determine if set is head or not
 */
void subtraction(Set* first, Set* second, int head) {

    if (first != 0 && second != 0) {

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
            // Subtract all of the second elements from the next element
            subtraction(first->next, second, 0);
        }
    }
}

/**
 * Find the intersection of two sets: first and second.
 *
 * The intersection between two sets is a set of all elements in the two sets that can be found in BOTH sets.
 *
 * first - First Set
 * second - Second Set
 * intersected - address to put the new set
 * head - unneeded
 *
 * Returns all the elements in first that are the same as those in second. These matching elements are placed in intersected.
 */
void intersection(Set* first, Set* second, Set* intersected, int head) {

    if (first != 0 && second != 0) {

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
 * Two sets are equal if all their elements are equal
 *
 * first - The first set
 * second - The set to compare with first
 *
 * Returns a bool stating if the sets are equal or not
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
        
    //If all else failed, they must not be equal
    return 0;
}

