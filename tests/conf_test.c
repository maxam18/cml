/* C My Lib conf file test
 * File: conf_test.c
 * Started: Sat Jan 12 14:05:19 MSK 2019
 * Author: Max Amzarakov (maxam18 _at_ gmail _._ com)
 * Copyright (c) 2019 ..."
 */

#include <cml_conf_file.h>

struct my_data
{
    char    *strval;
    int      intval;
};

int main(int argc, char **argv)
{
    struct my_data    conf;
    cml_conf_param_t  conf_params[] = {
                {     "string_parameter"
                    , offsetof(struct my_data, strval)
                    , 0
                    , NULL
                },
                {     "int_parameter"
                    , offsetof(struct my_data, intval)
                    , CML_CONF_FLG_INT
                    , NULL
                }
                , { NULL, 0, 0, NULL }
            };

    conf.strval = argv[0];
    conf.intval = 9999;

    if( cml_conf_file((argc > 1 ? argv[1] : "/mnt/hdd/max/Work/Devel/cml/tests/test.conf")
                      , conf_params, &conf) )
        return -1;

    fprintf( stdout, "String param value: %s\n"
                     "Numeric param value: %d\n"
                    , conf.strval
                    , conf.intval );

    return 0;
}

