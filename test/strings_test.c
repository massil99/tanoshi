
#include "tau/tau.h"
#include "log-manager.h"
#include "clogger.h"
#include "tshStrings.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void init(){
    tshInitLogManger();
}

void quit(){
    tshQuitLogManger();
}

TAU_MAIN()

#define arr_size 15000

TEST(strings, main) { 
    init();

    tshString s;
    tshStringFromCStr(&s, "hello");

    REQUIRE(s.buf != NULL);
    CHECK_EQ(s.size, 5);
    CHECK_EQ(s.capacity, 32);
    CHECK(strcmp(s.buf, "hello") == 0);

    tshString ss;
    tshSubString(&s, 1, 3, &ss);

    REQUIRE(ss.buf != NULL);
    CHECK_EQ(ss.size, 3);
    CHECK_EQ(ss.capacity, 32);
    CHECK(strcmp(ss.buf, "ell") == 0);

    tshStringDistroy(&s);
    quit();
}
