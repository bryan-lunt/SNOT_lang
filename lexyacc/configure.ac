#From the tutorial at http://projects.csail.mit.edu/cgi-bin/wiki/view/SLS/GNUAutoconfSetup
#                                               -*- Autoconf -*-
# This files generates the configure file.
#
# If you change this file, you usually should run
# autoreconf -fvi
#AC_PREREQ(2.61)

# This is where the version information is set
AC_INIT([myproj], [1.0.0], [me@mit.edu])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])

# Set this file to one that is in your directory
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_SRCDIR([src/test.cpp])


# Automake initialization
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_LIBTOOL

AC_PROG_YACC
AC_PROG_LEX
AC_PROG_CXX
AC_LANG(C++)


# Checks for libraries.
AC_CHECK_LIB([l], [yyparse])


# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_CONFIG_FILES([Makefile
		 src/Makefile])
AM_MISSING_PROG([AUTOM4TE], [autom4te])
AC_OUTPUT
