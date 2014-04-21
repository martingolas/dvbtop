# Makefile for DVB-T frontend parser
CC=gcc
CFLAGS=-g -gdwarf-2 -Wall -std=gnu99
LDFLAGS=-lncurses

SOURCES=main.c dvb.c ui.c
OBJECTS=$(SOURCES:.cpp=.o)
EXEC=dvbtop

all: $(SOURCES) $(EXEC)

$(EXEC): $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


.PHONY: clean install

install:
	cp $(EXEC) /usr/bin

clean:
	rm $(EXEC)