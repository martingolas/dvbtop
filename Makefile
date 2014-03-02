# Makefile for DVB-T frontend parser
CC=gcc
CFLAGS=-g -gdwarf-2 -Wall
LDFLAGS=-lncurses

SOURCES=main.c dvb.c ui.c
OBJECTS=$(SOURCES:.cpp=.o)
EXEC=dvbtop

all: $(SOURCES) $(EXEC)

$(EXEC): $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


.PHONY: clean

clean:
	rm $(EXEC)