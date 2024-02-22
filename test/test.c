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
    u32* a = (u32*)__tshStackAllocate(&st, 4); 
    *a = 69;
    CHECK(*a == 69);

    uintptr_t a1 = __tshStackAllocate(&st, 4); 
    u32* a2 = (u32*)__tshStackAllocate(&st, 4); 
    *a2 = 420;
    CHECK(*a2 == 420);

    uintptr_t addr = tshStackFree(&st); 
    CHECK(addr == a1);
    addr = tshStackFree(&st); 
    CHECK(addr == (uintptr_t)a);
    addr = tshStackFree(&st); 
    CHECK(addr == 0);
    CHECK(st.stackBaseAddress == st.stackHeadAddress);

    free((void*)start);
}


TAU_MAIN()
TEST(memory, stack_allocator) { 
    test_stack_allocator();
}

