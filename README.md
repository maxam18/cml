#C My Library  - general purpose library collection

Modules (libraries)
1. List implementation
exclusive and inclusive
cml_list.h
cml_list.c

ilist use example

Example:
you have a list of parameters
and want to have a list of only strings

typedef struct my_stru my_stru_t;
struct my_stru
{
    cml_ilist_t    strings;
    cml_ilist_t    parameters;

    cml_str_t      string;
};

then you have list header somewhere

int main(void)
{
    cml_ilist_t  str_head;
    cml_ilist_t  par_head;
    cml_ilist_t *listp;
    my_stru_t   *datap;

    CML_LIST_INIT(&str_head);
    CML_LIST_INIT(&par_head);

    datap = malloc(sizeof(*datap));

    CML_LIST_ADD(&str_head, &datap->strings);
    CML_LIST_ADD(&par_head, &datap->parameters);

    CML_ILIST_LOOP(&par_head, listp)
    {
        datap = CML_ILIST_ENTRY(listp, my_stru_t, strings);
        CML_REMOVE(listp); /* can be removed inside the loop */
    }

    return 0;
}

2. String and buffer 
cml_string.h
cml_string.c

cml_str_t {data - char pointer, len - length }

and enchancement of str_t
cml_buf_t {buf - char pointer, pos - pointer to the current position, end - pointer to the buffer's end, size - allocated size}

3. base64 (en/de)code

4. cml_log
Logging functions

5. cml_pool
simple memory buffer management

6. Various binary trees
btree - key and data are pointers
btree16 - key is int16 with internal cmp function
btree32 - key is int32 with internal cmp function

Red/Black tree implemented

Copyright Max Amzarakov maxam18 _at_ gmail.com

