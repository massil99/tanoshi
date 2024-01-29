
#include "../include/tanoshi/memory-allocator.h"
#include "../include/tanoshi/custom-types.h"
#include "../include/clogger/clogger.h"

i8* masterBuffer;
i32 tshMasterBufferSize = 0;
i32 tshPersistentMemoryOffset = 0;
i32 tshDynamiqueMemoryOffset = 0;
i32 tshOneFrameMemoryOffset = 0;
i32 tshCurrentMemoryOffset = 0;


/**
 * sets the master buffer size
 * @param {i32} BuffSize master buffer size
 */
void setMasterBufferSize(i32 BuffSize){
    tshMasterBufferSize = BuffSize;
}

/**
 * Returns the master buffer size
 * @return {i32} master buffer size
 */
i32 getMasterBufferSize(i32 BuffSize){
    tshMasterBufferSize = BuffSize;
}

void linear_allocator() {

}

void* tshPAalloc(size_t size, i8* filename, u16 line) {
    if( tshMasterBufferSize - tshCurrentMemoryOffset <= 0 ) {
        LOG_ERROR("Not enough memory %s:%d\n", filename, line);
        return NULL;
    }

    return NULL;
}

