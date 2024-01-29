
#define CLOGGER_IMPLEMENTATION
#include "../include/clogger/clogger.h"
#include <stdbool.h>

void tshInitLogManger(){
    clogger_init(NULL, true);
}

void tshPrintLogs(){
    flush_queue();
}

void tshQuitLogManger(){
    flush_queue();
    clogger_quit();
}
