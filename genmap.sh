#!/bin/bash

genmap(){

local width height i j carac charArray randNum

let carac='0'

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
	for((i = 1; i<= width; i++))
	do
		for((j=1; j<= height; j++))
		do
			#Get random num for ASCII
			let randNum=$((RANDOM%90+33))
			carac=\\$(printf '%03o' $(($randNum)))
			printf "\e[$i;${j}f\e[1;34m${carac}\e[0m";
		done
	done
}
genmap "$@"
