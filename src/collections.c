
#include "collections.h"
#include "hash.h"
#include "clogger.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

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

/******************** HashTable *******************************/

void tshInitHashTable(tshHashTable *table) {
    table->$ = calloc(TSH_HASHTABLE_DEFAULT_SIZE, sizeof(struct __hash_table_elt));
    table->size = 0;
    table->capacity = TSH_HASHTABLE_DEFAULT_SIZE;
}

void __insert(struct __hash_table_elt *table, size_t index, char *key, void* value) {
    table[index].key = strdup(key); 
    table[index].value = value; 
}

void __add_entry(struct __hash_table_elt *table, char* key, void* value, size_t capacity) {
    i32 h = crc_32(key) % capacity;
    if (table[h].key == NULL) {
        __insert(table, h, key, value);
    } else {
        LOG_WARN("There is a collision at %d", h);
        size_t i = (h + 1) % capacity;
        while (table[i].key != NULL && i != h) i = (i + 1) % capacity; 
        __insert(table, i, key, value);
    }
}

void __null_realocate(tshHashTable *table, size_t new_capacity) {
    struct __hash_table_elt *tmp = calloc(new_capacity, sizeof(struct __hash_table_elt));
    
    for(size_t i = 0; i < table->capacity; i++) {
        if (table->$[i].key != NULL) {
            __add_entry(tmp, table->$[i].key, table->$[i].value, new_capacity);
            free(table->$[i].key);
        }
    }
    
    free(table->$);
    table->$ = tmp;
    table->capacity = new_capacity;
}

void __expandHT(tshHashTable *table) {
    __null_realocate(table, table->capacity * 2);
}

void tshAddHashTable(tshHashTable *table, char* key, void* value) {
    assert (table != NULL && key != NULL);

    if (table->size >= (table->capacity / 2)) {
        __expandHT(table);
    }

    __add_entry(table->$, key, value, table->capacity);
    table->size += 1;
}

void __shrinkHT(tshHashTable *table) {
    if (table->capacity / 4 > 0) {
        __null_realocate(table, table->capacity / 2);
    }
}

void __remove(struct __hash_table_elt* table, size_t index) {
    free(table[index].key);
    table[index].key = NULL;
    table[index].value = NULL;
}

void __remove_entry(struct __hash_table_elt* table, char* key, size_t capacity) {
    i32 h = crc_32(key) % capacity;

    if (table[h].key != NULL && strcmp(key, table[h].key) == 0) {
        __remove(table, h);
    } else {
        u32 i = (h + 1) % capacity;
        while ((table[i].key == NULL || strcmp(table[i].key, key) != 0) && h != i) i = (i + 1) % capacity;

        if (h != i) {
            __remove(table, i);
        } else { 
            LOG_WARN("key (%s) does not exist in table", key);
        }
    }
}

void tshRemoveHashTable(tshHashTable *table, char* key) {
    assert(table != NULL && key != NULL && table->size > 0);

    __remove_entry(table->$, key, table->capacity);
    table->size -= 1;

    if (table->size <= (table->capacity / 4)) {
        __shrinkHT(table);
    }
}

void* tshHashTableGet(tshHashTable *table, char* key) {
    if (key == NULL || table == NULL) {
        return NULL;
    }

    i32 h = crc_32(key) % table->capacity;
    if (table->$[h].key != NULL && strcmp(table->$[h].key, key) == 0) {
        return table->$[h].value;
    } else {
        u32 i = (h + 1) % table->capacity;
        while ((table->$[i].key == NULL || strcmp(table->$[i].key, key) != 0) && i != h) i = (i + 1) % table-> capacity;
        
        if (i == h) return NULL;

        return table->$[i].value;
    }
}

bool tshHashTableHasKey(tshHashTable *table, char* key) {
    if (key == NULL || table == NULL) {
        return NULL;
    }

    i32 h = crc_32(key) % table->capacity;
    if (table->$[h].key != NULL && strcmp(table->$[h].key, key) == 0) {
        return true;
    } else {
        u32 i = (h + 1) % table->capacity;
        while ((table->$[i].key == NULL || strcmp(table->$[i].key, key) != 0) && i != h) i = (i + 1) % table-> capacity;
        
        if (i == h) return false;

        return true;
    }
}

void tshHashTableKeys(tshHashTable *table, tshVec *vec) {
    assert(table != NULL && vec != NULL);

    tshInitVec(vec);
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->$[i].key != NULL) {
            // is it wise to not duplicate the key ??? time will tell us
            tshAppendVec(vec, (void*)table->$[i].key);
        }
    }
}

void tshHashTableValues(tshHashTable *table, tshVec *vec) {
    tshInitVec(vec);
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->$[i].key != NULL) {
            tshAppendVec(vec, table->$[i].value);
        }
    }
}

void tshFreeHashTable(tshHashTable *table) {
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->$[i].key != NULL) {
            free(table->$[i].key);
        }
    }
    free(table->$);
}

