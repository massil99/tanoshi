// reads yaml config file

#include <string.h>

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../../include/tanoshi/config.h"
#include "../../include/clogger/clogger.h"
#include "../../include/tanoshi/custom_types.h"
#include "../../include/tanoshi/utils.h"

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


// CRC-32
u32 strHash(char name[]){
    u32 lookupTable[] = { 
        0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
        0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
        0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
        0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
        0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
        0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
        0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
        0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
        0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
        0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
        0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
        0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
        0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
        0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
        0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
        0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
        0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
        0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
        0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
        0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
        0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
        0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
        0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
        0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
        0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
        0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
        0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
        0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
        0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
        0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
        0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
        0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d };

    u32 res = 0xFFFFFFFF; 

    for(u16 i = 0; name[i] != '\0'; i++){
        u16 lookupIndex = (res ^ name[i]) & 0xff;
        res = (res >> 8) ^ lookupTable[lookupIndex];
    }
    return res ^ 0xFFFFFFFF;
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

    u32 hash  = strHash(name) % confList->capacity; 
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
    u32 hash = strHash(name) % confList->capacity; 
    
    for (u32 i = hash; i + 1 != hash; i = (i + 1) % confList->capacity) {
        if(confList->enteries[i].key != NULL){
            u32 condidat = strHash(confList->enteries[i].key) % confList->capacity;

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
    parser->conf_file = fopen(filepath, "r");
    parser->event.type = TSH_CONF_NOEVENT;
    if (parser->conf_file == NULL){
        LOG_ERROR("Configuration file not found at (%s)", CONFIG_FILE_PATH);
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

int get_config(){
    tshConfParser parser;
    tshInitConfParser(&parser, CONFIG_FILE_PATH);
    tshInitConf(&tsh_config);

    do{
        tshConfPoolEvent(&parser);
        if(parser.event.type == TSH_CONF_ERROR){
            break;
        }
        switch(parser.event.type) {
            case TSH_CONF_STRING_VALUE:
                LOG_DEBUG("%s = %s", parser.event.property, parser.event.data.propertyString.value);
                tshAddConfig(&tsh_config, parser.event.property, 
                        (kv_data){.object_value.value=parser.event.data.propertyString.value});
                break;
            case TSH_CONF_INTEGER_VALUE:
                LOG_DEBUG("%s = %d", parser.event.property, parser.event.data.propertyInteger.value);
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
    return EXIT_SUCCESS;
}

