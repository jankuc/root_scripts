#!/bin/bash

# builds all *Tree.C files

for i in *.C; do
	echo "building $i"
	echo
	g++ -Wall `root-config --libs --cflags` -g -o ${i/.C/} ${i}
done
