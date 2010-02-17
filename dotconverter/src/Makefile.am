AUTOMAKE_OPTIONS = gnu

bin_PROGRAMS = dotconverter
dotconverter_CXXFLAGS = $(AM_CFLAGS)
dotconverter_SOURCES = DotConverter.cpp

dotconverter_CPPFLAGS = @LIBDOTREADER_CFLAGS@
dotconverter_LDADD = @LIBDOTREADER_LIBS@


AM_YFLAGS = -d
AM_CPPFLAGS = \
	-I$(top_srcdir)/dotconverter/include \
	$(DLVHEX_CFLAGS)

install-static: dotconverter
	$(LIBTOOL) --mode=install $(INSTALL) -s dotconverter $(DESTDIR)$(plugindir)/dotconverter

uninstall-static:
	$(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(plugindir)/dotconverter
