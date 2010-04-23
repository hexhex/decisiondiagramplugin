bin_PROGRAMS = graphconverter
graphconverter_SOURCES = GraphConverter.cpp tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp IFormat.cpp DotFormat.cpp RmxmlFormat.cpp HexFormat.cpp AsFormat.cpp
graphconverter_LDADD = @LIBCURL@ $(top_srcdir)/src/DecisionDiagram.o $(top_srcdir)/src/StringHelper.o

AM_YFLAGS = -d
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/graphconverter/include \
	$(DLVHEX_CFLAGS) \
	$(GRAPHVIZ_CFLAGS)

AM_LDFLAGS = \
	$(DLVHEX_LIBS) \
	$(GRAPHVIZ_LIBS)

install-static: graphconverter
	$(LIBTOOL) --mode=install $(INSTALL) -s graphconverter $(DESTDIR)$(plugindir)/graphconverter

uninstall-static:
	$(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(plugindir)/graphconverter
