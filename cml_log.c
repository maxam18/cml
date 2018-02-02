/* C My Lib logging
 * File: cml_log.c
 * Started: Wed Jan 24 11:50:19 MSK 2018
 * Author: Max Amzarakov (maxam18 _at_ gmail _._ com)
 * Copyright (c) 2018 ..."
 */

#include <cml.h>
#include <cml_log.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/time.h>
#include <stdarg.h>
#include <errno.h>

cml_log_t   _cml_log_info;

const char *_cml_log_priostr[] = 
{ "NONE", "ERROR", "CRITICAL", "WARNING", "EMERG", "NOTICE", "INFO", "DEBUG"};

#ifdef CML_LOG_LOG

void cml_log_init(unsigned char flags, const char *filename, const char *progname)
{
    bzero(&_cml_log_info, sizeof(_cml_log_info));

    cml_log_reopen(filename);


    if( progname )
        _cml_log_info.progname = progname;
    else
        _cml_log_info.progname = strdup("cml_log");

    _cml_log_info.flags = flags;

}

void cml_log_reopen(const char *filename)
{
    if( filename )
    {
        if( _cml_log_info.fd >= 0 )
            close(_cml_log_info.fd);
            
        _cml_log_info.fd = open( filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
        
        if( _cml_log_info.fd >= 0 )
            return;
        
        fprintf( stderr, "Cannot open logfile: %s Using stderr instead\n"
                    , strerror(errno));
    }
    
    _cml_log_info.fd = fileno(stderr);
}

void cml_log(unsigned char prio, char *format, ... )
{
    struct timespec   tsc;
    struct tm        *tp;
    va_list           args;
    char             *p;
    size_t            n, len;

    p   = _cml_log_info.buf;
    len = sizeof(_cml_log_info.buf) - 2;

    if( _cml_log_info.flags & CML_LOG_FLAG_TIME )
    {
        clock_gettime(CML_LOG_CLOCK_SRC, &tsc);

	tp = localtime(&tsc.tv_sec);
        n  = strftime(p, len, "%Y/%m/%d %H:%M:%S ", tp);
    
        p   += n;
        len -= n;
    }

    n = snprintf( p, len, "%s[%d]: ", _cml_log_info.progname, getpid());

    p   += n;
    len -= n;

    if( _cml_log_info.flags & CML_LOG_FLAG_PRIO 
            && prio < sizeof(_cml_log_priostr) )
    {
        n = snprintf( p, len, "%s ", _cml_log_priostr[prio]);
        
        p   += n;
        len -= n;
    }

    va_start(args, format);
    n = vsnprintf( p, len, format, args);
    va_end(args);

    p   += n;
    len -= n;

    *--p  = '\n';
    *p    = '\0';

    write( _cml_log_info.fd, _cml_log_info.buf, p - _cml_log_info.buf);
}

#endif

