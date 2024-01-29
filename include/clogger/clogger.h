#ifndef CLOGGER_H_
#define CLOGGER_H_

#define FATAL_PREFIX "\033[1;31m[FTL]: \033[0m"
#define ERROR_PREFIX "\033[1;31m[ERR]: \033[0m"
#define WARN_PREFIX  "\033[1;33m[WAR]: \033[0m"
#define INFO_PREFIX  "\033[1;37m[INF]: \033[0m"
#define DEBUG_PREFIX "\033[1;36m[DBG]: \033[0m"
#define TRACE_PREFIX "\033[1;35m[TRC]: \033[0m"
#define GRAY_TEXT(m)\
	   "\033[1;90m"m"\033[1;0m "


void clogger_init(char *file_name, int showTimestamp);
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

#define bool int
#define true 1
#define false 0

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

void clogger_init(char *file_name, bool showTimestamp){
	   timestamp = showTimestamp;
	   if(file_name != NULL){
			 FILE* temp = fopen(file_name, "aa");
			 if(temp != NULL){
				    output_file = temp;
				    style = false;
			 }else
				    fprintf(stderr, "No such file (%s)", file_name);
	   }else
			 output_file = stdout;
	   q.first = NULL;
	   q.last = NULL;
}

char* get_date_time(){
	   time_t rawtime = time(NULL);
	   if (rawtime == -1) {
			 fprintf(stderr, "The time() function failed");
			 exit(1);
	   }

	   struct tm *ptm = localtime(&rawtime);
	   if (ptm == NULL) {
			 fprintf(stderr, "The localtime() function failed");
			 exit(1);
	   }

	   char *res = malloc(sizeof(char) * 256);
	   if(style)
			 sprintf(res, "\033[1m[%04d-%02d-%02d %02d:%02d:%02d]\033[0m", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	   else
			 sprintf(res, "[%04d-%02d-%02d %02d:%02d:%02d]", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	   return res;
}

void queue_up(char *msg, va_list args){
	   struct message* m_msg = malloc(sizeof(struct message));

	   if(timestamp){
			 char *dt = get_date_time();
			 strcpy(m_msg->content, strcat(dt, msg));	
			 free(dt);
	   }else
			 strcpy(m_msg->content, msg);	

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

void LOG_FATAL(char *m, ...){
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
}

void LOG_ERROR(char *m, ...){
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
}

void LOG_WARN(char *m, ...){
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
}

void LOG_INFO(char *m, ...){
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
}

void LOG_DEBUG(char *m, ...){
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
}

void LOG_TRACE(char *m, ...){
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
}

struct message* dequeue(){
	struct message* l = q.first;
	if(l != NULL)
		q.first = l->prev; 
	return l;
}

void flush_queue(){
	while(q.first != NULL){
		struct message* l = dequeue();
		fprintf(output_file, "%s\n", l->content);
		free(l);
	}
}

void clogger_quit(){
	while(q.first != NULL){
		struct message* l = dequeue();
		free(l);
	}
	if(output_file != stdout);
		fclose(output_file);
}

#endif //CLOGGER_IMPLEMENT
