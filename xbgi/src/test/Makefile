# Makefile for test Xbgi programs

XBGI = ../libXbgi.a

# C compiler: gcc or clang
CC = gcc
CFLAGS = -O2
AR = ar
ARFLAGS = rc
LIBS = -lX11 -lm

PROGRAMS = bgidemo cellular fern floodfilltest hopalong mandelbrot\
mousetest moveit simple turtledemo userpalette

all: $(PROGRAMS)

bgidemo: bgidemo.c $(XBGI)
	$(CC) $(CFLAGS) -o bgidemo bgidemo.c $(XBGI) $(LIBS)

cellular: cellular.c $(XBGI)
	$(CC) $(CFLAGS) -o cellular cellular.c $(XBGI) $(LIBS)

fern: fern.c $(XBGI)
	$(CC) $(CFLAGS) -o fern fern.c $(XBGI) $(LIBS)

floodfilltest: floodfilltest.c $(XBGI)
	$(CC) $(CFLAGS) -o floodfilltest floodfilltest.c $(XBGI) $(LIBS)

hopalong: hopalong.c $(XBGI)
	$(CC) $(CFLAGS) -o hopalong hopalong.c $(XBGI) $(LIBS)

mandelbrot: mandelbrot.c $(XBGI)
	$(CC) $(CFLAGS) -o mandelbrot mandelbrot.c $(XBGI) $(LIBS)

mousetest: mousetest.c $(XBGI)
	$(CC) $(CFLAGS) -o mousetest mousetest.c $(XBGI) $(LIBS)

moveit: moveit.c $(XBGI)
	$(CC) $(CFLAGS) -o moveit moveit.c $(XBGI) $(LIBS)

simple: simple.c $(XBGI)
	$(CC) $(CFLAGS) -o simple simple.c $(XBGI) $(LIBS)

turtledemo: turtledemo.c turtle.c $(XBGI)
	$(CC) $(CFLAGS) -o turtledemo turtledemo.c turtle.c $(XBGI) $(LIBS)

userpalette: userpalette.c turtle.c $(XBGI)
	$(CC) $(CFLAGS) -o userpalette userpalette.c turtle.c $(XBGI) $(LIBS)

clean:
	rm -f $(PROGRAMS)
