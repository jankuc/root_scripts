#!/bin/sh

# Run STMVACreateMethodsTree for all samples and given methods
# STMVACreateMethodsTree.C creates root file with one tree,
# where discriminant from s,t, and s+t channels are presented and 
# add lists with EventWeight and ...

input_root_files_path=/work/budvar-clued0/francji/subsets/
output_root_files_path=/work/budvar-clued0/francji/results/
paramethers_path=/work/budvar-clued0/francji/results/STMVA/
script_path=/work/budvar-clued0/francji/scripts/STMVA/STMVACreateMethodsTree

for methods in BNN; do
  echo $methods
  for samples in small_training_sample testing_sample yield_sample; do
    echo $samples
    outputs=${output_root_files_path}/${samples}/${methods}
    mkdir -p $outputs
    for tagbin in EqOneTag EqTwoTag; do
      for jetbin in EqTwoJet EqThreeJet; do
      param=${paramethers_path}/STMVA.params.out_${methods}_${tagbin}_${jetbin}
        for channel in diboson QCD tb tqb ttbar-dilepton ttbar-lepjets wbb wcc wlp zbb zcc zlp; do
          input=${input_root_files_path}/${samples}/p17_CC_${channel}_${tagbin}_${jetbin}_zero_Topo_${samples}.root
          output=${output_root_files_path}/${samples}/${methods}/p17_CC_${channel}_${tagbin}_${jetbin}_zero_Topo_${samples}.root
          echo $input
          echo $parm
	  $script_path \
	  $param \
	  $input\
          $output
        done
      done
    done
  done
  for samples in yield_sample; do
    echo $samples
    for tagbin in EqOneTag EqTwoTag; do
      for jetbin in EqTwoJet EqThreeJet; do
        param=${paramethers_path}/STMVA.params.out_${methods}_${tagbin}_${jetbin}
        input=${input_root_files_path}/${samples}/p17_CC_DATA_${tagbin}_${jetbin}_zero_Topo_${samples}.root
        output=${output_root_files_path}/${samples}/${methods}/p17_CC_DATA_${tagbin}_${jetbin}_zero_Topo_${samples}.root
        echo $input
        $script_path \
        $param \
        $input\
        $output
      done
    done
  done
done



