
#include "../include/tanoshi/master-manager.h"


bool tshInit(void){
    tshInitLogManger();
    LOG_INFO("Tanoshi engine startup");

    return true;
}

void tshQuit(void){
    LOG_INFO("Tanoshi engine shuting down");
    tshQuitLogManger();
}
