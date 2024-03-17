#OPTIM=-O
DBG=-g
OPTS=$(DBG) $(OPTIM) -Wall -ansi -pedantic

CC=g++ $(OPTS)  -c
LNK=g++

OBJS=Token.o main.o Grammar.o
GLBL=Makefile Token.hpp Grammar.hpp

run: $(OBJS)
	$(LNK) -o run $(OBJS)

main.o: main.cpp Token.hpp $(GLBL)
	$(CC) main.cpp

Token.o: Token.cpp Token.hpp $(GLBL)
	$(CC) Token.cpp

Grammar.o: Grammar.cpp Grammar.hpp Token.hpp
	$(CC) Grammar.cpp

clean:
	/bin/rm -f *.o run *~
