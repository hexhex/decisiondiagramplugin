EXTRA_DIST = \
  emptygraph.dot \
  onlynodes.dot \
  elseedge.dot \
  conditionaledge.dot \
  moreedges.dot

TESTS = tests/run-graphconverter-tests.sh
TESTS_ENVIRONMENT = DLVHEX=dlvhex CMPSCRIPT=$(top_srcdir)/examples/compare.sh GRAPHCONVERTER=$(top_srcdir)/graphconverter/src/graphconverter TESTDIR=$(top_srcdir)/graphconverter/examples/tests DLVHEXPARAMETERS=" --plugindir=$(top_builddir)/src"
