#!/bin/sh

# Create txt file from root file

input_root_files_path=/work/budvar-clued0/francji/subsets/
output_root_files_path=/work/budvar-clued0/francji/subsets_txt_some_variables/
script_path=/work/budvar-clued0/francji/scripts/top_extractor_some_variables

for samples in small_training_sample testing_sample yield_sample; do
  echo $samples;
  for i in ${input_root_files_path}/${samples}/*.root; do 
    echo $i
    ${script_path} \
    $i \
    ${output_root_files_path}/${samples}/$(basename ${i/.root/})
  done 
done



