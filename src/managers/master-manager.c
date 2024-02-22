
#include "../../include/tanoshi/master-manager.h"
#include <stdio.h>


bool tshInit(void){
    tshInitLogManger();
    LOG_INFO("Tanoshi engine startup");
    tshInitAllocators(0);

    return true;
}

void tshQuit(void){
    LOG_INFO("Tanoshi engine shuting down");
    tshQuitAllocators();
    tshQuitLogManger();
    printf("bey...\n");
}
