
#include "../../include/tanoshi/linked-list.h"
#include <stdlib.h>

/********************* LINKED LIST *********************/

tshLinkedList* tshInitLinkedList(void *value) {
    tshLinkedList* ret = malloc(sizeof(tshLinkedList));

    ret->value = value;
    ret->next = NULL;

    return ret;
}

void tshAppendLinkedList(tshLinkedList* list, void *value) {
    tshLinkedList* tmp = list, *prev = NULL;

    while(tmp != NULL) {
        prev = tmp;
        tmp = tmp->next;
    }

    if (prev != NULL) {
        prev->next = tshInitLinkedList(value);
    }
}

tshLinkedList* tshPreppendLinkedList(tshLinkedList* list, void *value) {
    tshLinkedList* tmp = tshInitLinkedList(value);

    tmp->next = list;

    return tmp;
}

tshLinkedList* tshInsertLinkedList(tshLinkedList* list, void *value, size_t index) {
    if (index == 0) {
        return tshPreppendLinkedList(list, value);     
    }

    if (list == NULL) {
        return NULL;
    }

    tshLinkedList *tmp = list;
    size_t counter = 0;

    while(tmp->next != NULL && counter < index) {
        tmp = tmp->next;
        counter += 1;
    }

    if (counter == index) {
        tshLinkedList *d = tshInitLinkedList(value);
        d->next = tmp->next;
        tmp->next = d;
    }

    return list;
}

void *tshGetLinkedList(tshLinkedList* list, size_t index) {
    if (list == NULL) {
        return NULL;
    }

    if (index == 0) {
        return list->value; 
    }

    tshLinkedList *tmp = list;
    size_t counter = 0;

    while(tmp->next != NULL && counter < index) {
        tmp = tmp->next; 
        counter += 1;
    }

    if (counter == index) {
        return tmp->value;
    }

    return NULL;
}

tshLinkedList*  tshDeleteLinkedList(tshLinkedList* list, size_t index) {
    tshLinkedList *tmp = list, *prev = NULL;
    if (list == NULL) {
        return NULL;
    }

    if (index == 0) {
        tmp = list->next;
        free(list);
        return tmp; 
    }

    size_t counter = 0;
    while(tmp->next != NULL && counter < index) {
        prev = tmp;
        tmp = tmp->next;
        counter += 1;
    }

    if (counter == index) {
        prev->next = tmp->next;
        free(tmp);
    }

    return list;
}

void tshFreeLinkedList(tshLinkedList* list) {
    tshLinkedList *tmp = list, *prev = NULL;

    while(tmp != NULL) {
        prev = tmp;
        tmp = tmp->next;
        free(prev);
    }
}

/******************** STACK *******************************/

