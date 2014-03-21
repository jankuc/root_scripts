#!/bin/bash

# builds all *.C files

for i in *.C; do
	if [ -e ${i/.C/} ] ; then
		if [ $( stat -c%Y $i) -ge $( stat -c%Y ${i/.C/}) ] ; then
			echo "building $i"
			g++ -Wall `root-config --libs --cflags` -g -o ${i/.C/} ${i}
		fi
	else
		echo "building $i"
		g++ -Wall `root-config --libs --cflags` -g -o ${i/.C/} ${i}
	fi
done
