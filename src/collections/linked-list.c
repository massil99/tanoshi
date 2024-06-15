
#include "../../include/tanoshi/linked-list.h"
#include <stdlib.h>

/********************* LINKED LIST *********************/
/**
 * initailizes the linked list with the given value
 * @param {tshLinkedlist*} list the linked list to initalize 
 * @param {tshDataUnion} value first value of the list
 */
void tshInitLinkedList(tshLinkedlist* list, tshDataUnion value) {
    list->value = value;
    list->next = NULL;
}

/**
 * adds a value to the linked list 
 * @param {tshLinkedlist*} list ths list to add the value to
 * @param {tshDataUnion} list the value to add to the linked list 
 */ 
void tshLinedListAdd(tshLinkedlist* list, tshDataUnion value) {
    tshLinkedlist *curr = list;

    while (curr->next != NULL) {
        curr = curr->next;
    }

    curr->next = (tshLinkedlist*)malloc(sizeof(tshLinkedlist));
    curr->next->value = value;
    curr->next->next = NULL;
}


/**
 * frees all values for the linked list
 * @param {tshLinkedlist*} list the linked lsit to free
 */  
void tshFreeLinkedList(tshLinkedlist* list) {
    tshLinkedlist *prev, *curr = list->next;
    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
        free(prev);
    }
}

/******************** STACK *******************************/

void tshInitStack() {
}


