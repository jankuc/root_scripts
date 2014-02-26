#!/bin/bash

# builds all *Tree.C files

for i in *.C; do
	echo 
	echo $i
	echo
	g++ -Wall `root-config --libs --cflags` -g -o ${i/.C/} $i
done
