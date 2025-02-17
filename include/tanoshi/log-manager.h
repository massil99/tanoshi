#ifndef _LOG_MANAGER_H_
#define _LOG_MANAGER_H_

#include "../clogger/clogger.h"
void tshInitLogManger();
void tshPrintLogs();
void tshSetLogLevel(cloggerLVL lvl);
void tshQuitLogManger();

#endif //_LOG_MANAGER_H_
