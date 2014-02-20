#!/bin/sh

# Create txt file from root file

input_root_files_path=/work/budvar-clued0/francji/subsets/
output_root_files_path=/work/budvar-clued0/francji/subsets_txt_all_variables/
script_path_STS=/work/budvar-clued0/francji/scripts/top_extractor_all_variables_STS
script_path_TS=/work/budvar-clued0/francji/scripts/top_extractor_all_variables_TS
script_path_YS=/work/budvar-clued0/francji/scripts/top_extractor_all_variables_YS

for samples in small_training_sample; do
  mkdir -p ${output_root_files_path}/${samples}
  echo $samples;
  for i in ${input_root_files_path}/${samples}/*.root; do
    echo $i
    ${script_path_STS} \
    $i \
    ${output_root_files_path}/${samples}/$(basename ${i/.root/})
  done
  cp /work/budvar-clued0/francji/subsets_txt/data_names_${samples}.txt  ${output_root_files_path}/${samples}/data_names_${samples}.txt
done

for samples in testing_sample; do
  mkdir -p ${output_root_files_path}/${samples}
  echo $samples;
  for i in ${input_root_files_path}/${samples}/*.root; do
    echo $i
    ${script_path_TS} \
    $i \
    ${output_root_files_path}/${samples}/$(basename ${i/.root/})
  done
  cp /work/budvar-clued0/francji/subsets_txt/data_names_${samples}.txt  ${output_root_files_path}/${samples}/data_names_${samples}.txt
done

for samples in yield_sample; do
  mkdir -p ${output_root_files_path}/${samples}
  echo $samples;
  for i in ${input_root_files_path}/${samples}/*.root; do
    echo $i
    ${script_path_YS} \
    $i \
    ${output_root_files_path}/${samples}/$(basename ${i/.root/})
  done
  cp /work/budvar-clued0/francji/subsets_txt/data_names_${samples}.txt  ${output_root_files_path}/${samples}/data_names_${samples}.txt
done


cp /work/budvar-clued0/francji/subsets_txt/data_names_DATA.txt  ${output_root_files_path}/yield_sample/data_names_DATA.txt

exit 0


