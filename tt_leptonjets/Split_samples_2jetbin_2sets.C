// To Compile
// g++ -g -O2 -Wall `root-config --cflags --libs` Split_samples_2jetbin_2sets.C -o Split_samples_2jetbin_2sets
//
// To run use CreateMethodsTree.sh (sh CreateMethodsTree.sh)

#ifndef __CINT__
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "TFile.h"
#include "TTree.h"
#include <stdio.h>
#include <stdlib.h>
#endif
using namespace std;

/**
  for filling this binary, use Split_samples_2jetbin_2sets.sh
  source_main       *.root full path of one of channels (QCD, data, Wlp, ...)
  outputFileName    *.root full output path of one of channels (QCD, data, Wlp, ...)
  cutString         string which will be used for cutting the tree
*/
void  cutTree(char *source_main, char *outputFileName, char *cutString){

    TFile *f_source = new TFile(source_main);                 // default parameter read only
    TFile *f_output = new TFile(outputFileName, "RECREATE");  // add parameter to recreate tree

    TTree *t_source = (TTree *) f_source->Get("nn_tree");
    TTree *t_output = t_source->CopyTree(cutString);

    t_output->Write();
    f_output->Close();
    f_source->Close();
}


#ifndef __CINT__
int main(int argc, char** argv)
{
    if (argc < 3){
        std::cout << "Usage: " << argv[0] << "source_main destination_path cutString" << std::endl;
        exit(1);
    }

    cutTree(argv[1], argv[2], argv[3]);
    return 0;
}
#endif
