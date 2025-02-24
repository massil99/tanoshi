
#include <stdlib.h>

#include "master-manager.h"

int main(int argc, char *argv[]){
    int exit_code = EXIT_SUCCESS;

    if(!tshInit()) {
        LOG_FATAL("Tanoshi engine initialization failed");
        exit_code = EXIT_FAILURE;
    }

    // tshPrintLogs();

    tshQuit();

    return exit_code;
}

