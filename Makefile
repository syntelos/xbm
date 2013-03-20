
CC      = gcc

SRCS    = test.c XBM.c

HEADERS = XBM.h

CFLAGS  = -I. 

LIBS    = -lm 

png : test.png

xbm : test.xbm

exe : test

clean:
	$(RM) *.s *.o *~ *.lst *.i test


.PHONY: png xbm exe clean


test : $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS)  -o $@  $(SRCS) $(LIBS)

test.xbm : test
	./test
	bmtoa test.xbm # package "x11-apps"

test.png : test.xbm
	convert test.xbm test.png # package "imagemagick"
