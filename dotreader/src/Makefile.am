AUTOMAKE_OPTIONS = gnu

bin_PROGRAMS = dotreader
dotreader_CXXFLAGS = $(AM_CFLAGS)
dotreader_SOURCES = DotReader.cpp

INCLUDES = @LIBDOTREADER_CFLAGS@
dotreader_LDADD = @LIBDOTREADER_LIBS@


AM_YFLAGS = -d
AM_CPPFLAGS = \
	-I$(top_srcdir)/dotreader/include

install-static: dotreader
	$(LIBTOOL) --mode=install $(INSTALL) -s dotreader $(DESTDIR)$(plugindir)/dotreader

uninstall-static:
	$(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(plugindir)/dotreader
