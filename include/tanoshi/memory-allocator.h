#ifndef _MEMORY_ALLOCATOR_H
#define _MEMORY_ALLOCATOR_H
#include "custom_types.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


#define TSH_MASTER_BUFFER_DEFAULT_SIZE 4096//2000000000
#define UINTPTR_SIZE sizeof(uintptr_t)

bool tshInitAllocators(u32 master_buff_size);
void tshQuitAllocators(void);
void setMasterBufferSize(u32 buff_size);
size_t getMasterBufferSize(void);

// Dynamic allocator
#define __TSH_END_DYNAMIC_ADDRESS_TRACKER 0x4D485354

typedef struct __tsh_dynamic_alloc_addr_tracker {
    uintptr_t start;
    size_t size;
}__tshDynamicTracker;

typedef struct __tsh_dynamic_allocator{
    uintptr_t baseAddress;
    uintptr_t addressTrackerBase;
    size_t capacity;
    u16 addressTrackerSize;
}tshDynamicAllocator;


void tshInitDynamicAllocator(tshDynamicAllocator *allocator, uintptr_t start, size_t size);
uintptr_t __tshDynamicAllocate(tshDynamicAllocator *allocator, size_t size);
bool tshDynamicFree(tshDynamicAllocator* allocator, uintptr_t addr);

#define tshDynamicAllocate(allocator, size, type) \
    (type)__tshDynamicAllocate(allocator, size)


// Linear allocator
typedef struct tsh_linear_allocator {
    uintptr_t baseAddress;
    size_t capacity;
    u32 offset;
} tshLinearAllocator;

#define tshLinearAllocate(allocator, size, type)\
    (type)__tshLinearAllocate(allocator, size)

void tshInitLinearAllocator(tshLinearAllocator *allocator, uintptr_t baseAddr, size_t size);
uintptr_t __tshLinearAllocate(tshLinearAllocator *allocator, size_t size);

// Stack allocator
typedef struct tsh_stack_allocator {
    uintptr_t stackBaseAddress;
    uintptr_t stackHeadAddress;
    u32 capacity;
    bool bottomTop;
} tshStackAllocator;

#define tshStackAllocate(allocator, size, type) \
    (type)__tshStackAllocate(allocator, size)

void tshInitStackAllocator(tshStackAllocator *allocator, uintptr_t start, size_t size, bool direction);
uintptr_t __tshStackAllocate(tshStackAllocator *allocator, size_t size);
void tshStackFree(tshStackAllocator *allocator);

#endif //_MEMORY_ALLOCATOR_H
