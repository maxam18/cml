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

#define CML_LOG_FLAG_TIME   0x01 /* print time */
#define CML_LOG_FLAG_PRIO   0x02 /* print prio string */
#define CML_LOG_FLAG_DUP    0x04 /* redirect stderr, stdout */

#define CML_LOG_STDERR_FD   2
#define CML_LOG_CLOCK_SRC   CLOCK_REALTIME

typedef struct cml_log_s cml_log_t;
struct cml_log_s
{
    int              fd;
    unsigned char    flags;
    unsigned char    level;
    unsigned char    name_len;
    char             name[32];
};

#ifndef NDEBUG 
#define cml_log_debug(args...)   cml_log( CML_LOG_DEBUG, ##args)
#else
#define cml_log_debug(args...)
#endif

#ifndef CML_LOG_LOG
#include <stdio.h>

#define cml_log_init(A, B, C)
#define cml_log(L, F, args...) fprintf(stderr, F "\n", ##args)
#define cml_log_reopen(A)
#define cml_log_close()

#else
/* some sophisticated stuff here. Like logging to the syslog */
extern cml_log_t _cml_log_info;
/**
 * \brief 
 *      open logfile and set flags and progname
 *      redirect stdout and stderr to <filename> if specified
 * \param flags - flags to set
 * \param filename - log file name path
 * \param progname  - progname to output if set
 */
void cml_log_init(unsigned char flags, const char *filename, const char *progname);
/** \brief 
 *      Set log level. If prio higher than level cml_log doesn't print
 *  \param level - highest priority level to print
 */
void cml_log_set_level(unsigned char level);
void cml_log(unsigned char prio, char *format, ...);
#define cml_log_close()   close(_cml_log_info.fd)

#endif

#ifndef NDEBUG

#define cml_d(F, args...)       fprintf( stderr, F "\n", ##args)

#else

#define cml_d(F, args...)

#endif

#endif

