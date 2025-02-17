
#include "../../include/tanoshi/master-manager.h"
#include "../../include/tanoshi/config.h"
#include <stdio.h>

extern tshConf tsh_config;

bool tshInit(void){
    tshInitLogManger();

    get_config();

    kv_data lvl = tshGetConf(&tsh_config, "LOG_LEVEL");
    tshSetLogLevel(lvl.integer_value.value);

    LOG_INFO("Tanoshi engine startup");

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
