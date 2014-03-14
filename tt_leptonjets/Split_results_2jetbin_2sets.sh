#!/bin/bash

# assumed output path: {output_root_files_path}/ {ele/muo} / {2,3,4}jet / {train,test,yield,data} /{data, QCD, ...}_miniTree.root
# assumed input path: {input_root_files_path}/${lepton} / {data, QCD, ...}_miniTree.root
# for checking what output it generates, use echo instead of script path
 
input_root_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root
output_root_files_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root_sets
script_path=/work/budvar-clued0/kuceraja/FNAL/scripts/tt_leptonjets/CutTree

set=(train test yield data)
setCutString=("(val==1)" "(val==2)" "(val==0)&&(type<2)" "type==2")

# methods are derived from CreateMethodsTree.sh
for method_path in ${input_root_files_path}/* ; do
		method=$(basename $method_path)
		echo $method
    for lepton in ele muo; do
        echo $lepton
        for jetBin in 2 3 4; do
            echo "----JetBin=$jetBin"
            for setI in 0 1 2 3; do
                echo "    ${set[$setI]}"
                inPath=${input_root_files_path}/$method/${lepton}
                for rootFile in ${inPath}/*.root; do
                    outPath=${output_root_files_path}/$method/${set[$setI]}/${lepton}/${jetBin}jet/$(basename $rootFile)
                    mkdir -p $(dirname $outPath)
                    if [ $setI -lt 3 -a "$(basename $rootFile)" != "data_miniTree.root" ]; then
                        # for train, test, yield we parse everything but data
                        echo "--------$(basename $rootFile)"
                        $script_path  \
                        $rootFile  \
                        $outPath  \
                        "(NJets=="${jetBin}")&&"${setCutString[$setI]} \
                        $method
                    fi
                    if [ $setI -eq 3 -a "$(basename $rootFile)" == "data_miniTree.root" ]; then
                        # for data we just parse it by number of jetbins
                        echo "--------$(basename $rootFile)"
                        $script_path  \
                        $rootFile  \
                        $outPath  \
                        "(NJets=="${jetBin}")" \
                        $method
                    fi
                done	
            done
        done
    done
done
