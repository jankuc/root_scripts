#ifndef __CINT__ 
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#endif 

void bla_buu() { 

   //TFile *f = new TFile("../SingleTop2011.v9.ext/LooseTight/subsets/p20_CC_DATA_EqOneTag_EqTwoJet_zero_Topo.root");
   TFile *f = new TFile("/data/SingleTop2011.v9.ext/LooseTight/subsets/p20_CC_DATA_EqOneTag_EqTwoJet_zero_Topo.root");

   TTree *t = (TTree *) f->Get("TopologicalVariables");

   Double_t Jet1Pt;
   t->SetBranchAddress("Jet1Pt",&Jet1Pt);
   Double_t Jet2Pt; 
   t->SetBranchAddress("Jet2Pt",&Jet2Pt);
   Double_t LeptonEta; 
   t->SetBranchAddress("LeptonEta",&LeptonEta);

   Int_t nevents = t->GetEntries();
   for (Int_t i = 0; i<  nevents; i++) {
     t->GetEvent(i);
     std::cout <<  "event "<<  i<<  ": Leading Jet pT = "<<  Jet1Pt << " --- " << Jet2Pt << " --- " << LeptonEta  << std::endl;
   }

   //   TTree *tree = (TTree *) f->Get("TopologicalVariables");
   TObjArray *arr = t->GetListOfBranches();
   for (int i = 0; i < arr->GetSize(); i++) {
      std::cout << arr->At(i)->GetTitle() << std::endl;
   }

}

#ifndef __CINT__
int main(int argc, char** argv) { 
   bla_buu();
   return 0;
}
#endif
