#!/usr/bin/env bash

# Needs both source files in .src file and input in .in file
# Runs through all of them

# Needs ifj18.rb file and your binary project:
OURPROGRAM="../test" # enter name of your compiled project

for i in ./*.src; do

	# print file name
	echo -e "FILE NAME: $i"

	# print source
	echo "Source:"
	echo "------------------------------"
	cat $i
	echo
	echo "------------------------------"

	# input
	IN=`echo $i | sed 's/\(.*\)\.src/\1.in/g'`
	echo "Input:"
	cat $IN

	# correct output
	echo "Correct output:"
	cat $IN | ruby -r ./ifj18.rb $i
	RETURNVALUE=$?
	# correct return value
	echo "Correct return value: $RETURNVALUE"

	# our output
	CODE=`echo $i | sed 's/\(.*\)\.src/\1.code/g'`
	OUTPUT=`echo $i | sed 's/\(.*\)\.src/\1.output/g'`
	echo "Our ifjcode output:"
	echo "------------------------------"
	$OURPROGRAM < $i > $CODE
	OURRETURNVALUE1=$?
	cat $CODE
	echo "------------------------------"
	echo -e "Our output:"
	./ic18int $CODE < $IN > $OUTPUT
	OURRETURNVALUE2=$?
	cat $OUTPUT
	# our return value
	echo "Our return value: $OURRETURNVALUE1"
	# our return ifjcode value
	echo "Our return ifjcode value: $OURRETURNVALUE2"

	# end of this file
	echo "---------------------------------------"
	echo "-------------END OF FILE---------------"
	echo "---------------------------------------"
	echo

done;

rm *.code *.output

exit 0
