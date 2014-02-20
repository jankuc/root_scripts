#!/bin/sh

# Create txt file from root file

input_root_files_path=/work/budvar-clued0/francji/subsets/
output_root_files_path=/work/budvar-clued0/francji/subsets_txt_BNNBDT_variables/
script_path1=/work/budvar-clued0/francji/scripts/top_extractor_BNNBDT_EqOneTag_EqTwoJet
script_path2=/work/budvar-clued0/francji/scripts/top_extractor_BNNBDT_EqTwoTag_EqTwoJet
script_path3=/work/budvar-clued0/francji/scripts/top_extractor_BNNBDT_EqOneTag_EqThreeJet
script_path4=/work/budvar-clued0/francji/scripts/top_extractor_BNNBDT_EqTwoTag_EqThreeJet

for samples in small_training_sample testing_sample yield_sample; do
  echo $samples;
  for channels in EqOneTag_EqTwoJet EqTwoTag_EqTwoJet EqOneTag_EqThreeJet EqTwoTag_EqThreeJet; do
    echo $channels;
    for signals in QCD tb tqb wlp wbb wcc ttbar-lepjets ttbar-dilepton zlp zbb zcc diboson DATA; do
    #for i in ${input_root_files_path}/${samples}/*${channels}*.root; do  #${input_root_files_path}/${samples}/p17_CC_{QCD,tb,tqb,wlp,wbb,wcc,ttbar-lepjets,ttbar-dilepton,zlp,zbb,zcc,diboson,DATA}_${channels}_zero_Topo_testing_sample.root; do
    i=${input_root_files_path}/${samples}/p17_CC_${signals}_${channels}_zero_Topo_${samples}.root
    echo $i;
	if  [ "${channels}" = "EqOneTag_EqTwoJet" ]; then
		${script_path1} \
		$i \
		${output_root_files_path}/${samples}/$(basename ${i/.root/})
	elif  [ "${channels}" = "EqTwoTag_EqTwoJet" ]; then
		${script_path2} \
		$i \
		${output_root_files_path}/${samples}/$(basename ${i/.root/})
	elif  [ "${channels}" = "EqOneTag_EqThreeJet" ]; then
		${script_path3} \
		$i \
		${output_root_files_path}/${samples}/$(basename ${i/.root/})
	elif  [ "${channels}" = "EqTwoTag_EqThreeJet" ]; then
		${script_path4} \
		$i \
		${output_root_files_path}/${samples}/$(basename ${i/.root/})
	fi
    done
  done
done

