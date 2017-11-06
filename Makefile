default:
	bison -d snot.ypp
	flex -o snot.lex.cpp --c++ snot.l
	g++ -o snot snot.tab.cpp snot.lex.cpp
