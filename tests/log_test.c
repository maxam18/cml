/* C My Lib log test
 * File: log_test.c
 * Started: Fri Feb  2 20:58:04 MSK 2018
 * Author: Max Amzarakov (maxam18 _@_ gmail _._ com)
 * Copyright (c) 2018 ...
 */

#include <cml_log.h>

int main(void)
{
    char filename[] = "/tmp/test.log";
    char progname[] = "test_prog";

    cml_log_init(CML_LOG_FLAG_PRIO | CML_LOG_FLAG_TIME, filename, progname);

    cml_log(CML_LOG_NOTICE, "test notification");
    cml_log(CML_LOG_NOTICE, "test reopen");
    cml_log_reopen(filename);
    cml_log(CML_LOG_INFO, "Log reopened");

    return 0;
}
