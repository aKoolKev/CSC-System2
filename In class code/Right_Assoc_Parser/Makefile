DBG = #-g	#remove '#' in front on -g to enable debugging
OPTS = -Wall -ansi -pedantic -std=c++20
CC = g++ -c $(DBG) $(OPTS)
LN = g++

OBJS = main.o Token.o Grammar.o

run: $(OBJS)
	$(LN) -o run $(OBJS)

main.o: main.cpp Grammar.hpp
	$(CC) main.cpp

Token.o: Token.cpp Token.hpp
	$(CC) Token.cpp

Grammar.o: Grammar.cpp Grammar.hpp Token.hpp
	$(CC) Grammar.cpp


clean:
	/bin/rm -f $(OBJS) run *~

