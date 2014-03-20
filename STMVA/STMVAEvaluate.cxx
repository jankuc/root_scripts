// ----------------------------------------------------------------------
//
// * setup root environment (`source STMVA.setup`)
// * build using Makefile (`make` command)
// * execute ./STMVAEvaluate method tagBin jetBin input.root
//
// ----------------------------------------------------------------------

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include <stdexcept>
#include <iostream>
//#include <fstream>
//#include <sstream>
#include <numeric>

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
#include "TDirectoryFile.h"

std::vector<double> STMVAEvaluate(const char *params, const char* input) {
  STMVAConfig config(params);
  config.dump();

  // join signal names without delimiter
  std::string sgn = std::accumulate(config.signals().begin(), config.signals().end(), std::string(""));

  // create TMVA::Reader object
  TMVA::Reader *reader = new TMVA::Reader();

  // root tree
  TFile *f = new TFile(input);
  TTree *t = (TTree*) f->Get(config.treeName().c_str());

  std::vector<float> varsf(config.variables().size(), 0.);
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

#ifndef __CINT__
int main(int argc, char** argv)
{
  if (argc < 3) {
    std::cout << "Evaluate data in \"input.root\" file by first method configured in \"STMVA.params\" and print discriminant value." << std::endl;
    std::cout << "Usage: " << argv[0] << " STMVA.params input.root" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Call you script function
  //STMVAReader(argv[1], argv[2]);
  std::string filePref(argv[1]);
  std::vector<double> ttA_172_vector = STMVAEvaluate(std::string(filePref+"ttA_172").c_str(), "/work/budvar-clued0/fjfi-D0/tt_leptonjets/small_training_sample/p17_CC_diboson_EqOneTag_EqThreeJet_zero_Topo_small_training_sample.root");
  //std::vector<double> tqb_vector = STMVAEvaluate(std::string(filePref+"_tqb").c_str(), "/work/budvar-clued0/francji/subsets/small_training_sample/p17_CC_diboson_EqOneTag_EqThreeJet_zero_Topo_small_training_sample.root");
  //std::vector<double> tbtqb_vector = STMVAEvaluate(std::string(filePref+"_tbtqb").c_str(), "/work/budvar-clued0/francji/subsets/small_training_sample/p17_CC_diboson_EqOneTag_EqThreeJet_zero_Topo_small_training_sample.root");

  for (size_t i = 0; i < ttA_172_vector.size(); i++) {
    double ttA_172 = ttA_172_vector[i];
    // fill TTree
  }

  return 0;
}
#endif
