// To Compile
// g++ -g -O2 -Wall `root-config --cflags --libs` Split_samples_2jetbin_2sets.C -o Split_samples_2jetbin_2sets
//
//

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
  Creates cut of tree 'nn_tree' from the input file and saves this new cut to the output file.
  Provide full paths!

  input_root_file     *.root full path of one of channels (QCD, data, Wlp, ...)
  output_root_file    *.root full output path of one of channels (QCD, data, Wlp, ...)
  cutString           string which will be used for cutting the tree eg. "val==1"
*/
void  cutTree(char *input_root_file, char *output_root_file, char *cutString){

  TFile *f_source = new TFile(input_root_file, "READ");
  TFile *f_output = new TFile(output_root_file, "RECREATE");

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
      cout << "Usage: " << argv[0] << "input_root_file output_root_file cutString" << endl;
      exit(1);
    }

  cutTree(argv[1], argv[2], argv[3]);
  return 0;
}
#endif
