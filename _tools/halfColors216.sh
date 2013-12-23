#!/bin/bash

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

rValid="0
0+0
0+3
3+0
1
1+1
1+3
3+1
2+3
3+2
3
3+3"

gValid="0
0+0
0+2
2+0
1
1+1
1+2
2+1
2
2+2
3
3+3"

bValid="0
0+0
0+2
2+0
0+3
3+0
2
2+2
2+3
3+2
2
3+3"

for line in $fileCat
do

	shopt -s extglob
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

#shopt -u extglob
	unset IFS
	read red green blue name <<< "$line"
	IFS="
"
	
	read -d g rTot <<< "$name"
	read -d b gTot <<< "$name"
	
	rTot="${rTot#r}"
	bTot="${name#r*b}"
	gTot="${gTot#r*g}"

#echo "r:'$rTot' g:'$gTot' b:'$bTot'"


	rOK=0
	for rTest in $rValid
	do
		if [ "$rTest" == "$rTot" ]
		then
			rOK=1
			break
		fi
	done

	gOK=0
	for gTest in $gValid
	do
		if [ "$gTest" == "$gTot" ]
		then
			gOK=1
			break
		fi
	done

	bOK=0
	for bTest in $gValid
	do
		if [ "$bTest" == "$bTot" ]
		then
			bOK=1
			break
		fi
	done


	if [ "$(( rOK + gOK + bOK ))" == "3" ]
	then
		echo "$line"
	else
		echo "'$line' invalid" >&2
	fi

done

return 0

}


fileCat=`cat LCDdirectLVDS_+HalfColors.gpl`
#halfColorsPurged.gpl`

#in case there's no newline at the end of the file...
fileCat="$fileCat
"

#unsorted=`parseColors 1 "" "$fileCat"`
parseColors 1 "" "$fileCat"


#Prune "$unsorted"
