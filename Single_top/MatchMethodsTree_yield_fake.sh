#!/bin/sh

# Match individual methods root files together 

input_rootfiles_path=/work/budvar-clued0/francji/subsets/
input_results_path=/work/budvar-clued0/francji/results/
output_results_path=/work/budvar-clued0/francji/results/

mkdir -p ${output_results_path}/yield_sample
mkdir -p ${output_results_path}/small_training_sample
mkdir -p ${output_results_path}/testing_sample

for i in ${input_rootfiles_path}/yield_sample/*; do
  echo $i
  hadd ${output_results_path}/yield_sample/$(basename ${i}) ${input_results_path}/{D0_ME,D0_DT,D0_BNN,D0_BNNComb,GLM_probit,GLM_probit_svd,GLM_logit,GLM_logit_svd,GLM_cloglog,GLM_cloglog_svd}/yield_sample/$(basename ${i})
# ${input_results_path}/${samples}/{BDT,BNN}/$(basename ${i})
  hadd ${output_results_path}/small_training_sample/$(basename ${i/Topo_yield_sample/small_training_sample}) ${input_results_path}/{D0_ME,D0_DT,D0_BNN,D0_BNNComb,GLM_probit,GLM_probit_svd,GLM_logit,GLM_logit_svd,GLM_cloglog,GLM_cloglog_svd}/yield_sample/$(basename ${i})
  hadd ${output_results_path}/testing_sample/$(basename ${i/Topo_yield_sample/testing_sample}) ${input_results_path}/{D0_ME,D0_DT,D0_BNN,D0_BNNComb,GLM_probit,GLM_probit_svd,GLM_logit,GLM_logit_svd,GLM_cloglog,GLM_cloglog_svd}/yield_sample/$(basename ${i})

done


