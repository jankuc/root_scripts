#!/bin/sh

# Match individual methods root files together 

input_rootfiles_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_root_sets
input_results_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root_sets
output_results_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root_matched_sets

for set in train test yield data; do
	echo $set
	for lepton in ele muo; do # muo ele
		echo $lepton;
		for jetBin in 2 3 4; do 
			outpath=${output_results_path}/${set}/${lepton}/${jetBin}jet
			mkdir -p $outpath
			for i in ${input_rootfiles_path}/${set}/${lepton}/${jetBin}jet/*; do
				echo "$i"
				# only for checking if current root file exists (in current set dont have to exist all of them)
				methods=`ls ${input_results_path}`
				method=`echo $methods|sed -e 's/.* //g'`
				# looks for current root file in one of the methods and if its present continues
				if [ -e ${input_results_path}/${method}/${set}/${lepton}/${jetBin}jet/$(basename $i) ] ; then
					hadd \
					$outpath/$(basename ${i}) \
					${input_results_path}/*/${set}/${lepton}/${jetBin}jet/$(basename $i) \
					$i
				fi
			done
		done	
	done
done
