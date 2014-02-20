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

for samples in small_training_sample testing_sample yield_sample; do  #small_training_sample testing_sample yield_sample
  echo $samples;
  for methods in GLM_probit GLM_probit_svd GLM_logit GLM_logit_svd GLM_cloglog GLM_cloglog_svd; do
   # GLM MBC NNSU MBC_20_1 MBC_20_5 MBC_40_1 MBC_40_5 MBC_40_20 MBC_80_20 
    echo $methods
    #mkdir -p ${output_root_files_path}/${samples}/$methods
    mkdir -p ${output_root_files_path}/${methods}/${samples}
    for i in ${input_root_files_path}/${samples}/*; do
        #i=$(basename ${i/_Topo/});
	echo $i;
	$script_path  \
	$i \
	${results_txt_path}/${methods}/${samples}/tb/$(basename ${i/.root/.txt})    \
	${results_txt_path}/${methods}/${samples}/tqb/$(basename ${i/.root/.txt})   \
	${results_txt_path}/${methods}/${samples}/tbtqb/$(basename ${i/.root/.txt}) \
	$methods \
        ${output_root_files_path}/${methods}/${samples}/$(basename ${i})
    done
  done
done