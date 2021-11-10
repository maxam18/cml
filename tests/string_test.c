#include <cml_string.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

const char *progname;

int usage(int retval, char *str)/*{{{*/
{
    if( str )
        fprintf( stderr, "Error: %s\n", str);

    fprintf( stderr,
             "CML string test\n"
             "Usage: %s -h -ns -b <base> \n"
             "Command line: \n"
             " -h          brief help\n"
             " -v          be verbose. More 'v' - more verbosity.\n"
             " -n          convert decimal to number.\n"
             " -s          convert string to number.\n"
             " -b <base>   number base for -n -s. 10, 6, 26.\n"

             "Copyright (c) 2021 by Max Amzarakov "
             " (maxam18 _at_ gmail _dot_ com)\n"
             , progname);

    return retval;
}/*}}}*/

#define MODE_FROM_STR   (1 << 1)
#define MODE_TO_STR     (1 << 2)
#define MODE_BASE_10    (1 << 3)
#define MODE_BASE_16    (1 << 4)
#define MODE_BASE_26    (1 << 5)


int main(int argc, char **argv)
{
    int         opt, verbosity = 0;
    int         mode = 0;
    char        buf[64];
    cml_buf_t   var;
    int32_t     i32, base = 10;
    uint32_t    ui32;

    if( NULL == (progname = strrchr(argv[0], '/')) )
        progname = argv[0];
    else
        progname++;

    while( (opt = getopt(argc, argv, "hvnsb:")) != -1 )
    {
        switch(opt)
        {
        case 'h':
            return usage(0, NULL);
        case 'v':
            verbosity++;
            break;
        case 'n':
            mode = MODE_TO_STR;
            break;
        case 's':
            mode = MODE_FROM_STR;
            break;
        case 'b':
            if( optarg[1] == '0' )
                base = 10;
            else if( optarg[0] == '2' )
                base = 26;
            else
                base = 16;
        }
    }
    argc -= optind;
    argv += optind;

    if( argc < 1 )
        return usage(1, "Argument is not set");

    var.buf  = argv[0];
    var.size = strlen(argv[0]);
    var.end  = var.buf + var.size;
    var.pos  = var.buf;

    if( mode & MODE_FROM_STR )
    {
        if( base == 10 )
        {
            i32 = cml_atoi(var.buf, var.end); 
            ui32 = (uint32_t)i32;
        } else if( base == 16 ) {
            ui32 = cml_htoi((u_char *)var.buf, var.size);
            i32 = (int32_t)ui32;
        } else if( base == 26 ) {
            ui32 = cml_26toi((u_char *)var.buf, var.size);
            i32 = (int32_t)ui32;
        } else
            return usage(1, "Number base is not set (-b)");
        
        fprintf( stdout, "Input '%.*s' base %d in dec is '%u' unsigned and '%d' signed.\n"
                       , (int)var.size, var.buf, base, ui32, i32);
    } else if( mode & MODE_TO_STR )
    {
        i32 = cml_atoi(var.buf, var.end);
        
        if( base == 10  )
            snprintf(buf, sizeof(buf), "%d", i32);
        else if( base == 16  )
          snprintf(buf, sizeof(buf), "%X", i32);
        else if( base == 26  )
          cml_ito26((u_char *)buf, i32);
        else
            return usage(1, "Number base is not set (-b)");
        
        fprintf( stdout, "Input string '%.*s' in dec is '%d' and in base %d is '%s'.\n"
                       , (int)var.size, var.buf, i32
                       , base, buf);
    } else 
        return usage(1, "Mode is not set");

    return 0;
}