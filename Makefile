default:
	bison -d snot.ypp
	flex snot.l
	g++ -o snot snot.tab.cpp snot.yy.c
