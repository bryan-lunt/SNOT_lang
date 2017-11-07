
AM_CFLAGS = -W -Wall -Wextra -ansi
AM_CXXFLAGS = -W -Wall -Wextra -ansi


#PARSER = snot.cpp snot.hpp snot.h snot.lex.cpp snot.lex.hpp
AM_YFLAGS = -d

#BUILT_SOURCES = snot.cpp snot.hpp snot.lex.cpp snot.lex.hpp


noinst_LIBRARIES = libsnot.a

#libsnot_a_SOURCES = snot.cpp snot.hpp snot.h snot.lex.cpp snot.lex.hpp
libsnot_a_SOURCES = snot.ypp snot.lpp snot.h

LIBS = libsnot.a


bin_PROGRAMS = test

test_SOURCES = test.cpp snot.h
test_DEPENDENCIES = libsnot.a
test_LDADD = -ll

AM_LFLAGS = -olex.yy.c
