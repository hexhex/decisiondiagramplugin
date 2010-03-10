AUTOMAKE_OPTIONS = gnu

bin_PROGRAMS = dotconverter
dotconverter_SOURCES = DotConverter.cpp tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp IFormat.cpp DotFormat.cpp RmxmlFormat.cpp HexFormat.cpp AsFormat.cpp StringHelper.cpp
dotconverter_LDADD = @LIBCURL@ $(top_srcdir)/src/DecisionDiagram.o

AM_YFLAGS = -d
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/dotconverter/include \
	$(DLVHEX_CFLAGS) \
	$(GRAPHVIZ_CFLAGS)

AM_LDFLAGS = \
	$(DLVHEX_LIBS) \
	$(GRAPHVIZ_LIBS)

install-static: dotconverter
	$(LIBTOOL) --mode=install $(INSTALL) -s dotconverter $(DESTDIR)$(plugindir)/dotconverter

uninstall-static:
	$(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(plugindir)/dotconverter
