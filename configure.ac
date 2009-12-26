AC_PREREQ(2.64)
AC_INIT([dlvhex-bmplugin],[0.0.1],[e0525250@mail.student.tuwien.ac.at])
AC_CONFIG_SRCDIR([src/BMPlugin.cpp])
AC_CONFIG_MACRO_DIR([m4])

MAJOR=`echo $PACKAGE_VERSION | sed "s,^\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\),\1,"`
MINOR=`echo $PACKAGE_VERSION | sed "s,^\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\),\2,"`
MICRO=`echo $PACKAGE_VERSION | sed "s,^\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\),\3,"`

AC_DEFINE_UNQUOTED([BMPLUGIN_MAJOR], [$MAJOR], [Major version number of the bm-plugin])
AC_DEFINE_UNQUOTED([BMPLUGIN_MINOR], [$MINOR], [Minor version number of the bm-plugin])
AC_DEFINE_UNQUOTED([BMPLUGIN_MICRO], [$MICRO], [Micro version number of the bm-plugin])

AM_INIT_AUTOMAKE

AC_CANONICAL_HOST

# by default we look into /opt/local and /sw for include files (macports vs. fink)
case $host_os in
     darwin*|macosx*)
	CPPFLAGS="$CPPFLAGS -I/opt/local/include -I/sw/include"
     ;;
esac 


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LEX
AC_PROG_YACC

# Checks for header files.
AC_HEADER_DIRENT
AC_CHECK_HEADERS([sys/param.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for library functions.
_LT_SET_OPTION([LT_INIT],[dlopen])
AC_DIAGNOSE([obsolete],[AC_LIBTOOL_DLOPEN: Remove this warning and the call to _LT_SET_OPTION when you
put the `dlopen' option into LT_INIT's first parameter.])
 # we build a module library
AC_DISABLE_STATIC # we don't need a static library
LT_INIT

#
# check for other libraries here
#

#
# of course we need dlvhex itself!
#
PKG_CHECK_MODULES([DLVHEX], [dlvhex >= 1.7])


# checking for boost libs
BOOST_REQUIRE([1.33.0])
BOOST_SMART_PTR
BOOST_STRING_ALGO
BOOST_TOKENIZER


#
# where the plugin will be installed by default.
# running configure will take sysplugindir from dlvhex.pc,
# running configure --enable-userinstall will take homeplugindir.
#

AC_ARG_ENABLE(userinstall, 
             [  --enable-userinstall    Install plugin into home directory],
             [plugindir=$HOME/`pkg-config dlvhex --variable=userplugindir`],
             [plugindir=`pkg-config dlvhex --variable=sysplugindir`]
             )
AC_SUBST([plugindir])

#
# all needed makefiles
#
AC_CONFIG_FILES([
           Makefile 
           src/Makefile
           include/Makefile
           examples/Makefile
           rpcompiler/Makefile 
           rpcompiler/src/Makefile
           rpcompiler/examples/Makefile
])

AC_OUTPUT

