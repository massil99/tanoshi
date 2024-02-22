#include "tau/tau.h"
#include "../include/tanoshi/memory-allocator.h"
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
    CHECK(st.stackBaseAddress != st.stackHeadAddress);

    uintptr_t a1 = __tshStackAllocate(&st, 4); 
    uintptr_t a2 = __tshStackAllocate(&st, 4); 

    uintptr_t addr = tshStackFree(&st); 
    CHECK(addr = a2);
    addr = tshStackFree(&st); 
    CHECK(addr = a1);
    addr = tshStackFree(&st); 
    CHECK(addr = st.stackBaseAddress);
    CHECK(st.stackBaseAddress == st.stackHeadAddress);

    free((void*)start);
}


TAU_MAIN()
TEST(memory, stack_allocator) { 
    test_stack_allocator();
}

