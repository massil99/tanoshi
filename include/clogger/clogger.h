#ifndef CLOGGER_H_
#define CLOGGER_H_

typedef enum __cloggerLVL {
    CLOGGER_DEBUG = 0,
    CLOGGER_INFO     ,
    CLOGGER_TRACE    ,
    CLOGGER_WARN     ,
    CLOGGER_ERROR    ,
    CLOGGER_FATAL    , 
} cloggerLVL;

#include <stdbool.h>

void setLogLevel(cloggerLVL lvl);
void clogger_init(char *file_name, bool showTimestamp, cloggerLVL lvl);
void flush_queue();
void clogger_quit();
void LOG_FATAL(char *m, ...);
void LOG_ERROR(char *m, ...);
void LOG_INFO(char *m, ...);
void LOG_DEBUG(char *m, ...);
void LOG_TRACE(char *m, ...);
void LOG_WARN(char *m, ...);

#endif //CLOGGER_H_

#ifdef CLOGGER_IMPLEMENTATION
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>

#define FATAL_PREFIX "\033[1;35m[FATAL]: \033[0m"
#define ERROR_PREFIX "\033[1;31m[ERROR]: \033[0m"
#define WARN_PREFIX  "\033[1;33m[WARN]: \033[0m"
#define INFO_PREFIX  "\033[1;37m[INFO]: \033[0m"
#define DEBUG_PREFIX "\033[1;36m[DEBUG]: \033[0m"
#define TRACE_PREFIX "\033[1;32m[TRACE]: \033[0m"
#define GRAY_TEXT(m)\
	   "\033[1;90m" m "\033[1;0m "

struct message{
    char content[256];
    struct message* prev;
};

struct message_queue{
    struct message* first;
    struct message* last;
};

FILE* output_file = NULL; 
struct message_queue q;
bool style = true;
bool timestamp = true;
cloggerLVL level;

/**
 * Sets logging level
 * @param {cloggerLVL} lvl logging level
 */
void setLogLevel(cloggerLVL lvl){
    level = lvl;
}

/**
 * Initializes the logging library
 * @param {char *} file_name file to log to (OPTIONAL) 
 * @param {bool} showTimestamp whether to print date time or not 
 * @param {cloggerLVL} lvl logging level
 */
void clogger_init(char *file_name, bool showTimestamp, cloggerLVL lvl){
    timestamp = showTimestamp;
    level = lvl;
    if (file_name != NULL){
        FILE* temp = fopen(file_name, "aa");
     	if (temp != NULL){
     	    output_file = temp;
     		style = false;
     	} else {
     		fprintf(stderr, "No such file (%s)", file_name);
        }
    } else {
        output_file = stdout;
    }
    q.first = NULL;
    q.last = NULL;
}

/**
 * @returns {char *} current date and time
 */
char* get_date_time(){
    time_t rawtime = time(NULL);
    if (rawtime == -1) {
        fprintf(stderr, "The time() function failed");
        return "";
    }
    
    struct tm *ptm = localtime(&rawtime);
    if (ptm == NULL) {
        fprintf(stderr, "The localtime() function failed");
        return "";
    }
    
    char *res = malloc(sizeof(char) * 256);
    if(style) {
     	 sprintf(res, "\033[1;90m[%04d-%02d-%02d %02d:%02d:%02d]\033[0;37m", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    } else {
     	 sprintf(res, "[%04d-%02d-%02d %02d:%02d:%02d]", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    }
    return res;
}

/**
 * Queues a message to the message queue
 * @param {char *} msg the message to queue
 * @param {va_list} args additional arguments to format the text
 */
void queue_up(char *msg, va_list args){
    struct message* m_msg = malloc(sizeof(struct message));
    
    if(timestamp){
        char *dt = get_date_time();
     	strcpy(m_msg->content, strcat(dt, msg));	
     	free(dt);
    }else {
     	strcpy(m_msg->content, msg);	
    }
    
    char tem[256];
    vsprintf(tem, m_msg->content, args);
    strcpy(m_msg->content, tem);
    
    if(q.last == NULL && q.first == NULL){
     	m_msg->prev = NULL;
     	q.first = m_msg;
     	q.last = m_msg;
    }else{
     	m_msg->prev = NULL;
     	q.last->prev = m_msg;
     	q.last = m_msg;
    }
}

/**
 * Logs a fatal level message to the standard output or to a log file 
 * @param {char *} m message to log
 * @param {any} ... additional parameters to format the text 
 */
void LOG_FATAL(char *m, ...){
    if(level <= CLOGGER_FATAL) {
        va_list args;
        va_start(args, m);
        
        if(style){
            char temp[256] = "";
         	strcat(temp, FATAL_PREFIX);
         	queue_up(strcat(temp, m), args);
        }else{
         	char temp[256] = "";
         	strcat(temp, "[FTL]: ");
         	queue_up(strcat(temp, m), args);
        }
        
        va_end(args);
        flush_queue();
    }
}

/**
 * Logs an error level message to the standard output or to a log file 
 * @param {char *} m message to log
 * @param {any} ... additional parameters to format the text 
 */
void LOG_ERROR(char *m, ...){
    if(level <= CLOGGER_ERROR) {
        va_list args;
        va_start(args, m);
        
        if(style){
            char temp[256] = "";
            strcat(temp, ERROR_PREFIX);
            queue_up(strcat(temp, m), args);
        }else{
         	char temp[256] = "";
         	strcat(temp, "[ERR]: ");
         	queue_up(strcat(temp, m), args);
        }
        
        va_end(args);
        flush_queue();
    }
}

/**
 * Logs a warning level message to the standard output or to a log file 
 * @param {char *} m message to log
 * @param {any} ... additional parameters to format the text 
 */
void LOG_WARN(char *m, ...){
    if(level <= CLOGGER_WARN) {
        va_list args;
        va_start(args, m);
        if(style){
         	char temp[256] = "";
         	strcat(temp, WARN_PREFIX);
         	queue_up(strcat(temp, m), args);
        }else{
         	char temp[256] = "";
         	strcat(temp, "[WRN]: ");
         	queue_up(strcat(temp, m), args);
        }
        va_end(args);
        flush_queue();
    }
}

/**
 * Logs an info level message to the standard output or to a log file 
 * @param {char *} m message to log
 * @param {any} ... additional parameters to format the text 
 */
void LOG_INFO(char *m, ...){
    if(level <= CLOGGER_INFO) {
        va_list args;
        va_start(args, m);
        if(style){
         	char temp[256] = "";
         	strcat(temp, INFO_PREFIX);
         	queue_up(strcat(temp, m), args);
        }else{
         	char temp[256] = "";
         	strcat(temp, "[INF]: ");
         	queue_up(strcat(temp, m), args);
        }
        va_end(args);
        flush_queue();
    }
}

/**
 * Logs a debug level message to the standard output or to a log file 
 * @param {char *} m message to log
 * @param {any} ... additional parameters to format the text 
 */
void LOG_DEBUG(char *m, ...){
    if(level <= CLOGGER_DEBUG) {
        va_list args;
        va_start(args, m);
        if(style){
         	char temp[256] = "";
         	strcat(temp, DEBUG_PREFIX);
         	queue_up(strcat(temp, m), args);
        }else{
         	char temp[256] = "";
         	strcat(temp, "[DBG]: ");
         	queue_up(strcat(temp, m), args);
        }
        va_end(args);
        flush_queue();
    }
}

/**
 * Logs a tracing level message to the standard output or to a log file 
 * @param {char *} m message to log
 * @param {any} ... additional parameters to format the text 
 */
void LOG_TRACE(char *m, ...){
    if(level <= CLOGGER_TRACE) {
        va_list args;
        va_start(args, m);
        if(style){
         	char temp[256] = "";
         	strcat(temp, TRACE_PREFIX);
         	queue_up(strcat(temp, m), args);
        }else{
         	char temp[256] = "";
         	strcat(temp, "[TRC]: ");
            queue_up(strcat(temp, m), args);
        }
        va_end(args);
        flush_queue();
    }
}

/**
 * Pops a message from the queue
 * @returns {struct message*} the poped message or NULL 
 */
struct message* dequeue(){
	struct message* l = q.first;
	if(l != NULL) {
		q.first = l->prev; 
    }
	return l;
}

/**
 * Flushes the queue and displays messages
 */
void flush_queue(){
    while(q.first != NULL){
        struct message* l = dequeue();
        fprintf(output_file, "%s\n", l->content);
        free(l);
	}

    q.last = NULL;
}

/**
 * Frees all queued messages and and closes log files if any are opened
 */
void clogger_quit(){
	while(q.first != NULL){
		struct message* l = dequeue();
		free(l);
	}
	if(output_file != stdout) {
		fclose(output_file);
    }
}

#endif //CLOGGER_IMPLEMENT
