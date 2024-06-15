#ifndef _UTILS_H
#define _UTILS_H
#include "custom_types.h"

#define STRVAL(s) s? s: "" 

typedef union tshDataUnion{
    struct property_float {
        f32 value;
    } propertyFloat;

    struct property_integer {
        i32 value;
    } propertyInteger;

    struct property_string {
        char *value; 
        u8 length;
    } propertyString;
} tshDataUnion;

#endif //_UTILS_H
