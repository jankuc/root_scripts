// To Compile
// g++ -g -O2 -Wall `root-config --cflags --libs` CreateMethodsTree.C -o  CreateMethodsTree
//
// To run use CreateMethodsTree.sh (sh CreateMethodsTree.sh)


#ifndef __CINT__
// Include all needed headers here 
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "TFile.h"
#include "TTree.h"
#include <TH1.h>
#include "TCanvas.h"
//#include <TRandom.h>
#include <stdio.h>
#include <stdlib.h>

#endif 

using namespace std;


void  CreateMethodsTree(char *inputFileName  = "colTree.root"){

  char *outputFileName = inputFileName;

  TFile *f_input  = new TFile(inputFileName);                 // default parameter read only
  TFile *f_output = new TFile(outputFileName, "RECREATE");   // add parameter to recreate tree

  TTree *t_input  = (TTree *) f_input->Get("TopologicalVariables");
  TTree *t_output = new TTree(treeName, treeName);

  printf("Integral of weighted hist. for %s:  %f,   sum of related EventWeights  %f\n", channel_names[i], integral, Sum_EventWeight);

  t_output->Write();
  f_output->Close();
  f_input->Close();
}

#ifndef __CINT__
int main(int argc, char** argv)
{
  if (argc < 2){
    std::cout << "Usage: " << argv[0] << "name.root" << std::endl;
    exit(1);
  }

   ReadSave(argv[1]);

  return 0;
}
#endif
