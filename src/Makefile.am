AUTOMAKE_OPTIONS = gnu

#
# replace 'plugin' on the right side by your plugin's name
#
plugin_LTLIBRARIES = libdlvhexbm.la

#
# replace 'plugin' on the left side as above and
# add all sources of your plugin
#
libdlvhexbm_la_SOURCES = BMPlugin.cpp
libdlvhexbm_la_LIBADD = $(top_srcdir)/rpcompiler/src/librpcompiler.la

#rpcompiler_OBJCETS = rpcompiler/src/scanner.o rpcompiler/src/parser.o rpcompiler/src/ParseTreeNode.o rpcompiler/src/CodeGenerator.o rpcompiler/src/ParseTreeNodeIterator.o rpcompiler/src/RPParser.o

#
# extend compiler flags by CFLAGS of other needed libraries
#
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/rpcompiler/include \
	-I/media/Data/Uni/Diplomarbeit/Tools/dlvhex/dlvhex/trunk/include/dlvhex \
	-I/media/Data/Uni/Diplomarbeit/Tools/dlvhex/dlvhex/trunk/include \
	$(BOOST_CPPFLAGS) \
	$(DLVHEX_CFLAGS)

libdlvhexbm_la_LDFLAGS = -avoid-version -module


libdlvhexbm-static.la: $(libdlvhexbm_la_OBJECTS) $(rpcompiler_OBJCETS)
	$(CXXLINK) -avoid-version -module -rpath $(plugindir) $(libdlvhexasp_la_OBJECTS) $(rpcompiler_OBJCETS)

install-static: libdlvhexbm-static.la
	$(LIBTOOL) --mode=install $(INSTALL) -s libdlvhexbm-static.la $(DESTDIR)$(plugindir)/libdlvhexbm-static.la

uninstall-static:
	$(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(plugindir)/libdlvhexbm-static.la
