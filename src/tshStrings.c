#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "clogger.h"
#include "tshStrings.h"


void tshStringFromCStr(tshString* s, const char* str) {
    s->size = strlen(str);

    f64 u = ceil(log2(s->size));
    if (u < 5) u = 5;

    s->buf = strdup(str);
    s->capacity = (size_t) pow(2, u);
}

void tshStringNew(tshString* s) {
    s->buf = malloc(sizeof(char) * __TSH_STRING_DEFAULT_LEN);
    s->buf[0] = '\0';
    s->size = 0;
    s->capacity = __TSH_STRING_DEFAULT_LEN;
}

void __expand(tshString* s) {
    s->capacity = s->capacity * 2;
    s->buf = realloc(s->buf, s->capacity);
}

void tshStringAppend(tshString* s, char c) {
    assert(s->buf != NULL);
        
    if (s->size >= s->capacity - 1) {
        __expand(s);
    }

    s->buf[s->size++] = c;
    s->buf[s->size] = '\0';
} 

void tshStringCat(tshString* first, tshString* last, tshString* dist) {
    tshStringNew(dist);


}

void tshSubString(const tshString* s, const u32 start, const u32 size, tshString *ret) {
    assert(size <= s->size - start);
    assert(s->buf != NULL);

    tshStringNew(ret);

    for (u32 i = start; i < start + size; i++) {
        tshStringAppend(ret, s->buf[i]);
    }
}

void tshStringDistroy(tshString* s) {
    free(s->buf);
    s->buf = NULL;
    s->size = 0;
    s->capacity = 0;
}

