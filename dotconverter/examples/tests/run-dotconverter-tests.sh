#!/bin/bash

#
# dlvhex -- Answer-Set Programming with external interfaces.
# Copyright (C) 2005, 2006, 2007 Roman Schindlauer
# 
# This file is part of dlvhex.
#
# dlvhex is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of the
# License, or (at your option) any later version.
#
# dlvhex is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with dlvhex; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
# USA.
#

MKTEMP="mktemp -t tmp.XXXXXXXXXX"
TMPFILE_DOT=$($MKTEMP)
TMPFILE_HEX=$($MKTEMP)

failed=0
warned=0
ntests=0

echo ============ dotconverter tests start ============

for t in $(find $TESTDIR -name '*.test' -type f)
do
    while read INPUT REFOUTPUT ADDPARM
    do
	let ntests++

	INPUT=$TESTDIR/$INPUT
	REFOUTPUT=$TESTDIR/$REFOUTPUT

	if [ ! -f $INPUT ] || [ ! -f $REFOUTPUT ]; then
	    test ! -f $INPUT && echo WARN: Could not find program file $INPUT
	    test ! -f $REFOUTPUT && echo WARN: Could not find reference output $REFOUTPUT
	    continue
	fi

	# check if output is a dot file or an answer-set;
	# use the appropriate script to compare the results
	if [ "$ADDPARM" = "--todot" ]
	then
		# to dot file

		# run dotconverter with specified parameters and input
		$DOTCONVERTER $PARAMETERS $ADDPARM < $INPUT > $TMPFILE_DOT

		# compare with reference output
		$CMPSCRIPT "$TMPFILE_DOT" "$REFOUTPUT" "dot" >/dev/null
		succ=$?
	else
		# to answer-set file

		# run dotconverter with specified parameters and input
		$DOTCONVERTER $PARAMETERS $ADDPARM < $INPUT > $TMPFILE_HEX

		# compare with reference output
		$CMPSCRIPT "$TMPFILE_HEX" "$REFOUTPUT" "hex" >/dev/null
		succ=$?
	fi

	if [ $succ = 0 ]
	then
	    echo PASS: $INPUT
	else
		echo "FAIL: [$INPUT $ADDPARM-->] $REFOUTPUT"
		let failed++
	fi
    done < $t # redirect test file to the while loop
done

# cleanup
rm -f $TMPFILE_DOT
rm -f $TMPFILE_HEX

echo ========== dotconverter tests completed ==========

echo Tested $ntests dotreader test cases
echo $failed failed tests, $warned warnings

echo ============= dotconverter tests end =============

exit $failed
