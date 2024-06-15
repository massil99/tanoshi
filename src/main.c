
#include <stdlib.h>

#include "../include/tanoshi/master-manager.h"
#include "../include/tanoshi/linked-list.h"


int main(int argc, char *argv[]){
    int exit_code = EXIT_SUCCESS;

    if(!tshInit()) {
        LOG_FATAL("Tanoshi engine initialization failed");
        exit_code = EXIT_FAILURE;
    }

    tshLinkedlist list;
    list.next = NULL;
    tshFreeLinkedList(&list);

    // tshPrintLogs();

    tshQuit();

    return exit_code;
}

