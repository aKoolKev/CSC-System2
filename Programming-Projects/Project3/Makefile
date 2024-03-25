#OPTIM=-O
DBG=-g
OPTS=$(DBG) $(OPTIM) -Wall -ansi -pedantic -std=c++20

CC=g++ $(OPTS)  -c
LNK=g++

OBJS=main.o Token.o Grammar.o Variable.o Command.o
GLBL=Makefile Token.hpp Grammar.hpp Variable.hpp Block.hpp Command.hpp

run: $(OBJS)
	$(LNK) -o run $(OBJS)

main.o: main.cpp $(GLBL)
	$(CC) main.cpp

Token.o: Token.cpp Token.hpp $(GLBL)
	$(CC) Token.cpp

Grammar.o: Grammar.cpp Grammar.hpp $(GLBL)
	$(CC) Grammar.cpp

Variable.o: Variable.hpp Variable.cpp $(GLBL)
	$(CC) Variable.cpp

Command.o: Command.hpp Command.cpp $(GLBL)
	$(CC) Command.cpp

clean:
	/bin/rm -f *.o run *~
