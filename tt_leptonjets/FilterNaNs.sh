#!/bin/bash

# Run FilterNaNs for all leptons (muo, ele) in samples_root_orig
#
# CreateMethodsTree.C creates root files for all channels, in which there is
# discriminant of given method and appropriate weight.

input_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_root_orig
output_path=/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_root
script_path=/work/budvar-clued0/kuceraja/FNAL/scripts/tt_leptonjets/CutTree

for lepton in muo ele; do
    echo $lepton
		for i in ${input_path}/split_trees_3samples_${lepton}_1119/* ; do
				echo $(basename $i)
				mkdir -p "${output_path}/${lepton}/"
				$script_path \
				$i \
				${output_path}/${lepton}/$(basename ${i}) \
				"((Wmt>0)||(Wmt<=0))&&((MEvent>0)||(MEvent<=0))&&((MT1NL>0)||(MT1NL<=0))&&((M0nl>0)||(M0nl<=0))&&((M1nl>0)||(M1nl<=0))&&((MT0nl>0)||(MT0nl<=0))" \
				nn_tree
		done
done
