#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

#include "../../include/tanoshi/utils.h"

typedef struct __linked_list {
    tshDataUnion value;
    struct __linked_list *next;
} tshLinkedlist;

void tshInitLinkedList(tshLinkedlist* list, tshDataUnion value);
void tshLinedListAdd(tshLinkedlist* list, tshDataUnion value);
void tshFreeLinkedList(tshLinkedlist* list);

typedef struct __stack{
    tshDataUnion value; 
    struct __stack *prev;
}tshStack;

void tshInitStack(){
}

#endif //__LINKED_LIST_H
