#!/bin/bash

# assumed output path: {output_root_files_path}/ {ele/muo} / {2,3,4}jet / {train,test,yield,data} /{data, QCD, ...}_miniTree.root
# assumed input path: {input_root_files_path}/split_trees_3samples_${lepton}_1119 / {data, QCD, ...}_miniTree.root
# for checking what output it generates, use echo instead of script path
 
input_root_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_root
output_root_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_root_sets
script_path=/work/budvar-clued0/kuceraja/FNAL/scripts/tt_leptonjets/CutTree

set=(train test yield data)
setCutString=("(val==1)" "(val==2)" "(val==0)&&(type<2)" "type==2")

for lepton in ele muo; do
	echo $lepton
	inPath=${input_root_files_path}/split_trees_3samples_${lepton}_1119
    for jetBin in 2 3 4; do
		echo "----JetBin=$jetBin"
		for setI in 0 1 2 3; do
			echo "    ${set[$setI]}"
			for rootFile in ${inPath}/*.root; do
				outPath=${output_root_files_path}/${set[$setI]}/${lepton}/${jetBin}jet/$(basename $rootFile)
				mkdir -p $(dirname $outPath)
				if [ $setI -lt 3 -a "$(basename $rootFile)" != "data_miniTree.root" ]; then
					# for train, test, yield we parse everything but data
					echo "--------$(basename $rootFile)"
					$script_path  \
					$rootFile  \
					$outPath  \
					"(NJets=="${jetBin}")&&"${setCutString[$setI]} \
					nn_tree
				fi
				if [ $setI -eq 3 -a "$(basename $rootFile)" == "data_miniTree.root" ]; then
					# for data we just parse it by number of jetbins
					echo "--------$(basename $rootFile)"
					$script_path  \
					$rootFile  \
					$outPath  \
					"(NJets=="${jetBin}")" \
					nn_tree
				fi
			done	
		done
	done
done
