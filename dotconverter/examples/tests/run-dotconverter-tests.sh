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
TMPFILE=$($MKTEMP) # global temp. file for generated output
TMPFILE2=$($MKTEMP) # global temp. file for reference output

failed=0
warned=0
ntests=0

echo ============ dotconverter tests start ============

for t in $(find $TESTDIR -name '*.test' -type f)
do
    while read INPUT OUTPUT ADDPARM
    do
	let ntests++

	INPUT=$TESTDIR/$INPUT
	OUTPUT=$TESTDIR/$OUTPUT

	if [ ! -f $INPUT ] || [ ! -f $OUTPUT ]; then
	    test ! -f $INPUT && echo WARN: Could not find program file $INPUT
	    test ! -f $OUTPUT && echo WARN: Could not find answer sets file $OUTPUT
	    continue
	fi

	# run dotconverter with specified parameters and input
	$DOTCONVERTER  $PARAMETERS $ADDPARM < $INPUT | uniq | sort -r > $TMPFILE

	# sort is necessary since equivalence is independent from the order of nodes and edges (nodes and edges form a set)
	uniq $OUTPUT | sort -r > $TMPFILE2

	if cmp -s $TMPFILE $TMPFILE2
	then
	    echo PASS: $INPUT
	else
		echo "FAIL: $DOTREADER $PARAMETERS $ADDPARM $INPUT"
		let failed++
	fi
    done < $t # redirect test file to the while loop
done

# cleanup
rm -f $TMPFILE

echo ========== dotconverter tests completed ==========

echo Tested $ntests dotreader test cases
echo $failed failed tests, $warned warnings

echo ============= dotconverter tests end =============

exit $failed
