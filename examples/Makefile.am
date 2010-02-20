EXTRA_DIST = \
  TNM.rp \
  OpUnfold1.rp \
  OpUnfold2.rp \
  OpUnfold3.rp \
  OpOrderBinaryDecisionTree1.rp \
  OpOrderBinaryDecisionTree2.rp \
  OpOrderBinaryDecisionTree3.rp \
  OpToBinaryDecisionTree1.rp \
  OpToBinaryDecisionTree2.rp \
  OpAvg1.rp \
  OpAvg2.rp \
  OpMajorityVoting1.rp \
  OpMajorityVoting2.rp \
  OpUserPreferences1.rp \
  OpUserPreferences2.rp \
  OpUserPreferences3.rp

TESTS = tests/run-decisiondiagramplugin-tests.sh
TESTS_ENVIRONMENT = DLVHEX=dlvhex RPCOMPILER=rpcompiler CMPSCRIPT=$(top_srcdir)/examples/compare.sh DOTCONVERTER=$(top_srcdir)/dotconverter/src/dotconverter TESTDIR=$(top_srcdir)/examples/tests DLVHEXPARAMETERS="--plugindir=$(top_builddir)/src --operatorpath=$(top_builddir)/src/.libs/" SYSPLUGINDIR=$(sysplugindir) USERPLUGINDIR=$(userplugindir)
