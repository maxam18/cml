/* C My Lib list implementation
 * File: cml_list.c
 * Started: Mon Jan 22 15:56:09 MSK 2018
 * Author: Max Amzarakov (maxam18 _at_ gmail _._ com)
 * Copyright (c) 2018 ..."
 */

#include <cml_list.h>

void cml_list_move_all(cml_list_t *from, cml_list_t *to)
{
    cml_list_t *listp;

    CML_ILIST_LOOP(from, listp)
    {
        CML_LIST_REMOVE(listp);
        CML_LIST_ADD(to, listp);
    }
}

void cml_ilist_move_all(cml_ilist_t *from, cml_ilist_t *to)
{
    cml_ilist_t *listp;

    CML_ILIST_LOOP(from, listp)
    {
        CML_LIST_REMOVE(listp);
        CML_LIST_ADD(to, listp);
    }
}
