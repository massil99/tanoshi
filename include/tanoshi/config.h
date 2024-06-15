#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include "../../include/tanoshi/custom_types.h"

#define CONFIG_FILE_PATH "/home/massil/projects/tanoshi/bin/config"

typedef enum {
    TSH_CONF_NOEVENT                 ,
    TSH_CONF_STREAM_START            ,
    TSH_CONF_STREAM_END              ,
    TSH_CONF_PARSING_PROPERTY        ,
    TSH_CONF_PROPERTY_AHEAD          ,
    TSH_CONF_PROPERTY                ,
    TSH_CONF_VALUE_AHEAD             ,
    TSH_CONF_PARSING_INTEGER         ,
    TSH_CONF_PARSING_NEGATIVE_NUMBER ,
    TSH_CONF_PARSING_FLOAT           ,
    TSH_CONF_PARSING_STRING          ,
    TSH_CONF_INTEGER_VALUE           ,
    TSH_CONF_FLOAT_VALUE             ,
    TSH_CONF_STRING_VALUE            ,
    TSH_CONF_ERROR                    
}tshConfigParserState;

typedef struct tsh_conf_event_t{
    tshConfigParserState type;

    char property[128]; 
    u8 length;
    union{
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
    } data;
}tshConfParserEvent;

typedef struct tsh_conf_parser {
    FILE *conf_file;
    tshConfParserEvent event;
}tshConfParser;

#define TSH_GET_KV_OBJECT_DATA(v)\
    (kv_data){.object_value.value=v}
#define TSH_GET_KV_FLT_DATA(v)\
    (kv_data){.float_value.value=v}
#define TSH_GET_KV_INT_DATA(v)\
    (kv_data){.integer_value.value=v}
#define TSH_GET_KV_NO_DATA()\
    (kv_data ){}

typedef union {
    struct float_value {
        f32 value; 
    }float_value;

    struct integer_value {
        i32 value; 
    }integer_value;

    struct object_value {
        void* value; 
    }object_value;
} kv_data;

struct tsh_conf_KV {
    char* key;
    kv_data data;
    bool occupied;
};

#define CONFIG_INITIIAL_SIZE 16
typedef struct tsh_conf {
    struct tsh_conf_KV *enteries;    
    u32 length;
    u32 capacity;
} tshConf;

bool get_config(void);
kv_data tshGetConf(tshConf *confList, char name[]);

#endif //_CONFIG_H
