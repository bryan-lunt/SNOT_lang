snot.tab.cpp snot.tab.hpp : snot.ypp
	bison -d snot.ypp

snot.lex.c : snot.l
	flex -o snot.lex.c snot.l

snot:	snot.tab.cpp snot.tab.hpp snot.lex.c
	g++ -o snot snot.tab.cpp snot.lex.c -ll

default:
	bison -d snot.ypp
	flex -o snot.lex.cpp --c++ snot.l
	g++ -o snot snot.tab.cpp snot.lex.cpp
	g++ -o snot snot.tab.cpp snot.lex.c -ll
