#!/bin/sh

# Run CreateMethodsTree for all samples and given methods
# CreateMethodsTree.C creates root file with one tree,
# where discriminant from all channels are presented and 
# add lists with EventWeight and ...

input_root_files_path=/work/budvar-clued0/francji/subsets
output_root_files_path=/work/budvar-clued0/francji/results
results_txt_path=/work/budvar-clued0/francji/results_txt
#script_path=~/scripts/CreateMethodsTree
script_path=/work/budvar-clued0/francji/scripts/CreateMethodsTree 

for samples in small_training_sample testing_sample yield_sample; do
  echo $samples;
  for methods in GLM ; do
    echo $methods
    for i in ${input_root_files_path}/${samples}/*; do
	echo $i
	$script_path  \
	$i \
	${results_txt_path}/${samples}/${methods}/tb/$(basename ${i/.root/.txt})    \
	${results_txt_path}/${samples}/${methods}/tqb/$(basename ${i/.root/.txt})   \
	${results_txt_path}/${samples}/${methods}/tbtqb/$(basename ${i/.root/.txt}) \
	MBC \
        ${output_root_files_path}/${samples}/MBC/$(basename ${i/_Topo/})
    done
  done
done