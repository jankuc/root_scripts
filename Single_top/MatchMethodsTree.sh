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
hadd ${output_results_path}/${samples}/$(basename ${i/_Topo/}) ${input_results_path}/${samples}/{GLM,MBC,NNSU}/$(basename ${i/_Topo/}) ${input_results_path}/${samples}/{D0_BNN,D0_BDT,D0_ME,D0_BNNComb}/$(basename  ${i/_Topo/})

  done
done