CC = g++
CFLAGS = -std=c++0x -Wall -O2 -I$(INCPATH)
LIBS = -lSDL -lGL -lGLEW -lSDL_ttf

BINPATH = ./bin
SRCPATH = ./src
INCPATH = ./include

MODULES = main life graphics event automaton shader

DEPS = $(INCPATH)/*.hpp Makefile
OBJ = $(MODULES:%=$(BINPATH)/%.o)

all: $(BINPATH)/life
	cp $< ./

debug: CFLAGS += -ggdb
debug: all

$(BINPATH)/%.o: $(SRCPATH)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINPATH)/life: $(OBJ)
	$(CC) $^ -o $@ $(LIBS)

clean:
	rm -f $(SRCPATH)/*~ $(INCPATH)/*~ $(BINPATH)/*.o

.PHONY: clean
