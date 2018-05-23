CC = clang++
CFLAGS = -std=c++17 -Weverything -O2 -Werror -pedantic-errors -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded
LIBS = -lSDL2 -lGL -lGLEW -lSDL2_ttf

MODULES = main life graphics event automaton shader

DEPS = *.hpp Makefile
OBJ = $(MODULES:%=bin/%.o)

BINPATH = ./bin

all: life

debug: CFLAGS += -ggdb
debug: all

$(BINPATH):
	mkdir -p $(BINPATH)

$(BINPATH)/%.o: %.cpp $(DEPS) | $(BINPATH)
	$(CC) -c -o $@ $< $(CFLAGS)

life: $(OBJ)
	$(CC) $^ -o $(BINPATH)/$@ $(LIBS)

clean:
	rm -f $(BINPATH)/*.o $(BINPATH)/life

parser_test: parsertest.cpp parser.cpp
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
