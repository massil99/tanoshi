#include <stdlib.h>

#include "../include/tanoshi/master-manager.h"


int main(int argc, char *argv[]){
    tshInit();

    int i = 1000000000;

    while(i--);

    tshPrintLogs();

    tshQuit();

    return EXIT_SUCCESS;
}
