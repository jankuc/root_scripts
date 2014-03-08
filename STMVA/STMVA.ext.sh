#!/bin/sh

# Run STMVA for all samples and given methods
# STMVA creates xml files for learned methods 

#input_root_files_path=/work/budvar-clued0/francji/subsets
#output_root_files_path=/work/budvar-clued0/francji/results
paramethers_path=/work/budvar-clued0/francji/scripts/STMVA/params/EXT1
script_path=/work/budvar-clued0/francji/scripts/STMVA/STMVA

for i in ${paramethers_path}/*; do
 echo $i
 #sleep 15[s]
 $script_path \
 $i
done


