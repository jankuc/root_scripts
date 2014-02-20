#!/bin/sh

# run all top_extractors


echo Extracted_BNN_variables
mkdir -p /work/budvar-clued0/francji/subsets_txt_BNN_variables
sh top_extractor_BNN_variables.sh

echo Extracted_BDT_variables
mkdir -p /work/budvar-clued0/francji/subsets_txt_BDT_variables
sh top_extractor_BDT_variables.sh

echo Extracted_BNNBDT_variables
mkdir -p /work/budvar-clued0/francji/subsets_txt_BNNBDT_variables
sh top_extractor_BNNBDT_variables.sh

echo Extracted_select_variables
mkdir -p /work/budvar-clued0/francji/subsets_txt_selected_variables
sh top_extractor_select_variables.sh

echo Extracted_all_variables
mkdir -p /work/budvar-clued0/francji/subsets_txt_all_variables
sh top_extractor_all_variables.sh
