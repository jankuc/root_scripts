#!/bin/bash

# Run CreateMethodsTree for all leptons (muo, ele) and given methods
#
# CreateMethodsTree.C creates root files for all channels, in which there is
# discriminant of given method and appropriate weight.

input_root_files_path=/work/budvar-clued0/kuceraja/tt_leptonjets/samples_root
output_root_files_path=/work/budvar-clued0/kuceraja/tt_leptonjets/results_root
results_txt_path=/work/budvar-clued0/kuceraja/tt_leptonjets/results_txt
# assumed path: results_txt/ {method name} / {ele/muo} / {data, QCD, ...}_miniTree.txt
script_path=/work/budvar-clued0/kuceraja/FNAL/scripts/tt_leptonjets/CreateMethodsTree

for leptons in muo ele; do
    echo $leptons
  #  for methods in IINC0_no_gain_var_all_nonzero_weights IINC2_no_gain_var_all_nonzero_weights MBC_1_1_MD_noTransformation MBC_1_1_noTransformation MBC_50_20_MD_noTransformation NNSU_no_gain_var_all_nonzero_weights RF_no_gain_var_all_nonzero_weights ; do
	  for methods in  MBC_1_1_MD_noTransformation MBC_1_1_noTransformation MBC_50_20_MD_noTransformation   ; do
        echo $methods
        for i in ${input_root_files_path}/split_trees_3samples_${leptons}_1119/*.root; do
            echo $(basename $i)
            mkdir -p "${output_root_files_path}/${methods}/${leptons}/"
            $script_path \
            $i \
            ${results_txt_path}/${methods}/${leptons}/$(basename ${i/.root/.txt}) \
            ${methods} \
            ${output_root_files_path}/${methods}/${leptons}/$(basename ${i})
        done
    done
done
