
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
    vec->_$$_ = realloc(vec->_$$_, vec->capacity * sizeof(void*));
}

void __shrinkVec(tshVec *vec) {
    if (vec->capacity / 2 > 0) {
        vec->capacity /= 2;
        vec->_$$_ = realloc(vec->_$$_, vec->capacity * sizeof(void*));
    }
}

void tshInitVec(tshVec* vec) {
    vec->capacity = __TSH_VEC_DEFAULT_SIZE;
    vec->size = 0;
    vec->_$$_ = malloc(sizeof(void*) * __TSH_VEC_DEFAULT_SIZE);
}

void tshAppendVec(tshVec *vec, void* value) {
    if (vec->size >= vec->capacity) {
        __expandVec(vec);
    }
    
    vec->_$$_[vec->size++] = value;
}

void tshPreppendVec(tshVec *vec, void* value) {
    tshInsertVec(vec, value, 0);
}

void* tshGetVec(tshVec vec, unsigned int index) {
    if (vec.size <= index) {
        return NULL;
    }
    return vec._$$_[index];
}

void tshInsertVec(tshVec *vec, void *value, unsigned int index) {
    if (vec->size == index) {
        tshAppendVec(vec, value);
    } else if (vec->size > index) {
        if (vec->size >= vec->capacity) {
            __expandVec(vec);
        }
        for (size_t i = vec->size; i > index; i--) {
            vec->_$$_[i - 1] = vec->_$$_[i];
        }
        vec->size += 1;
        vec->_$$_[index] = value;
    }
}

void tshRemoveVec(tshVec *vec, unsigned int index) {
    for (size_t i = index; i < vec->size - 1; i++) {
        vec->_$$_[i] = vec->_$$_[i + 1];
    }

    if (index < vec->size) {
        vec->_$$_[--vec->size] = NULL;
    }

    if (vec->size < (vec->capacity / 2)) {
        __shrinkVec(vec);
    }
}

void tshFreeVec(tshVec vec) {
    free(vec._$$_);
}

/******************** HashTable *******************************/

void tshInitHashTable(tshHashTable *table) {
    table->_$$_ = calloc(TSH_HASHTABLE_DEFAULT_SIZE, sizeof(struct __hash_table_elt));
    table->size = 0;
    table->capacity = TSH_HASHTABLE_DEFAULT_SIZE;
}

void __insert(struct __hash_table_elt *table, size_t index, char *key, void* value, size_t prob_dist) {
    table[index].key = strdup(key); 
    table[index].value = value; 
    table[index].prob_distance = prob_dist; 
}

void __add_entry(struct __hash_table_elt *table, char* key, void* value, size_t capacity) {
    u32 h = crc_32(key) % capacity;
    size_t prob_dist = 0;

    if (table[h].key == NULL) {
        __insert(table, h, key, value, prob_dist);
    } else {
        size_t i = (h + 1) % capacity;
        prob_dist += 1;

        while (
            table[i].key != NULL && 
            strcmp(table[i].key, key) != 0 &&
            table[i].prob_distance != 0 && 
            i != h
        ) { 
            i = (i + 1) % capacity; 
            prob_dist += 1;
        } 

        if (table[i].key == NULL || strcmp(table[i].key, key) == 0) {
            if (table[i].key != NULL) free(table[i].key);
            __insert(table, i, key, value, prob_dist);
        } else if (table[i].prob_distance == 0) {
            struct __hash_table_elt to_insert;
            to_insert.key = strdup(key);
            to_insert.value = value;
            to_insert.prob_distance = prob_dist;
            struct __hash_table_elt to_move;

            while (table[i].key != NULL) {
                to_move.key = table[i].key;
                to_move.value = table[i].value;
                to_move.prob_distance = table[i].prob_distance;
                __insert(table, i, to_insert.key, to_insert.value, to_insert.prob_distance);
                free(to_insert.key);
                to_insert.key = to_move.key;
                to_insert.value = to_move.value;
                to_insert.prob_distance = to_move.prob_distance + 1;
                i = (i + 1) % capacity;
            }
            __insert(table, i, to_insert.key, to_insert.value, to_insert.prob_distance);
            free(to_insert.key);
        } else {
            LOG_ERROR("Unreachable code: Hashtable is full");
        }
    }
}

void __null_realocate(tshHashTable *table, size_t new_capacity) {
    struct __hash_table_elt *tmp = calloc(new_capacity, sizeof(struct __hash_table_elt));
    
    for(size_t i = 0; i < table->capacity; i++) {
        if (table->_$$_[i].key != NULL) {
            __add_entry(tmp, table->_$$_[i].key, table->_$$_[i].value, new_capacity);
            free(table->_$$_[i].key);
        }
    }
    
    free(table->_$$_);
    table->_$$_ = tmp;
    table->capacity = new_capacity;
}

void __expandHT(tshHashTable *table) {
    __null_realocate(table, table->capacity * 2);
}

void tshAddHashTable(tshHashTable *table, char* key, void* value) {
    assert (table != NULL && key != NULL);

    if (table->size == table->capacity) {
        __expandHT(table);
    }

    __add_entry(table->_$$_, key, value, table->capacity);
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
    table[index].prob_distance = 0;
}

void __shift_back(struct __hash_table_elt* table, size_t index, size_t capacity) {
    size_t i = index;
    size_t j = (i + 1) % capacity;

    while (table[j].key != NULL && table[j].prob_distance != 0) {
        __insert(table, i, table[j].key, table[j].value, table[j].prob_distance - 1);
        __remove(table, j);
        i = j;
        j = (j + 1) % capacity;
    }
    __remove(table, i);
}

void __remove_entry(struct __hash_table_elt* table, char* key, size_t capacity) {
    u32 h = crc_32(key) % capacity;

    if (table[h].key != NULL && strcmp(key, table[h].key) == 0) {
        __remove(table, h);
        __shift_back(table, h, capacity);
    } else {
        u32 i = (h + 1) % capacity;
        // Factor fetching index of element
        while (
            table[i].key != NULL && 
            strcmp(table[i].key, key) != 0 &&
            table[i].prob_distance != 0 && 
            i != h
        ) {
            i = (i + 1) % capacity;
        }

        if (strcmp(table[i].key, key) == 0) {
            __remove(table, i);
            __shift_back(table, i, capacity);
        } else { 
            LOG_WARN("key (%s) does not exist in table", key);
        }
    }
}

void tshRemoveHashTable(tshHashTable *table, char* key) {
    assert(table != NULL && key != NULL && table->size > 0);

    __remove_entry(table->_$$_, key, table->capacity);
    table->size -= 1;

    if (table->size <= (table->capacity / 4)) {
        __shrinkHT(table);
    }
}

void* tshHashTableGet(tshHashTable *table, char* key) {
    if (key == NULL || table == NULL) {
        return NULL;
    }

    u32 h = crc_32(key) % table->capacity;
    if (table->_$$_[h].key != NULL && strcmp(table->_$$_[h].key, key) == 0) {
        return table->_$$_[h].value;
    } else {
        u32 i = (h + 1) % table->capacity;
        while (
            table->_$$_[i].key != NULL &&
            table->_$$_[i].prob_distance != 0 &&
            strcmp(table->_$$_[i].key, key) != 0 &&
            i != h
        ) {
            i = (i + 1) % table->capacity;
        }
        
        if (i == h || table->_$$_[i].key == NULL || table->_$$_[i].prob_distance == 0) {
            return NULL;
        }

        return table->_$$_[i].value;
    }
}

bool tshHashTableHasKey(tshHashTable *table, char* key) {
    if (key == NULL || table == NULL) {
        return NULL;
    }

    u32 h = crc_32(key) % table->capacity;
    if (table->_$$_[h].key != NULL && strcmp(table->_$$_[h].key, key) == 0) {
        return true;
    } else {
        u32 i = (h + 1) % table->capacity;
        while (
            table->_$$_[i].key != NULL &&
            table->_$$_[i].prob_distance != 0 &&
            strcmp(table->_$$_[i].key, key) != 0 && 
            i != h
        ) { 
            i = (i + 1) % table-> capacity; 
        }
        
        if (i == h || table->_$$_[i].key == NULL || table->_$$_[i].prob_distance == 0) return false;

        return true;
    }
}

// depricated for the time being 
void tshHashTableKeys(tshHashTable *table, tshVec *vec) {
    assert(table != NULL && vec != NULL);

    tshInitVec(vec);
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->_$$_[i].key != NULL) {
            // is it wise to not duplicate the key ??? time will tell us
            tshAppendVec(vec, (void*)table->_$$_[i].key);
        }
    }
}

// depricated for the time being 
void tshHashTableValues(tshHashTable *table, tshVec *vec) {
    tshInitVec(vec);
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->_$$_[i].key != NULL) {
            tshAppendVec(vec, table->_$$_[i].value);
        }
    }
}


void tshPrintHashTable(tshHashTable table) {
    for (size_t i = 0; i < table.capacity; i +=1) {
        LOG_INFO("HT[%d] = {key: '%s', value: %p, prob_distance: %d}",i,  table._$$_[i].key, table._$$_[i].value, table._$$_[i].prob_distance);
    }
}

void tshFreeHashTable(tshHashTable *table) {
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->_$$_[i].key != NULL) {
            free(table->_$$_[i].key);
        }
    }
    free(table->_$$_);
}

