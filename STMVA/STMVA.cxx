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
//    Root> .x STMVA.C
//
//  or interpreted as ROOT macro directly from command line:
//
//    root STMVA.C
//
//  or you can compile it to a program with
//
//    g++ -g -O2 -Wall `root-config --cflags --libs` -lTMVA STMVA.C -o STMVA
//
//  and execute it as
//
//    ./STMVA
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
#include "TMVA/Config.h"
#endif

#include "STMVAConfig.h"
//#include "ZDeflate.h"

struct type_and_val {
    char type; // is 'I', 'D' or 'F'

    union {
        Int_t I;
        Double_t D;
        Float_t F;
    } val;
};

void STMVA(char *params) {
    STMVAConfig config(params);
    config.dump();

    // join signal names without delimiter
    std::string sgn = std::accumulate(config.signals().begin(), config.signals().end(), std::string(""));

    // This loads the library
    TMVA::Tools::Instance();

    // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
    TFile* outputFile = TFile::Open(std::string(config.outputPath() + "/" + config.outputName()).c_str(), "RECREATE");
    // Create the TMVA factory object
    TMVA::Factory *factory = new TMVA::Factory(
            std::string(config.lepton() + "_" + config.jetBin() + "_" + sgn).c_str(), outputFile,
            "!V:!Silent:Color:DrawProgressBar:Transformations=I;P;G:AnalysisType=Classification");
    //"!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
    (TMVA::gConfig().GetIONames()).fWeightFileDir = config.outputPath();

    // Define the input variables that shall be used for the MVA training
    for (std::vector<std::string>::const_iterator it = config.variables().begin();
            it != config.variables().end(); ++it) {
        std::string varName(*it);
        if (varName.size() > 2 && varName[varName.size() - 2] == '/') {
            factory->AddVariable(varName.substr(0, varName.size() - 2).c_str(), varName[varName.size() - 1]);
        } else
            factory->AddVariable(varName.c_str(), 'F');
    }
    // Add so-called "Spectator variables" (not used for training but will appear in final tree)
    for (std::vector<std::string>::const_iterator it = config.spectators().begin();
            it != config.spectators().end(); ++it) {
        std::string varName(*it);
        if (varName.size() > 2 && varName[varName.size() - 2] == '/') {
            factory->AddSpectator(varName.substr(0, varName.size() - 2).c_str(), varName[varName.size() - 1]);
        } else
            factory->AddSpectator(varName.c_str(), 'F');
    }



    //    // Old version
    //    // BEGIN

    TChain sigTr(config.treeName().c_str());
    TChain sigTe(config.treeName().c_str());
    TChain backTr(config.treeName().c_str());
    TChain backTe(config.treeName().c_str());

    // training data
    for (std::vector<std::string>::const_iterator it = config.trainingSignals().begin();
            it != config.trainingSignals().end(); ++it)
        sigTr.Add((*it).c_str());
    for (std::vector<std::string>::const_iterator it = config.trainingBackgrounds().begin();
            it != config.trainingBackgrounds().end(); ++it)
        backTr.Add((*it).c_str());

    // testing data
    for (std::vector<std::string>::const_iterator it = config.testingSignals().begin();
            it != config.testingSignals().end(); ++it)
        sigTe.Add((*it).c_str());
    for (std::vector<std::string>::const_iterator it = config.testingBackgrounds().begin();
            it != config.testingBackgrounds().end(); ++it)
        backTe.Add((*it).c_str());

    // END

    // yields data
    //for (std::vector<std::string>::const_iterator it = config.yieldsSignals().begin();
    //     it != config.yieldsSignals().end(); ++it)
    //  signal.Add((*it).c_str());
    //for (std::vector<std::string>::const_iterator it = config.yieldsBackgrounds().begin();
    //     it != config.yieldsBackgrounds().end(); ++it)
    //  background.Add((*it).c_str());
    //Int_t nYieldsSignals = signal.GetEntries() - nTrainingSignals - nTestingSignals;
    //Int_t nYieldsBackgrounds = background.GetEntries() - nTrainingBackgrounds - nTestingBackgrounds;

    // add friends for external methods
    std::map<std::string, TChain *> methodSigTr;
    std::map<std::string, TChain *> methodSigTe;
    std::map<std::string, TChain *> methodBackTr;
    std::map<std::string, TChain *> methodBackTe;
    for (std::vector<std::string>::const_iterator it = config.methods().begin();
            it != config.methods().end(); ++it) {
        std::string method(*it);
        std::cout << "***** First method-cycle Method: " << method << std::endl;
        if (config.methodType(method) != TMVA::Types::kExternal)
            continue;

        methodSigTr[method] = new TChain(config.methodTreeName(method).c_str());
        methodSigTe[method] = new TChain(config.methodTreeName(method).c_str());
        methodBackTr[method] = new TChain(config.methodTreeName(method).c_str());
        methodBackTe[method] = new TChain(config.methodTreeName(method).c_str());

        //config.methodTreeName(method);
        //config.methodLeaveName(method);
        //config.methodTrainingSignals(method);



        // training data
        for (std::vector<std::string>::const_iterator it = config.methodTrainingSignals(method).begin();
                it != config.methodTrainingSignals(method).end(); ++it)
            methodSigTr[method]->Add((*it).c_str());
        for (std::vector<std::string>::const_iterator it = config.methodTrainingBackgrounds(method).begin();
                it != config.methodTrainingBackgrounds(method).end(); ++it)
            methodBackTr[method]->Add((*it).c_str());

        // testing data
        for (std::vector<std::string>::const_iterator it = config.methodTestingSignals(method).begin();
                it != config.methodTestingSignals(method).end(); ++it)
            methodSigTe[method]->Add((*it).c_str());
        for (std::vector<std::string>::const_iterator it = config.methodTestingBackgrounds(method).begin();
                it != config.methodTestingBackgrounds(method).end(); ++it)
            methodBackTe[method]->Add((*it).c_str());

        // yields data
        //for (std::vector<std::string>::const_iterator it = config.methodYieldsSignals(method).begin();
        //     it != config.methodYieldsSignals(method).end(); ++it)
        //  methodSignal[method]->Add((*it).c_str());
        //for (std::vector<std::string>::const_iterator it = config.methodYieldsBackgrounds(method).begin();
        //     it != config.methodYieldsBackgrounds(method).end(); ++it)
        //  methodBackground[method]->Add((*it).c_str());

        sigTr.AddFriend(methodSigTr[method]);
        sigTe.AddFriend(methodSigTe[method]);
        backTr.AddFriend(methodBackTr[method]);
        backTe.AddFriend(methodBackTe[method]);
    }

    //std::cout << "Test: training(" << nTrainingSignals << "," << nTrainingBackgrounds << ")" << std::endl;
    //std::cout << "Test: testing(" << nTestingSignals << "," << nTestingBackgrounds << ")" << std::endl;
    //std::cout << "Test: yields(" << nYieldsSignals << "," << nYieldsBackgrounds << ")" << std::endl;

    // You can add an arbitrary number of signal or background trees

    // NEW
    // --- begin ----------------------------------------------------------

    factory->AddSignalTree(&sigTr, 1.0, "Training" );
    factory->AddSignalTree(&sigTe, 1.0, "Testing");
    factory->AddBackgroundTree(&backTr, 1.0, "Training");
    factory->AddBackgroundTree(&backTe, 1.0, "Testing");
    factory->SetWeightExpression(config.eventWeight());
    //factory->SetSignalWeightExpression(config.eventWeight());
    //factory->SetBackgroundWeightExpression(config.eventWeight());
    
    //    // Set individual event weights (the variables must exist in the tree)
    //   
    //    // Tell the factory how to use the training and testing events
        std::ostringstream iss;
        iss << "" //"nTrain_Signal=" << nTrainingSignals << ":"
    //            << "nTrain_Background=" << nTrainingBackgrounds << ":"
    //            << "nTest_Signal=" << nTestingSignals << ":"
    //            << "nTest_Background=" << nTestingBackgrounds << ":"
    //            << "SplitMode=Block:NormMode=NumEvents:
                << "!V:VerboseLevel=Debug:NormMode=NumEvents";
    factory->PrepareTrainingAndTestTree("", "", iss.str());

    // ---- Book MVA methods
    for (std::vector<std::string>::const_iterator it = config.methods().begin();
            it != config.methods().end(); ++it) {
        std::string method(*it);
        std::cout << "***** Method: " << method << std::endl;
        factory->BookMethod(config.methodType(method), method, config.methodParams(method));
    }
    //std::cout << "***** Method: " << "BDT" << std::endl;
    //factory->BookMethod(TMVA::Types::kBDT, "BDT", "!V:BoostType=Grad:nCuts=20:NNodesMax=5");
    // ---- Now you can tell the factory to train, test, and evaluate the MVAs

    // Train MVAs using the set of training events
    factory->TrainAllMethods();

    // ---- Evaluate all MVAs using the set of test events
    factory->TestAllMethods();

    // ----- Evaluate and compare performance of all configured MVAs
    factory->EvaluateAllMethods();
    
//    for (std::vector<std::string>::const_iterator it = config.methods().begin();
//            it != config.methods().end(); ++it) {
//        std::string method(*it);
//        if (strcmp(config.methodType(method) != TMVA::Types::kExternal ) { // Method isn't external, so we will 
//            
//        }
//    }

    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;

    delete factory;

    // Save the output
    outputFile->Close();
}

void STMVAReader(char *params = "STMVA.test.params", char* input = "input.root") {
    STMVAConfig config(params);
    config.dump();

    // join signal names without delimiter
    std::string sgn = std::accumulate(config.signals().begin(), config.signals().end(), std::string(""));

    // create TMVA::Reader object
    TMVA::Reader *reader = new TMVA::Reader();

    // root tree
    TFile *f = new TFile(input);
    TTree *t = (TTree*) f->Get(config.treeName().c_str());

    std::vector<float> vars(config.variables().size(), 0.);

    // create a set of variables and declare them to the reader
    //for (std::vector<std::string>::const_iterator it = config.variables().begin();
    //     it != config.variables().end(); ++it) {
    //std::string varName(*it);
    for (size_t i = 0; i < config.variables().size(); i++) {
        std::string varName(config.variables()[i]);
        if (!(varName.size() > 2 && varName[varName.size() - 2] == '/' && varName[varName.size() - 1] == 'D'))
            throw std::runtime_error("currently this support only double variables, can't use " + varName);
        reader->AddVariable(varName.substr(0, varName.size() - 2).c_str(), &vars[i]);
    }

    std::string method(config.methods()[0]);
    reader->BookMVA(method, std::string("weights/" + config.lepton() + "_" + config.jetBin() + "_" + sgn + "_" + method + ".weights.xml").c_str());

    for (Long64_t ievt = 0; ievt < t->GetEntries(); ievt++) {
        t->GetEntry(ievt);
        std::cout << reader->EvaluateMVA(method) << std::endl;
    }
    f->Close();

    delete reader;
}



#ifndef __CINT__

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Train all TMVA methods specified in configuration file \"STMVA.params\"" << std::endl;
        std::cout << "Usage: " << argv[0] << " STMVA.params" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Call you script function
    STMVA(argv[1]);

    return 0;
}
#endif
