# define cleanup list (list of file names)
cl=()	# empty list
cc=0	# cleanup counter


cleanup(){
	for i in ${cl[*]}
	do
		rm $i
	done
}

# process command line arguments
eval mergingplan=\${$#}
for i in $*
do
	if [ "$i" = "-f" ]
	then
		force=1
	fi
done

echo "Processing merging plan $mergingplan ..."

# find all input diagrams refered to in the merging plan
inputs=$(cat $mergingplan | egrep -e "source:" | sed 's/source: *\"\(.*\)\".*/\1/g')
echo $inputs
for inp in $inputs
do
	# determine the input file type

	ftype=$(echo $inp | sed "s/.*\.\(.*\)/\1/g" | tr '[A-Z]' '[a-z]')
	inphex=$(echo $inp | sed "s/\(.*\.\).*/\1\hex/g")

	# check if source file exists
	if [ ! -f $inp ]
	then
		echo "Error: Input diagram $inp was used in merging plan, but file does not exist!"
		cleanup
		exit 1
	elif [ "$force" != "1" -a "$ftype" != "hex" -a -f $inphex ]
	then
		echo "Error: $inphex does already exist, use -f to force overwriting"
		cleanup
		exit 1
	else
		case $ftype in
		'hex')
			# is already in hex format, nothing to do
			;;
		'xml')
			echo "Converting $inp to $inphex"
			cl[$cc]=$inphex
			let cc=cc+1
			graphconverter rmxml hex < $inp > $inphex
			;;
		'dot')
			echo "Converting $inp to $inphex"
			cl[$cc]=$inphex
			let cc=cc+1
			graphconverter dot hex < $inp > $inphex
			;;
		esac
		if [ "$?" != 0 ]
		then
			echo "graphconverter failed!"
			cleanup
			exit 1
		fi
	fi
done

# Replace all references to input files with references to according hex programs
## echo $mergingplan.bak | grep -regexp 'source:\"\.dot\"'
echo "Preparing merging plan $mergingplan -> $mergingplan.bak"
if [ "$force" != "1" -a -f $mergingplan.bak ]
then
	echo "Error: $mergingplan.bak does already exist, use -f to force overwriting"
	cleanup
	exit 1
else
	cl[$cc]="$mergingplan.bak"
	let cc=cc+1
	cat $mergingplan | sed "s/\(source: *\".*\.\).*\(\".*\)/\1hex\2/g" > "$mergingplan.bak"
fi

# Execution
echo "Execute merging plan $mergingplan.bak"
echo "Result is stored in ${mergingplan/.mp/.as}"
cl[$cc]="${mergingplan/.mp/.as}"
let cc=cc+1
dlvhex --merging --silent $mergingplan.bak > ${mergingplan/.mp/.as}
if [ "$?" != 0 ]
then
	echo "dlvhex/MELD failed!"
	cleanup
	exit 1
fi

# Convert the result back into desired format
# (the type of the last input file will determine the format)
echo "Converting the result to $ftype"

if [ "$force" != "1" -a -f ${mergingplan/.mp/$ftype} ]
then
	echo "Error: ${mergingplan/.mp/$ftype} does already exist, use -f to force overwriting"
	cleanup
	exit 1
fi

case $ftype in
'hex')
	graphconverter as hex < ${mergingplan/.mp/.as} > ${mergingplan/.mp/.hex}
	;;
'xml')
	graphconverter as rmxml < ${mergingplan/.mp/.as} > ${mergingplan/.mp/.xml}
	;;
'dot')
	graphconverter as dot < ${mergingplan/.mp/.as} > ${mergingplan/.mp/.dot}
	;;
esac
if [ "$?" != 0 ]
then
	echo "graphconverter failed!"
	cleanup
	exit 1
fi

echo "Clearnup"
cleanup

echo "Final result was written to ${mergingplan/.mp/.$ftype}"
