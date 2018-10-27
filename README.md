# C My Library  - general purpose library collection

## Modules (libraries)
### List implementation
exclusive and inclusive
cml_list.h
cml_list.c

ilist use example

*Example:*
you have a list of parameters
and want to have a list of only strings

```C
typedef struct my_stru my_stru_t;
struct my_stru
{
    cml_ilist_t    strings;
    cml_ilist_t    parameters;

    cml_str_t      string;
};
```

then you have list header somewhere

```C
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
```

### String and buffer 
cml_string.h
cml_string.c

cml_str_t {data - char pointer, len - length }

and enchancement of str_t
cml_buf_t {buf - char pointer, pos - pointer to the current position, end - pointer to the buffer's end, size - allocated size}

### base64 (en/de)code

### cml_log
Logging functions

### cml_pool
simple memory buffer management

### Various binary trees
btree - key and data are pointers
btree16 - key is int16 with internal cmp function
btree32 - key is int32 with internal cmp function

Red/Black tree implemented

## How to use

```
> cd <your project dir>
> <path_to_cml>/configure
```
This will make two files inside you project folder:
- Makefile.cml
- Makefile.cml.incl

*Makefile.cml* - is the Makefile to build libcml.a
`> make -f Makefile.cml`

*Makefile.cml.incl* - CFLAGS and LDFLAGS definitions. Add this into your Makefile `include Makfile.cml.incl`

Run `<path_to_cml>/configure --help` to read how to manage *libcml.a* container
Run `<path_to_cml>/configure` and see notes it shows at the end.
