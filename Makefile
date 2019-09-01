CC=			g++
CFLAGS= 	-std=c++11 -Wall -w -g
INCLUDE:= 	-I include/
BIN=		bin/*.cpp
SOURCE:= 	src/*.cpp


BIN=		lml.bin
EXE=		lml.bin

bin:
	$(CC) $(CFLAGS) $(INCLUDE) $(BIN) $(SOURCE) -o $(BIN)

exe:
	$(CC) $(CFLAGS) $(INCLUDE) $(BIN) $(SOURCE) -o $(EXE)

clean:
	rm $(EXE)
