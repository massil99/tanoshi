
#include "../../include/tanoshi/master-manager.h"
#include <stdio.h>


bool tshInit(void){
    tshInitLogManger();
    LOG_INFO("Tanoshi engine startup");
    get_config();
    if (!tshInitAllocators(0)){
        return false;
    }

    return true;
}

void tshQuit(void){
    LOG_INFO("Tanoshi engine shutting down");
    tshQuitAllocators();
    tshQuitLogManger();
    printf("bey...\n");
}
