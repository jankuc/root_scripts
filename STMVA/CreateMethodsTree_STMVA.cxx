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

std::vector<double> STMVAEvaluate(const char *params,const char* input) {
  STMVAConfig config(params);
  config.dump();

  // join signal names without delimiter
  std::string sgn = std::accumulate(config.signals().begin(), config.signals().end(), std::string(""));

  // create TMVA::Reader object
  TMVA::Reader *reader = new TMVA::Reader();

  // root tree
  TFile *f = new TFile(input);
  TTree *t = (TTree*) f->Get(config.treeName().c_str());

  std::vector<float>  varsf(config.variables().size(), 0.);
  std::vector<double> varsd(config.variables().size(), 0.);

  // create a set of variables and declare them to the reader
  //for (std::vector<std::string>::const_iterator it = config.variables().begin();
  //     it != config.variables().end(); ++it) {
  //std::string varName(*it);
  for (size_t i = 0; i < config.variables().size(); i++) {
    std::string varName(config.variables()[i]);
    if (!(varName.size() > 2 && varName[varName.size()-2] == '/' && varName[varName.size()-1] == 'D'))
      throw std::runtime_error("currently this support only double variables, can't use "+varName);
    t->SetBranchAddress(varName.substr(0, varName.size()-2).c_str(), &varsd[i]);
    reader->AddVariable(varName.substr(0, varName.size()-2).c_str(), &varsf[i]);
  }

  std::string method(config.methods()[0]);
  reader->BookMVA(method, std::string(config.outputPath()+"/"+config.lepton()+"_"+config.jetBin()+"_"+sgn+"_"+method+".weights.xml").c_str());

  std::vector<double> ret;
  for (Long64_t ievt=0; ievt < t->GetEntries(); ievt++) {
    t->GetEntry(ievt);
    std::copy(varsd.begin(), varsd.end(), varsf.begin());
    //std::cout << "EvaluateMVA\t" << reader->EvaluateMVA(method) << std::endl;
    ret.push_back(reader->EvaluateMVA(method));
  }
  f->Close();

  delete reader;
  return ret;
}

void  CreateMethodsTree_STMVA(const char *params, const char *inputFileName, std::vector<double> &tb_vector,  std::vector<double> &tqb_vector , std::vector<double> &tbtqb_vector, const char *outputFileName){

   STMVAConfig config(params);
   config.dump();
   const char *treeName = config.methods()[0].c_str();

   TFile *f_input  = new TFile(inputFileName);                 // default parameter read only
   TFile *f_output = new TFile(outputFileName, "RECREATE");  // add parameter to recreate tree

   TTree *t_input  = (TTree *) f_input->Get("TopologicalVariables");
   TTree *t_output = new TTree(treeName, treeName);

  //declare variables and get pointers to required original variables from source tree
  Double_t EventWeight; 			t_input->SetBranchAddress("EventWeight", &EventWeight);
  Double_t HT_AllJetsLeptonMET; 		t_input->SetBranchAddress("HT_AllJetsLeptonMET", &HT_AllJetsLeptonMET);

  // Check number of events is equal to vector size
  Int_t n_events_original =  t_input->GetEntries();
  if (n_events_original != tb_vector.size()  || n_events_original != tqb_vector.size() || n_events_original != tbtqb_vector.size() ){
      std::cout << "no events:"        << n_events_original << "in" << inputFileName << std::endl;
      std::cout << "tb_vector size"    << tb_vector.size()      << std::endl;
      std::cout << "tqb_vector size"   << tqb_vector.size()     << std::endl;
      std::cout << "tbtqb_vector size" << tbtqb_vector.size()   << std::endl;
      throw std::runtime_error("No. of events in the discriminant files differs from number of EventWeights in the input file");
    }

  // declare required variables and create output branches
  t_output->Branch("EventWeight",         &EventWeight,         "EventWeight/D");
  t_output->Branch("HT_AllJetsLeptonMET", &HT_AllJetsLeptonMET, "HT_AllJetsLeptonMET/D");
  // Double_t tb,tqb,tbtqb;  
  // t_output->Branch("tb", &tb, "tb/D");
  // t_output->Branch("tgb", &tqb, "tqb/D");
  // t_output->Branch("tbtqb", &tbtqb, "tbtqb/D");
  Double_t  channel_links[3];
  char     *channel_names[]   =  {"tb", "tqb", "tbtqb",NULL};
  char     *channel_types[]   =  {"tb/D", "tqb/D", "tbtqb/D"};
  for (Int_t i = 0; channel_names[i]; i++){
    t_output->Branch(channel_names[i],   &channel_links[i],   channel_types[i]);
  }

  //double maxelement[] = {-1*(std::numeric_limits<double>::infinity()),-1*(std::numeric_limits<double>::infinity()),-1*(std::numeric_limits<double>::infinity())};
  //double minelement[] = {(std::numeric_limits<double>::infinity()),(std::numeric_limits<double>::infinity()),(std::numeric_limits<double>::infinity())};
    double maxelement[] = {1,1,1};
    double minelement[] = {0,0,0};

  // main loop: record required values into given branches
  for (size_t i = 0; i < tb_vector.size(); i++) {
    channel_links[0] = tb_vector[i];
    channel_links[1] = tqb_vector[i];
    channel_links[2] = tbtqb_vector[i];
    //std::cout  <<  tb_vector[i] << tqb_vector[i] << tbtqb_vector[i] << std::endl;
    t_input  -> GetEvent(i);
    t_output -> Fill();
    if (minelement[0] > tb_vector[i])    minelement[0] = tb_vector[i];
    if (minelement[1] > tqb_vector[i])   minelement[1] = tqb_vector[i];
    if (minelement[2] > tbtqb_vector[i]) minelement[2] = tbtqb_vector[i];
    if (maxelement[0] < tb_vector[i])    minelement[0] = tb_vector[i];
    if (maxelement[1] < tqb_vector[i])   minelement[1] = tqb_vector[i];
    if (maxelement[2] < tbtqb_vector[i]) minelement[2] = tbtqb_vector[i];
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
    std::cout << "Evaluate data in \"input.root\" file by first method configured in \"STMVA.params\" files for tb, tqb and tbtqb channels and save results in to \"output.root\" file" << std::endl;
    std::cout << "Usage: " << argv[0] << " STMVA.params input.root output.root" << std::endl;
   }

  std::string params_path(argv[1]);

  std::string params_tb    = params_path+"_tb";
  std::string params_tqb   = params_path+"_tqb";
  std::string params_tbtqb = params_path+"_tbtqb";

  std::cout << "XXXXXX Evaulate for: "<< params_tb    << std::endl;
  std::vector<double> tb_vector    = STMVAEvaluate(params_tb.c_str(),argv[2]);
  //std::copy(tb_vector.begin(),tb_vector.end(),std::ostream_iterator<double>(std::cout," "));
  //std::cout << std::endl;

  std::cout << "XXXXXX Evaulate for: "<<  params_tqb   << std::endl;
  std::vector<double> tqb_vector   = STMVAEvaluate(params_tqb.c_str(),argv[2]);

  std::cout << "XXXXXX Evaulate for: "<<  params_tbtqb << std::endl;
  std::vector<double> tbtqb_vector = STMVAEvaluate(params_tbtqb.c_str(),argv[2]);


  CreateMethodsTree_STMVA(params_tb.c_str(), argv[2], tb_vector, tqb_vector, tbtqb_vector, argv[3]);

  return 0;
}
#endif
