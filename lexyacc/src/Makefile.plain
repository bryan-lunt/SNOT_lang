
all: lib

lib: ./lib/libsnot.a


snot.tab.cpp snot.tab.hpp : snot.ypp
	bison -d snot.ypp

snot.lex.cpp snot.yy.hpp: snot.lpp
	flex -o snot.lex.cpp --header-file=snot.yy.hpp snot.lpp

snot.tab.o snot.lex.o:	snot.tab.cpp snot.tab.hpp snot.lex.cpp snot.yy.hpp param_storage.h
	g++ -g -c snot.tab.cpp snot.lex.cpp

./lib/libsnot.a: snot.tab.o snot.lex.o
	mkdir -p lib
	ar -rcs ./lib/libsnot.a snot.tab.o snot.lex.o

test: test.cpp snot.h param_storage.h ./lib/libsnot.a
	g++ -c test.cpp
	g++ -o test test.o -L./lib -lsnot -ll -ly

clean:
	rm snot snot.tab.* snot.lex.*
