#!/usr/bin/env bash

# IFJ18 - Priklady pro studenty pro dokresleni zadani

#COMPILER=cat  # zde muze byt vas prekladac IFJ18 (napr. ./ifj18)
#INTERPRETER="ruby -r ./ifj18.rb"  # zde muze byt interpret IFJcode18 (./ic18int)
COMPILER="./ifj2018"  # zde muze byt vas prekladac IFJ18 (napr. ./ifj18)
INTERPRETER="./ic18int"  # zde muze byt interpret IFJcode18 (./ic18int)

if [ -e ./ic18int ]
then
    echo "SCHOOL TESTS"
	echo "------------"
else
    echo "ic18int IS REQUIRED IN PROJECT ROOT FOLDER!!!"
	exit 0
fi

for i in ./tests/tests_from_school/*.src; do

	IN=`echo $i | sed 's/\(.*\)\.src/\1.in/g'`
	OUT=`echo $i | sed 's/\(.*\)\.src/\1.out/g'`
	PRG=`echo $i | sed 's/\(.*\)\.src/\1.prg/g'`

	$COMPILER $i > $PRG   # nekontroluji se chyby pekladu (pro chybove testy nutno rozsirit)
	RETURNED_COMPILER=$?

	echo -n "DIFF: $INTERPRETER $i: "
	cat $IN | $INTERPRETER $PRG | diff - $OUT > /dev/null 2> /dev/null
	DIFFOK=$?
	if [ "x$DIFFOK" == "x0" ]; then
		echo "OK"
	else
		echo "DIFFERENCE to $OUT"
		cat $IN | $INTERPRETER $PRG | diff - $OUT
	fi
done;

rm ./tests/tests_from_school/*.prg

exit 0
