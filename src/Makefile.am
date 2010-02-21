AUTOMAKE_OPTIONS = gnu

#
# replace 'plugin' on the right side by your plugin's name
#
plugin_LTLIBRARIES = libdlvhexdd.la

#
# replace 'plugin' on the left side as above and
# add all sources of your plugin
#
libdlvhexdd_la_SOURCES = DecisionDiagramPlugin.cpp DecisionDiagram.cpp OpUnfold.cpp OpMajorityVoting.cpp OpToBinaryDecisionTree.cpp OpUserPreferences.cpp OpOrderBinaryDecisionTree.cpp OpAvg.cpp OpSimplify.cpp
#libdlvhexdd_la_LIBADD = $(libdir)/dlvhex/libdlvhexbase.la $(libdir)/dlvhex/libaspsolver.la

#
# extend compiler flags by CFLAGS of other needed libraries
#
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/rpcompiler/include \
	$(BOOST_CPPFLAGS) \
	$(DLVHEX_CFLAGS)

libdlvhexdd_la_LDFLAGS = -avoid-version -module

libdlvhexdd-static.la: $(libdlvhexdd_la_OBJECTS)
	$(CXXLINK) -avoid-version -module -rpath $(plugindir) $(libdlvhexdd_la_OBJECTS)

install-static: libdlvhexdd-static.la
	$(LIBTOOL) --mode=install $(INSTALL) -s libdlvhexdd-static.la $(DESTDIR)$(plugindir)/libdlvhexdd-static.la

uninstall-static:
	$(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(plugindir)/libdlvhexdd-static.la
