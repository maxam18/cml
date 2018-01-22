/***************************************************************************
 *
 *                 base64.h  -  MIME base64 functions
 *                 ------------------------------------------
 *   begin         : Sun Feb  9 16:27:09 MSK 2003
 *   copyright     : Copyright (c) 2003-2017 by Max Amzarakov
 *   email         : maxam18 _at_ gmail _dot_ com
 *   version       : $Id$
***************************************************************************/

#include <sys/types.h>

#ifndef CML_BASE64_H
#define CML_BASE64_H

/**
 * \brief encodes to base64
 * \param str - source char buffer
 * \param len - len of the source
 * \peram buf - initilized output buffer of enough size
 * \return returns output string len including ending '\0'
 */
int cml_base64_encode(const unsigned char *str, size_t len, unsigned char *buf);

/**
 * \brief encodes to base64
 * \param str - source char buffer
 * \param len - len of the source. Should be more than str len if null-terminated.
 * \peram buf - initilized output buffer of enough size
 * returns output buflen including ending '\0'
 */
int cml_base64_decode(const unsigned char *str, size_t len, unsigned char *buf);

#endif

