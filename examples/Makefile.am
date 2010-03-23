EXTRA_DIST = \
  TNM.mp \
  OpUnfold1.mp \
  OpUnfold2.mp \
  OpUnfold3.mp \
  OpOrderBinaryDecisionTree1.mp \
  OpOrderBinaryDecisionTree2.mp \
  OpOrderBinaryDecisionTree3.mp \
  OpToBinaryDecisionTree1.mp \
  OpToBinaryDecisionTree2.mp \
  OpAvg1.mp \
  OpAvg2.mp \
  OpMajorityVoting1.mp \
  OpMajorityVoting2.mp \
  OpUserPreferences1.mp \
  OpUserPreferences2.mp \
  OpUserPreferences3.mp \
  OpSimplify1.mp \
  OpSimplify2.mp \
  OpSimplify3.mp \
  OpSimplify4.mp

TESTS = tests/run-decisiondiagramplugin-tests.sh
TESTS_ENVIRONMENT = DLVHEX=dlvhex MPCOMPILER=mpcompiler CMPSCRIPT=$(top_srcdir)/examples/compare.sh GRAPHCONVERTER=$(top_srcdir)/graphconverter/src/graphconverter TESTDIR=$(top_srcdir)/examples/tests DLVHEXPARAMETERS="--plugindir=$(top_builddir)/src --operatorpath=$(top_builddir)/src/.libs/" SYSPLUGINDIR=$(sysplugindir) USERPLUGINDIR=$(userplugindir)
