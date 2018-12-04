#!/usr/bin/env bash

# Needs both source files in .src file and input in .in file
# Runs through all of them

# Needs ifj18.rb file and your binary project:
OURPROGRAM="./ifj2018" # enter name of your compiled project

if [ -e ./ic18int ]
then
    echo "SCHOOL TESTS"
	echo "------------"
else
    echo "ic18int IS REQUIRED IN PROJECT ROOT FOLDER!!!"
	exit 0
fi

for i in ./tests/our_tests/*.src; do

	# print file name
	echo -e "FILE NAME: $i"

	# print source
	echo "Source:"
	echo "------------------------------"
	cat $i
	echo
	echo "------------------------------"

	# input
	# IN=`echo $i | sed 's/\(.*\)\.src/\1.in/g'`
	# echo "Input:"
	# cat $IN

	# correct output
	echo "Correct output:"
	ruby -r ./ifj18.rb $i
	RETURNVALUE=$?
	# correct return value
	echo "Correct return value: $RETURNVALUE"
	echo "------------------------------"

	# our output
	CODE=`echo $i | sed 's/\(.*\)\.src/\1.code/g'`
	OUTPUT=`echo $i | sed 's/\(.*\)\.src/\1.output/g'`
	echo "Our ifjcode output:"
	$OURPROGRAM < $i > $CODE
	OURRETURNVALUE1=$?
	cat $CODE
	echo "------------------------------"
	echo -e "Our output:"
	./ic18int $CODE > $OUTPUT
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

rm ./tests/our_tests/*.code ./tests/our_tests/*.output

exit 0
