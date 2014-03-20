#!/bin/sh

# Run CreateMethodsTree_STMVA for all sample and given method
# STMVACreateMethodsTree.C creates root file with one tree,
# where discriminant from ttA_172 channels are presented and 
# add lists with EventWeight and ...

input_root_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_root_sets
output_root_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root_matched_sets
paramethers_path=/work/budvar-clued0/kuceraja/FNAL/scripts/STMVA/params
script_path=/work/budvar-clued0/kuceraja/FNAL/scripts/STMVA/CreateMethodsTree_STMVA

signal=ttA_172

for method in MBC_and_BDT; do
	echo $method
	for sample in train test yield data; do
		echo $sample
		for lepton in ele muo; do
			echo $lepton
			# WARNING we have to loop over number of jets in the script_path!!!
			for jetbin in 2jet 3jet 4jet; do
				outputs=${output_root_files_path}/$sample/$lepton/$jetbin
				mkdir -p $outputs
				param=${paramethers_path}/STMVA.params.out.${method}_${lepton}_${jetbin}_${signal}
				for channel in $outputs/* ; do
					input=${input_root_files_path}/$sample/${lepton}/$jetbin/$(basename ${channel})
					output=${outputs}/$(basename ${channel})
					echo $input
					echo $param
					$script_path \
					$param \
					$input\
					$output
				done			
			done
		done
	done
done
