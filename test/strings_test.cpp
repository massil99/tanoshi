
#include <gtest/gtest.h>

extern "C" {
#include "log-manager.h"
#include "clogger.h"
#include "tshStrings.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
}

void strings_init(){
    tshInitLogManger();
}

void strings_quit(){
    tshQuitLogManger();
}

#define arr_size 15000

TEST(strings, main) { 
    strings_init();

    tshString s;
    tshStringFromCStr(&s, "hello");

    ASSERT_TRUE(s.buf != NULL);
    EXPECT_EQ(s.size, 5);
    EXPECT_EQ(s.capacity, 32);
    EXPECT_TRUE(strcmp(s.buf, "hello") == 0);

    tshString ss;
    tshSubString(&s, 1, 3, &ss);

    ASSERT_TRUE(ss.buf != NULL);
    EXPECT_EQ(ss.size, 3);
    EXPECT_EQ(ss.capacity, 32);
    EXPECT_TRUE(strcmp(ss.buf, "ell") == 0);

    tshStringDistroy(&s);
    strings_quit();
}
