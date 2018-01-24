/* C My Lib general definitions
 * File: cml.h
 * Started: Wed Jan 24 12:28:49 MSK 2018
 * Author: Max Amzarakov (maxam18 _at_ gmail _._ com)
 * Copyright (c) 2018 ..."
 */

#include <time.h>

#ifndef _CML_H
#define _CML_H


#ifdef CML_OSMACOS
#   define CML_CLOCK_SOURCE   CLOCK_MONOTONIC_RAW_APPROX
#else
#   define CML_CLOCK_SOURCE   CLOCK_MONOTONIC_COARSE
#endif

#endif

