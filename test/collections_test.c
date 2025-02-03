#include "tau/tau.h"
#include "../include/tanoshi/log-manager.h"
#include "../include/clogger/clogger.h"
#include "../include/tanoshi/collections.h"
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

#define arr_size 35

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
    REQUIRE(v.$ != NULL);

    struct t arr[arr_size];
    for (int i = 0; i < arr_size; i++) {
        arr[i].x = ((float)rand() / RAND_MAX) * 100;
        arr[i].y = ((float)rand() / RAND_MAX) * 100;

        tshAppendVec(&v, (void*)&arr[i]);
    }

    CHECK_EQ(v.capacity, 64);
    CHECK_EQ(v.size, arr_size);

    for (int i = 0; i < arr_size; i++) {
        struct t *a = (struct t *)v.$[i];
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
