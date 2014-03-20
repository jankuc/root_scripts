// ----------------------------------------------------------------------
//
// * setup root environment (`source STMVA.setup`)
// * build using Makefile (`make` command)
// * execute ./STMVAEvaluate params input.root
//  ./STMVACreateMethodsTree STMVA.params input.root output.root
// ----------------------------------------------------------------------

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include <stdexcept>
#include <iostream>
//#include <fstream>
//#include <sstream>
#include <numeric>
#include <limits>
//#include "TEnv.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include "TMVA/Types.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"
#endif

#include "STMVAConfig.h"

struct type_and_val {
    char type; // is 'I', 'D' or 'F'

    union {
        Int_t I;
        Double_t D;
        Float_t F;
    } val;
};

std::vector<double> STMVAEvaluate(const char *params,const char* input) {
  STMVAConfig config(params);
  //config.dump();

  // join signal names without delimiter
  std::string sgn = std::accumulate(config.signals().begin(), config.signals().end(), std::string(""));

  // create TMVA::Reader object
  TMVA::Reader *reader = new TMVA::Reader();

  // root tree
  TFile* f = new TFile(input);
  TTree* treeWithNans = (TTree*) f->Get(config.treeName().c_str());
  char * cutString = "Wmt>0";
  
  if (strcmp(config.lepton().c_str(), "ele")) {
      cutString = "Wmt>0";
  } else {
      cutString = "(Wmt>0)&&(Wmt>0)"; //...
  }
  TTree* t = treeWithNans->CopyTree(cutString);
  t->Write();
  
  

  std::vector<Float_t>  varsf(config.variables().size(), 0.);
  std::vector<Double_t>  spectatorsd(config.spectators().size(), 0.);
  std::vector<Float_t>  spectatorsf(config.spectators().size(), 0.);

  // create a set of variables and declare them to the reader
  //for (std::vector<std::string>::const_iterator it = config.variables().begin();
  //     it != config.variables().end(); ++it) {
    //std::string varName(*it);
    for (size_t i = 0; i < config.variables().size(); i++) {
        std::string varName(config.variables()[i]);
        if (!(varName.size() > 2 && varName[varName.size() - 2] == '/' && varName[varName.size() - 1] == 'F'))
            throw std::runtime_error("currently this support only floats variables, can't use " + varName);
        //t->SetBranchAddress(varName.substr(0, varName.size()-2).c_str(), &varsd[i]);
        t->SetBranchAddress(varName.substr(0, varName.size() - 2).c_str(), &varsf[i]);
        reader->AddVariable(varName.substr(0, varName.size() - 2).c_str(), &varsf[i]);
    }
  
  for (size_t i = 0; i < config.spectators().size(); i++) {
        std::string spectatorName(config.spectators()[i]);
        if (!(spectatorName.size() > 2 && spectatorName[spectatorName.size() - 2] == '/' && spectatorName[spectatorName.size() - 1] == 'D')) {
            t->SetBranchAddress(spectatorName.substr(0, spectatorName.size() - 2).c_str(), &spectatorsd[i]);
            spectatorsf[i] = (Float_t) spectatorsd[i];
        } else {
            t->SetBranchAddress(spectatorName.substr(0, spectatorName.size() - 2).c_str(), &spectatorsf[i]);
        }
        if (std::strstr(spectatorName.c_str(), "/") == nullptr) {
            reader->AddSpectator(spectatorName.c_str(), &spectatorsf[i]);
        } else {
            reader->AddSpectator(spectatorName.substr(0, spectatorName.size() - 2).c_str(), &spectatorsf[i]);
        }
    }

  std::string method(config.methods()[0]);
  reader->BookMVA(method, std::string(config.outputPath()+"/"+config.lepton()+"_"+config.jetBin()+"_"+sgn+"_"+method+".weights.xml").c_str());

  std::vector<double> ret;
  for (Long64_t ievt=0; ievt < t->GetEntries(); ievt++) {
    t->GetEntry(ievt);
    //std::copy(varsd.begin(), varsd.end(), varsf.begin());
    //std::cout << "EvaluateMVA\t" << reader->EvaluateMVA(method) << std::endl;
    ret.push_back(reader->EvaluateMVA(method));
  }
  f->Close();

  delete reader;
  return ret;
}

void  STMVACreateMethodsTree(const char *params, const char *inputFileName, std::vector<double> &ttA_172_vector, const char *outputFileName){

   STMVAConfig config(params);
   config.dump();
   const char *treeName = config.methods()[0].c_str();

   TFile *f_input  = new TFile(inputFileName);                 // default parameter read only
   TFile *f_output = new TFile(outputFileName, "RECREATE");  // add parameter to recreate tree

   TTree *t_input  = (TTree *) f_input->Get("nn_tree");
   TTree *t_output = new TTree(treeName, treeName);

  //declare variables and get pointers to required original variables from source tree
  Float_t EventWeight;         t_input->SetBranchAddress("Weight", &EventWeight);
  Float_t Met; 		t_input->SetBranchAddress("Met", &Met);

  // Check number of events is equal to vector size
  Int_t n_events_original =  t_input->GetEntries();
  if (n_events_original != ttA_172_vector.size() ){ //|| n_events_original != tqb_vector.size() || n_events_original != tbtqb_vector.size() ){
      std::cout << "no. events:"        << n_events_original << "in" << inputFileName << std::endl;
      std::cout << "ttA_172_vector size"    << ttA_172_vector.size()      << std::endl;
      //std::cout << "tqb_vector size"   << tqb_vector.size()     << std::endl;
      //std::cout << "tbtqb_vector size" << tbtqb_vector.size()   << std::endl;
      throw std::runtime_error("No. of events in the discriminant files differs from number of EventWeights in the input file");
    }

  // declare required variables and create output branches
  t_output->Branch("EventWeight",         &EventWeight,         "Weight/F");
  t_output->Branch("Met", &Met, "Met/F");
  // Double_t tb,tqb,tbtqb;  
  // t_output->Branch("tb", &tb, "tb/D");
  // t_output->Branch("tgb", &tqb, "tqb/D");
  // t_output->Branch("tbtqb", &tbtqb, "tbtqb/D");
  Float_t  channel_links[1];
  char     *channel_names[]   =  {"ttA_172", NULL};
  char     *channel_types[]   =  {"ttA_172/F",};
  for (Int_t i = 0; channel_names[i]; i++){
    t_output->Branch(channel_names[i],   &channel_links[i],   channel_types[i]);
  }

  //double maxelement[] = {-1*(std::numeric_limits<double>::infinity()),-1*(std::numeric_limits<double>::infinity()),-1*(std::numeric_limits<double>::infinity())};
  //double minelement[] = {(std::numeric_limits<double>::infinity()),(std::numeric_limits<double>::infinity()),(std::numeric_limits<double>::infinity())};
    Float_t maxelement[] = {1};
    Float_t minelement[] = {0};

  // main loop: record required values into given branches
  for (size_t i = 0; i < ttA_172_vector.size(); i++) {
    channel_links[0] = ttA_172_vector[i];
//    channel_links[1] = tqb_vector[i];
//    channel_links[2] = tbtqb_vector[i];
    //std::cout  <<  tb_vector[i] << tqb_vector[i] << tbtqb_vector[i] << std::endl;
    t_input  -> GetEvent(i);
    t_output -> Fill();
    if (minelement[0] > ttA_172_vector[i])    minelement[0] = ttA_172_vector[i];
//    if (minelement[1] > tqb_vector[i])   minelement[1] = tqb_vector[i];
//    if (minelement[2] > tbtqb_vector[i]) minelement[2] = tbtqb_vector[i];
    if (maxelement[0] < ttA_172_vector[i])    minelement[0] = ttA_172_vector[i];
//    if (maxelement[1] < tqb_vector[i])   minelement[1] = tqb_vector[i];
//    if (maxelement[2] < tbtqb_vector[i]) minelement[2] = tbtqb_vector[i];
  }
   // double minelement = *std::min_element(tb_vector.begin(),tb_vector.end());



  double hist_input;
  for (Int_t i = 0; channel_names[i]; i++){
     // one way ReadFile(const source_channel[i], const char* branchDescriptor = "x/D")
     // second  way TTree t;  t.ReadFile(source_channel[i], "x")
    char parameter[100], title[100];
    sprintf(parameter, "%s_%s", treeName, channel_names[i]); 
    sprintf(title, "Weighted %s, separation method: %s", channel_names[i], treeName);
    TH1F *hist = new TH1F(parameter, title, 25, 0 ,1);
   // Double_t  Sum_EventWeight = 0;
    for (Int_t j = 0; j<n_events_original ; j++){
      t_output -> GetEvent(j);
      hist_input = (channel_links[i]-minelement[i])/maxelement[i];
      hist     -> Fill(hist_input,EventWeight);
   //  Sum_EventWeight += EventWeight;
   //  hist->Draw();
    }
  hist->Write();
  // Double_t integral = hist->Integral();
  // printf("Integral of weighted hist. for %s:  %f,   sum of related EventWeights  %f\n", channel_names[i], integral, Sum_EventWeight);
  }

  t_output->Write();
  f_output->Close();
  f_input->Close();
}


#ifndef __CINT__
int main(int argc, char** argv)
{
  if (argc < 4) {
    std::cout << "Evaluate data in \"input.root\" file by first method configured in \"STMVA.params\" files for ttA_172 channel and save results in to \"output.root\" file" << std::endl;
    std::cout << "Usage: " << argv[0] << " STMVA.params input.root output.root" << std::endl;
   }

  std::string params_path(argv[1]);

  std::string params_ttA_172    = params_path+"ttA_172";

  std::cout << "-------- Evaluate for: "<< params_ttA_172    << std::endl;
  std::vector<double> ttA_172_vector = STMVAEvaluate(params_ttA_172.c_str(),argv[2]);
  //std::copy(tb_vector.begin(),tb_vector.end(),std::ostream_iterator<double>(std::cout," "));
  //std::cout << std::endl;

  STMVACreateMethodsTree(params_ttA_172.c_str(), argv[2], ttA_172_vector, argv[3]);

  return 0;
}
#endif
