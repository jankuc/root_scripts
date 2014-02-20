#!/bin/sh

# print variables from root file

input_root_files_path=/work/budvar-clued0/francji/subsets/
script_path=/work/budvar-clued0/francji/scripts/VariablesPrint3

for samples in yield_sample; do  #small_training_sample testing_sample yield_sample
  echo $samples;
  for channels in EqOneTag_EqTwoJet; do #EqOneTag_EqTwoJet EqTwoTag_EqTwoJet EqOneTag_EqThreeJet EqTwoTag_EqThreeJet; do
    echo $channels;
    i=${input_root_files_path}/${samples}/p17_CC_tb_${channels}_zero_${samples}.root
    #echo $i;
    ${script_path} \
    $i
  done
done

# /work/budvar-clued0/francji/tt_new/test/test_tree/QCD_miniTree.root    nn_tree