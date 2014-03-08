#!/bin/sh

# Create txt file from root file which was generated from txt file (samples->samples_txt->samples_root_sets->samples_txt_sets)

input_root_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_root_sets
output_txt_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_txt_sets
script_path=/work/budvar-clued0/kuceraja/FNAL/scripts/top_extractor/top_extractor_tt_leptonjets


# for samples in split_trees_3samples_ele_1119 split_trees_3samples_muo_1119; do #test_tree train_tree split_trees
#   mkdir -p ${output_root_files_path}/${samples}
#   echo $samples;
#   for i in ${input_root_files_path}/${samples}/*.root; do
#     echo $i
#     ${script_path} \
#     $i \
#     ${output_root_files_path}/${samples}/$(basename ${i/.root/})
#   done
# done
# 
# exit 0
# /work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_root_sets/ele/2jet/train/

set=(train test yield data)

for lepton in ele muo; do
	echo $lepton
	inPath=${input_root_files_path}
    for jetBin in 2 3 4; do
		echo "----JetBin=$jetBin"
		for setI in 0 1 2 3; do
			echo "    ${set[$setI]}"
			for rootFile in ${inPath}/${set[$setI]}/${lepton}/${jetBin}jet/*.root; do
				outPath=${output_txt_files_path}/${set[$setI]}/${lepton}/${jetBin}jet/$(basename ${rootFile/.root/})
				mkdir -p $(dirname $outPath)
				if [ $setI -lt 3 -a "$(basename $rootFile)" != "data_miniTree.root" ]; then
					# for train, test, yield we parse everything but data
					echo "--------$(basename $rootFile)"
					$script_path  \
					$rootFile  \
					$outPath
				fi
				if [ $setI -eq 3 -a "$(basename $rootFile)" == "data_miniTree.root" ]; then
					# for data we just parse it by number of jetbins
					echo "--------$(basename $rootFile)"
					$script_path  \
					$rootFile  \
					$outPath
				fi
			done	
		done
	done
done

exit 0
