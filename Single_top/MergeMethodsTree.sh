#!/bin/sh

# Run MergeMethodsTree.sh for all samples and given methods
# MergeMethodsTree creates root file with one tree TopologicalVariables,
# where discriminants from all channels are presented and 
# add lists with EventWeight and ...

input_root_files_path=/work/budvar-clued0/francji/results/
output_root_files_path=/work/budvar-clued0/francji/
#script_path=~/scripts/CreateMethodsTree
script_path=/work/budvar-clued0/francji/scripts/MergeMethodsTree
#yield_sample
for samples in yield_sample; do # small_training_sample testing_sample yield_sample
  echo $samples;
  for methods in ME DT BNN BNNComb GLM_probit GLM_probit_svd GLM_logit GLM_logit_svd GLM_cloglog GLM_cloglog_svd; do # NNSU DO_BNNComb TMVA_BDT TMVA_BNN
   echo $methods
   mkdir -p ${output_root_files_path}/results_TopologicalVariables/${samples}
    for i in ${input_root_files_path}/$methods/${samples}/*; do
	echo $i
	$script_path  \
	$i \
	$methods \
	${output_root_files_path}/results_TopologicalVariables/${samples}/$(basename ${i})
	#${output_root_files_path}/D0_${methods}/${samples}/$(basename ${i/yield_sample.root/${samples}.root})
    done
  done
done
