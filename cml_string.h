/***************************************************************************
 *
 *                 cml_string.h  -  CML string declaration and manipulation
 *                 ------------------------------------------
 *   begin         : Sun Jun 29 20:58:47 MSK 2014
 *   copyright     : Copyright (c) 2014 by Maxim Amzarakov
 *   email         : maxam18 _@_ gmail _._ com
 ***************************************************************************/

#ifndef CML_STRING_H
#define CML_STRING_H

#include <sys/types.h>
#include <inttypes.h>
#include <stdint.h>

#define CML_STRING_BUF_MAX          8192 /* need some idea how big string buffer is :o) */
#define CML_MAXPATHLEN              1024
#define cml_a2string(A)             #A

#define CML_STR_IP_FLAG_COLON       0x01
#define CML_STR_IP_FLAG_DOT         0x02
#define CML_STR_IP_FLAG_ANY         (CML_STR_IP_FLAG_COLON | CML_STR_IP_FLAG_DOT)

typedef struct cml_buf_s {
    char    *buf; /** allocated buffer */
    char    *pos; /** current position in the buffer */
    char    *end; /** end of the buffer */
    size_t   size; /** buffer size */
} cml_buf_t;

typedef struct {
    char     *data;
    size_t    len;
} cml_sstr_t;

typedef struct {
    char     *data;
    int16_t   len;
} cml_str_t;

extern const char cml_ch_tolower[];
#define cml_tolower(X)   cml_ch_tolower[(int)X];

void cml_str_tolower(cml_str_t *str);
void cml_urldecode(cml_str_t *str);
int cml_atoi(const char *ptr, const char *eptr);
int32_t cml_26toi(u_char *str, size_t len);
u_char *cml_ito26(u_char *buf, uint32_t val);
uint32_t cml_htoi(u_char *str, size_t len);
uint64_t cml_htoll(u_char *str, size_t len);

/**
 * \brief extracts fields from the string pointed by line
 * \param line      - pointer to the string line
 * \param fields    - pointer to the initialized fileds[numfields] zero filled memory buffer
 * \param numfields - number of fields to find in the line
 * \param sep       - field separator character
 * \return number of fields extracted
 */
int cml_split_line(cml_str_t *line, cml_str_t *fields, int numfields, const char sep);

int32_t cml_parse_time(const char *buf, int len, const char *format);
#define cml_parse_time_str_t(s) cml_parse_time(s->data, s->len, 0)
#define cml_parse_time_str(s) cml_parse_time(s, (int)strlen(s)-1, 0)

#define cml_str_dup(GSTR, STR, LEN) GSTR.data = strdup(STR); GSTR.len = LEN
cml_str_t *cml_dup_str(cml_str_t *);

u_char cml_aton(const char *buf, size_t len, u_char *ip);
u_char cml_ston(cml_str_t *str, u_char *ip);
u_char cml_ntoa(cml_str_t *str, uint32_t ip);

void *cml_alloc_assert(size_t n);
void *cml_calloc_assert(size_t n);
void *cml_memdup(void *, size_t n);
//void cml_free(void *);
#define cml_free(B) free(B)

/**
 \brief period name maker
 \return const char of period (hour, day, week, month, year)
*/
const char *cml_time_period_name(time_t from, time_t to);

/**
 \brief print time in format YYYYmmDDHHMMSS
 \param buf - print to buf (optional)
 \return buf or pointer to internal buffer
 */
char *cml_strrtime(time_t timestamp, char *buf);

#define CML_CHECK_STRU_NIL_SET(A, B, C)     if( !(A)->C ) (A)->C = (B)->C

#ifdef HAVE_BYTESWAP_H
#   include <byteswap.h>
#else
#   define  bswap_16(value)  ((((value) & 0xff) << 8) | ((value) >> 8))
#   define  bswap_32(value)  \
(((uint32_t)bswap_16((uint16_t)((value) & 0xffff)) << 16) | \
(uint32_t)bswap_16((uint16_t)((value) >> 16)))
#   define  bswap_64(value) \
(((uint64_t)bswap_32((uint32_t)((value) & 0xffffffff)) << 32) \
| (uint64_t)bswap_32((uint32_t)((value) >> 32)))
#endif

#ifndef NDEBUG
#include <stdio.h>
#   define CML_DEBUG1(A)              fprintf( stderr, A)
#   define CML_DEBUG2(A, B)           fprintf( stderr, A, B)
#   define CML_DEBUG3(A, B, C)        fprintf( stderr, A, B, C)
#   define CML_DEBUG4(A, B, C, D)     fprintf( stderr, A, B, C, D)
#   define CML_DEBUG5(A, B, C, D, E)  fprintf( stderr, A, B, C, D, E)
#else
#   define CML_DEBUG1(A)
#   define CML_DEBUG2(A, B)
#   define CML_DEBUG3(A, B, C)
#   define CML_DEBUG4(A, B, C, D)
#   define CML_DEBUG5(A, B, C, D, E)
#endif /* NDEBUG */


#endif

