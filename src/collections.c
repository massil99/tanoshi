
#include "../include/tanoshi/collections.h"
#include "../include/clogger/clogger.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/********************* LINKED LIST *********************/

tshLinkedList* tshInitLinkedList(void *value) {
    tshLinkedList* ret = malloc(sizeof(tshLinkedList));

    if (ret == NULL) {
        return NULL;
    }

    ret->value = value;
    ret->next = NULL;

    return ret;
}

tshLinkedList* tshAppendLinkedList(tshLinkedList* list, void *value) {
    tshLinkedList* tmp = list, *prev = NULL;
    
    if (list == NULL) {
        return tshInitLinkedList(value);
    } 

    while(tmp != NULL) {
        prev = tmp;
        tmp = tmp->next;
    }

    if (prev != NULL) {
        prev->next = tshInitLinkedList(value);
    }

    return list;
}

tshLinkedList* tshPreppendLinkedList(tshLinkedList* list, void *value) {
    tshLinkedList* tmp = tshInitLinkedList(value);

    tmp->next = list;

    return tmp;
}

tshLinkedList* tshInsertLinkedList(tshLinkedList* list, void *value, unsigned int index) {
    if (index == 0) {
        return tshPreppendLinkedList(list, value);     
    }

    if (list == NULL) {
        return NULL;
    }

    tshLinkedList *tmp = list;
    size_t counter = 0;

    while(tmp->next != NULL && counter < index - 1) {
        tmp = tmp->next;
        counter += 1;
    }

    if (counter == index - 1) {
        tshLinkedList *d = tshInitLinkedList(value);
        d->next = tmp->next;
        tmp->next = d;
    }

    return list;
}

void *tshGetLinkedList(tshLinkedList* list, unsigned int index) {
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

tshLinkedList* tshRemoveLinkedList(tshLinkedList* list, unsigned int index) {
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

bool tshLinkedListHasNext(tshLinkedList* list) {
    if (list == NULL) {
        return false;
    } else {
        return list->next != NULL;
    }
}

tshLinkedList*  tshLinkedListNext(tshLinkedList* list) {
    if (list == NULL) {
        return NULL;
    } else {
        return list->next;
    }
}

/******************** STACK *******************************/

tshStack* tshInitStack(void *value) {
    tshStack* ret = malloc(sizeof(tshStack));

    if (ret == NULL) {
        return NULL;
    }
    ret->value = value;
    ret->prev = NULL;

    return ret;
}

tshStack* tshPushStack(tshStack* stack, void *value) {
    tshStack *tmp = tshInitStack(value);
    tmp->prev = stack;
    return tmp;
}

tshStack* tshPopStack(tshStack* stack, void **value) {
    if (stack == NULL) {
        *value = NULL;
        return NULL;
    }

    if (value != NULL) {
        *value = stack->value;
    }

    tshStack *ret = stack->prev;
    free(stack);
    return ret;
}

bool tshStackIsEmpty(tshStack* stack) {
    return stack == NULL;
}

void tshFreeStack(tshStack* stack) {
    tshStack* tmp = stack;
    while(tmp != NULL) {
        tshStack* t = tmp->prev;
        free(tmp);
        tmp = t;
    } 
}

/******************** QUEUE *******************************/

tshQueue* tshInitQueue(void *value) {
    tshQueue* ret = malloc(sizeof(tshQueue));

    if (ret == NULL) {
        return NULL;
    }

    ret->value = value;
    ret->next = NULL;

    return ret;
}

tshQueue* tshEnqueue(tshQueue* queue, void *value) {
    tshQueue *tmp = queue;

    if (queue == NULL) {
        return tshInitQueue(value);
    }

    while (tmp->next != NULL) tmp = tmp->next; 

    tmp->next = tshInitQueue(value);

    return queue;
}

tshQueue* tshDequeue(tshQueue* queue, void **value) {
    if (queue == NULL) {
        *value = NULL;
        return NULL;
    }

    if (value != NULL) {
        *value = queue->value;
    } 

    tshQueue * ret = queue->next;
    free(queue);
    return ret;
}

bool tshQueueIsEmpty(tshQueue* queue) {
    return queue == NULL;
}

void tshFreeQueue(tshQueue* queue) {
    tshQueue* tmp = queue;
    while (tmp != NULL) {
        printf("No common sene left in these empty heads\n");
        tshQueue* t = tmp->next;
        free(tmp);
        tmp = t;
    }
}

/******************** VECTOR *******************************/

void __expandVec(tshVec *vec) {
    vec->capacity *= 2;
    vec->$ = realloc(vec->$, vec->capacity * sizeof(void*));
}

void __shrinkVec(tshVec *vec) {
    if (vec->capacity / 2 > 0) {
        vec->capacity /= 2;
        vec->$ = realloc(vec->$, vec->capacity * sizeof(void*));
    }
}

void tshInitVec(tshVec* vec) {
    vec->capacity = __TSH_VEC_DEFAULT_SIZE;
    vec->size = 0;
    vec->$ = malloc(sizeof(void*) * __TSH_VEC_DEFAULT_SIZE);
}

void tshAppendVec(tshVec *vec, void* value) {
    if (vec->size >= vec->capacity) {
        __expandVec(vec);
    }
    
    vec->$[vec->size++] = value;
}

void tshPreppendVec(tshVec *vec, void* value) {
    tshInsertVec(vec, value, 0);
}

void* tshGetVec(tshVec vec, unsigned int index) {
    if (vec.size <= index) {
        return NULL;
    }
    return vec.$[index];
}

void tshInsertVec(tshVec *vec, void *value, unsigned int index) {
    if (vec->size == index) {
        tshAppendVec(vec, value);
    } else if (vec->size > index) {
        if (vec->size >= vec->capacity) {
            __expandVec(vec);
        }
        for (size_t i = vec->size; i > index; i--) {
            vec->$[i - 1] = vec->$[i];
        }
        vec->size += 1;
        vec->$[index] = value;
    }
}

void tshRemoveVec(tshVec *vec, unsigned int index) {
    for (size_t i = index; i < vec->size - 1; i++) {
        vec->$[i] = vec->$[i + 1];
    }

    if (index < vec->size) {
        vec->$[--vec->size] = NULL;
    }

    if (vec->size < (vec->capacity / 2)) {
        __shrinkVec(vec);
    }
}

void tshFreeVec(tshVec vec) {
    free(vec.$);
}

