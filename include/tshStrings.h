#ifndef __TSH_STRINGS_H_
#define __TSH_STRINGS_H_

#include <stdlib.h>
#include "custom_types.h"

#define __TSH_STRING_DEFAULT_LEN 32

typedef struct __tsh_string {
    char *buf;
    size_t size;
    size_t capacity;
} tshString;


void tshStringFromCStr(tshString* s, const char* str);
void tshStringNew(tshString* s);
void tshStringAppend(tshString* s, char c);
void tshSubString(const tshString* s, const u32 start, const u32 size, tshString *ret);
void tshStringCat(tshString* first, tshString* last, tshString* dist);
void tshStringDistroy(tshString* s);

#endif //__TSH_STRINGS_H_
