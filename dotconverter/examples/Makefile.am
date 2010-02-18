EXTRA_DIST = \
  emptygraph.dot \
  onlynodes.dot \
  elseedge.dot \
  conditionaledge.dot \
  moreedges.dot

TESTS = tests/run-dotconverter-tests.sh
TESTS_ENVIRONMENT = DLVHEX=dlvhex CMPSCRIPT=$(top_srcdir)/examples/compare.sh DOTCONVERTER=$(top_srcdir)/dotconverter/src/dotconverter TESTDIR=$(top_srcdir)/dotconverter/examples/tests DLVHEXPARAMETERS=" --plugindir=$(top_builddir)/src"
