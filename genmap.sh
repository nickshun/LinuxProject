#!/bin/bash

genmap(){

local width height i j carac charArray randNum blanks half badRows
declare -A map

let carac='0'
let blanks=0
let badRows=0

if [ -z "$1" ] 
then
	width=50
        height=50
elif [ -z "$2" ]
then
	width=$1
	height=10
else
        width=$1
        height=$2
fi
let half=width/2
	for((i = 1; i<= width; i++))
	do
		let blanks=0
		for((j=1; j<= height; j++))
		do
			#Get random num for ASCII
			let randNum=$((RANDOM%90+32))
			if [ $randNum -eq 32 ]
			then
				let "blanks++"
			fi
			if [ "$blanks" -gt "$half" ]
			then
				let "badRows++"
			fi
			carac=\\$(printf '%03o' $(($randNum)))
		
			if [ "$j" -eq "$width" ]
			then
				map[$i,$j]='\n'
			else
				map[$i,$j]=$carac
			fi
		done
	done
if [ "$badRows" -gt "$half" ]
then
	echo "TOO MANY BLANKS IN THIS DOCUMENT"
else
	for((i=1; i<= width; i++))
	do
		for((j=1; j<= height; j++))
		do
			printf "${map[$i,$j]}"
		done
	done
fi
}
genmap "$@"
