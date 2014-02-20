// The following is a skeleton for a macro that can be executed in
// CINT and compiled.

// To use this script, first you have to setup ROOT in your environment
//
// On FNAL clued0 machine you can just setup some ROOT version, e.g.
//
//    setup root v5_32_02
//
// or on VKSTAT you can use
//
//    source /opt/root.setup

//  This file can used with interactive ROOT as:
//
//    Root> .x colTree.C
//
//  or interpreted as ROOT macro directly from command line:
//
//    root colTree.C
//
//  or you can compile it to a program with
//
//    g++ -g -O2 -Wall `root-config --cflags --libs` colTree.C -o colTree
//
//  and execute it as
//
//    ./colTree
//
// for i in *.txt; do  echo $i; ~/scripts/colTree $i /work/budvar-clued0/francji/results/GLM/s_channel/${i/.txt/.root} GLM; done
//
// ----------------------------------------------------------------------

#ifndef __CINT__ 
// Include all needed headers here 
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "TFile.h"
#include "TTree.h"
#endif 

void colTree(char *columnFileName = "colTree.in", char *outputFileName = "colTree.root", char *treeName = "col") {
  Float_t disc,max_disc = 0;
  TFile *f = new TFile(outputFileName, "RECREATE");
  TTree *t = new TTree(treeName, treeName);
  t->Branch("disc", &disc, "disc/F");
  

  std::ifstream indata(columnFileName);
  if (!indata || !indata.is_open())
    throw std::runtime_error("unable to open input file: "+std::string(columnFileName));
  while (true){
    indata >> disc;
    if (!indata) break;    
    if (max_disc < disc){
        max_disc = disc;
     }
  }
  indata.clear();
  indata.seekg(0, indata.beg);
  while (true) {
    indata >> disc;
    if (!indata) break;
    disc = disc/max_disc;
    t->Fill();
  }
  t->Write();
  f->Close();
}

#ifndef __CINT__
int main(int argc, char** argv)
{
  if (argc < 4){
    std::cout << "Usage: " << argv[0] << " columnFileName outputFileName treeName" << std::endl;
    exit(1);
  }

  // Call you script function
  colTree(argv[1], argv[2], argv[3]);   // [1] - input (e.g. txt file)
					// [2] - output root file name
     					// [3] - tree name in the root file

  return 0;
}
#endif
