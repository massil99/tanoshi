
#define CLOGGER_IMPLEMENTATION
#include "../../include/clogger/clogger.h"
#include <stdbool.h>


void tshInitLogManger(){
    clogger_init(NULL, true, 0);
}

void tshPrintLogs(){
    flush_queue();
}

void tshSetLogLevel(cloggerLVL lvl) {
    setLogLevel(lvl);
}

void tshQuitLogManger(){
    flush_queue();
    clogger_quit();
}
