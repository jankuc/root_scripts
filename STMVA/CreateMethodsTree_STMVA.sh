#!/bin/sh

# Run CreateMethodsTree_STMVA for all samples and given methods
# STMVACreateMethodsTree.C creates root file with one tree,
# where discriminant from ttA_172 channels are presented and 
# add lists with EventWeight and ...


input_rootfiles_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples
output_root_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root/STMVA

paramethers_path=/work/budvar-clued0/kuceraja/FNAL/scripts/STMVA
script_path=/work/budvar-clued0/kuceraja/FNAL/scripts/STMVA/CreateMethodsTree_STMVA

for methods in all; do
  echo $methods
  for leptons in ele muo ; do
    echo $leptons
    outputs=${output_root_files_path}/split_trees_3samples_${leptons}_1119/${methods}
    mkdir -p $outputs
    for tagbin in beforeTag; do
    ##
    # WARNING we have to loop over number of jets in the script_path!!!
    #
      for jetbin in 2jet 3jet 4jet; do
      param=${paramethers_path}/STMVA.params.out.tt_lepjets_${methods}_${tagbin}_${jetbin}
        for channel in diboson QCD tb tqb ttbar-dilepton ttbar-lepjets wbb wcc wlp zbb zcc zlp; do
          input=${input_root_files_path}/${leptons}/p17_CC_${channel}_${tagbin}_${jetbin}_zero_Topo_${leptons}.root
          output=${output_root_files_path}/${leptons}/${methods}/p17_CC_${channel}_${tagbin}_${jetbin}_zero_Topo_${leptons}.root
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
  for leptons in yield_sample; do
    echo $leptons
    for tagbin in EqOneTag EqTwoTag; do
      for jetbin in EqTwoJet EqThreeJet; do
        param=${paramethers_path}/STMVA.params.out_${methods}_${tagbin}_${jetbin}
        input=${input_root_files_path}/${leptons}/p17_CC_DATA_${tagbin}_${jetbin}_zero_Topo_${leptons}.root
        output=${output_root_files_path}/${leptons}/${methods}/p17_CC_DATA_${tagbin}_${jetbin}_zero_Topo_${leptons}.root
        echo $input
        $script_path \
        $param \
        $input\
        $output
      done
    done
  done
done



####


#!/bin/sh

# Match individual methods root files together 

input_rootfiles_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples
input_results_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root
output_results_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root/matched_methods



for leptons in ele muo; do # muo ele
	echo $leptons;
	for i in ${input_rootfiles_path}/split_trees_3samples_${leptons}_1119/*; do	
		mkdir -p ${output_results_path}/split_trees_3samples_${leptons}_1119
		hadd \
		${output_results_path}/split_trees_3samples_${leptons}_1119/$(basename ${i}) \
		${input_results_path}/{MBC_1_1_MD_noTrans,MBC_50_20_MD_noTrans}/split_trees_3samples_${leptons}_1119/$(basename ${i}) \
		${i}
  done
done
