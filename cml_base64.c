/***************************************************************************
 *
 *                 base64.c  -  MIME base64 functions
 *                 ------------------------------------------
 *   begin         : Sun Feb  9 16:27:04 MSK 2003
 *   copyright     : Copyright (c) 2003-2017 by Max Amzarakov
 *   email         : maxam18 _dot_ gmail.com
 *   version       : $Id$
***************************************************************************/

#include <string.h>
#include <cml_base64.h>

static const char *table      = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
                                "abcdefghijklmnopqrstuvwxyz" \
                                "0123456789+/";
static const char gap         = '=';
static const char rtable[256] = {
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

int cml_base64_encode(const unsigned char *str, size_t len, unsigned char *buf)
{
  unsigned char *output = buf;
  
  while( len > 2 )
  {
    output[0] = table[(str[0] >> 2) & 0x3F];
    output[1] = table[((str[0] & 0x03) << 4) |
		((int) (str[1] & 0xF0) >> 4)];
    output[2] = table[((str[1] & 0x0F) << 2) |
		((int) (str[2] & 0xC0) >> 6)];
    output[3] = table[(str[2] & 0x3F)];
    output += 4;
    str	+= 3;
    len -= 3;
  }
  if( len )
  {
    *output++ = table[(str[0] >> 2) & 0x3F];
    if( len > 1 )
    {
      *output++ = table[((str[0] & 0x03) << 4) |
		  ((int) (str[1] & 0xF0) >> 4)];
      *output++ = table[((str[1] & 0xF) << 2)];
      
    } else
    {
      *output++ = table[((str[0] & 0x03) << 4)];
      *output++ = gap;
    }
    *output++ = gap;
  }
  *output = '\0';

  return (int)(output - buf);
}

int cml_base64_decode(const unsigned char *str, size_t len, unsigned char *buf)
{
  register const unsigned char *inbuf = str;
  register unsigned char *outbuf;
  register int nbytes;

  while (rtable[*inbuf++] <= 63 && len--);
  
  nbytes = (inbuf - str) - 1;

  inbuf  = str;
  outbuf = buf;

  while( nbytes > 4 )
  {
    *outbuf++ = 
      (unsigned char) ((rtable[inbuf[0]] << 2) | (rtable[inbuf[1]] >> 4));
    *outbuf++ = 
      (unsigned char) ((rtable[inbuf[1]] << 4) | (rtable[inbuf[2]] >> 2));
    *outbuf++ = 
      (unsigned char) ((rtable[inbuf[2]] << 6) | rtable[inbuf[3]]);
    inbuf  += 4;
    nbytes -= 4;
  }
  if( nbytes > 1 ) 
  {
    *outbuf++ = 
      (unsigned char) ((rtable[inbuf[0]] << 2) | (rtable[inbuf[1]] >> 4));
  }
  if( nbytes > 2 )
  {
    *outbuf++ = 
      (unsigned char) ((rtable[inbuf[1]] << 4) | (rtable[inbuf[2]] >> 2));
  }
  if( nbytes > 3 )
  {
    *outbuf++ = 
      (unsigned char) ((rtable[inbuf[2]] << 6) | rtable[inbuf[3]]);
  }
  *outbuf = '\0';

  return (int)(outbuf - buf);
}

