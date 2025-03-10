#include "tau/tau.h"
#include "log-manager.h"
#include "clogger.h"
#include "collections.h"
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

void init(){
    srand(time(NULL));
    tshInitLogManger();
}
void quit(){
    tshQuitLogManger();
}

struct t {
    float x;
    float y;
};

TAU_MAIN()

#define arr_size 15000

TEST(collections, LinkedList) { 
    init();
    tshLinkedList *v = NULL;

    struct t arr[arr_size];
    for (int i = 0; i < arr_size; i++) {
        arr[i].x = ((float)rand() / RAND_MAX) * 100;
        arr[i].y = ((float)rand() / RAND_MAX) * 100;

        v = tshAppendLinkedList(v, (void*)&arr[i]);
        REQUIRE(v != NULL);
    }

    for (int i = 0; i < arr_size; i++) {
        struct t *a = (struct t *) tshGetLinkedList(v, i);
        CHECK_EQ(arr[i].x, a->x);
        CHECK_EQ(arr[i].y, a->y);
    }

    struct t tmp = {.x=1.55f, .y=3.14f};
    v = tshPreppendLinkedList(v, (void*)&tmp);

    struct t *a = (struct t *) tshGetLinkedList(v, 0);
    REQUIRE(a != NULL);
    CHECK_EQ(tmp.x, a->x);
    CHECK_EQ(tmp.y, a->y);
    v = tshRemoveLinkedList(v, 0);
    v = tshRemoveLinkedList(v, arr_size - 1);

    v = tshInsertLinkedList(v, &tmp, 4);
    a = (struct t*) tshGetLinkedList(v, 4);
    REQUIRE(a != NULL);
    CHECK_EQ(tmp.x, a->x);
    CHECK_EQ(tmp.y, a->y);
    

    tshFreeLinkedList(v);
    quit();
}

TEST(collections, queue) { 
    init();
    tshQueue *s = NULL;
    struct t arr[arr_size];
    for (int i = 0; i < arr_size; i++) {
        arr[i].x = ((float)rand() / RAND_MAX) * 100;
        arr[i].y = ((float)rand() / RAND_MAX) * 100;

        s = tshEnqueue(s, (void*)&arr[i]);
        REQUIRE(s != NULL);
    }

    size_t i = 0;
    while (!tshQueueIsEmpty(s)) {
        struct t *a;
        s = tshDequeue(s, (void**)&a);
        CHECK_EQ(arr[i].x, a->x);
        CHECK_EQ(arr[i].y, a->y);
        i += 1;
    }

    CHECK(s == NULL);

    tshFreeQueue(s);
    quit();
}

TEST(collections, stack) { 
    init();
     
    tshStack *s = NULL;
    struct t arr[arr_size];
    for (int i = 0; i < arr_size; i++) {
        arr[i].x = ((float)rand() / RAND_MAX) * 100;
        arr[i].y = ((float)rand() / RAND_MAX) * 100;

        s = tshPushStack(s, (void*)&arr[i]);
        REQUIRE(s != NULL);
    }

    size_t i = arr_size - 1;
    while (!tshStackIsEmpty(s)) {
        struct t *a;
        s = tshPopStack(s, (void**)&a);
        CHECK_EQ(arr[i].x, a->x);
        CHECK_EQ(arr[i].y, a->y);
        i -= 1;
    }

    CHECK(s == NULL);

    tshFreeStack(s);
    quit();
}


TEST(collections, vector) { 
    init();
    tshVec v;
    tshInitVec(&v); 
    REQUIRE(v._$$_ != NULL);

    struct t arr[arr_size];
    for (int i = 0; i < arr_size; i++) {
        arr[i].x = ((float)rand() / RAND_MAX) * 100;
        arr[i].y = ((float)rand() / RAND_MAX) * 100;

        tshAppendVec(&v, (void*)&arr[i]);
    }

    CHECK_EQ(v.capacity, 16384);
    CHECK_EQ(v.size, arr_size);

    for (int i = 0; i < arr_size; i++) {
        struct t *a = (struct t *)v._$$_[i];
        CHECK_EQ(arr[i].x, a->x);
        CHECK_EQ(arr[i].y, a->y);
    }

    tshRemoveVec(&v, arr_size - 1);
    CHECK_EQ(v.size, arr_size - 1);

    tshInsertVec(&v, &((struct t){.x=1.3f, 3.4f}), 4);
    struct t *val = (struct t*) tshGetVec(v, 4);
    REQUIRE(val != NULL);
    CHECK_EQ(val->x, 1.3f);
    CHECK_EQ(val->y, 3.4f);
    
    tshFreeVec(v);
    quit();
}

// void tshInitHashTable(tshHashTable *table);
// void tshAddHashTable(tshHashTable *table, char* key, void* value);
// void tshRemoveashTable(tshHashTable *table, char* key);
// void tshFreeHasTable(tshHashTable *table);
TEST(collections, Hahstables)  {
    init();
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
        struct t *val = tshHashTableGet(&table, key);
        REQUIRE(val != NULL);
        CHECK(tshHashTableHasKey(&table, key));
        CHECK_EQ(val->x, stuff[i].x);
        CHECK_EQ(val->y, stuff[i].y);
    }

    tshRemoveHashTable(&table, "key-3");
    CHECK(tshHashTableHasKey(&table, "key-3") == false);
    tshRemoveHashTable(&table, "key-22");
    CHECK(tshHashTableHasKey(&table, "key-22") == false);
    tshRemoveHashTable(&table, "key-5");
    CHECK(tshHashTableHasKey(&table, "key-5") == false);
    tshRemoveHashTable(&table, "key-16");
    CHECK(tshHashTableHasKey(&table, "key-16") == false);
    tshRemoveHashTable(&table, "key-9");
    CHECK(tshHashTableHasKey(&table, "key-9") == false);


    tshFreeHashTable(&table);

    quit();
}

