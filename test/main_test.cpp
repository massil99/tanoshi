#include <gtest/gtest.h>

extern "C" {
#include "memory-allocator.h"
#include "log-manager.h"
#include "clogger.h"
#include <stdint.h>
}

void init(){
    tshInitLogManger();
}
void quit(){
    tshQuitLogManger();
}

TEST(memory, forward_stack_allocator) { 
    init();
    uintptr_t start = (uintptr_t)malloc(100);
    size_t size = 70;
    bool dir = true;
    tshStackAllocator st;
    tshInitStackAllocator(&st, start,  size, dir);

    EXPECT_EQ(st.stackBaseAddress, start);
    EXPECT_EQ(st.stackBaseAddress, st.stackHeadAddress);
    u16* addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 1;
    EXPECT_EQ(*addr, 1);
    addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 2;
    EXPECT_EQ(*addr, 2);
    addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 3;
    EXPECT_EQ(*addr, 3);
    addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 4;
    EXPECT_EQ(*addr, 4);

    tshStackFree(&st);
    EXPECT_EQ(*(u16*)(st.stackHeadAddress), 4);
    tshStackFree(&st);
    EXPECT_EQ(*(u16*)(st.stackHeadAddress), 3);
    tshStackFree(&st);
    EXPECT_EQ(*(u16*)(st.stackHeadAddress), 2);
    tshStackFree(&st);
    EXPECT_EQ(*(u16*)(st.stackHeadAddress), 1);
    tshStackFree(&st);
    EXPECT_EQ(*(u16*)(st.stackHeadAddress), 1);

    addr = tshStackAllocate(&st, sizeof(u16)*30, u16*);
    *addr = 0x69;
    EXPECT_EQ(*addr, 0x69);
    addr = tshStackAllocate(&st, sizeof(u16), u16*);
    EXPECT_EQ((uintptr_t)addr, 0);

    free((void*)start);
    quit();
}

TEST(memory, backward_stack_allocator) { 
    init();
    uintptr_t start = (uintptr_t)malloc(100);
    size_t size = 70;
    tshStackAllocator st;
    bool dir = false;

    tshInitStackAllocator(&st, start + size,  size, dir);

    EXPECT_EQ(st.stackBaseAddress, start + size);
    EXPECT_EQ(st.stackBaseAddress, st.stackHeadAddress);
    u16* addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 1;
    EXPECT_EQ(*addr, 1);
    addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 2;
    EXPECT_EQ(*addr, 2);
    addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 3;
    EXPECT_EQ(*addr, 3);
    addr = tshStackAllocate(&st, sizeof(u16),u16*);
    *addr = 4;
    EXPECT_EQ(*addr, 4);

    tshStackFree(&st);
    EXPECT_EQ(*(u16*)(st.stackHeadAddress -1), 4);
    tshStackFree(&st);
    EXPECT_EQ(*(u16*)(st.stackHeadAddress -1), 3);
    tshStackFree(&st);
    EXPECT_EQ(*(u16*)(st.stackHeadAddress -1), 2);
    tshStackFree(&st);
    EXPECT_EQ(*(u16*)(st.stackHeadAddress -1), 1);
    tshStackFree(&st);
    EXPECT_EQ(*(u16*)(st.stackHeadAddress -1), 1);

    addr = tshStackAllocate(&st, sizeof(u16)*30, u16*);
    *addr = 0x69;
    EXPECT_EQ(*addr, 0x69);
    addr = tshStackAllocate(&st, sizeof(u16), u16*);
    EXPECT_EQ((uintptr_t)addr, 0);

    free((void*)start);
    quit();
}

TEST(memory, linear_allocator) { 
    init();
    tshLinearAllocator li;
    uintptr_t start = (uintptr_t)malloc(100);
    size_t size = 70;

    tshInitLinearAllocator(&li, start, size);
    u16* addr = tshLinearAllocate(&li, sizeof(u16)*35, u16*);
    for(u16 i = 0; i < 35; i++){
        addr[i] = i;
        EXPECT_EQ(addr[i], i); 
    }
    EXPECT_EQ((uintptr_t)addr, li.baseAddress); 

    addr = tshLinearAllocate(&li, sizeof(u16), u16*);
    EXPECT_EQ((uintptr_t)addr, 0); 

    addr = tshLinearAllocate(&li, 1, u16*);
    EXPECT_EQ((uintptr_t)addr, 0); 

    free((void*) start);
    quit();
}


TEST(memory, dynamic_allocator) {

}
