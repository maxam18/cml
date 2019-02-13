CC = gcc
cc = cc
LA = ar
MAKE = make

# BUILDDIR and INLIB should be set in the environment
CMLLIB = $(BUILDDIR)/libcml.a
OBJS = $(INLIB)
INCPATH	= -I./
LDADD = $(XLDADD)
LIBPATH	= 
#be carefull of OFFSET_BITS it changes offsetof to long long int I guess
#CFLAGS = -Wall $(INCPATH) -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -fPIC -std=gnu99 $(XCFLAGS)
CFLAGS = -Wall $(INCPATH) -fPIC -std=gnu99 $(XCFLAGS)

.SUFFIXES: .c .cpp .o

include Makefile.inc

%.h:

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $<

libcml: $(OBJS)
	$(LA) rcs "$(CMLLIB)" $(OBJS) 

clean:
	rm -f *.*~ *.o *.core core* *.gch

distclean: clean
	rm -rf *.a

