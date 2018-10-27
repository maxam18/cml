/* C My Lib list implementations
 * File: cml_list.h
 * Started: Fri Jan 19 12:42:41 MSK 2018
 * Author: Max Amzarakov (maxam18 _at_ gmail _._ com)
 * Copyright (c) 2018 ..."
 */

#ifndef _CML_LIST_H
#define _CML_LIST_H

#include <stddef.h>

typedef struct cml_vec cml_vec_t;

struct cml_vec {
    void                *elem;
    struct cml_vec      *next;
};

typedef struct cml_list cml_list_t;

struct cml_list {
    void                *elem;
    struct cml_list     *next;
    struct cml_list     *prev;
};

typedef struct cml_ilist cml_ilist_t;

struct cml_ilist {
    struct cml_ilist    *next;
    struct cml_ilist    *prev;
};

#ifndef offsetof
#define offsetof(s,m) (size_t)&(((s *)0)->m)
#endif

#define CML_ILIST_ENTRY(L,T,N)  (T *)((char *)L - (offsetof(T,N)))
#define CML_LIST_INIT(L)        (L)->next = (L)->prev = L
#define CML_ILIST_INIT          CML_LIST_INIT

#define CML_LIST_ADD(L,N)       (N)->prev       = L; \
                                (N)->next       = (L)->next; \
                                (L)->next->prev = N; \
                                (L)->next       = N
#define CML_ILIST_ADD           CML_LIST_ADD
#define CML_LIST_REMOVE(L)      (L)->prev->next = (L)->next; \
                                (L)->next->prev = (L)->prev
#define CML_ILIST_REMOVE        CML_LIST_REMOVE
#define CML_LIST_LOOP(H,L)      for( L = (H)->next \
                                    ; L != (H) \
                                    ; L = L->next )
#define CML_ILIST_LOOP          CML_LIST_LOOP
#define CML_LIST_LOOP_BW(H,L)   for( L = (H)->prev \
                                    ; L != (H) \
                                    ; L = L->prev )
#define CML_ILIST_LOOP_BW       CML_LIST_LOOP_BW

#define CML_LIST_MOVE_ALL(F,T)  if( (F)->next != (F) ) { \
                                (F)->next->prev = (T);\
                                (F)->prev->next = (T)->next;\
                                (T)->next->prev = (F)->prev;\
                                (T)->next       = (F)->next;\
                                (F)->prev = (F)->next = (F); }
#define CML_ILIST_MOVE_ALL      CML_LIST_MOVE_ALL


void cml_ilist_move_all(cml_ilist_t *from, cml_ilist_t *to);
void cml_list_move_all(cml_list_t *from, cml_list_t *to);

#endif

