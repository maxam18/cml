/* C My Lib general definitions
 * File: cml.h
 * Started: Wed Jan 24 12:28:49 MSK 2018
 * Author: Max Amzarakov (maxam18 _at_ gmail _._ com)
 * Copyright (c) 2018 ..."
 */

#include <time.h>

#ifndef _CML_H
#define _CML_H

#define CML_VERSION     1.5
#define CML_VERSION_STR #CML_VERSION

#ifdef CML_OSMACOS
# define CML_CLOCK_SOURCE   CLOCK_MONOTONIC_RAW_APPROX
# ifdef CML_OSMACOS_OLDER1012
#  define clock_gettime(S,D) (D)->tv_sec = time(NULL); (D)->tv_nsec = 0
# endif
#else
# define CML_CLOCK_SOURCE   CLOCK_MONOTONIC_COARSE
#endif

#endif

