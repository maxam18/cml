/* C My Lib simple config file
 * File: cml_conf_file.h
 * Started: Sat Jan 12 13:20:07 MSK 2019
 * Author: Max Amzarakov (maxam18 _at_ gmail _._ com)
 * Copyright (c) 2019 ..."
 */

#include <cml_string.h>
#include <stddef.h> /* offsetof */

#ifndef _CML_CONF_FILE_H
#define _CML_CONF_FILE_H

#define CML_CONF_FLG_INT    0x0001

typedef struct 
{
    char        *pname;
    int          plen;
    off_t        offset;
    int          flags;
    void        *func_for_future_use;
} cml_conf_params_t;

int cml_conf_file(const char *fname, cml_conf_params_t *params, void *conf);

#endif

