#! /bin/sh

# /work/budvar-clued0/kuceraja/FNAL/scripts/tmva_root/root_tmva
# 0
# 22
# /work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_txt/converted_for_nnsu/ele_4+jets/variables_all/DATA/
# d_ele_4+jets_variables_all-LEARN-nnsu-even
# d_ele_4+jets_variables_all-LEARN-nnsu-odd
# /work/budvar-clued0/fjfi-D0/tt_leptonjets/results_txt/tmva/
# /work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_txt/converted_for_nnsu/ele_4+jets/variables_all/DATA/d_ele_4+jets_variables_all-LEARN-GAIN-even
# /work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_txt/converted_for_nnsu/ele_4+jets/variables_all/DATA/d_ele_4+jets_variables_all-LEARN-GAIN-odd

#  // argv[0]       ... name of program
#  // argv[1]       ... mode - if 0 learning and testing phase of TMVA
#  //                       will be performed
#  // argv[2]       ... number of variables inside input file
#  // argv[3]       ... absolute path to the data file
#  // argv[4]       ... name of learn data file
#  // argv[5]       ... name of test data file
#  // argv[6]       ... path for output files
#  // argv[7..argc-1] ... absolute paths to weightfiles
# Run CreateMethodsTree for all samples and given methods
# CreateMethodsTree.C creates root file with one tree,
# where discriminant from all channels are presented and
# add lists with EventWeight and ...

EXE=/work/budvar-clued0/kuceraja/FNAL/scripts/tmva_root/root_tmva

# order of running: ele4, ele3, ele2, muo4, muo3, muo2
NUM_VARS=( 23 23 22 ) #(24 24 23) for electron. We will subtract one for muon in the cycle
NJETS=( 4+ 3 2 )
LEPTS=( muo ) # for  lept in ${lepts[@]}

BASE_PATH=/work/budvar-clued0/fjfi-D0/tt_leptonjets

for lept in ${LEPTS[@]}; do
    echo $lept;
    for i in 0 1 2 ; do
        njet=${NJETS[i]}
        echo $njet

        # working out the right amount of variables
        nvar=${NUM_VARS[i]};
        if [ $lept == muo ]
            then
            nvar=$(( nvar-1 ))
        fi

        outDir=${BASE_PATH}/results_root/split_trees_3samples_${lept}_1119/TMVA-raw/
        inDir=${BASE_PATH}/samples_txt/converted_for_nnsu/${lept}_${njet}jets/variables_all/DATA/
        fileBaseName=d_${lept}_${njet}jets_variables_all-LEARN

        # running root_tmva with specific parameters
        $EXE \
        0 \
        $nvar \
        $inDir \
        ${fileBaseName}-nnsu-even \
        ${fileBaseName}-nnsu-odd \
        $outDir \
        ${inDir}/${fileBaseName}-GAIN-even \
        ${inDir}/${fileBaseName}-GAIN-odd
    done
done
