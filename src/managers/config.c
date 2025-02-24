// reads yaml config file

#include <string.h>

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "config.h"
#include "clogger.h"
#include "custom_types.h"
#include "hash.h"
#include "utils.h"

tshConf tsh_config;

void tshInitConf(tshConf *confList){
    confList->enteries = (struct tsh_conf_KV*)calloc(CONFIG_INITIIAL_SIZE, sizeof(struct tsh_conf_KV));
    if(confList->enteries == NULL){
        LOG_ERROR("Could not allocate memory to the config list");
    }

    confList->length = 0;
    confList->capacity = CONFIG_INITIIAL_SIZE;
}


// free values
void tshDestroyConf(tshConf *confList){
    free(confList->enteries);
}

// reallocates a chunk of meory and sets new memory to zero
void* realloc_zero(void* pBuffer, size_t oldSize, size_t newSize) {
    void* pNew = realloc(pBuffer, newSize);
    if ( newSize > oldSize && pNew ) {
        size_t diff = newSize - oldSize;
        void* pStart = ((char*)pNew) + oldSize;
        memset(pStart, 0, diff);
    }
    return pNew;
}


// ands an entry to the config data structure
void tshAddConfig(tshConf *confList, char name[], kv_data value){
    if(confList->length ==  confList->capacity){
        u32 newSize = confList->capacity * 2;

        confList->enteries = realloc_zero(confList->enteries,
                confList->capacity * sizeof(struct tsh_conf_KV),
                newSize * sizeof(struct tsh_conf_KV));
        confList->capacity = newSize; 
        if(confList->enteries == NULL){
            LOG_ERROR("Failed config list extension");
        }
    }

    u32 hash  = crc_32(name) % confList->capacity; 
    if(confList->enteries[hash].occupied){
        hash = (hash + 1) % confList->capacity;
        while(confList->enteries[hash].occupied == true) { 
            hash = (hash + 1) % confList->capacity;
        }
    }

    confList->enteries[hash].key = STRVAL(strdup(name));
    confList->enteries[hash].data = value;
    confList->enteries[hash].occupied = true;
    confList->length++;
}


kv_data tshGetConf(tshConf *confList, char name[]){
    u32 hash = crc_32(name) % confList->capacity; 
    
    for (u32 i = hash; i + 1 != hash; i = (i + 1) % confList->capacity) {
        if(confList->enteries[i].key != NULL){
            u32 condidat = crc_32(confList->enteries[i].key) % confList->capacity;

            if(hash == condidat &&
                strcmp(name, confList->enteries[i].key) == 0){
                return confList->enteries[i].data;
            }
        }
    }

    LOG_WARN("Key (%s) not found", name);
    return TSH_GET_KV_NO_DATA();
}


void print_state(tshConfigParserState state){
    switch(state){
        case TSH_CONF_NOEVENT:                 
            LOG_DEBUG("Current parser state is TSH_CONF_NOEVENT");
            break;
        case TSH_CONF_STREAM_START:            
            LOG_DEBUG("Current parser state is TSH_CONF_STREAM_START");
            break;
        case TSH_CONF_STREAM_END:              
            LOG_DEBUG("Current parser state is TSH_CONF_STREAM_END");
            break;
        case TSH_CONF_PARSING_PROPERTY:
            LOG_DEBUG("Current parser state is TSH_CONF_PARSING_PROPERTY");
            break;
        case TSH_CONF_PROPERTY_AHEAD:          
            LOG_DEBUG("Current parser state is TSH_CONF_PROPERTY_AHEAD");
            break;
        case TSH_CONF_PROPERTY:                
            LOG_DEBUG("Current parser state is TSH_CONF_PROPERTY");
            break;
        case TSH_CONF_VALUE_AHEAD:             
            LOG_DEBUG("Current parser state is TSH_CONF_VALUE_AHEAD");
            break;
        case TSH_CONF_PARSING_INTEGER:         
            LOG_DEBUG("Current parser state is TSH_CONF_PARSING_INTEGER");
            break;
        case TSH_CONF_PARSING_NEGATIVE_NUMBER: 
            LOG_DEBUG("Current parser state is TSH_CONF_PARSING_NEGATIVE_NUMBER");
            break;
        case TSH_CONF_PARSING_FLOAT:           
            LOG_DEBUG("Current parser state is TSH_CONF_PARSING_FLOAT");
            break;
        case TSH_CONF_PARSING_STRING:          
            LOG_DEBUG("Current parser state is TSH_CONF_PARSING_STRING");
            break;
        case TSH_CONF_INTEGER_VALUE:           
            LOG_DEBUG("Current parser state is TSH_CONF_INTEGER_VALUE");
            break;
        case TSH_CONF_FLOAT_VALUE:             
            LOG_DEBUG("Current parser state is TSH_CONF_FLOAT_VALUE");
            break;
        case TSH_CONF_STRING_VALUE:            
            LOG_DEBUG("Current parser state is TSH_CONF_STRING_VALUE");
            break;
        case TSH_CONF_ERROR:                   
            LOG_DEBUG("Current parser state is TSH_CONF_ERROR");
            break;
    }
}

void tshInitConfParser(tshConfParser *parser, char *filepath){
    LOG_INFO("Configuration file found at (%s)", filepath);
    parser->conf_file = fopen(filepath, "r");
    parser->event.type = TSH_CONF_NOEVENT;
    if (parser->conf_file == NULL){
        LOG_ERROR("Configuration file not found at (%s)", filepath);
        assert(false);
    } 
}

void tshConfPoolEvent(tshConfParser *parser){
    char buf , string_buf[256];
    u16 string_length = 0;

    if(parser->conf_file == NULL){
        parser->event.type = TSH_CONF_ERROR;
    } else {
        do {
            switch(parser->event.type){
                case TSH_CONF_NOEVENT:
                    parser->event.type = TSH_CONF_STREAM_START;
                    break;
            
                case TSH_CONF_STREAM_START:
                    buf = fgetc(parser->conf_file);
                    if(isalpha(buf)){
                        parser->event.type = TSH_CONF_PARSING_PROPERTY;
                        fseek(parser->conf_file, -1, SEEK_CUR);
                    } else if(buf == ' ' || buf == '\n'){
                        parser->event.type = TSH_CONF_PROPERTY_AHEAD;
                    } else if(buf == EOF) {
                        parser->event.type = TSH_CONF_STREAM_END;
                    } else {
                        parser->event.type = TSH_CONF_ERROR;
                        LOG_ERROR("[Config]: illegal character in property name (%c)", buf);
                    }
                    break;
            
                case TSH_CONF_PARSING_PROPERTY:
                    strcpy(parser->event.property, "");
                    parser->event.length = 0;
                    while(parser->event.type != TSH_CONF_PROPERTY && parser->event.type != TSH_CONF_ERROR){
                        buf = fgetc(parser->conf_file);
                        if(isalpha(buf) || isdigit(buf) || buf == '_'){
                            parser->event.property[parser->event.length++] = buf;
                        } else if(buf == ' ' || buf == '\n'){
                            parser->event.type = TSH_CONF_PROPERTY;
                        } else if(buf == '='){
                            parser->event.type = TSH_CONF_PROPERTY;
                            fseek(parser->conf_file, -1, SEEK_CUR);
                        } else if(buf == EOF) {
                            parser->event.type = TSH_CONF_ERROR;
                            LOG_ERROR("[Config]: unexpected end of file at (%s)", parser->event.property);
                        } else {
                            parser->event.type = TSH_CONF_ERROR;
                            LOG_ERROR("[Config]: illegal character in property name (%c)", buf);
                        }
                    }
                    parser->event.property[parser->event.length] = '\0';
                    break;
            
                case TSH_CONF_PROPERTY_AHEAD:
                    while(parser->event.type == TSH_CONF_PROPERTY_AHEAD){
                        buf = fgetc(parser->conf_file);
                        if(isalpha(buf)){
                            parser->event.type = TSH_CONF_PARSING_PROPERTY;
                            fseek(parser->conf_file, -1, SEEK_CUR);
                        } else if (buf == ' ' || buf == '\n'){
                            parser->event.type = TSH_CONF_PROPERTY_AHEAD;
                        } else if (buf == EOF) {
                            parser->event.type = TSH_CONF_STREAM_END;
                        } else {
                            parser->event.type = TSH_CONF_ERROR;
                            LOG_ERROR("[Config]: illegal character in property name (%c)", buf);
                        }
                    }
                    break;
            
                case  TSH_CONF_PROPERTY:
                    while(parser->event.type == TSH_CONF_PROPERTY){
                        buf = fgetc(parser->conf_file);
                        if (buf == ' ' || buf == '\n'){
                            parser->event.type = TSH_CONF_PROPERTY;
                        } else if (buf == '=') {
                            parser->event.type = TSH_CONF_VALUE_AHEAD;
                        } else if (buf == EOF) {
                            parser->event.type = TSH_CONF_ERROR;
                            LOG_ERROR("[Config]: unexpected end of file");
                        } else {
                            parser->event.type = TSH_CONF_ERROR;
                            LOG_ERROR("[Config]: illegal character in property name (%c)", buf);
                        }
                    }
                    break;
            
                case TSH_CONF_VALUE_AHEAD:
                    while(parser->event.type == TSH_CONF_VALUE_AHEAD){
                        buf = fgetc(parser->conf_file);
                        if (buf == '-'){
                            string_length = 0;
                            strcpy(string_buf, "");
                            parser->event.type = TSH_CONF_PARSING_NEGATIVE_NUMBER;
                        } else if (isdigit(buf)){
                            string_length = 0;
                            strcpy(string_buf, "");
                            parser->event.type = TSH_CONF_PARSING_INTEGER;
                            fseek(parser->conf_file, -1, SEEK_CUR);
                        } else if (buf == '"') {
                            parser->event.type = TSH_CONF_PARSING_STRING;
                        } else if (buf == ' ' || buf == '\n') {
                            parser->event.type = TSH_CONF_VALUE_AHEAD;
                        } else if (buf == EOF) {
                            parser->event.type = TSH_CONF_ERROR;
                            LOG_ERROR("[Config]: unexpected end of file");
                        } else {
                            parser->event.type = TSH_CONF_ERROR;
                            LOG_ERROR("[Config]: here illegal value (%c)", buf);
                        }
                    }
                    break;
            
                case TSH_CONF_PARSING_NEGATIVE_NUMBER:
                    parser->event.type = TSH_CONF_PARSING_INTEGER;
                    string_buf[string_length++] = '-';
                    break;
            
                case TSH_CONF_PARSING_INTEGER:
                    while(parser->event.type == TSH_CONF_PARSING_INTEGER){
                        buf = fgetc(parser->conf_file);
                        if (isdigit(buf)){
                            parser->event.type = TSH_CONF_PARSING_INTEGER;
                            string_buf[string_length++] = buf;
                        } else if (buf == '.') {
                            parser->event.type = TSH_CONF_PARSING_FLOAT;
                            string_buf[string_length++] = buf;
                        } else if (buf == ' ' || buf == '\n') {
                            parser->event.type = TSH_CONF_INTEGER_VALUE;
                            string_buf[string_length] = '\0';
                            parser->event.data.propertyInteger.value = atoi(string_buf);
                        } else if (buf == EOF) {
                            parser->event.type = TSH_CONF_ERROR;
                            LOG_ERROR("[Config]: unexpected end of file");
                        } else {
                            parser->event.type = TSH_CONF_ERROR;
                            LOG_ERROR("[Config]: illegal value (%c)", buf);
                        }
            
                    }
                    break;
            
                case TSH_CONF_PARSING_FLOAT:
                    while(parser->event.type == TSH_CONF_PARSING_FLOAT){
                        buf = fgetc(parser->conf_file);
                        if (isdigit(buf)){
                            string_buf[string_length++] = buf;
                        } else if (buf == ' ' || buf == '\n') {
                            parser->event.type = TSH_CONF_FLOAT_VALUE;
                            string_buf[string_length] = '\0';
                            parser->event.data.propertyFloat.value = atof(string_buf);
                        } else if (buf == EOF) {
                            parser->event.type = TSH_CONF_ERROR;
                            LOG_ERROR("[Config]: unexpected end of file");
                        } else {
                            parser->event.type = TSH_CONF_ERROR;
                            LOG_ERROR("[Config]: illegal value (%c)", buf);
                        }
                    }
                    break;
            
                case TSH_CONF_PARSING_STRING:
                    strcpy(string_buf, "");
                    string_length= 0;

                    while(parser->event.type == TSH_CONF_PARSING_STRING){
                        buf = fgetc(parser->conf_file);
                        if(buf == '"'){
                            parser->event.type = TSH_CONF_STRING_VALUE;
                            string_buf[string_length] = '\0';
                            parser->event.data.propertyString.length = string_length - 1;
                            // TODO: free string here
                            parser->event.data.propertyString.value = (char*) malloc(string_length);
                            strcpy(parser->event.data.propertyString.value,  string_buf);
                        } else if (buf == '\n') {
                            parser->event.type = TSH_CONF_ERROR;
                            LOG_ERROR("[Config]: illegal value (%c)", buf);
                        } else if (buf == EOF) {
                            parser->event.type = TSH_CONF_ERROR;
                            LOG_ERROR("[Config]: unexpected end of file");
                        } else {
                            string_buf[string_length++] = buf;
                        }
                    }
                    break;
            
                case TSH_CONF_INTEGER_VALUE:
                case TSH_CONF_FLOAT_VALUE:
                case TSH_CONF_STRING_VALUE:
                    buf = fgetc(parser->conf_file);
                    if (buf == EOF) {
                        parser->event.type = TSH_CONF_STREAM_END;
                    } else {
                        parser->event.type = TSH_CONF_PROPERTY_AHEAD;
                        fseek(parser->conf_file, -1, SEEK_CUR);
                    } 
                    break;
            
                default:
                    parser->event.type = TSH_CONF_ERROR;
                    LOG_ERROR("Unknwon parser state");
                    break;
            }
        } while (
                parser->event.type != TSH_CONF_NOEVENT &&
                parser->event.type != TSH_CONF_STREAM_START &&
                parser->event.type != TSH_CONF_STREAM_END &&
                parser->event.type != TSH_CONF_PROPERTY &&
                parser->event.type != TSH_CONF_INTEGER_VALUE &&
                parser->event.type != TSH_CONF_FLOAT_VALUE &&
                parser->event.type != TSH_CONF_STRING_VALUE &&
                parser->event.type != TSH_CONF_ERROR
        );
    }
}


void tshDestroyConfParser(tshConfParser *parser){
    fclose(parser->conf_file);
}

#define STRING(x) #x
#define XSTRING(x) STRING(x)

bool get_config(){
    tshConfParser parser;
    char path[240] = "";
    sprintf(path, "%s/config", XSTRING(ROOT_DIR));
    tshInitConfParser(&parser, path);
    tshInitConf(&tsh_config);

    do{
        tshConfPoolEvent(&parser);
        if(parser.event.type == TSH_CONF_ERROR){
            break;
        }
        switch(parser.event.type) {
            case TSH_CONF_STRING_VALUE:
                // LOG_DEBUG("%s = %s", parser.event.property, parser.event.data.propertyString.value);
                tshAddConfig(&tsh_config, parser.event.property, 
                        (kv_data){.object_value.value=parser.event.data.propertyString.value});
                break;
            case TSH_CONF_INTEGER_VALUE:
                // LOG_DEBUG("%s = %d", parser.event.property, parser.event.data.propertyInteger.value);
                tshAddConfig(&tsh_config, parser.event.property, 
                        (kv_data){.integer_value.value=parser.event.data.propertyInteger.value});
                break;
            case TSH_CONF_FLOAT_VALUE:
                tshAddConfig(&tsh_config, parser.event.property, 
                        (kv_data){.float_value.value=parser.event.data.propertyFloat.value});
                break;
            default:
                break;
        }
    } while(parser.event.type != TSH_CONF_STREAM_END);

    tshDestroyConf(&tsh_config);
    tshDestroyConfParser(&parser);
    return true;
}

