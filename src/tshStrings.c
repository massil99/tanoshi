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

    s->buf = strndup(str, s->size);
    s->capacity = (size_t) pow(2, u);
}

void tshStringNew(tshString* s) {
    s->buf = malloc(sizeof(char) * __TSH_STRING_DEFAULT_LEN);
    s->buf[0] = '\0';
    s->size = 0;
    s->capacity = __TSH_STRING_DEFAULT_LEN;
}

void tshStringNewWithCapacity(tshString* s, size_t capacity) {
    s->buf = malloc(sizeof(char) * capacity);
    s->buf[0] = '\0';
    s->size = 0;
    s->capacity = capacity;
}

void tshStringDup(tshString s, tshString *out) {
    tshStringNewWithCapacity(out, s.capacity);

    for (u32 x = 0; x < s.size; x++) {
        out->buf[x] = s.buf[x];
    }

    out->size = s.size;
}

void __expand(tshString* s) {
    s->capacity = s->capacity * 2;
    s->buf = realloc(s->buf, s->capacity);
}

void tshStringAppend(tshString* s, char *c) {
    tshStringInsert(s, c, s->size);
} 

void tshStringPrepend(tshString* s, char* c) {
    tshStringInsert(s, c, 0);
} 

u32 max(u32 a, u32 b) { return a > b ? a: b; }

void tshStringInsert(tshString* s, char* c, u32 index) {
    assert(s->buf != NULL && c != NULL);

    const size_t extension_size = strlen(c);
    if (extension_size > 0) {
        while ((s->size + extension_size) >= s->capacity - 1) {
            __expand(s);
        }

        size_t start = s->size + extension_size;
        for (i64 x = start; x >= index; x--) {
            s->buf[x + extension_size] = s->buf[x];
            if (x < extension_size) {
                s->buf[x] = c[x - index];
            }
        }

        s->size += extension_size;
        s->buf[s->size] = '\0';
    }
}

void tshSubString(const tshString* s, const u32 start, const u32 size, tshString *ret) {
    assert(size <= s->size - start);
    assert(s->buf != NULL);

    tshStringNewWithCapacity(ret, s->capacity);

    const u32 end = start + size;
    for (u32 x = start; x < end; x++) {
        ret->buf[x - start] = s->buf[x];
    }

    ret->size = size;
    ret->buf[size] = '\0';
}

void tshStringDistroy(tshString* s) {
    free(s->buf);
    s->buf = NULL;
    s->size = 0;
    s->capacity = 0;
}

