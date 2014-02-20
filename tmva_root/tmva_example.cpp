

#include "TROOT.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TSelector.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TEventList.h"
#include "TEntryList.h"
#include "TString.h"
#include "TBranch.h"
#include "TSystem.h"
#include "TFunction.h"
#include "TVector3.h"
#include "TMath.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TProfile.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "../tmva/inc/TMVA/Factory.h"

#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <math.h>
#include <string.h>
#include <math.h>
#include "TMath.h"
#include <iomanip>

// run with: root -l TMVAExample.C

/**
 * @brief Converts int to string
 * @param number
 * @return string instead of number
 */
std::string intToStr(int number) {
  std::stringstream ss;//create a stringstream
  ss << number;//add number to the stream
  return ss.str();//return a string with the contents of the stream
}

void tmva_example(TString inPath, int jet)
{

  TFile* outFile = TFile::Open( "TMVA.root", "RECREATE" );

  TMVA::Factory *factory = new TMVA::Factory( "MVAnalysis", outFile,"!V");

  const TString VARS[] =  {"Apla", "Spher", "HTL", "JetMt", "HT3", "MEvent", "MT1NL", "M01mall", "M0nl", "M1nl", "Mt0nl", "Met", "Mtt", "Mva_max", "Wmt", "Wpt", "Centr", "DRminejet", "DiJetDrmin", "Ht", "Ht20", "Ktminp", "Lepdphimet", "Lepemv"};
  const int nVARS = 24; // last variable from the tree is always -inf => we don't use it.
  const TString LABELS[] = {"NJets", "type", "Weight", "train","test"};
  const int nLABELS = 5;
  const TString CHANNELS[] = {"data", "ttAll_172", "WZ", "ZlpEE", "QCD", "Wbb", "ZbbEE", "ZlpTauTau", "tb", "Wcc", "ZbbTauTau", "ZZ", "tqb", "Wlp", "ZccEE", "ttA_172", "WW", "ZccTauTau"};
  const int nCHANNELS = 18;
  const TString TREE_NAME = "nn_tree";
  const TString MINITREE_STR = "_miniTree.root";

  TChain *jetChain = new TChain(); // Chain with files corresponding to selected number of jets.
  for (int i = 0; i < nCHANNELS; ++i) { // loop is from 1, because I don't want data in the chain
      TFile f(inPath + CHANNELS[i] + MINITREE_STR);
      TTree *chTree = (TTree*) f.Get(TREE_NAME);
      chTree->SetBranchStatus("*", 1);
      TTree *jetTree = chTree->CopyTree( TString("NJets==") + intToStr(jet) );
      jetChain->Add(jetTree);
    }

  jetChain->SetBranchStatus("*",1);

  //TChain *trainChain = jetChain->CopyTree("(val==1)&&(type<2)");
  //TChain *testChain = jetChain->CopyTree("(val==2)&&(type<2)");
  //TChain *yieldChain = jetChain->CopyTree("(val==0)&&(type<2)");
  //TChain *DataChain = jetChain->CopyTree("(type==2)");

  TChain *sigTrainChain;
  sigTrainChain->Add((jetChain->GetTree()->CopyTree("(val==1)&&(type==1)")));
  TChain *bkgTrainChain;
  bkgTrainChain->Add(jetChain->GetTree()->CopyTree("(val==1)&&(type==0)"));
  TChain *sigTestChain;
  sigTestChain->Add(jetChain->GetTree()->CopyTree("(val==2)&&(type==1)"));
  TChain *bkgTestChain;
  bkgTestChain->Add(jetChain->GetTree()->CopyTree("(val==2)&&(type==0)"));

  sigTrainChain->SetBranchStatus("*",1);
  bkgTrainChain->SetBranchStatus("*",1);
  sigTestChain->SetBranchStatus("*",1);
  bkgTestChain->SetBranchStatus("*",1);

  for (int i = 0; i < nVARS; i++) {
      factory->AddVariable(VARS[i], );
    }

  factory->AddSignalTree(sigTrainChain, 1.0, "Training");
  factory->AddSignalTree(sigTestChain, 1.0, "Test");

  factory->AddBackgroundTree( bkgTrainChain, 1.0, "Training");
  factory->AddBackgroundTree( bkgTestChain, 1.0, "Test");

  factory->SetSignalWeightExpression("Weight");
  factory->SetBackgroundWeightExpression("Weight");

  //factory->PrepareTrainingAndTestTree( "", "","nTrain_Signal=200:nTrain_Background=200:nTest_Signal=200:nTest_Background=200:!V" );

  factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood","!V:NAvEvtPerBin=50" );

  factory->BookMethod( TMVA::Types::kMLP, "MLP", "!V:NCycles=50:HiddenLayers=10,10:TestRate=5" );

  factory->BookMethod( TMVA::Types::kBDT, "BDT", "!V:BoostType=Grad:nCuts=20:NNodesMax=5" );

  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();

  outFile->Close();
  std::cout << "Caution, this was example code, please edit for real use!"<<std::endl;
  delete factory;
}
