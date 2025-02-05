#ifndef __COLLECTIONS_H__
#define __COLLECTIONS_H__

#include "utils.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct __linked_list {
    void *value;
    struct __linked_list *next;
} tshLinkedList;

tshLinkedList*  tshInitLinkedList(void *value);
tshLinkedList*  tshAppendLinkedList(tshLinkedList* list, void *value);
tshLinkedList*  tshPreppendLinkedList(tshLinkedList* list, void *value);
tshLinkedList*  tshInsertLinkedList(tshLinkedList* list, void *value, unsigned int index);
void*           tshGetLinkedList(tshLinkedList* list, unsigned int index);
tshLinkedList*  tshRemoveLinkedList(tshLinkedList* list, unsigned int index);
void            tshFreeLinkedList(tshLinkedList* list);
bool            tshLinkedListHasNext(tshLinkedList* list);
tshLinkedList*  tshLinkedListNext(tshLinkedList* list);

typedef struct __stack{
    void* value; 
    struct __stack *prev;
}tshStack;

tshStack* tshInitStack(void *value);
tshStack* tshPushStack(tshStack* stack, void *value);
tshStack* tshPopStack(tshStack* stack, void **value);
bool tshStackIsEmpty(tshStack* stack);
void tshFreeStack(tshStack* stack);

typedef struct __queue{
    void* value; 
    struct __queue *next;
}tshQueue;

tshQueue* tshInitQueue(void *value);
tshQueue* tshEnqueue(tshQueue* queue, void *value);
tshQueue* tshDequeue(tshQueue* queue, void **value);
bool tshQueueIsEmpty(tshQueue* queue);
void tshFreeQueue(tshQueue* queue);

typedef struct __vec {
    size_t capacity;
    size_t size;
    void **$;
} tshVec;
#define __TSH_VEC_DEFAULT_SIZE 32

void tshInitVec(tshVec* vec);
void tshAppendVec(tshVec *vec, void* value);
void tshPreppendVec(tshVec *vec, void* value);
void *tshGetVec(tshVec vec, unsigned int index);
void tshInsertVec(tshVec *vec, void *value, unsigned int index);
void tshRemoveVec(tshVec *vec, unsigned int index);
void tshFreeVec(tshVec vec);


enum tshHashTableType {
    TSH_STR_HASH_TABLE,
    TSH_U32_HASH_TABLE,
};

typedef struct __hash_table {
    struct {
        union {
            char* str_key;
            u32 u32_key;
        } key;
        void *value;
    } *$;
    size_t size;
    size_t capacity;
    tshHashTableType type;
} tshHashTable;

void tshInitHashTable(tshHashTable *map, tshHashTableType type);
void tshAddHashTable_str(tshHashTable *map, char* key, void* value);
void tshAddHashTable_u32(tshHashTable *map, u32 key, void* value);
void tshAddHashTable_str(tshHashTable *map);
void tshAddHashTable_u32(tshHashTable *map);
void tshFreeHasTable(tshHashTable *map);

#endif //__COLLECTIONS_H__
