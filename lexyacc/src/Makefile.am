PARSER = snot.ypp snot.lpp snot.h
AM_YFLAGS = -d

BUILT_SOURCES = snot.hpp snot.cpp snot.lpp

bin_PROGRAMS = test

test_SOURCES = test.cpp $(PARSER)
test_LDADD = -ll

AM_LFLAGS = -olex.yy.c
