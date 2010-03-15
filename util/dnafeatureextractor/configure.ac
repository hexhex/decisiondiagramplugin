AC_PREREQ(2.64)
AC_INIT([DNAFeatureExtractor],[0.0.5],[e0525250@mail.student.tuwien.ac.at])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_MACRO_DIR([m4])

#
# where the plugin will be installed by default.
# running configure will take sysplugindir from dlvhex.pc,
# running configure --enable-userinstall will take homeplugindir.
#

AC_SUBST(INSTALLDIR)

AC_PROG_CXX
AC_PROG_CC

LT_INIT

AM_INIT_AUTOMAKE

#
# all needed makefiles
#
AC_CONFIG_FILES([
           Makefile 
           src/Makefile
           include/Makefile
])

AC_OUTPUT

