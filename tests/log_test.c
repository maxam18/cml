/* C My Lib log test
 * File: log_test.c
 * Started: Fri Feb  2 20:58:04 MSK 2018
 * Author: Max Amzarakov (maxam18 _@_ gmail _._ com)
 * Copyright (c) 2018 ...
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include <cml_log.h>

int main(int argc, char **argv)
{
    pid_t   pid;
    char    filename[] = "/tmp/test.log";
    char    newfname[] = "/tmp/test.log.1";
    char    progname[] = "test_prog";

    fprintf( stdout, "Log test. Read output at /tmp/test.log /tmp/test.log.1\n");

    cml_log_init(CML_LOG_FLAG_PRIO | CML_LOG_FLAG_TIME, filename, progname);

    cml_log(CML_LOG_NOTICE, "test notification");
    cml_log(CML_LOG_NOTICE, "test reopen");
    cml_log(CML_LOG_INFO, "Log reopened");
    cml_log_close();

    if( argc > 1 )
    {
        signal(SIGCHLD, SIG_IGN);

        pid = fork();
        if( pid < 0 )
        {
            perror("daemonize");
            return -1;
        }
        if( pid )
        {
            fprintf( stdout, "Child running at %d.\n", pid);
            return 0;
        }

        if( setsid() < 0 )
        {
            perror("create session");
            return -1;
        }

        fclose(stdin);
    }
    fprintf( stderr, "Stderr out redirecting to /tmp/test.log\n");
    fprintf( stdout, "Stdout out redirecting to /tmp/test.log\n");
    cml_log_init(CML_LOG_FLAG_PRIO | CML_LOG_FLAG_TIME | CML_LOG_FLAG_DUP, filename, progname);

    cml_log( CML_LOG_INFO, "Log opened");
    fprintf( stderr, "Print line to stderr before reopen.\n");
    fprintf( stdout, "Print line to stdout before reopen.\n");
    rename(filename, newfname);

    cml_log(CML_LOG_INFO, "Log reopened");
    fprintf( stderr, "Print line to stderr after reopen.\n");
    fprintf( stdout, "Print line to stdout after reopen.\n");
    cml_log_close();

    return 0;
}
