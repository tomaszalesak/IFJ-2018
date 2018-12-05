#!/usr/bin/env bash

# Needs both source files in .src file and input in .in file
# Runs through all of them

OURPROGRAM="./ifj2018" # enter name of your compiled project

if [ -e ./ic18int ]
then
    echo "OUR COLOURFUL TESTS"
	echo "-------------------"
else
    echo "ic18int IS REQUIRED IN PROJECT ROOT FOLDER!!!"
	exit 0
fi

for i in ./tests/our_tests/*.src; do

	# print file name
	echo -e "\e[32m\c"
	echo -e "FILE NAME:\e[0m $i"

	# print source
	echo -e "\e[32m\c"
	echo "Source:"
	echo -e "\e[0m\c"
	cat $i
	echo

	# correct output
	echo -e "\e[34m\c"
	echo -e "Correct output:"
	echo -e "\e[0m\c"
	ruby -r ./tests/ifj18.rb $i
	RETURNVALUE=$?
	# correct return value
	echo -e "\e[34m\c"
	echo -e "Correct return value:\e[0m $RETURNVALUE"

	# our output
	CODE=`echo $i | sed 's/\(.*\)\.src/\1.code/g'`
	OUTPUT=`echo $i | sed 's/\(.*\)\.src/\1.output/g'`
	echo -e "\e[35m\c"
	echo -e "Our ifjcode output:"
	echo -e "\e[0m\c"
	$OURPROGRAM < $i > $CODE
	OURRETURNVALUE1=$?
	cat $CODE
	echo -e "\e[35m\c"
	echo -e "Our output:"
	echo -e "\e[0m\c"
	./ic18int $CODE > $OUTPUT
	OURRETURNVALUE2=$?
	cat $OUTPUT
	# our return value
	echo -e "\e[35m\c"
	echo -e "Our return value:\e[0m $OURRETURNVALUE1"
	# our return ifjcode value
	echo -e "\e[35m\c"
	echo -e "Our return ifjcode value:\e[0m $OURRETURNVALUE2"

	# end of this file
	echo "---------------------------------------"
	echo "------------------END------------------"
	echo "---------------------------------------"
	echo

done;

rm ./tests/our_tests/*.code ./tests/our_tests/*.output

exit 0
