/* C My Lib simple config file
 * File: cml_conf_file.c
 * Started: Sat Jan 12 13:20:14 MSK 2019
 * Author: Max Amzarakov (maxam18 _at_ gmail _._ com)
 * Copyright (c) 2019 ..."
 */

#include <cml_conf_file.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cml_conf_param_t *get_entry(cml_conf_param_t *conf, const char *name, int len)
{
    int          n;
    const char  *pl, *pn;

    for(; conf->pname; conf++)
    {
        pn = name;
        pl = conf->pname;
        n  = len;
        while( *pn++ == *pl++ )
            if( --n == 0 )
                return conf;
    }

    return NULL;
}

static int check_set(cml_conf_param_t *param)
{
    int retval = 0;
    for(; param->pname; ++param)
        if( (param->flags & (CML_CONF_FLG_MUST | CML_CONF_FLG_SET))
            == CML_CONF_FLG_MUST )
        {
            fprintf( stderr, "Param '%s' not found.\n", param->pname);
            retval++;
        }

    return retval;
}

int cml_conf_file(const char *fname, cml_conf_param_t *params, void *conf)
{
    FILE              *fp;
    int                line_no, len;
    char               buf[1024], *p, ch;
    cml_conf_param_t  *param;

    if( (fp = fopen(fname, "r")) == NULL )
    {
        fprintf( stderr, "Cannot open file %s.\n", fname);
        return -1;
    }

    line_no = 0;
    while( fgets(buf, sizeof(buf), fp) )
    {
        line_no++;

        p = buf + strlen(buf) - 1;
        if( *p-- != '\n' )
            fprintf( stderr, "Line %d too long.\n", line_no);

        for( ch = *p; p > buf;)
            if( ch == ' ' || ch == '"' || ch == '\'' || ch == ';' )
                ch = *--p;
            else
                break;

        *(p + 1) = 0;

        p     = buf;
        param = NULL;
        ch    = 1;
        while( (ch = *p++) != 0 )
            if( ch == ' ' )
            {
                len = p - buf - 1;
                for(ch = *p; ch != 0;)
                    if( ch == ' ' || ch == '"' || ch == '\'' )
                        ch = *++p;
                    else
                        break;

                param = get_entry(params, buf, len);
                break;
            }

        if( param == NULL )
            continue;

        if( param->flags & CML_CONF_FLG_SET )
            fprintf( stderr, "Param '%s' is duplicate at line %d.\n"
                           , param->pname, line_no);
        param->flags |= CML_CONF_FLG_SET;

        if( param->flags & CML_CONF_FLG_INT )
            *(int *)(conf + param->offset)   = strtol(p, NULL, 10);
        else
            *(char **)(conf + param->offset) = strdup(p);
    }

    fclose(fp);
    return check_set(params);
}

void cml_conf_file_free(cml_conf_param_t *param, void *conf)
{
    for( ; param->pname; ++param )
        if( (param->flags & (CML_CONF_FLG_SET | CML_CONF_FLG_INT))
             == CML_CONF_FLG_SET )
            free(conf + param->offset);
}

