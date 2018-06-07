
all: lib

lib: ./lib/libsnot.a


./src/snot.tab.cpp ./src/snot.tab.hpp : ./src/snot.ypp
	bison -d ./src/snot.ypp

./src/snot.lex.cpp ./src/snot.yy.hpp: ./src/snot.lpp
	flex -o snot.lex.cpp --header-file=snot.yy.hpp snot.l
	flex -o ./src/snot.lex.cpp --header-file=./src/snot.yy.hpp ./src/snot.lpp

./src/snot.tab.o ./src/snot.lex.o:	./src/snot.tab.cpp ./src/snot.tab.hpp ./src/snot.lex.cpp ./src/snot.yy.hpp ./src/param_storage.h
	g++ -g -c ./src/snot.tab.cpp ./src/snot.lex.cpp

./lib/libsnot.a: ./src/snot.tab.o ./src/snot.lex.o
	ar -rcs ./lib/libsnot.a ./src/snot.tab.o ./src/snot.lex.o

test1: ./tests/test1.cpp ./src/snot.h ./src/param_storage.h ./lib/libsnot.a
	g++ -c ./tests/test1.cpp
	g++ -o test1 ./tests/test1.o -L./lib -lsnot -ll -ly

clean:
	rm snot
