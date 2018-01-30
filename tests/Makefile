CC = gcc
cc = cc
LA = ar
MAKE = make
PROGS = list_test base64_test

include Makefile.cml.incl

# BUILDDIR and INLIB should be set in the environment
OBJS = $(INLIB)
INCPATH	= -I./
LDADD = $(XLDADD) $(CML_LDADD)
LIBPATH	= 
CFLAGS = -O0 -Wall $(INCPATH) -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -fPIC -std=gnu99 $(XCFLAGS) $(CML_XCFLAGS)

.SUFFIXES: .c .cpp .o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(PROGS)

base64_test: base64_test.o
	$(CC) $(LDADD) -o $@ $<

list_test: list_test.o
	$(CC) $(LDADD) -o $@ $<

clean:
	rm -f *.*~ *.o *.core core* $(PROGS)

distclean: clean
	rm -rf *.a Makefile.cml*

