#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

#include "../../include/tanoshi/utils.h"
#include <stdlib.h>

typedef struct __linked_list {
    void *value;
    struct __linked_list *next;
} tshLinkedList;

tshLinkedList*  tshInitLinkedList(void *value);
void            tshAppendLinkedList(tshLinkedList* list, void *value);
tshLinkedList*  tshPreppendLinkedList(tshLinkedList* list, void *value);
tshLinkedList*  tshInsertLinkedList(tshLinkedList* list, void *value, size_t index);
void*           tshGetLinkedList(tshLinkedList* list, size_t index);
tshLinkedList*  tshDeleteLinkedList(tshLinkedList* list, size_t index);
void            tshFreeLinkedList(tshLinkedList* list);

typedef struct __stack{
    tshDataUnion value; 
    struct __stack *prev;
}tshStack;

#endif //__LINKED_LIST_H
