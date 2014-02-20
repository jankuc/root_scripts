#!/bin/sh

# Create directories for diferent settings of given methods 

target_path=/work/budvar-clued0/francji/results_txt
script_path=/work/budvar-clued0/francji/scripts/

for links in logit probit cloglog; do
  for samples in small_training_sample testing_sample yield_sample; do
    for signals in tb tqb tbtqb; do
        mkdir -p  ${target_path}/GLM_BDT_${links}/${samples}/${signals}
        mkdir -p  ${target_path}/GLM_BDT_${links}_svd/${samples}/${signals}
        mkdir -p  ${target_path}/GLM_BDT_${links}_step/${samples}/${signals}
        mkdir -p  ${target_path}/GLM_BDT_${links}_svd_step/${samples}/${signals}
    done
  done
done