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
    // Check the set points somewhere
    if (el != 0 && el != NULL) {
        if (el->next != 0) {
            destroy_set(el->next);
        } 

        if (!el->head) {
            destroy_value(&el->val);
        }
        /*free(el);*/
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
 */
void subtraction(Set* first, Set* second) {

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
            subtraction(first->next, second);
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
 *
 * Returns all the elements in first that are the same as those in second. These matching elements are placed in intersected.
 */
void intersection(Set* first, Set* second, Set* intersected) {

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
            intersection(first->next, second, intersected);
        }
    }
}

/**
 * Check if an element exists in the set
 */
int set_membership(Value element, Set set) {

    Set* set_temp = set.next;
    int found = 0;

    while (set_temp && !found) {
        if (value_equality(&element, &set_temp->val)) {
            found =  1;
        } else {
            set_membership(element, *set.next);
        }

        set_temp = set_temp->next;
    }

    return found;
}

/**
 * Check if two sets are equal
 *
 * Two sets are equal if all their elements are equal
 *
 * DOES NOT CHECK LENGTH OF SETS (Check externally)
 * TODO: Make a set comparison method that checks length and contents
 *
 
 * If the second set is longer than the first, then those extra values go unchecked
 * If something in the second set isn't in the first, the test could still pass. Bad.
 *
 * first - The first set
 * second - The set to compare with first
 *
 * Returns a bool stating if the sets are equal or not
 */
int set_contents_equality(Set* first, Set* second) {

    int is_equal = 0;

    if (first != 0 && second != 0) {

        if (first->next != 0) {

            // Copy the second set
            Set* sec_temp = (Set*) (malloc ( sizeof (Set) ));
            *sec_temp = *second;

            // Iterate through all of the second sets children
            do {
                // Check if the children are equal
                if (value_equality(&(first->next->val), &(sec_temp->val))) {
                    is_equal = 1;
                }

                if (sec_temp->next != 0) {
                    *sec_temp = *(sec_temp->next);
                } else {
                    sec_temp = 0;
                }
            } while (!is_equal && sec_temp != 0);

            if (is_equal) {
                // Check the next in the sequence
                return set_contents_equality(first->next, second);
            } else {
                //if first isn't equal to any second, not equal
                return 0;
            }

            free(sec_temp);
        } else {
            // If reached the end of the sets without error, they're the same
            return 1;
        }
    }
        
    // Otherwise, must be false
    return 0;
}

