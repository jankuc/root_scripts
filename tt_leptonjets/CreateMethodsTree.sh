#!/bin/sh

# Run CreateMethodsTree for all samples and given methods
# CreateMethodsTree.C creates root file with one tree,
# where discriminant from all channels are presented and
# add lists with EventWeight and ...

input_root_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples
output_root_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root
results_txt_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_txt
#script_path=~/scripts/CreateMethodsTree
script_path=/work/budvar-clued0/kuceraja/FNAL/scripts/tt_leptonjets/CreateMethodsTree

for samples in muo; do
  echo $samples;
  for methods in MBC ; do
    echo $methods
    for i in ${input_root_files_path}/split_trees_3samples_${samples}_1119/*.root; do
        echo $(basename $i)
        $script_path \
        $i \
        ${results_txt_path}/${methods}/${samples}/$(basename ${i/.root/.txt}) \
        ${methods} \
        ${output_root_files_path}/split_trees_3samples_${samples}_1119/${methods}/$(basename ${i/_Topo/})
    done
  done
done
