
all: snot

snot.tab.cpp snot.tab.hpp : snot.ypp
	bison -d snot.ypp

snot.lex.cpp snot.yy.hpp: snot.l
	flex -o snot.lex.cpp --header-file=snot.yy.hpp snot.l

snot:	snot.tab.cpp snot.tab.hpp snot.lex.cpp snot.yy.hpp
	g++ -g -o snot snot.tab.cpp snot.lex.cpp -ll

clean:
	rm snot snot.tab.* snot.lex.*
