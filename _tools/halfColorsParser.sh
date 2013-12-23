#!/bin/bash

file="$1"

function purgeColors {
local red="$1"
local green="$2"
local blue="$3"
local name="$4"
local remainingLines="$5"
local red2=""
local green2=""
local blue2=""
local extracted=""
local otherColor=""
local line=""

IFS="
"

# Print the original color...
#redName="$(( red * 3 / 255 ))"
#greenName="$(( green * 3 / 255 ))"
#blueName="$(( blue * 3 / 255 ))"

for line in $remainingLines
do

#	oldifs="$IFS"
	unset IFS
#extracted="`extractColors "$otherColor"`"
	read red2 green2 blue2 name2 <<< "$line"
#"$extracted"
	IFS="
"

#echo "comparing to: '$red2' '$green2' '$blue2' '$name2'"

	if [ "$red2" == "$red" ]
	then
		if [ "$green2" == "$green" ]
		then
			if [ "$blue2" == "$blue" ]
			then
				echo "match found" >&2
				echo "  '$line'" >&2
				printf "   %3d %3d %3d %s\n" $red $green $blue "$name" >&2
				return 1
			fi
		fi
	fi
done

return 0
}


function extractColors {
local colorLine="$1"
local red=""
local green=""
local blue=""


	red="${colorLine%%+([[:blank:]])+([[:digit:]])+([[:blank:]])+([[:digit:]])}"

	green="${colorLine%%+([[:blank:]])+([[:digit:]])}"
	green="${green##*([[:blank:]])+([[:digit:]])+([[:blank:]])}"

	blue="${colorLine##*([[:blank:]])+([[:digit:]])+([[:blank:]])+([[:digit:]])+([[:blank:]])}"


#	echo "extractColors: '$red', '$green', '$blue'" >&2
	echo "$red $green $blue"
}


function parseColors {
local fileCat="$3"
local startLine="$2"
local doRecurse="$1"
local indent=0
local startLineFound=0
local red=0
local green=0
local blue=0

local line=""
local colorLine=""

local others=""

if [ "$doRecurse" == "0" ]
then
#echo "looking for '$startLine'"
	indent=3
fi

IFS="
"

shopt -s extglob

for line in $fileCat
do
#echo "$line"


# This should no longer be necessary a/o lineRemovalFromFileCat
#	if [ "$doRecurse" == "0" ]
#	then
#		if [ "$startLineFound" == "0" ]
#		then
#			if [ "$line" == "$startLine" ]
#			then
#echo "...found"
#				startLineFound=1
#			fi
#
#			continue
#		fi		
#	fi


# This should speed things up a bit...
# And could probably remove the necessity for recursion...
	fileCat="${fileCat#*"
"}"

	colorLine="${line%+([[:blank:]])r*g*b*}"

	# Repeat the GIMP header...
	if [ "$colorLine" == "$line" ]
	then
		echo "$line"
		continue
	fi

#echo "Line: '$line'"

	unset IFS
	read red green blue name <<< "$line"
	IFS="
"
	
#echo "parseColors:   '$red', '$green', '$blue' '$name'" >&2

	if [ "$doRecurse" == "1" ]
	then
#		echo "recursing" >&2
#		others=`parseColors 0 "$line" "$fileCat"`
#		echo "others: '$others'" >&2
		purgeColors $red $green $blue "$name" "$fileCat"

		if [ "$?" == "0" ]
		then
			echo "$line"
			echo "Adding line: '$line'" >&2
		fi
	#echo "next...?"
	#WTF, once the variables in this function are declared, they're reused
	#in all recursions?!
	#How is it I never ran into this before...?!
	# Maybe I've never used a recursive function, and instead had recursive
	# calls to script files...?
	# Either way, it looks like "local" in the definitions fixes it.
#		doRecurse=1
	else
		echo "$line"
#"$red $green $blue"
	fi


done

return 0

}


#function Prune {
#local lines="%1"
#
#IFS="
#"
#
#for line in $lines
#do
#	
#
#}

fileCat=`cat "$file"`

#in case there's no newline at the end of the file...
fileCat="$fileCat
"

#unsorted=`parseColors 1 "" "$fileCat"`
parseColors 1 "" "$fileCat"


#Prune "$unsorted"
