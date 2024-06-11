#ifndef _MASTER_MANAGER_H
#define _MASTER_MANAGER_H

#include <stdbool.h>

#include "log-manager.h"
#include "memory-allocator.h"
#include "config.h"


bool tshInit(void);
void tshQuit(void);

#endif //_MASTER_MANAGER_H
