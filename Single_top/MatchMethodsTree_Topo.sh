#!/bin/sh

# Match individual methods root files together 

input_rootfiles_path=/work/budvar-clued0/francji/subsets
input_results_path=/work/budvar-clued0/francji/results_Topo
output_results_path=/work/budvar-clued0/francji/results_Topo

for samples in small_training_sample testing_sample yield_sample; do
  for i in ${input_rootfiles_path}/${samples}/*; do
    echo $samples;
    echo $i;
    hadd ${output_results_path}/${samples}/$(basename ${i}) ${input_results_path}/${samples}/{GLM,MBC}/$(basename ${i})
  done
done