#!/bin/bash

function mergeColors {
local red="$1"
local green="$2"
local blue="$3"
local others="$4"
local red2=""
local green2=""
local blue2=""
local redH=""
local greenH=""
local blueH=""
local redHname=""
local greenHname=""
local blueHname=""
local extracted=""
local otherColor=""

IFS="
"

# Print the original color...
redName="$(( red * 3 / 255 ))"
greenName="$(( green * 3 / 255 ))"
blueName="$(( blue * 3 / 255 ))"

printf "%3d %3d %3d r%sg%sb%s\n" $red $green $blue \
							"$redName" "$greenName" "$blueName"	

for otherColor in $others
do

	oldifs="$IFS"
	unset IFS
	extracted="`extractColors "$otherColor"`"
	read red2 green2 blue2 <<< "$extracted"
	IFS="
"

#echo "mergeColors:   '$red', '$green', '$blue'" >&2

#echo "$red2" >&2

	redH="$(( (red+red2)/2 ))"
	greenH="$(( (green+green2)/2 ))"
	blueH="$(( (blue+blue2)/2 ))"

	redHname="$(( red * 3 / 255))+$(( red2 * 3 / 255 ))"
   greenHname="$(( green * 3 / 255))+$(( green2 * 3 / 255 ))"
	blueHname="$(( blue * 3 / 255))+$(( blue2 * 3 / 255 ))"

	printf "%3d %3d %3d r%sg%sb%s\n" $redH $greenH $blueH \
							"$redHname" "$greenHname" "$blueHname"	


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

	if [ "$doRecurse" == "0" ]
	then
		if [ "$startLineFound" == "0" ]
		then
			if [ "$line" == "$startLine" ]
			then
#echo "...found"
				startLineFound=1
			fi

			continue
		fi		
	fi


	colorLine="${line%+([[:blank:]])r[[:digit:]]g[[:digit:]]b[[:digit:]]}"

	# Repeat the GIMP header...
	if [ "$colorLine" == "$line" ]
	then
		echo "$line"
		continue
	fi

	colorLine="${colorLine##*([[:blank:]])}"


	unset IFS
	extractedColors="`extractColors "$colorLine"`"
	read red green blue <<< "$extractedColors"
	IFS="
"
	
#	echo "parseColors:   '$red', '$green', '$blue'" >&2

	if [ "$doRecurse" == "1" ]
	then
#		echo "recursing" >&2
		others=`parseColors 0 "$line" "$fileCat"`
#		echo "others: '$others'" >&2
		mergeColors $red $green $blue "$others"
	#echo "next...?"
	#WTF, once the variables in this function are declared, they're reused
	#in all recursions?!
	#How is it I never ran into this before...?!
	# Maybe I've never used a recursive function, and instead had recursive
	# calls to script files...?
	# Either way, it looks like "local" in the definitions fixes it.
#		doRecurse=1
	else
		echo "$red $green $blue"
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

fileCat=`cat LCDdirectLVDS_48c.gpl`

#unsorted=`parseColors 1 "" "$fileCat"`
parseColors 1 "" "$fileCat"


#Prune "$unsorted"
