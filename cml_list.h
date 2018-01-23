/* C My Lib list implementations
 * File: cml_list.h
 * Started: Fri Jan 19 12:42:41 MSK 2018
 * Author: Max Amzarakov (maxam18 _at_ gmail _._ com)
 * Copyright (c) 2018 ..."
 */

#ifndef _CML_LIST_H
#define _CML_LIST_H

typedef struct cml_vec cml_vec_t;

struct cml_slist {
    void                *elem;
    struct cml_vec_t    *next;
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

#define CML_ILIST_ENTRY(L,T,N)  (T *)(L - offsetof(T,N))
#define CML_LIST_INIT(L)        (L)->next = (L)->prev = L
#define CML_LIST_ADD(L,E)       (E)->next       = L; \
                                (E)->prev       = (L)->prev; \
                                (L)->prev->next = E; \
                                (L)->prev       = E
#define CML_LIST_REMOVE(L)      (L)->prev->next = (L)->next; \
                                (L)->next->prev = (L)->prev
#define CML_LIST_LOOP           CML_ILIST_LOOP
#define CML_ILIST_LOOP(H,L)     for( L = (H)->next \
                                    ; L != (H) \
                                    ; L = L->next )

void cml_ilist_move_all(cml_ilist_t *from, cml_ilist_t *to);
void cml_list_move_all(cml_list_t *from, cml_list_t *to);

#endif

