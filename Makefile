CC=			g++
CFLAGS= 	-std=c++11 -Wall -w -g
INCLUDE:= 	-I include/
BIN=		bin/*.cpp
SOURCE:= 	src/*.cpp


EXE=lml.bin

all:
	$(CC) $(CFLAGS) $(INCLUDE) $(SOURCE) $(BIN) -o $(EXE)

clean:
	rm $(EXE)
