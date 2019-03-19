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

const char *_cml_log_priostr[] = 
{ "NONE", "ERROR", "CRITICAL", "WARNING", "EMERG", "NOTICE", "INFO", "DEBUG"};

cml_log_t   _cml_log_info = { CML_LOG_STDERR_FD, 0, sizeof(_cml_log_priostr)-1, 0, { '\0' } };


#ifdef CML_LOG_LOG
static void cml_log_reopen(const char *filename);

void cml_log_init(unsigned char flags, const char *filename, const char *progname)
{
    bzero(&_cml_log_info, sizeof(_cml_log_info));

    _cml_log_info.fd    = -1;
    _cml_log_info.flags = flags;

    cml_log_reopen(filename);

    if( progname )
        _cml_log_info.name_len = snprintf(_cml_log_info.name,  sizeof(_cml_log_info.name)
                    , "%s[%d]: ", progname, getpid());

}

void cml_log_set_level(unsigned char level)
{
    _cml_log_info.level = level;
}

static void cml_log_reopen(const char *filename)
{
    int fd;

    if( filename )
    {
        fd = open( filename, O_CREAT | O_WRONLY | O_APPEND, 0644);

        if( _cml_log_info.fd >= 0 )
        {
            close(_cml_log_info.fd);
            dup2(fd, _cml_log_info.fd );
            close(fd);
        } else
            _cml_log_info.fd = fd;

        if( _cml_log_info.fd >= 0 )
        {
            if( _cml_log_info.flags & CML_LOG_FLAG_DUP )
            {
                close( fileno(stdout) );
                close( fileno(stderr) );
                dup2( _cml_log_info.fd, fileno(stdout) );
                dup2( _cml_log_info.fd, fileno(stderr) );
            }
            return;
        }
                
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
    char              buf[2048];

    if( prio > _cml_log_info.level )
        return;

    p   = buf;
    len = sizeof(buf) - 2;

    if( _cml_log_info.flags & CML_LOG_FLAG_TIME )
    {
        clock_gettime(CML_LOG_CLOCK_SRC, &tsc);

        tp = localtime(&tsc.tv_sec);
        n  = sprintf(p, "%4d/%02d/%02d %02d:%02d:%02d "
                            , tp->tm_year + 1900, tp->tm_mon + 1, tp->tm_mday 
                            , tp->tm_hour, tp->tm_min, tp->tm_sec );
    
        p   += n;
        len -= n;
    }

    if( _cml_log_info.name_len )
    {
        memcpy(p, _cml_log_info.name, _cml_log_info.name_len);
        p += _cml_log_info.name_len;
        n -= _cml_log_info.name_len;
    }

    if( _cml_log_info.flags & CML_LOG_FLAG_PRIO )
    {
        n = snprintf( p, len, "%s ", _cml_log_priostr[prio]);
        
        p   += n;
        len -= n;
    }

    va_start(args, format);
    n = vsnprintf( p, len, format, args);
    va_end(args);

    p    += n;
    len  -= n;

    *p++  = '\n';
    *p    = '\0';

    write( _cml_log_info.fd, buf, p - buf);
}

#endif

