CC = g++
CFLAGS = -std=c++11 -Wall -O2
LIBS = -lSDL -lGL -lGLEW -lSDL_ttf

MODULES = main life graphics event automaton shader

DEPS = *.hpp Makefile
OBJ = $(MODULES:%=%.o)

all: life

debug: CFLAGS += -ggdb
debug: all

%.o: %.cpp $(DEPS) | $(BINPATH)
	$(CC) -c -o $@ $< $(CFLAGS)

life: $(OBJ)
	$(CC) $^ -o $@ $(LIBS)

clean:
	rm -f *~ *.o life

parser_test: parsertest.cpp parser.cpp
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
