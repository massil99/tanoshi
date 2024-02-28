#include "tau/tau.h"
#include "../include/tanoshi/memory-allocator.h"
#include "../include/clogger/clogger.h"
#include <stdint.h>
#include <stdio.h>


TEST(memory, forward_stack_allocator) { 
    uintptr_t start = (uintptr_t)malloc(100);
    size_t size = 70;
    bool dir = true;
    tshStackAllocator st;
    tshInitStackAllocator(&st, start,  size, dir);

    CHECK_EQ(st.stackBaseAddress, start);
    CHECK_EQ(st.stackBaseAddress, st.stackHeadAddress);
    u16* addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 1;
    CHECK_EQ(*addr, 1);
    addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 2;
    CHECK_EQ(*addr, 2);
    addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 3;
    CHECK_EQ(*addr, 3);
    addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 4;
    CHECK_EQ(*addr, 4);

    tshStackFree(&st);
    CHECK_EQ(*(u16*)(st.stackHeadAddress), 4);
    tshStackFree(&st);
    CHECK_EQ(*(u16*)(st.stackHeadAddress), 3);
    tshStackFree(&st);
    CHECK_EQ(*(u16*)(st.stackHeadAddress), 2);
    tshStackFree(&st);
    CHECK_EQ(*(u16*)(st.stackHeadAddress), 1);
    tshStackFree(&st);
    CHECK_EQ(*(u16*)(st.stackHeadAddress), 1);

    addr = tshStackAllocate(&st, sizeof(u16)*30, u16*);
    *addr = 0x69;
    CHECK_EQ(*addr, 0x69);
    addr = tshStackAllocate(&st, sizeof(u16), u16*);
    CHECK_EQ((uintptr_t)addr, 0);

    free((void*)start);
}

TEST(memory, backward_stack_allocator) { 
    uintptr_t start = (uintptr_t)malloc(100);
    size_t size = 70;
    tshStackAllocator st;
    bool dir = false;

    tshInitStackAllocator(&st, start + size,  size, dir);

    CHECK_EQ(st.stackBaseAddress, start + size);
    CHECK_EQ(st.stackBaseAddress, st.stackHeadAddress);
    u16* addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 1;
    CHECK_EQ(*addr, 1);
    addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 2;
    CHECK_EQ(*addr, 2);
    addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 3;
    CHECK_EQ(*addr, 3);
    addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 4;
    CHECK_EQ(*addr, 4);

    tshStackFree(&st);
    CHECK_EQ(*(u16*)(st.stackHeadAddress -1), 4);
    tshStackFree(&st);
    CHECK_EQ(*(u16*)(st.stackHeadAddress -1), 3);
    tshStackFree(&st);
    CHECK_EQ(*(u16*)(st.stackHeadAddress -1), 2);
    tshStackFree(&st);
    CHECK_EQ(*(u16*)(st.stackHeadAddress -1), 1);
    tshStackFree(&st);
    CHECK_EQ(*(u16*)(st.stackHeadAddress -1), 1);

    addr = tshStackAllocate(&st, sizeof(u16)*30, u16*);
    *addr = 0x69;
    CHECK_EQ(*addr, 0x69);
    addr = tshStackAllocate(&st, sizeof(u16), u16*);
    CHECK_EQ((uintptr_t)addr, 0);

    free((void*)start);
}

TEST(memory, linear_allocator) { 
    tshLinearAllocator li;
    uintptr_t start = (uintptr_t)malloc(100);
    size_t size = 70;

    tshInitLinearAllocator(&li, start, size);
    u16* addr = tshLinearAllocate(&li, sizeof(u16)*35, u16*);
    for(u16 i = 0; i < 35; i++){
        addr[i] = i;
        CHECK_EQ(addr[i], i); 
    }
    CHECK_EQ((uintptr_t)addr, li.baseAddress); 

    addr = tshLinearAllocate(&li, sizeof(u16), u16*);
    CHECK_EQ((uintptr_t)addr, 0); 

    addr = tshLinearAllocate(&li, 1, u16*);
    CHECK_EQ((uintptr_t)addr, 0); 

    free((void*) start);
}


TAU_MAIN()

