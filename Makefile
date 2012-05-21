CC = g++
CFLAGS = -ggdb -std=c++0x -Wall
LIBS = -lSDL -lGL -lGLEW
OBJ = Life.o Set.o Graphics.o

life: 
	$(CC) -c -o Life.o Life.cpp $(LIBS) $(CFLAGS)

set:
	$(CC) -c -o Set.o Set.cpp $(LIBS) $(CFLAGS)

life_test:
	$(CC) -c -o LifeTest.o LifeTest.cpp $(LIBS) $(CFLAGS)

bin_life_test: life set life_test
	$(CC) -o lifetest LifeTest.o $(OBJ) $(LIBS) $(CFLAGS)

graphics:
	$(CC) -c -o Graphics.o Graphics.cpp $(LIBS) $(CFLAGS)

lapp:
	$(CC) -c -o Lapp.o LApp.cpp $(LIBS) $(CFLAGS)

events:
	$(CC) -c -o LEvent.o LEvent.cpp $(LIBS) $(CFLAGS)

shader:
	$(CC) -c -o Shader.o Shader.cpp $(LIBS) $(CFLAGS)

app: life set lapp graphics events shader
	$(CC) -o LApp Lapp.o LEvent.o Shader.o $(OBJ) $(LIBS) $(CFLAGS)

clean:
	rm -f ./*~ ./*.o

count:
	wc -l *.{hpp,cpp,glsl} Makefile