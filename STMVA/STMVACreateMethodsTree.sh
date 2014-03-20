#!/bin/sh

# Run STMVACreateMethodsTree for all samples and given methods
# STMVACreateMethodsTree.C creates root file with one tree,
# where discriminant from s,t, and s+t channels are presented and 
# add lists with EventWeight and ...

input_root_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_root_sets
output_root_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root_matched_sets
paramethers_path=/work/budvar-clued0/kuceraja/FNAL/scripts/STMVA/params
script_path=/work/budvar-clued0/kuceraja/FNAL/scripts/STMVA/STMVACreateMethodsTree

for method in MBC_and_BDT; do
	echo "$method"
	for sample in train test yield data; do
		echo "--$sample"
		for lepton in ele muo; do
			echo "---- $lepton"
			# WARNING we have to loop over number of jets in the script_path!!!
			for jetbin in 2jet 3jet 4jet; do
				outputs=${output_root_files_path}/$sample/$lepton/$jetbin
				mkdir -p $outputs
				param=${paramethers_path}/STMVA.params.out.${method}_${lepton}_${jetbin}_ # in the STMVACreateMethodsTree.C we add string defining signal type 
				for channel in $outputs/* ; do
					input=${input_root_files_path}/$sample/${lepton}/$jetbin/$(basename ${channel})
					output=${outputs}/$(basename ${channel})
					echo "------ Input: $input"
					echo "------ Param: $param"
					$script_path \
					$param \
					$input\
					$output
				done			
			done
		done
	done
done
