
#include <gtest/gtest.h>
extern "C" {
#include "log-manager.h"
#include "clogger.h"
#include "collections.h"
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
}

void colls_init(){
    srand(time(NULL));
    tshInitLogManger();
}

void colls_quit(){
    tshQuitLogManger();
}

struct t {
    float x;
    float y;
};

#define arr_size 15000

TEST(collections, LinkedList) { 
    colls_init();
    tshLinkedList *v = NULL;

    struct t arr[arr_size];
    for (int i = 0; i < arr_size; i++) {
        arr[i].x = ((float)rand() / RAND_MAX) * 100;
        arr[i].y = ((float)rand() / RAND_MAX) * 100;

        v = tshAppendLinkedList(v, (void*)&arr[i]);
        ASSERT_TRUE(v != NULL);
    }

    for (int i = 0; i < arr_size; i++) {
        struct t *a = (struct t *) tshGetLinkedList(v, i);
        EXPECT_EQ(arr[i].x, a->x);
        EXPECT_EQ(arr[i].y, a->y);
    }

    struct t tmp = {.x=1.55f, .y=3.14f};
    v = tshPreppendLinkedList(v, (void*)&tmp);

    struct t *a = (struct t *) tshGetLinkedList(v, 0);
    ASSERT_TRUE(a != NULL);
    EXPECT_EQ(tmp.x, a->x);
    EXPECT_EQ(tmp.y, a->y);
    v = tshRemoveLinkedList(v, 0);
    v = tshRemoveLinkedList(v, arr_size - 1);

    v = tshInsertLinkedList(v, &tmp, 4);
    a = (struct t*) tshGetLinkedList(v, 4);
    ASSERT_TRUE(a != NULL);
    EXPECT_EQ(tmp.x, a->x);
    EXPECT_EQ(tmp.y, a->y);
    

    tshFreeLinkedList(v);
    colls_quit();
}

TEST(collections, queue) { 
    colls_init();
    tshQueue *s = NULL;
    struct t arr[arr_size];
    for (int i = 0; i < arr_size; i++) {
        arr[i].x = ((float)rand() / RAND_MAX) * 100;
        arr[i].y = ((float)rand() / RAND_MAX) * 100;

        s = tshEnqueue(s, (void*)&arr[i]);
        ASSERT_TRUE(s != NULL);
    }

    size_t i = 0;
    while (!tshQueueIsEmpty(s)) {
        struct t *a;
        s = tshDequeue(s, (void**)&a);
        EXPECT_EQ(arr[i].x, a->x);
        EXPECT_EQ(arr[i].y, a->y);
        i += 1;
    }

    EXPECT_TRUE(s == NULL);

    tshFreeQueue(s);
    colls_quit();
}

TEST(collections, stack) { 
    colls_init();
     
    tshStack *s = NULL;
    struct t arr[arr_size];
    for (int i = 0; i < arr_size; i++) {
        arr[i].x = ((float)rand() / RAND_MAX) * 100;
        arr[i].y = ((float)rand() / RAND_MAX) * 100;

        s = tshPushStack(s, (void*)&arr[i]);
        ASSERT_TRUE(s != NULL);
    }

    size_t i = arr_size - 1;
    while (!tshStackIsEmpty(s)) {
        struct t *a;
        s = tshPopStack(s, (void**)&a);
        EXPECT_EQ(arr[i].x, a->x);
        EXPECT_EQ(arr[i].y, a->y);
        i -= 1;
    }

    EXPECT_TRUE(s == NULL);

    tshFreeStack(s);
    colls_quit();
}


TEST(collections, vector) { 
    colls_init();
    tshVec v;
    tshInitVec(&v); 
    ASSERT_TRUE(v._$$_ != NULL);

    struct t arr[arr_size];
    for (int i = 0; i < arr_size; i++) {
        arr[i].x = ((float)rand() / RAND_MAX) * 100;
        arr[i].y = ((float)rand() / RAND_MAX) * 100;

        tshAppendVec(&v, (void*)&arr[i]);
    }

    EXPECT_EQ(v.capacity, 16384);
    EXPECT_EQ(v.size, arr_size);

    for (int i = 0; i < arr_size; i++) {
        struct t *a = (struct t *)v._$$_[i];
        EXPECT_EQ(arr[i].x, a->x);
        EXPECT_EQ(arr[i].y, a->y);
    }

    tshRemoveVec(&v, arr_size - 1);
    EXPECT_EQ(v.size, arr_size - 1);

    struct t dummy = {.x=1.3f, .y=3.4f};
    tshInsertVec(&v, &dummy, 4);
    struct t *val = (struct t*) tshGetVec(v, 4);
    ASSERT_TRUE(val != NULL);
    EXPECT_EQ(val->x, 1.3f);
    EXPECT_EQ(val->y, 3.4f);
    
    tshFreeVec(v);
    colls_quit();
}

// void tshInitHashTable(tshHashTable *table);
// void tshAddHashTable(tshHashTable *table, char* key, void* value);
// void tshRemoveashTable(tshHashTable *table, char* key);
// void tshFreeHasTable(tshHashTable *table);
TEST(collections, Hahstables)  {
    colls_init();
    tshHashTable table;
    tshInitHashTable(&table);
    struct t stuff[arr_size];
    for (int i = 0; i < arr_size; i++) {
        stuff[i].x = ((float)rand() / RAND_MAX) * 100;
        stuff[i].y = ((float)rand() / RAND_MAX) * 100;
        char key[20];
        sprintf(key, "key-%d", i);
        tshAddHashTable(&table, key, (void*) &stuff[i]);
    }

    for (int i = 0; i < arr_size; i++) {
        char key[20];
        sprintf(key, "key-%d", i);
        struct t *val = (struct t*) tshHashTableGet(&table, key);
        ASSERT_TRUE(val != NULL);
        EXPECT_TRUE(tshHashTableHasKey(&table, key));
        EXPECT_EQ(val->x, stuff[i].x);
        EXPECT_EQ(val->y, stuff[i].y);
    }

    tshRemoveHashTable(&table, "key-3");
    EXPECT_TRUE(tshHashTableHasKey(&table, "key-3") == false);
    tshRemoveHashTable(&table, "key-22");
    EXPECT_TRUE(tshHashTableHasKey(&table, "key-22") == false);
    tshRemoveHashTable(&table, "key-5");
    EXPECT_TRUE(tshHashTableHasKey(&table, "key-5") == false);
    tshRemoveHashTable(&table, "key-16");
    EXPECT_TRUE(tshHashTableHasKey(&table, "key-16") == false);
    tshRemoveHashTable(&table, "key-9");
    EXPECT_TRUE(tshHashTableHasKey(&table, "key-9") == false);


    tshFreeHashTable(&table);

    colls_quit();
}

