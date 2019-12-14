CC=			g++
CFLAGS= 	-std=c++11 -Wall -w -g -pthread
INCLUDE:= 	-I include/
BIN=		bin/*.cpp
SOURCE:= 	src/*.cpp

EXE=		lml.bin

exe:
	$(CC) $(CFLAGS) $(INCLUDE) $(BIN) $(SOURCE) -o $(EXE)

clean:
	rm $(BIN)
