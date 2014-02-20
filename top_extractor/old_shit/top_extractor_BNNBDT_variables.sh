#!/bin/sh

# Create txt file from root file

input_root_files_path=/work/budvar-clued0/francji/subsets/
output_root_files_path=/work/budvar-clued0/francji/subsets_txt_BNNBDT_variables/
script_path=/work/budvar-clued0/francji/scripts/top_extractor_BNNBDT


for samples in small_training_sample testing_sample yield_sample; do
  mkdir -p ${output_root_files_path}/${samples}
  echo $samples;
  for channels in EqOneTag_EqTwoJet EqTwoTag_EqTwoJet EqOneTag_EqThreeJet EqTwoTag_EqThreeJet; do
  #for channels in EqOneTag_EqThreeJet; do
    echo $channels;
    for signals in QCD tb tqb wlp wbb wcc ttbar-lepjets ttbar-dilepton zlp zbb zcc diboson; do
       i=${input_root_files_path}/${samples}/p17_CC_${signals}_${channels}_zero_${samples}.root
       echo $i;
       ${script_path}_${channels} \
       $i \
       ${output_root_files_path}/${samples}/$(basename ${i/.root/.txt})
    done
  done
  cp /work/budvar-clued0/francji/subsets_txt/data_names_${samples}.txt  ${output_root_files_path}/${samples}/data_names_${samples}.txt
done

for samples in small_training_sample testing_sample yield_sample; do
  echo $samples;
  for channels in EqOneTag_EqTwoJet EqTwoTag_EqTwoJet EqOneTag_EqThreeJet EqTwoTag_EqThreeJet; do
    echo $channels;
    for signals in DATA; do
       i=${input_root_files_path}/${samples}/p17_CC_${signals}_${channels}_zero_${samples}.root
       echo $i;
       ${script_path}_${channels} \
       $i \
       ${output_root_files_path}/${samples}/$(basename ${i/.root/.txt})
    done
  done
done
cp /work/budvar-clued0/francji/subsets_txt/data_names_DATA.txt  ${output_root_files_path}/yield_sample/data_names_DATA.txt
exit 0
