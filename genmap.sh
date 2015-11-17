#!/bin/bash

genmap(){

local width height i j carac charArray randNum blanks half

let carac='0'
let blanks=0

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
				printf "STDERR too many blanks"
			fi
			carac=\\$(printf '%03o' $(($randNum)))
			printf "\e[$i;${j}f\e[1;34m${carac}\e[0m";
		done
	done
}
genmap "$@"
