#!/bin/sh

# Create txt file from root file

input_root_files_path=/work/budvar-clued0/francji/subsets/
output_root_files_path=/work/budvar-clued0/francji/subsets_txt_selected_variables/
script_path=/work/budvar-clued0/francji/scripts/top_extractor_select_variables

for samples in small_training_sample testing_sample yield_sample; do
  mkdir -p ${output_root_files_path}/${samples}
  echo $samples;
  for i in ${input_root_files_path}/${samples}/*.root; do 
    echo $i
    ${script_path} \
    $i \
    ${output_root_files_path}/${samples}/$(basename ${i/.root/})
  done 
  cp /work/budvar-clued0/francji/subsets_txt/data_names_${samples}.txt  ${output_root_files_path}/${samples}/data_names_${samples}.txt
done
cp /work/budvar-clued0/francji/subsets_txt/data_names_DATA.txt  ${output_root_files_path}/yield_sample/data_names_DATA.txt

exit 0