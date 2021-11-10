/***************************************************************************
 *
 *                 cml_string.c  - CML string manipulation
 *                 ------------------------------------------
 *   begin         : Sun Jun 29 20:58:47 MSK 2014
 *   copyright     : Copyright (c) 2014 by Maxim Amzarakov
 *   email         : maxam18 _@_ gmail _._ com
 ***************************************************************************/

#include <cml_string.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const char cml_ch_tolower[128] = {/*{{{*/
0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 
0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 
0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 
0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 
0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 
0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 
0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 
0x78, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 
0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 
0X68, 0X69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 
0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 
0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F 
};/*}}}*/

void cml_urldecode(cml_str_t *str)/*{{{*/
{
    char *ptr, *dptr, *end;
    unsigned char ch, nch=0;
    static unsigned char littlebits[] = {/*{{{*/
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF
    };/*}}}*/
    static unsigned char bigbits[] = {/*{{{*/
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF 
    };/*}}}*/
    enum {
        sw_usual,
        sw_in_quote,
        sw_quote_second
    } state;
    ptr = dptr = str->data;
    end = str->data + str->len;
    state = sw_usual;
    while( ptr < end )
    {
        ch = (unsigned char)*ptr++;
        switch( state )
        {
            case sw_usual:
                switch( ch )
                {
                    case '%' :
                        state = sw_in_quote;
                        break;
                    case '+' :
                        *dptr++ = ' ';
                        break;
                    default :
                        *dptr++ = ch;
                        break;
                }
                break;
            case sw_in_quote:
                if( (nch = bigbits[ch]) == 0xFF )
                {
                    *dptr++ = ch;
                    state = sw_usual;
                } else
                    state = sw_quote_second;
                break;
            case sw_quote_second:
                nch |= littlebits[ch];
                switch( nch )
                {
                    case '\n':
                    case '\t':
                        ch = ' ';
                        break;
                    case 0xFF: /* bad second byte in urlencode */
                        break;
                    default:
                        ch = nch;
                        break;
                }
                state = sw_usual;
                *dptr++ = ch;
                break;
        }
    }
    str->len = dptr - str->data;
}/*}}}*/

int cml_atoi(const char *ptr, const char *eptr)
{
    unsigned char ch;
    int retval = 0;
    int isneg = 0;
    if( *ptr == '-' )
    {
        isneg = 1;
        ptr++;
    }
    if( *ptr == '+' )
        ptr++;
    while( *ptr == '0' )
        ++ptr;
    ch = *ptr;
    if( !eptr )
        eptr = ptr + 10; /* int cannot be more then 10 digits */
    while( ch > '0' - 1 && ch < '9' + 1 && ptr < eptr)
    {
        retval *= 10;
        retval += ch - '0';
        ch = *++ptr;
    }

    return isneg ? -retval : retval;
}

u_char *cml_ito26(u_char *buf, uint32_t val)
{
    static u_char ibuf[] = { "AAAAAAA" };
    u_char *p;
    
    if( buf )
    {
        memcpy(buf, "AAAAAAA", sizeof("AAAAAAA") - 1);
        p = buf + sizeof("AAAAAAA") - 1;
    } else
        p = ibuf + sizeof("AAAAAAA") - 1;

    do {
        *--p = (unsigned char)('A' + val % 26);
    } while( val /= 26 );
    
    return buf ? buf : ibuf;
}

uint32_t cml_26toi(u_char *str, size_t len)/*{{{*/
{
    unsigned char c;
    uint32_t val = 0;

    while( len-- )
    {
        c = (unsigned char)*str++ | 0x20;

        if( c < 'a' || c > 'z' )
            return INT32_MIN;

        val = val*26 + (c - 'a');
    }

    return val;
}/*}}}*/

uint32_t cml_htoi(u_char *str, size_t len)
{
    unsigned char c;
    uint32_t val;
    
    if( 0 == len )
        return 0;

    for( val = 0; len--; str++ )
    {
        c = *str;
        if( c >= '0' && c <= '9'  )
        {
            val = val * 16 + (c - '0');
            continue;
        }

        c = (unsigned char) (c | 0x20);

        if( c >= 'a' && c <= 'f' )
        {
            val = val * 16 + (c - 'a' + 10);
            continue;
        }

        return 0;
    }

    return val;
}

uint64_t cml_htoll(u_char *str, size_t len)
{
    unsigned char c;
    uint64_t      val;
    
    if( 0 == len )
        return 0;
    
    for( val = 0; len--; str++ )
    {
        c = *str;
        if( c >= '0' && c <= '9'  )
        {
            val = val * 16 + (c - '0');
            continue;
        }
        
        c = (unsigned char) (c | 0x20);
        
        if( c >= 'a' && c <= 'f' )
        {
            val = val * 16 + (c - 'a' + 10);
            continue;
        }
        
        return 0;
    }
    
    return val;
}


int cml_split_line(cml_str_t *line, cml_str_t *fields, int numfields, const char sep)
{
    char *ptr, *start, *end;
    int field;
    
    ptr = start = line->data;
    end = start + line->len;
    field = 0;
    
    for( ; ptr < end; ++ptr )
    {
        if( *ptr == sep )
        {
            fields[field].data = start;
            fields[field].len  = ptr - start;
            start = ptr + 1;
            field++;
            if( field > numfields )
                return 0;
        }
    }
    fields[field].data = start;
    fields[field].len  = ptr - start;
    
    return field + 1;
}

int32_t cml_parse_time(const char *buf, int len, const char *format)
{
    int32_t ts, relts;
    struct tm tm;
    const char *p;
    
    
    /* strip spaces */
    for( p = buf + len - 1; *p == ' '; p--, len-- )
        if( len < 3 )
            break;
    
    for( p = buf; *p == ' '; p++, len--)
        if( len < 3 )
            break;
    
    
    if( len < 3 )
        return -1;
    
    /* check now and today */
    if( strncmp(p, "now", len) == 0 )
    {
        ts = (int32_t)time(NULL);
    } else if( strncmp(p, "today", len) == 0 )
    {
        ts = (int32_t)time(NULL);
        ts -= ts % (24*60*60);
    } else if( *p == '-' || *p == '+' )
    { /* relative time */
        relts = cml_atoi(p, p + len - 1);
        ts = (int32_t)time(NULL);
        switch( p[len-1] )
        {
            case 's':
            case 'S':
                ts += relts;
                break;
            case 'M':
                ts += relts * 60;
                break;
            case 'H':
                ts += relts * 60 * 60;
                break;
            case 'd':
            case 'D':
                ts += relts * 60 * 60 * 24;
                break;
            case 'w':
            case 'W':
                ts += relts * 60 * 60 * 24 * 7;
                break;
            case 'm':
                ts += relts * 60 * 60 * 24 * 30;
                break;
            case 'y':
            case 'Y':
                ts += relts * 60 * 60 * 24 * 365;
                break;
        }
    } else if( format )
    {
        strptime(p, format, &tm);
        ts = (int32_t)mktime(&tm);
    } else
        ts = -1;
    
    
    return ts;
}

cml_str_t *cml_dup_str(cml_str_t *from)
{
    cml_str_t *to = malloc(sizeof(cml_str_t));
    if( !to )
        return NULL;
    if( (to->data = malloc(from->len)) == NULL )
        return NULL;
    memcpy(to->data, from->data, from->len);
    to->len = from->len;
    
    return to;
}

inline void cml_str_tolower(cml_str_t *str)
{
    char *p  = str->data + str->len - 1;
    
    for(; p != str->data; --p )
        *p = cml_tolower(*p);
}

u_char cml_ston(cml_str_t *str, u_char *ip)
{
    return cml_aton(str->data, str->len, ip);
}

u_char cml_aton(const char *buf, size_t len, u_char *ip)
{
    const char *p, *start, *end;
    char have = 0;
    int i, j;
    
    memset(ip, 0, 16);
    
    for( p = (buf + len); p > buf; --p )
    {
        if( *p == '.' )
            have |= CML_STR_IP_FLAG_DOT;
        else if( *p == ':' )
        {
            have |= CML_STR_IP_FLAG_COLON;
            break;
        }
    }
    
    if( !(have & CML_STR_IP_FLAG_ANY) )
        return 1;

    end = p = buf + len;
    i = 0;
    
    /* extract rightmost numbers */
    if( have & CML_STR_IP_FLAG_DOT )
    {
        for(; p > buf; --p )
                if( *p == '.' )
                {
                    ip[i] = cml_atoi(p+1, end );
                    end = p;
                    if( ++i == 3 )
                        break;
                }
    }
    if( have & CML_STR_IP_FLAG_COLON )
    {
        while( p > buf )
            if( *p-- == ':' )
            {
                if( *p == ':' )
                { /* start from the beginning */
                    break;
                }
                ip[i] = cml_atoi(p + 2, end);
                if( ++i == 7 )
                    break;
            }
        
        if( i < 7 )
        {
            end = p;
            j = 0;
            for(start = buf; p < end; ++p )
                if( *p == ':' )
                {
                    ip[j++] = cml_atoi(start, p);
                    if( j == i )
                        return 1; /* parse error */
                    start = p + 1;
                }
        }

    }
    
    ip[i] = cml_atoi(buf, p);

    return (i == 3 || i == 7 ) ? 0 : 1;
}

u_char cml_ntoa(cml_str_t *str, uint32_t ip)
{
    u_char *ipc = (u_char *)&ip;

#ifndef CML_ENDIAN_BIG
    return snprintf(str->data, str->len, "%03d.%03d.%03d.%03d", ipc[3], ipc[2], ipc[1], ipc[0]);
#else
    return snprintf(str->data, str->len, "%03d.%03d.%03d.%03d", ipc[0], ipc[1], ipc[3], ipc[3]);
#endif
}

void *cml_alloc_assert(size_t n)
{
    void *p = malloc(n);
    
    if( p == NULL )
        abort();
    
    return p;
}

void *cml_calloc_assert(size_t n)
{
    void *p = malloc(n);
    
    if( p == NULL )
        abort();
    
    bzero(p, n);

    return p;
}

void *cml_memdup(void *src, size_t n)
{
    void *p = malloc(n);
    
    if( p == NULL )
        abort();
    
    if( memcpy(p, src, n) == NULL )
        abort();

    return p;
}

const char *cml_time_period_name(time_t from, time_t to)
{
    time_t diff = to - from;
    
    if( diff > 60 && diff < 60*60 )
        return "hour";
    if( diff < 60*60*24 )
        return "day";
    else if( diff < 60*60*24*7 )
        return "week";
    else if( diff < 60*60*24*31 )
        return "month";
    else if( diff < 60*60*24*366 )
        return "year";

    return "undef";
}

char *cml_strrtime(time_t timestamp, char *buf)
{
    char  buffer[64];
    char *p = buf ? buf : buffer;
    struct tm ts;
    
    localtime_r(&timestamp, &ts);
    
    snprintf(p, sizeof(buffer), "%04d%02d%02d%02d%02d%02d"
             , ts.tm_year + 1900, ts.tm_mon, ts.tm_mday
             , ts.tm_hour, ts.tm_min, ts.tm_sec);
    
    return p;
}
/* vim>500: set foldmethod=marker:
 * vim: set ts=4:
 * vim: set cindent:
 * vim: set autoindent:
 * vim: set shiftwidth=4: */
