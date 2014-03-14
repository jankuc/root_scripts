#!/bin/sh

# Match individual methods root files together 

input_rootfiles_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples
input_results_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root
output_results_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root_matched



for leptons in ele muo; do # muo ele
	echo $leptons;
	for i in ${input_rootfiles_path}/split_trees_3samples_${leptons}_1119/* ; do	
		mkdir -p ${output_results_path}/split_trees_3samples_${leptons}_1119
		hadd \
		${output_results_path}/split_trees_3samples_${leptons}_1119/$(basename ${i}) \
		${input_results_path}/*/${leptons}/$(basename ${i}) \
		${i}
  done
done
