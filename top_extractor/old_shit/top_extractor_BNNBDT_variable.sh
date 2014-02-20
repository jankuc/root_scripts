#!/bin/sh

# Create txt file from root file

input_root_files_path=/work/budvar-clued0/francji/subsets/
output_root_files_path=/work/budvar-clued0/francji/subsets_txt_BNNBDT_variables/
script_path=/work/budvar-clued0/francji/scripts/top_extractor_BNNBDT

for samples in small_training_sample testing_sample yield_sample; do
  echo $samples;
  for channels in EqOneTag_EqTwoJet EqTwoTag_EqTwoJet EqOneTag_EqThreeJet EqTwoTag_EqThreeJet; do
    echo $channels;
    for signals in QCD tb tqb wlp wbb wcc ttbar-lepjets ttbar-dilepton zlp zbb zcc diboson DATA; do
       i=${input_root_files_path}/${samples}/p17_CC_${signals}_${channels}_zero_Topo_${samples}.root
       echo $i;
       ${script_path}_${channels} \
       $i \
       ${output_root_files_path}/${samples}/$(basename ${i/.root/})
    done
  done
done

