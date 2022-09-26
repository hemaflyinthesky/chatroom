CC=gcc
STD=-std=gnu99
FLAG=-Wall -Werror
OBJ=server.o queue.o threadpool.o
BIN=ser

all:$(OBJ)
	$(CC) $(OBJ) -pthread -o $(BIN) && ./$(BIN)

%.o:%.c queue.h 
	$(CC) $(STD) $(FALG) -c $< -pthread -o $@

clean:
	rm -rf $(OBJ) $(BIN)
	rm -rf *~ *.h.gch
