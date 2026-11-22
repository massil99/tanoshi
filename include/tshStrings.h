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

typedef struct __tsh_string_view {
    tshString *target; 
    size_t start;
    size_t end;
} tshStringView;

void tshStringFromCStr(tshString* s, const char* str);
void tshStringNew(tshString* s);
void tshStringNewWithCapacity(tshString* s, size_t capacity);
void tshStringDup(tshString s, tshString *out);
void tshStringAppend(tshString* s, char* c);
void tshStringPrepend(tshString* s, char* c);
void tshStringInsert(tshString* s, char* c, u32 index);
void tshSubString(const tshString* s, const u32 start, const u32 size, tshString *ret);
void tshStringDistroy(tshString* s);

#endif //__TSH_STRINGS_H_
