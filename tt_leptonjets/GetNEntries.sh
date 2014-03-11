#!/bin/bash

# assumed output path: {output_root_files_path}/ {ele/muo} / {2,3,4}jet / {train,test,yield,data} /{data, QCD, ...}_miniTree.root
# assumed input path: {input_root_files_path}/${lepton} / {data, QCD, ...}_miniTree.root
# for checking what output it generates, use echo instead of script path
 
root_files_path=/work/budvar-clued0/kuceraja/tt_leptonjets/results_root_matched_sets
script_path=/work/budvar-clued0/kuceraja/FNAL/scripts/tt_leptonjets/GetNEntries

set=(train test)
setCutString=("(val==1)" "(val==2)" "(val==0)&&(type<2)" "type==2")

# methods are derived from CreateMethodsTree.sh
for tree in MBC_50_20_MD_noTransformation nn_tree ; do #MBC_1_1_MD_noTransformation MBC_1_1_noTransformation 
		echo $tree
    for lepton in ele; do
        echo $lepton
        for jetBin in 2; do
            echo "----JetBin=$jetBin"
            for setI in 0 1; do
                echo "    ${set[$setI]}"
                inPath="${root_files_path}/${set[$setI]}/${lepton}/${jetBin}jet"
                for rootFile in ${inPath}/*.root; do
                    if [ $setI -lt 3 -a "$(basename $rootFile)" != "data_miniTree.root" ]; then
                        # for train, test, yield we parse everything but data
                        $script_path  \
                        $rootFile  \
                        $tree
                    fi
                    if [ $setI -eq 3 -a "$(basename $rootFile)" == "data_miniTree.root" ]; then
                        # for data we just parse it by number of jetbins
                        $script_path  \
                        $rootFile  \
                        $tree
                    fi
                done
            done
        done
    done
done