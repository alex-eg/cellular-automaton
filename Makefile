CC = clang++
CFLAGS = -std=c++20 -Weverything -Werror -pedantic-errors -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded
LIBS = -lSDL -lGL -lGLEW -lSDL_ttf

MODULES = main life graphics event automaton shader

DEPS = *.hpp Makefile
OBJ = $(MODULES:%=bin/%.o)

BINPATH = ./bin

all: CFLAGS += -O2
all: life

debug: CFLAGS += -ggdb
debug: life

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

format-all:
	echo *.hpp *.cpp | xargs clang-format --dry-run

.PHONY: clean format-all
