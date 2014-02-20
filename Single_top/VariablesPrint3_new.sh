#!/bin/sh

# print variables from root file

input_root_files_path=/work/budvar-clued0/francji/tt_leptonjets/samples/split_trees_3samples_ele_1119/
script_path=/work/budvar-clued0/francji/scripts/VariablesPrint3

    i=${input_root_files_path}/ttAll_172_miniTree.root
    #echo $i;
    ${script_path} \
    $i


# /work/budvar-clued0/francji/tt_new/test/test_tree/QCD_miniTree.root    nn_tree
# /work/budvar-clued0/francji/tt_leptonjets/samples/split_trees_3samples_ele_1119/ttAll_172_miniTree.root