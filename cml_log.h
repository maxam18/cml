/* CML logging
 * File: cml_log.h
 * Started: Thu Jan 18 12:38:38 MSK 2018
 * Author: Max Amzarakov (maxam18 _at_ gmail _._ com)
 * Copyright (c) 2018 ..."
 */

#ifndef _CML_LOG_H
#define _CML_LOG_H

#define CML_LOG_ERR         1
#define CML_LOG_CRIT        2
#define CML_LOG_WARN        3
#define CML_LOG_EMERG       4
#define CML_LOG_NOTICE      5
#define CML_LOG_INFO        6
#define CML_LOG_DEBUG       7

#define CML_LOG_ANY         8

#ifndef CML_LOG_LOG

#define cml_log_log(L, F, args...) fprintf(stderr, F "\n", ##args)

#else
/* some sophisticated stuff here. Like logging to the syslog */
#endif

#endif

