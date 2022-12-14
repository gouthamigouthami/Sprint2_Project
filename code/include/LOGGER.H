#ifndef __Logger_H__
#define __Logger_H__

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#else
#include <time.h>
#include<stdio.h>
#include <string.h>
#include <stdarg.h>
#include<unistd.h>


#endif
#define LOG_FILE_NAME "logger.txt"
static FILE *logfile=NULL;
void LOG_INIT1(void)
{
   
    if (access(LOG_FILE_NAME, F_OK) == 0)
    {
        //printf("enter1");
        logfile = fopen(LOG_FILE_NAME,"a+");
        
    }
    else{
        logfile = fopen(LOG_FILE_NAME,"a");
         //printf("enter2");
    }
}
void LOG_DEINIT1(void){
    fclose(logfile);
}
void log(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    va_start(args, fmt);
    vfprintf(logfile, fmt, args);
    va_end(args);
}
// === auxiliar functions
static inline char *timenow();

#define _FILE strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__

#define NO_LOG          0x00
#define ERROR_LEVEL     0x01
#define INFO_LEVEL      0x02
#define DEBUG_LEVEL     0x03

#ifndef LOG_LEVEL
#define LOG_LEVEL   DEBUG_LEVEL
#endif

#ifdef __OBJC__

#if __has_feature(objc_arc)
#define AUTORELEASEPOOL_BEGIN   @sftp {
#define AUTORELEASEPOOL_END     }
#define RELEASE(OBJ)            OBJ = nil
#else
#define AUTORELEASEPOOL_BEGIN   NSAutoreleasePool *_pool = [[NSAutoreleasePool alloc] init];
#define AUTORELEASEPOOL_END     [_pool release];
#define RELEASE(OBJ)            [OBJ release];
#endif

#define PRINTFUNCTION(format, ...)      objc_print(@format, __VA_ARGS__)
#else
///#define PRINTFUNCTION(format, ...)      fprintf(stderr, format, __VA_ARGS__)
#define PRINTFUNCTION(fmt, ...)      do {                                       \
                                             LOG_INIT1();                          \
                                             fprintf(stderr, fmt, __VA_ARGS__);  \
                                             fprintf(logfile, fmt, __VA_ARGS__);  \
                                             LOG_DEINIT1();                          \
                                         } while (0)
#endif

#define LOG_FMT             "@sftp %s | %-7s | %-15s | %s:%d | "
#define LOG_ARGS(LOG_TAG)   timenow(), LOG_TAG, _FILE, __FUNCTION__, __LINE__

#define NEWLINE     "\n"

#define ERROR_TAG   "ERROR"
#define INFO_TAG    "INFO"
#define DEBUG_TAG   "DEBUG"

#if LOG_LEVEL >= DEBUG_LEVEL
#define LOG_DEBUG(message, args...)     PRINTFUNCTION(LOG_FMT message NEWLINE, LOG_ARGS(DEBUG_TAG), ## args)
#else
#define LOG_DEBUG(message, args...)
#endif

#if LOG_LEVEL >= INFO_LEVEL
#define info(message, args...)      PRINTFUNCTION(LOG_FMT message NEWLINE, LOG_ARGS(INFO_TAG), ## args)
#else
#define info(message, args...)
#endif

#if LOG_LEVEL >= ERROR_LEVEL
#define error(message, args...)     PRINTFUNCTION(LOG_FMT message NEWLINE, LOG_ARGS(ERROR_TAG), ## args)
#else
#define error(message, args...)
#endif

#if LOG_LEVEL >= NO_LOGS
#define LOG_IF_ERROR(condition, message, args...) if (condition) PRINTFUNCTION(LOG_FMT message NEWLINE, LOG_ARGS(ERROR_TAG), ## args)
#else
#define LOG_IF_ERROR(condition, message, args...)
#endif

static inline char *timenow() {
    static char buffer[64];
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", timeinfo);
    return buffer;
}

#ifdef __OBJC__

static inline void objc_print(NSString *format, ...) {
    AUTORELEASEPOOL_BEGIN
    va_list args;
    va_start(args, format);
    NSString *logStr = [[NSString alloc] initWithFormat:format arguments:args];
    fprintf(stderr, "%s", [logStr UTF8String]);
    RELEASE(logStr);
    va_end(args);
    AUTORELEASEPOOL_END
}

#endif

#endif
