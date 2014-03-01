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

/**
  source_main       *.root full path of one of channels (QCD, data, Wlp, ...)
  source_channel1   *.txt full path of discriminant of one of channels (QCD, data, Wlp, ...)
  treeName          name of separation method -> we will name tree after it
  outputFileName    *.root full output path of one of channels (QCD, data, Wlp, ...)

  EXAMPLE:          /work/budvar-clued0/fjfi-D0/tt_leptonjets/samples/split_trees_3samples_muo_1119/ZZ_miniTree.root
                    /work/budvar-clued0/fjfi-D0/tt_leptonjets/results_txt/GLM/muo/ZZ_miniTree.txt
                    GLM
                    /work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root/GLM/split_trees_3samples_muo_1119/ZZ_miniTree.root
*/
void  CreateMethodsTree(char *source_main = "colTree.root",
                        char *source_channel1 = "colTree.in",
                        char *treeName = "col",
                        char *outputFileName = "colTree.root"){

    TFile *f_source = new TFile(source_main);                 // default parameter read only
    TFile *f_output = new TFile(outputFileName, "RECREATE");  // add parameter to recreate tree

    TTree *t_source = (TTree *) f_source->Get("nn_tree");
    TTree *t_output = new TTree(treeName, treeName);

    //declare variables and get pointers to required original variables from source tree
    Float_t Weight;
    t_source->SetBranchAddress("Weight", &Weight);
    //Double_t HT_AllJetsLeptonMET; 		t_source->SetBranchAddress("HT_AllJetsLeptonMET", &HT_AllJetsLeptonMET);
    //Char_t    MCkey[MCkeylength]; 		t_source->SetBranchAddress("MCkey[MCkeylength]", &MCkey[MCkeylength]);

    // declare required variables
    Int_t     n_events_original =  t_source->GetEntries();
    Int_t     n_events[]        =  {0}; //there was {0,0,0}
    Double_t  channel_links[3];
    char     *source_channel[]  =  {source_channel1, NULL};  //there was {src_ch1,src_ch2,src_ch3,NULL}
    char     *channel_names[]   =  {"ttA_172",NULL}; //there was {"src_ch1","src_ch2","src_ch3",NULL}
    char     *channel_types[]   =  {"ttA_172/D"}; //there was {"src_ch1","src_ch2","src_ch3"}
    Float_t   max_disc[]        =  {0}; //there was {0,0,0}
    Float_t   min_disc[]        =  {0}; //there was {0,0,0}
    Double_t  disc;
    std::vector<Double_t> channels[1];

    // create output branches
    t_output->Branch("Weight",         &Weight,         "Weight/D");

    //t_output->Branch(channel_names[i].c_str(),   &s,   std::string(channel_names[i]+"/D").c_str());
    for (Int_t i = 0; channel_names[i]; i++){
        t_output->Branch(channel_names[i],   &channel_links[i],   channel_types[i]);
    }

    // control input paths, find maximal discriminants, compare number of events
    for (Int_t i = 0; source_channel[i]; i++){
        std::ifstream indata(source_channel[i]);
        if (!indata || !indata.is_open())
            throw std::runtime_error("unable to open input file: "+std::string(source_channel[i]));
        while (true){
            indata >> disc;
            if (!indata) break;
            channels[i].push_back(disc);
            n_events[i]++;
            if (max_disc[i] < disc)
                max_disc[i] = disc;
            if (min_disc[i] > disc)
                min_disc[i] = disc;
        }
        if (n_events_original != n_events[i]){
            cout << "pocet v source:" <<  n_events_original << endl;
            cout << "pocet v " << i << "-tem kanalu:" <<  n_events[i] << endl;
            throw std::runtime_error("No. of events in the discriminant file differs from number of Weights in the source file: "
                                     + std::string(source_channel[i]));
        }
    }


    // main loop: record required values into given branches
    for (Int_t j = 0; j<n_events_original ; j++){
        for (Int_t i = 0; channel_names[i]; i++){
            channel_links[i] = (channels[i][j]-min_disc[i])/(max_disc[i]-min_disc[i]);
        }
        t_source -> GetEvent(j);
        t_output -> Fill();
    }

    for (Int_t i = 0; channel_names[i]; i++){
        // one way ReadFile(const source_channel[i], const char* branchDescriptor = "x/D")
        // second  way TTree t;  t.ReadFile(source_channel[i], "x")
        char parameter[100], title[100];
        sprintf(parameter, "%s_%s", treeName, channel_names[i]);
        sprintf(title, "Weighted %s, separation method: %s", channel_names[i], treeName);
        TH1F *hist = new TH1F(parameter, title, 31, 0, 1);
        // Double_t  Sum_Weight = 0;
        for (Int_t j = 0; j<n_events_original ; j++){
            t_output -> GetEvent(j);
            hist->Fill(channel_links[i], Weight);
            //hist_input = channel_links[i]-min_disc[i])/max_disc[i];
            //hist      -> Fill(hist_input,Weight);
            //Sum_Weight += Weight;
        }
        //hist->Draw();
        hist->Write();
        // Double_t integral = hist->Integral();
        // printf("Integral of weighted hist. for %s:  %f,   sum of related Weights  %f\n", channel_names[i], integral, Sum_Weight);
    }

    t_output->Write();
    f_output->Close();
    f_source->Close();
}

#ifndef __CINT__
int main(int argc, char** argv)
{
    if (argc < 5){
        std::cout << "Usage: " << argv[0] << "source_main source_channel1 methods destination_path" << std::endl;
        exit(1);
    }

    CreateMethodsTree(argv[1], argv[2], argv[3], argv[4]);

    return 0;
}
#endif
