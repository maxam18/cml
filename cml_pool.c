//
//  cml_pool.c
//  CML memory pool 
//
//  Created by Max Amzarakov on 05.10.14.
//  Copyright (c) 2014 Uptime Technology. All rights reserved.
//

#include <cml_pool.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

void *cml_pool_get( cml_pool_t *pool, size_t size )
{
    cml_pool_map_t *poolmap;
    
    void       *current = pool->current;
    
 
    pool->current += size;
    
    if( pool->current >= pool->end )
    {

        /* try to find clean map */

        if( !(poolmap = malloc(sizeof(*poolmap))) )
        {
            cml_log_log(CML_LOG_ERR, "Cannot allocate memory for map.");
            abort();
        }
        
        poolmap->next = pool->mempool;
        pool->mempool = poolmap;
        
        poolmap->flag = CML_POOL_FLAG_DIRTY;
        
        poolmap->size = CML_POOL_CHUNK_SIZE;

        /* check is that too much? */
        if( size > pool->mempool->size )
        { /* allocate big block */
            poolmap->size = size;
            poolmap->flag |= CML_POOL_FLAG_BIGPOOL;
        }
        
        if( (poolmap->map = malloc(poolmap->size)) == NULL )
        {
            cml_log_log(CML_LOG_ERR, "Cannot open new map for %zd bytes."
                    , poolmap->size);
            abort();
        } else
        {
            bzero(poolmap->map, poolmap->size);
        }
                
        pool->current = poolmap->map;
        pool->end     = poolmap->map + poolmap->size;

        current        = pool->current;
        pool->current += size;

        pool->allocated_size += poolmap->size;
    }
    
    return current;
}

void cml_pool_destroy( cml_pool_t *pool )
{
    cml_pool_map_t *poolmap = pool->mempool, *tmap;

    while( poolmap )
    {
        free(poolmap->map);

        tmap    = poolmap;
        poolmap = poolmap->next;

        free(tmap);
    }

    bzero(pool, sizeof(*pool));
}

void cml_pool_clear( cml_pool_t *pool )
{
    cml_pool_map_t *poolmap = pool->mempool;
    
    while( poolmap )
    {
        bzero(poolmap->map, poolmap->size);
        poolmap->flag ^= CML_POOL_FLAG_DIRTY;
        poolmap->flag |= CML_POOL_FLAG_CLEAN;
        poolmap = poolmap->next;
    }
    pool->count   = 0;
    pool->current = poolmap->map;
    pool->end     = poolmap->map + poolmap->size;
}

void cml_pool_report( cml_pool_t *pool, const char *verb)
{
    uint32_t max, chunks;
    cml_pool_map_t *mempool = pool->mempool;
    for( chunks = max = 0; mempool; chunks++ )
    {
        if( mempool->size > max )
            max = (uint32_t)mempool->size;
        mempool = mempool->next;
    }

    if( verb )
    {
        cml_log_log(CML_LOG_INFO
            , "%s %" PRIu32 " bytes allocated in %" PRIu32 " chunks of %" PRIu32 " bytes max.\n"
            , verb, (uint32_t)pool->allocated_size, chunks, max);
    } else
    {
        cml_log_log(CML_LOG_INFO
                    , "%" PRIu32 " bytes allocated in %" PRIu32 " chunks of %" PRIu32 " bytes max.\n"
                    , (uint32_t)pool->allocated_size, chunks, max);
    }
}
