#!/bin/bash


# THE IDEA:
# If we preprocess mainConfig.h before running 'make' on the project
# we can conditionally include stuff as needed

# A good test: 'configParser.sh TRUE'
# Also: __REGISTER_PREFIX__ (defined, with no value)

macroToFind="$1"

if [ "$1" == "" ]
then
	echo "Usage: 'configParser.sh <macroName>'"
	exit 1
fi

ppFile="_BUILD/mainConfig.preprocessed"
cfgFile="mainConfig.h"


gcc -o "$ppFile" -E -dM -D'TRUE=1' -D'FALSE=0' "$cfgFile"


if [ "$?" != "0" ]
then
	echo "preprocessing of mainConfig.h failed..."
	exit 1
fi


lines="`grep "$macroToFind" "$ppFile"`"

if [ "$?" != "0" ]
then
	echo "'$macroToFind' Not Defined"
	exit 1
fi

IFS="
"
definitionFound=0
for line in $lines
do

	value="${line#"#define "$macroToFind }"

	if [ "$value" != "$line" ]
	then
		echo "Found Definition: '$line' with value: '$value'"
		echo "MAKE_MAINCONFIG__$macroToFind=\"$value\""
		definitionFound=$((definitionFound+1))
	else
		echo "Found something else: '$line'"
	fi
done

if [ "$definitionFound" != "0" ]
then
	exit 0
else
	exit 1
fi


#while [ 1 ]
#do
#	read -r line <&10 || break

#done 10<"$ppFile"


