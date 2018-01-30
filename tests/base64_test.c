/* C My Lib base64 test
 * File: base64_test.c
 * Started: Tue Jan 30 14:56:18 MSK 2018
 * Author: Max Amzarakov (maxam18 _at_ gmail _._ com)
 * Copyright (c) 2018 ..."
 */

#include <cml_base64.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

int main(void)
{
    unsigned char   buf[1024], encbuf[1024], decbuf[1024];
    size_t len;

    bzero(&buf, sizeof(buf));

    memcpy(buf, "1234567890", sizeof("12345") - 1);
    len = 5;

    fprintf( stdout, "Initial string: [%d] = '%.*s'\n"
            , (int)len, (int)len, buf);

    len = cml_base64_encode(buf, len, encbuf);
    fprintf( stdout, "Encoded string: [%d] = '%.*s'\n"
            , (int)len, (int)len, encbuf);

    len = cml_base64_decode(encbuf, len, decbuf);
    fprintf( stdout, "Decoded string: [%d] = '%.*s'\n"
            , (int)len, (int)len, decbuf);

    return 0;
}
