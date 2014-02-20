#!/bin/sh

# Match individual methods root files together 

input_rootfiles_path=/work/budvar-clued0/francji/subsets
input_results_path=/work/budvar-clued0/francji/results
output_results_path=/work/budvar-clued0/francji/results

for samples in yield_sample; do
  mkdir -p ${output_results_path}/${samples}
  for i in ${input_rootfiles_path}/${samples}/*; do
    echo $samples;
    hadd ${output_results_path}/${samples}/$(basename ${i/_Topo/}) ${input_results_path}/{GLM,MBC,NNSU,D0_ME,D0_BNN,D0_DT,D0_BNNComb}/${samples}/$(basename ${i/_Topo/})
# ${input_results_path}/${samples}/{BDT,BNN}/$(basename ${i})
  done
done