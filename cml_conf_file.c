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

cml_conf_params_t *get_entry(cml_conf_params_t *conf, const char *name, int len)
{
    int          n;
    const char  *pl, *pn;

    for(; conf->pname; conf++)
    {
        if( conf->plen != len )
            continue;

        n  = len;
        pn = name;
        pl = conf->pname;
        for(; n; n-- )
            if( *pn++ != *pl++ )
                break;

        if( n == 0 )
            return conf;
    }

    return NULL;
}

int cml_conf_file(const char *fname, cml_conf_params_t *params, void *conf)
{
    FILE              *fp;
    int                line_no, len;
    char               buf[1024], *p, ch;
    cml_conf_params_t *param;

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

        if( param->flags & CML_CONF_FLG_INT )
            *(int *)(conf + param->offset)   = strtol(p, NULL, 10);
        else
            *(char **)(conf + param->offset) = strdup(p);
    }

    fclose(fp);
    return 0;
}

