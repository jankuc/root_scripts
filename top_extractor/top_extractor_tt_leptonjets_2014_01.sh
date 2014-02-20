#!/bin/sh

# Create txt file from root file

input_root_files_path=/work/budvar-clued0/francji/tt_leptonjets_2014_01/samples/
output_root_files_path=/work/budvar-clued0/francji/tt_leptonjets_2014_01/samples_txt
script_path=/work/budvar-clued0/francji/scripts/top_extractor_new_2014_01

for samples in split_trees_3samples_ele_1119 split_trees_3samples_muo_1119; do #test_tree train_tree split_trees
  mkdir -p ${output_root_files_path}/${samples}
  echo $samples;
  for i in ${input_root_files_path}/${samples}/*.root; do
    echo $i
    ${script_path} \
    $i \
    ${output_root_files_path}/${samples}/$(basename ${i/.root/})
  done
done

exit 0