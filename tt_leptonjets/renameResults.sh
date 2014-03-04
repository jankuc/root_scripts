#!/bin/bash

# BEWARE: all files must already be in their respective directories {method} / {muo/ele}
# renames all .txt files, si they match {QCD, data,...}_miniTree.txt

result_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_txt/

for i in $result_path/*; do # loop over all method directories 
	# rename *ele* -> ele, *muo* -> muo
	for j in $i/muo $i/ele; do #loop over leptons in each method
		cd $j
		rename 's/([.]*)txt.+/$1txt/g' * 
	done
done