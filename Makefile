
all: snot

snot.tab.cpp snot.tab.hpp : snot.ypp
	bison -d snot.ypp

snot.lex.cpp snot.yy.hpp: snot.l
	flex -o snot.lex.cpp --header-file=snot.yy.hpp snot.l

snot.tab.o snot.lex.o:	snot.tab.cpp snot.tab.hpp snot.lex.cpp snot.yy.hpp
	g++ -g -c snot.tab.cpp snot.lex.cpp

test: test.cpp snot.h
	g++ -c test.cpp
	g++ -o test test.o snot.tab.o snot.lex.o -ll

clean:
	rm snot snot.tab.* snot.lex.*
