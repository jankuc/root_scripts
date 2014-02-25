#!/bin/sh

# Match individual methods root files together 

input_rootfiles_path=/work/budvar-clued0/francji/subsets
input_results_path=/work/budvar-clued0/francji/results
output_results_path=/work/budvar-clued0/francji/results

for samples in small_training_sample testing_sample yield_sample yield_sample; do
  for i in ${input_rootfiles_path}/${samples}/*; do
    echo $samples;
    mkdir -p ${output_results_path}/${samples}
#    hadd ${output_results_path}/${samples}/$(basename ${i/_Topo/}) ${input_results_path}/${samples}/{GLM,GLM2,GLM_step,GLM2_step,MBC}/$(basename ${i/_Topo/}) ${input_results_path}/${samples}/{TMVA_BDT,TMVA_BNN,D0_BNN,D0_BDT,D0_ME,D0_BNNComb}/$(basename  ${i/_Topo/})
	hadd \
	${output_results_path}/${samples}/$(basename ${i/_Topo/}) \
	${input_results_path}/${samples}/{GLM,MBC,NNSU}/$(basename ${i/_Topo/}) \
	${input_results_path}/${samples}/{D0_BNN,D0_BDT,D0_ME,D0_BNNComb}/$(basename  \
	${i/_Topo/})

  done
done

#!/bin/bash

# Run MergeMethodsTree.sh for all leptons and given methods
# MergeMethodsTree creates root file with one tree nn_Tree,
# where discriminants from all channels are presented and 
# add lists with EventWeight and ...

input_root_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root
output_root_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root/merged_methods
#script_path=~/scripts/CreateMethodsTree
script_path=/work/budvar-clued0/kuceraja/FNAL/scripts/tt_leptonjets/MergeMethodsTree

for leptons in ele muo; do # muo ele
	echo $leptons;
	for methods in MBC_1_1_MD_noTrans MBC_50_20_MD_noTrans/; do # NNSU DO_BNNComb TMVA_BDT TMVA_BNN
		echo $methods
		mkdir -p ${output_root_files_path}/ssplit_trees_3samples_${leptons}_1119/
		for i in ${input_root_files_path}/${methods}/split_trees_3samples_${leptons}_1119/*; do
			echo $i
			$script_path  \
			$i \
			$methods \
			${input_root_files_path}/${methods}/split_trees_3samples_${leptons}_1119/$(basename ${i})
		done
	done
done
