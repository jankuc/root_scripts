#!/bin/sh

# Create txt file from root file

input_root_files_path=/work/budvar-clued0/francji/subsets/
output_root_files_path=/work/budvar-clued0/francji/results_txt/D0_Discriminants
script_path=/work/budvar-clued0/francji/scripts/top_extractor_Discriminants


for samples in yield_sample; do  #small_training_sample testing_sample
  mkdir -p ${output_root_files_path}/${samples}
  echo $samples;
  for channels in EqOneTag_EqTwoJet EqTwoTag_EqTwoJet EqOneTag_EqThreeJet EqTwoTag_EqThreeJet; do
    echo $channels;
    for signals in QCD tb tqb wlp wbb wcc ttbar-lepjets ttbar-dilepton zlp zbb zcc diboson; do
       i=${input_root_files_path}/${samples}/p17_CC_${signals}_${channels}_zero_${samples}.root
       echo $i;
       ${script_path}\
       $i \
       ${output_root_files_path}/${samples}/$(basename ${i/.root/.txt})
    done
  done
 # cp /work/budvar-clued0/francji/subsets_txt/data_names_${samples}.txt  ${output_root_files_path}/${samples}/data_names_${samples}.txt
done



for samples in yield_sample; do #small_training_sample testing_sample y
  echo $samples;
  for channels in EqOneTag_EqTwoJet EqTwoTag_EqTwoJet EqOneTag_EqThreeJet EqTwoTag_EqThreeJet; do
    echo $channels;
    for signals in DATA; do
       i=${input_root_files_path}/${samples}/p17_CC_${signals}_${channels}_zero_${samples}.root
       echo $i;
       ${script_path}\
       $i \
       ${output_root_files_path}/${samples}/$(basename ${i/.root/.txt})
    done
  done
done
#cp /work/budvar-clued0/francji/subsets_txt/data_names_DATA.txt  ${output_root_files_path}/yield_sample/data_names_DATA.txt
exit 0
