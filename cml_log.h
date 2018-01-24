/* CML logging
 * File: cml_log.h
 * Started: Thu Jan 18 12:38:38 MSK 2018
 * Author: Max Amzarakov (maxam18 _at_ gmail _._ com)
 * Copyright (c) 2018 ..."
 */

#include <cml.h>

#ifndef _CML_LOG_H
#define _CML_LOG_H

#define CML_LOG_ERR         1
#define CML_LOG_CRIT        2
#define CML_LOG_WARN        3
#define CML_LOG_EMERG       4
#define CML_LOG_NOTICE      5
#define CML_LOG_INFO        6
#define CML_LOG_DEBUG       7

#define CML_LOG_ANY         8

#define CML_LOG_FLAG_TIME   0x01
#define CML_LOG_FLAG_PRIO   0x02

typedef struct cml_log_s cml_log_t;
struct cml_log_s
{
    int              fd;
    unsigned char    flags;
    const char      *progname;
    char             buf[2048];
};

#ifndef CML_LOG_LOG

#define cml_log_init(A, B)
#define cml_log(L, F, args...) fprintf(stderr, F "\n", ##args)

#else
/* some sophisticated stuff here. Like logging to the syslog */
extern cml_log_t _cml_log_info;
void cml_log_init(unsigned char flags, const char *filename, const char *progname);
void cml_log(unsigned char prio, char *format, ...);

#endif

#ifndef NDEBUG

#define cml_d(F, args...)       fprintf( stderr, F "\n", ##args)

#else

#define cml_d(F, args...)

#endif

#endif

