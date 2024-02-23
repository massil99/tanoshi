#include "tau/tau.h"
#include "../include/tanoshi/memory-allocator.h"
#include "../include/clogger/clogger.h"
#include "../include/tanoshi/custom_types.h"
#include <stdint.h>

void test_stack_allocator() {
    tshStackAllocator st;
    uintptr_t start = (uintptr_t)malloc(100);
    size_t size = 70;

    tshInitStackAllocator(&st, start,  size);

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


TAU_MAIN()
TEST(memory, stack_allocator) { 
    test_stack_allocator();
}

