#!/bin/bash


variable="$1"
from="$2"
to="$3"
line="$4"


if [ "$(( from > to ))" == "1" ]
then
	echo "from < to... NYI"
	exit 1
fi

for (( val=from ; val<=to ; val++ ))
do

	lastTempLine=""
	tempLine="$line"
	asdfNum=0

#Replace all occurances of "variable" with a discernable one...
	while [ 1 ]
	do
		lastTempLine=$tempLine
		tempLine=${tempLine/$variable/asdf.Temp.$asdfNum}

		if [ "$lastTempLine" == "$tempLine" ]
		then
			break
		fi
#		echo "temp:'$tempLine'"
#		echo "last:'$lastTempLine'"
		asdfNum=$((asdfNum+1))
	done

#	echo "found $asdfNum occurances of '$variable'"
#	echo "temp: '$tempLine'"

#Check each occurance for trailing +#'s
# THIS IS HIGHLY LIMITED
# It only if it's *exactly* "var+n:::" no spaces,
# etc. and n=is a digit.
	for (( thisOccurance=0 ; thisOccurance < asdfNum ; thisOccurance++ ))
	do
		tempLinePost="${tempLine#*asdf.Temp.$thisOccurance}"
#		echo "post: '$tempLinePost'"
		doAddition="${tempLinePost#+[[:digit:]]}"
#		echo "do: '$doAddition'"
		if [ "$doAddition" != "$tempLinePost" ]
		then
			addition="${tempLinePost#+}"
#			echo "add: '$addition'"
			addition="${addition:0:1}"
#			echo "add: '$addition'"

			newVal=$val
			isdigit.sh "$addition"

			if [ "$?" == "0" ]
			then
				newVal=$(( val + addition ))
#				echo "adding '$newVal'..."
				tempLine=${tempLine/asdf.Temp.$thisOccurance+[[:digit:]]/$newVal}
			else
#				echo "replacing 'asdf.Temp.$thisOccurance' with '$val'"
				tempLine=${tempLine/asdf.Temp.$thisOccurance/$val}
			fi
		else
#			echo "replacing 'asdf.Temp.$thisOccurance' with '$val'"
			tempLine=${tempLine/asdf.Temp.$thisOccurance/$val}
		fi
	done


	echo "$tempLine"
#	echo "${line//$variable/$val}"
done 

