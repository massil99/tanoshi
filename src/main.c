#include <stdlib.h>
#include <stdio.h>

#include "../include/tanoshi/master-manager.h"


int main(int argc, char *argv[]){

    tshInit();

    // int i = 100000000;
    // while(--i);

    tshPrintLogs();

    LOG_INFO("hello");

    tshPrintLogs();

    tshQuit();

    return EXIT_SUCCESS;
}
