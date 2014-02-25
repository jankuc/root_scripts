// To Compile
// g++ -g -O2 -Wall `root-config --cflags --libs` MergeMethodsTree.C -o  MergeMethodsTree
//
// To run use MergeMethodsTree.sh (sh CreateMethodsTree.sh)


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
#include <sys/stat.h>

#endif 

using namespace std;


void  CreateMethodsTree(const char *inputFileName, std::string method , const char *outputFileName){

  TString *TREE_NAME = "nn_tree";

  TFile *f_input  = new TFile(inputFileName, "read");
  TTree *t_input  = (TTree*)f_input->Get(method.c_str());

  Double_t Weight; 	t_input->SetBranchAddress("Weight", &Weight);
  Double_t Ht;
  t_input->SetBranchAddress("Ht", &Ht);
  
  std::ifstream check_output(outputFileName);

  TFile *f_output = new TFile(outputFileName, "update");   // add parameter to update tree

  //if (!check_output || !check_output.is_open())   
  if (!check_output.good())
  {
    TTree *t_output = new TTree(TREE_NAME, TREE_NAME);
    // create output branches
    t_output->Branch("Weight", &Weight, "Weight/D");
    t_output->Branch("Ht", &Ht, "Ht/D");
  }

  Int_t n_channels = 1;
  Double_t leafLinks[n_channels]; // maybe it has to be 3?

  std::string leafName_input[n_channels];
  std::string leafName_output[n_channels];
  std::string leafName_types[n_channels];
  std::string channelName[] = {"ttA_172"};

  for (Int_t i = 0; i<n_channels; i++){
    leafName_input[i]  = channelName[i];
    leafName_output[i] = method+"_"+channelName[i];
    leafName_types[i]  = method+"_"+channelName[i]+"/D";
  }

  TTree *t_output  = (TTree*)f_output->Get(TREE_NAME);
  for (Int_t i = 0; i<n_channels; i++){
    t_input->SetBranchAddress(leafName_input[i].c_str(), &leafLinks[i]);
    t_output->Branch(leafName_output[i].c_str(), &leafLinks[i], leafName_types[i].c_str());
  }
  // TBranch *Branch_tb    = t_output->Branch(leafName_output[1].c_str(), &leafLinks[1], leafName_types[1].c_str());
  // TBranch *Branch_tqb   = t_output->Branch(leafName_output[2].c_str(), &leafLinks[2], leafName_types[2].c_str());
  // TBranch *Branch_tbtqb = t_output->Branch(leafName_output[3].c_str(), &leafLinks[3], leafName_types[3].c_str());

   Int_t    n_events_input   =  t_input->GetEntries();
   Int_t    n_events_output1 =  t_output->GetEntries();

   for (Int_t j = 0; j<n_events_input ; j++){
      t_input     ->GetEvent(j);
      t_output    ->Fill();
   }

// for checking what's doing :(
    Int_t    n_events_output2 =  t_output->GetEntries();
    std::cout << "Check "<< method.c_str()<<"  "<< n_events_input <<" - " << n_events_output1 << " - " << n_events_output2<<std::endl;


  double hist_input;
  for (Int_t i = 0; i<n_channels; i++){
    char parameter[100], title[100];
    sprintf(parameter, "%s", leafName_output[i].c_str()); 
    sprintf(title, "Weighted %s, separation method: %s", channelName[i].c_str(), method.c_str());
    TH1F *hist = new TH1F(parameter, title, 25, 0 ,1);

    for (Int_t j = 0; j<n_events_input ; j++){
      t_output  -> GetEvent(j);
      hist_input = leafLinks[i];
      hist     -> Fill(hist_input,Weight);
    }
  hist->Write();
  }

  t_output->Write("", TObject::kOverwrite);
  f_output->Close();
  f_input->Close();
}



#ifndef __CINT__
int main(int argc, char** argv)
{
  if (argc < 4){
    std::cout << "Usage: " << argv[0] << "input_path   methods output_path" << std::endl;
    exit(1);
  }
  CreateMethodsTree(argv[1], argv[2], argv[3]);
  return 0;
}
#endif
