/* C My Lib list test
 * File: list_test.c
 * Started: Wed Jan 24 19:40:18 MSK 2018
 * Author: Max Amzarakov (maxam18 _at_ gmail _._ com)
 * Copyright (c) 2018 ..."
 */

#include <cml.h>
#include <cml_list.h>
#include <cml_log.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void print_i_t_list(cml_ilist_t *head);
void print_i_t_list_slave(cml_ilist_t *head);
void remove_list_print(cml_ilist_t *listp);

typedef struct i_s i_t;
struct i_s
{
    cml_ilist_t   slave;
    cml_ilist_t   main;

    int           i;
};

int main(void)
{
    cml_ilist_t  i_list;
    cml_ilist_t  i_list_free;
    cml_ilist_t  i_odds;
    cml_ilist_t  i_evens;
    cml_ilist_t *listp;
    i_t         *ip;
    int          i;

    cml_d("Test list operations on the structure:\n" \
            "   cml_ilist_t   main;\n" \
            "   cml_ilist_t   slave;\n" \
            "   int           i;\n");

    cml_d("Initialize lists");

    bzero(&i_list, sizeof(cml_ilist_t));
    bzero(&i_list_free, sizeof(cml_ilist_t));
    bzero(&i_odds, sizeof(cml_ilist_t));
    bzero(&i_evens, sizeof(cml_ilist_t));

    CML_LIST_INIT(&i_list);
    CML_LIST_INIT(&i_list_free);
    CML_LIST_INIT(&i_odds);
    CML_LIST_INIT(&i_evens);

    cml_d("Make list of number from 1 to 10. Add them to the 'main' list\n" \
            " main offset is: %X", (int)offsetof(i_t, main));

    for( i = 0; i < 10; ++i )
    {
        ip = malloc(sizeof(*ip));

        ip->i = (i+1);

        cml_d(" i[%d] address:\n" \
              "   head: %X <- %X -> %X\n" \
              "   list: %X, data: %X" \
              , i, (int)i_list.prev, (int)&i_list, (int)i_list.next \
              , (int)&ip->main, (int)ip);
        CML_ILIST_ADD(&i_list, &ip->main);
        cml_d("   new list: %X <- %X -> %X" \
                , (int)ip->main.prev, (int)&ip->main, (int)ip->main.next);
    }

    cml_d("Print list with neighbours");
    print_i_t_list(&i_list);

    cml_d("Remove 6th element");
    CML_ILIST_LOOP(&i_list, listp)
    {
        ip = CML_ILIST_ENTRY(listp, i_t, main);

        if( ip->i == 5 )
            break;
    }
    CML_ILIST_REMOVE(listp);

    cml_d("Print list with neighbours");
    print_i_t_list(&i_list);

    cml_d("Set odds and evens to the different lists by use of 'slave' list");

    i = 0;
    CML_ILIST_LOOP(&i_list, listp)
    {
        ip = CML_ILIST_ENTRY(listp, i_t, main);

        cml_d(" i[%d]: %d", i++, ip->i);

        if( (ip->i) % 2)
        {
            CML_ILIST_ADD(&i_evens, &ip->slave);
            cml_d(" even");
        } else
        {
            CML_ILIST_ADD(&i_odds, &ip->slave);
            cml_d(" odd");
        }
    }

    cml_d("Print list with neighbours");
    print_i_t_list(&i_list);

    cml_d("Print evens:");
    print_i_t_list_slave(&i_evens);

    cml_d("Print odds:");
    print_i_t_list_slave(&i_odds);

    cml_d("Print list backwards");

    i = 0;
    CML_ILIST_LOOP_BW(&i_list, listp)
    {
        ip = CML_ILIST_ENTRY(listp, i_t, main);

        cml_d(" i[%d]: %d", i++, ip->i);
    }

    cml_d("Remove 6th element in the loop");
    i = 0;
    CML_ILIST_LOOP(&i_list, listp)
    {
        ip = CML_ILIST_ENTRY(listp, i_t, main);

        cml_d(" i[%d]: %d", i++, ip->i);

        if( ip->i == 6 )
        {
            CML_ILIST_REMOVE(listp);
        }
    }

    cml_d("Print list again");
    print_i_t_list(&i_list);

    cml_d("Print list backwards");

    i = 0;
    CML_ILIST_LOOP_BW(&i_list, listp)
    {
        ip = CML_ILIST_ENTRY(listp, i_t, main);

        cml_d(" i[%d]: %d", i++, ip->i);
    }


    cml_d("Move all at main list to free list");
    CML_ILIST_MOVE_ALL(&i_list, &i_list_free);

    cml_d("Print main list:");
    print_i_t_list(&i_list);

    cml_d("Print free list:");
    print_i_t_list(&i_list_free);

    return 0;
}

void remove_list_print(cml_ilist_t *listp)
{
    cml_ilist_t *prev, *curr, *next;

    prev = listp->prev;
    curr  = listp;
    next = listp->next;

    cml_d("prev: %X <- %X -> %X", (int)prev->prev, (int)prev, (int)prev->next);
    cml_d("cur : %X <- %X -> %X", (int)curr->prev, (int)curr, (int)curr->next);
    cml_d("next: %X <- %X -> %X", (int)next->prev, (int)next, (int)next->next);
    
    cml_d("relink (L)->prev->next");
    listp->prev->next = listp->next;

    cml_d("prev: %X <- %X -> %X", (int)prev->prev, (int)prev, (int)prev->next);
    cml_d("cur : %X <- %X -> %X", (int)curr->prev, (int)curr, (int)curr->next);
    cml_d("next: %X <- %X -> %X", (int)next->prev, (int)next, (int)next->next);
    cml_d("relink (L)->next->prev");
    listp->next->prev = listp->prev;

    cml_d("prev: %X <- %X -> %X", (int)prev->prev, (int)prev, (int)prev->next);
    cml_d("cur : %X <- %X -> %X", (int)curr->prev, (int)curr, (int)curr->next);
    cml_d("next: %X <- %X -> %X", (int)next->prev, (int)next, (int)next->next);

    cml_d("listp: %X <- %X -> %X", (int)listp->prev, (int)listp, (int)listp->next);
}
void print_i_t_list(cml_ilist_t *head)
{ 
    cml_ilist_t *listp, *listp_o;
    int          i;
    i_t         *ip, *lip, *rip;

    listp = head;
    cml_d(" head:                 %X <- %X -> %X" \
                , (int)listp->prev, (int)listp, (int)listp->next);


    i = 0;
    CML_ILIST_LOOP(head, listp)
    {
        ip  = CML_ILIST_ENTRY(listp, i_t, main);
        lip = CML_ILIST_ENTRY(listp->prev, i_t, main);
        if( listp->prev == head )
            lip = ip;
        rip = CML_ILIST_ENTRY(listp->next, i_t, main);
        if( listp->next == head )
            rip = ip;

        cml_d(" i[%d]: %02d <- %02d -> %02d; %X <- %X -> %X"
                , i++, lip->i, ip->i, rip->i
                , (int)listp->prev, (int)listp, (int)listp->next);
    }

    cml_d("List links check");
    listp_o = head;
    i = 0;
    CML_ILIST_LOOP(head, listp)
    {
        if( listp->prev != listp_o )
            cml_d(" error at %d", i);
        listp_o = listp;
        ++i;
    }
}

void print_i_t_list_slave(cml_ilist_t *head)
{ 
    cml_ilist_t *listp, *listp_o;
    int          i;
    i_t         *ip, *lip, *rip;

    listp = head;
    cml_d(" head:                 %X <- %X -> %X" \
                , (int)listp->prev, (int)listp, (int)listp->next);


    i = 0;
    CML_ILIST_LOOP(head, listp)
    {
        ip  = CML_ILIST_ENTRY(listp, i_t, slave);
        lip = CML_ILIST_ENTRY(listp->prev, i_t, slave);
        if( listp->prev == head )
            lip = ip;
        rip = CML_ILIST_ENTRY(listp->next, i_t, slave);
        if( listp->next == head )
            rip = ip;

        cml_d(" i[%d]: %02d <- %02d -> %02d; %X <- %X -> %X"
                , i++, lip->i, ip->i, rip->i
                , (int)listp->prev, (int)listp, (int)listp->next);
    }

    cml_d("List links check");
    listp_o = head;
    i = 0;
    CML_ILIST_LOOP(head, listp)
    {
        if( listp->prev != listp_o )
            cml_d(" error at %d", i);
        listp_o = listp;
        ++i;
    }
}
