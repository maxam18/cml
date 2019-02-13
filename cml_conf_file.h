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
#define CML_CONF_FLG_MUST   0x0002 /* mandatory parameter rises error
                                      if absent */
#define CML_CONF_FLG_SET    0x0004

typedef struct
{
    char        *pname;
    off_t        offset;
    int          flags;
    void        *func_for_future_use;
} cml_conf_param_t;

int cml_conf_file(const char *fname, cml_conf_param_t *params, void *conf);
cml_conf_param_t *get_entry(cml_conf_param_t *conf, const char *name, int len);
void cml_conf_file_free(cml_conf_param_t *param, void *conf);

#endif

