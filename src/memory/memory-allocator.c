
#include "../../include/tanoshi/memory-allocator.h"
#include "../../include/clogger/clogger.h"
#include <stdint.h>
#include <sys/types.h>

uintptr_t tshMasterBuffer;

u32 tshMasterBufferSize = 0;
i32 tshPersistentMemoryOffset = 0;
i32 tshDynamicMemoryOffset = 0;
i32 tshOneFrameMemoryOffset = 0;
i32 tshCurrentMemoryOffset = 0;


/*
 * Inetializes the custom memeory allocation system
 * @param {u32} master_buff_size the size of the master memory block
 *      master_buff_size = 0: set default master block size
 * @return {bool} true if the initializiation went without problems false otherwise
 */
bool tshInitAllocators(u32 master_buff_size){
    LOG_INFO("Initializing allocators");

    if(master_buff_size != 0){
        setMasterBufferSize(master_buff_size);
    } else {
        setMasterBufferSize(TSH_MASTER_BUFFER_DEFAULT_SIZE);
    }

    tshMasterBuffer = (uintptr_t) malloc(tshMasterBufferSize);
    LOG_INFO("[MEM] Master buffer size = %ld", tshMasterBufferSize);

    /***************************STACK ALLOC TEST*************************************/
    // tshStackAllocator stAllocator;
    // tshInitStackAllocator(&stAllocator, tshMasterBuffer, 100);
    //
    // u16* addr = (u16*) tshStackAllocate(&stAllocator, sizeof(u16));
    //
    // *addr = 4;
    // LOG_DEBUG("*addr = %d ", *addr);
    // addr = (u16*) tshStackAllocate(&stAllocator, sizeof(u16));
    // *addr = 12;
    // LOG_DEBUG("*addr = %d ", *addr);
    // addr = (u16*)tshStackFree(&stAllocator);
    // LOG_DEBUG("*addr = %ld ", *addr);
    // addr = (u16*) tshStackFree(&stAllocator);
    // LOG_DEBUG("*addr = %ld ", *addr);
    // tshStackFree(&stAllocator); 
    /****************************************************************/
    tshDynamicAllocator dAlloc;  

    tshInitDynamicAllocator(&dAlloc, tshMasterBuffer, 100);

    
    i32 *val = tshDynamicAllocate(&dAlloc, sizeof(i32), i32*);
    *val = 0xff; 
    i32 *todel = tshDynamicAllocate(&dAlloc, sizeof(i32)*4, i32*);
    *todel = 0xdd; 
    val = tshDynamicAllocate(&dAlloc, sizeof(i32), i32*);
    *val = 0xcc; 
    tshDynamicFree(&dAlloc, (uintptr_t) todel);
    val = tshDynamicAllocate(&dAlloc, sizeof(i32), i32*);
    *val = 0xee; 
    val = tshDynamicAllocate(&dAlloc, sizeof(i32), i32*);
    *val = 0xaa; 
    val = tshDynamicAllocate(&dAlloc, sizeof(i32)*2, i32*);

    /****************************************************************/
    return true;
}


/**
 */
void tshQuitAllocators(void){
    LOG_INFO("Destroying allocators");
    free((void*)tshMasterBuffer);
}


/*
 * sets the master buffer size
 * @param {i32} BuffSize master buffer size
 */
void setMasterBufferSize(u32 BuffSize){
    tshMasterBufferSize = BuffSize;
}

/**
 * Returns the master buffer size
 * @return {i32} master buffer size
 */
size_t getMasterBufferSize(){
    return tshMasterBufferSize;
}

/********************* DYNAMIC ALLOCATOR *****************************/
void tshInitDynamicAllocator(tshDynamicAllocator *allocator, uintptr_t start, size_t size) {
    allocator->baseAddress = start;
    allocator->capacity = size;
    allocator->addressTrackerBase = start + allocator->capacity - sizeof(__tshDynamicTracker);
    u32 *endAddr = (u32*)allocator->addressTrackerBase;
    *endAddr = __TSH_END_DYNAMIC_ADDRESS_TRACKER;
    allocator->addressTrackerSize = 0;
}

uintptr_t __dynamicAllocate(tshDynamicAllocator *allocator, size_t size) {
    uintptr_t addr, tmp = allocator->addressTrackerBase;

    // if first addresse of tracker contains the end tracker it means that the memory is empty
    if(allocator->addressTrackerSize == 0){
        // so we allocate the base addresse
        addr = allocator->baseAddress;
    } else { 
        // if memory was already allocated we look for the last addresse in the tracker
        while(*((u32*)tmp) != __TSH_END_DYNAMIC_ADDRESS_TRACKER){
            tmp -= sizeof(__tshDynamicTracker);
        }
        __tshDynamicTracker* trk = (__tshDynamicTracker*) (tmp + sizeof(__tshDynamicTracker));

        // new address comes just after the last allocated block
        addr = trk->start + trk->size;
    }

    // if the new address cannot containe the requested size and the address tracker return out of memory error
    if((i32)(addr - allocator->baseAddress - 1) > 
          (i32)(allocator->capacity - (sizeof(__tshDynamicTracker) * (allocator->addressTrackerSize + 2)) - size)){
        return 0;
    }

    // if there are no capacity issues
    // add new addresse to the tracker
    *(__tshDynamicTracker*)tmp = (__tshDynamicTracker){0, 0}; 
    *(__tshDynamicTracker*)tmp = (__tshDynamicTracker){addr, size}; 
    tmp -= sizeof(__tshDynamicTracker);
    *(u32*)tmp = __TSH_END_DYNAMIC_ADDRESS_TRACKER;


    LOG_DEBUG("allocated memory addresse = %p | size %ld", addr, size);
    //increment the addresse tracker size;
    allocator->addressTrackerSize += 1;  

    return addr;
}


void __reallocate(uintptr_t src, uintptr_t dist, size_t size){
    for(int i = 0; i < size; i++){
        i8* src_byte = (i8*)(src + i);
        i8* dist_byte = (i8*)(dist + i);
        *dist_byte = *src_byte;
    }
}


// follows the naive algorithme to defragment the memory
void __mem_defragmentation(tshDynamicAllocator allocator){
    uintptr_t prev = 0, tmp = allocator.addressTrackerBase;

    if(*((u32*)tmp) != __TSH_END_DYNAMIC_ADDRESS_TRACKER){
        prev = tmp;
        while(*((u32*)(tmp - sizeof(__tshDynamicTracker))) != __TSH_END_DYNAMIC_ADDRESS_TRACKER){
            __tshDynamicTracker *previous_block = (__tshDynamicTracker*)prev; 
            tmp -= sizeof(__tshDynamicTracker);
            __tshDynamicTracker *current_block = (__tshDynamicTracker*)(tmp) ; 
            
            uintptr_t opti_current_address = previous_block->start + previous_block->size; 
        
            if(opti_current_address != current_block->start){
                __reallocate(current_block->start, opti_current_address, current_block->size);
                current_block->start = opti_current_address;
            } 
             
            prev = tmp;
        }
    }
}


uintptr_t __tshDynamicAllocate(tshDynamicAllocator *allocator, size_t size) {
    uintptr_t addr = __dynamicAllocate(allocator, size);

    if(addr == 0){
        LOG_INFO("[MEM] Memory defragmentation on going...");
        __mem_defragmentation(*allocator);
        uintptr_t ret = __dynamicAllocate(allocator, size);

        if (ret == 0){
            LOG_ERROR("[MEM] Out of memory");
        }

        return ret; 
    }

    return addr;
}


bool tshDynamicFree(tshDynamicAllocator* allocator, uintptr_t addr){
    uintptr_t tmp = allocator->addressTrackerBase;

    // look for the address we want to free in the address tracker
    while(((__tshDynamicTracker*)tmp)->start != addr &&
            *((u32*)tmp) != __TSH_END_DYNAMIC_ADDRESS_TRACKER){
        tmp -= sizeof(__tshDynamicTracker);
    }

    // if address found 
    if (((__tshDynamicTracker*)tmp)->start == addr) {
        // Shift all above entries and crash current entry
        while(*((u32*)tmp) != __TSH_END_DYNAMIC_ADDRESS_TRACKER){
            uintptr_t next = tmp - sizeof(__tshDynamicTracker);
            *(__tshDynamicTracker*)tmp = *(__tshDynamicTracker*)next; 
            tmp = next;
        }

        // decrease mem tracker size
        allocator->addressTrackerSize -= 1;
        return true;
    // else unknown address to the allocator
    } else {
        LOG_ERROR("Cannot free unknown memory address");
        return false;
    }
}


/********************* LINEARE ALLOCATOR *****************************/
void tshInitLineareAllocator(tshLineareAllocator *allocator, uintptr_t baseAddr, size_t size) {
    allocator->baseAddress = baseAddr;
    allocator->capacity = size;
    allocator->offset = 0;
}


uintptr_t tshLineareAllocate(tshLineareAllocator *allocator, size_t size) {
    uintptr_t addr = allocator->baseAddress + allocator->offset;

    if((i32)(addr - allocator->baseAddress - 1) > (i32)(allocator->capacity - size)){
        LOG_ERROR("Not enough memory");
        return 0;
    }

    allocator->offset += size;

    return addr;
}

// void tshInitLineareFree(tshLineareAllocator *allocator, size_t size) {
// }


/********************* STACK ALLOCATOR *****************************/

/**
 * Initialize a stack allocator
 */
void tshInitStackAllocator(tshStackAllocator *allocator, uintptr_t start, size_t size) {
    allocator->stackBaseAddress = start;
    allocator->stackHeadAddress = start;
    allocator->capacity = size;
}


uintptr_t __tshStackAllocate(tshStackAllocator *allocator, size_t size){
    uintptr_t addr = allocator->stackHeadAddress;
    if((i32)(addr - allocator->stackBaseAddress - 1) > (i32)(allocator->capacity - size)){
        LOG_ERROR("Not enough memory");
        return 0;
    }

    uintptr_t prev_marker = allocator->stackHeadAddress;
    allocator->stackHeadAddress += size;

    uintptr_t *tmp = (uintptr_t*)allocator->stackHeadAddress;
    *tmp = prev_marker;
    allocator->stackHeadAddress += sizeof(uintptr_t);

    return addr;
}


// void __tshStackFree(tshStackAllocator *allocator, char* filename, u16 line){
uintptr_t tshStackFree(tshStackAllocator *allocator){
    if((i32)allocator->stackBaseAddress >= (i32)allocator->stackHeadAddress){
        LOG_WARN("[MEM]: Attempting to free from an empty stack allocator");
    } else {
        uintptr_t *prev_marker = (uintptr_t *) (allocator->stackHeadAddress - sizeof(uintptr_t));
        allocator->stackHeadAddress = *prev_marker;
    }
    return allocator->stackBaseAddress;
}



