//
//  cml_pool.h
//  CML memory pool management
//
//  Created by Max Amzarakov on 05.10.14.
//  Copyright (c) 2014 Uptime Technology. All rights reserved.
//

#ifndef CML_POOL_H
#define CML_POOL_H

#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>

#define CML_POOL_FLAG_CLEAN   0x01
#define CML_POOL_FLAG_DIRTY   0x02
#define CML_POOL_FLAG_BIGPOOL 0x04

#define CML_POOL_CHUNK_SIZE   16 * 4096

typedef struct cml_pool_map_s cml_pool_map_t;

struct cml_pool_map_s {
    void            *map;            /** poolmap buffer */
    size_t           size;           /** allocated bytes in the buffer */
    u_char           flag;           /** poolmap flags (CLEAN, DIRTY, BIGPOOL) */
    cml_pool_map_t  *next;           /** pointer to the next (previous) pool buffer */
};

typedef struct cml_pool_s cml_pool_t;

struct cml_pool_s {
    void        *end;                /** pointer to the end of the poolmap buffer */
    void        *current;            /** pointer of the current position in the poolmap buffer */

    uint32_t         count;          /** number of pools. Not in use for now*/
    size_t           allocated_size; /** total memory size of all allocated segments */
    cml_pool_map_t  *mempool;        /** pointer to the current poolmap */
};

#define cml_pool_init(P)    bzero(P, sizeof(cml_pool_t))

/**
 \brief get new mem block from the pool
 the pool doesn't have to be initialized. This must be zeroed
 */
void *cml_pool_get( cml_pool_t *pool, size_t size );

void cml_pool_clear( cml_pool_t *pool);

/**
 \brief deallocate mappings if found
 */
void cml_pool_destroy( cml_pool_t *pool );
/**
 \brief print memory usage report
 */
void cml_pool_report( cml_pool_t *pool, const char *verb );

#endif
