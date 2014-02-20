// The following is a skeleton for a macro that can be executed in
// CINT and compiled.

// To use this script, first you have to setup ROOT in your environment
//
// On FNAL clued0 machine you can just setup some ROOT version, e.g.
// setup root v5_32_02
//
// or on VKSTAT you can use
// source /opt/root.setup
//
//  This file can used with interactive ROOT as:
//
//    Root> .x foo.C
//
//  or interpreted as ROOT macro directly from command line:
//
//    root foo.C
//
//  or you can compile it to a program with
//
//    g++ -g -O2 -Wall `root-config --cflags --libs` foo.C -o foo
//
//  and execute it as
//
//    ./foo
//
// ----------------------------------------------------------------------

#ifndef __CINT__ 
// Include all needed headers here 
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#endif 

void foo() { 
  // Put your function body here. 
  // TFile *f = new TFile("/prj_root/7007/top_write/jyoti/SingleTop2011.v9.ext/LooseTight/subsets/p20_CC_DATA_EqOneTag_EqTwoJet_zero_Topo.root");
  TFile *f = new TFile("/data/SingleTop2011.v9.ext/LooseTight/subsets/p20_CC_DATA_EqOneTag_EqTwoJet_zero_Topo.root");
  TTree *t = (TTree *) f->Get("TopologicalVariables");
  Double_t Jet1Pt;
  t->SetBranchAddress("Jet1Pt",&Jet1Pt);
  Int_t nevents = t->GetEntries();
  for (Int_t i = 0; i<  nevents; i++){
    t->GetEvent(i);
    std::cout <<  "event "<<  i<<  ": Leading Jet pT = "<<  Jet1Pt << std::endl;
   }
}

#ifndef __CINT__
int main(int argc, char** argv)
{ 
  // Call you script function
  foo();

  return 0;
}
#endif
