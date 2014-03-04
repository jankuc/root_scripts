// text file must not have nan, inf and other chars*.
#ifndef __CINT__
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "TFile.h"
#include "TTree.h"
#include <TH1.h>
#include "TCanvas.h"
#include "float.h"
#include <stdio.h>
#include <stdlib.h>
#endif
#include <cmath>

TTree* Txt2Root_tt_leptonjets(const char* in_file_char,
        const char *out_file_char) {

  TString in_file_txt = in_file_char;
    TString out_file_root = out_file_char;
    out_file_root = out_file_root.ReplaceAll(".root", 5, "_tmp.root", 9);

    char* fileHeader = "Apla:Spher:HTL:JetMt:HT3:MEvent:MT1NL:M01mall:M0nl:M1nl:MT0nl:Met"\
      ":Mtt:Mva_max:Wmt:Wpt:Centr:DRminejet:DiJetDrmin:Ht:Ht20:Ktminp:Lepdphimet:Lepemv:"\
      "Jetm/F:NJets/I:type/I:Weight/F:train/I:val/I";

  TFile* f_out = new TFile(out_file_root,"RECREATE");
  TTree* t = new TTree("nn_tree","MC from 2014/01/28 loaded from txt files.");
  Long64_t n_lines = t->ReadFile(in_file_txt,fileHeader);
    printf("    Loaded %lld lines.\n", n_lines);
    //t->Print();
    t->Write();
    f_out->Write();
    f_out->Close();
    return t;
}

/**
 * @brief restoreNans
 * @param in_file_char  "full_path_to_root_file.root"
 * @param muo_ele       one of {"muo", "ele"}
 */
int restoreNans(const char* in_file_char, const char* muo_ele) {
    TString in_file_root = in_file_char;
  in_file_root.ReplaceAll(".root",5,"_tmp.root",9);

  std::cout<<"Input File: "<< in_file_root<<std::endl;
  std::cout<<"Lepton: "<<muo_ele<<std::endl;
    Float_t myNAN = -99999;
    Float_t newNAN = (Float_t) atof("nan");
    Float_t newINF = (Float_t) atof("inf");

    TFile* f_in = new TFile(in_file_root, "READ");
    TTree* t_in = (TTree*) f_in->Get("nn_tree");

    TString var_names[] = {"Apla", "Spher", "HTL", "JetMt", "HT3", "MEvent", \
                         "MT1NL", "M01mall", "M0nl", "M1nl", "MT0nl", "Met",\
                         "Mtt", "Mva_max", "Wmt", "Wpt", "Centr", "DRminejet",\
                         "DiJetDrmin", "Ht", "Ht20", "Ktminp", "Lepdphimet",\
                         "Lepemv", "Jetm", "NJets", "type", "Weight", "train", "val"};
    int n_changing;
    const int n_float_vars = 25;
    int changing_vars[7];
    // variables, which will be changed (Beware the 0-based numbering!!!)
    const int changing_ele[] = {14, 24};
  const int changing_muo[] = {5,6,8,9,10,14,24};

    Float_t float_addresses[n_float_vars];

    if (strcmp(muo_ele, "ele") == 0) {
        n_changing = 2;
        for (int i = 0; i < n_changing; i++) {
            changing_vars[i] = changing_ele[i];
        }
    } else if (strcmp(muo_ele, "muo") == 0) {
        n_changing = 7;
        for (int i = 0; i < n_changing; i++) {
            changing_vars[i] = changing_muo[i];
        }
    } else {
        std::cout << "INPUT muo_ele has to be one of \"muo\" or \"ele\"" << std::endl;
        return 1;
    }

    // LOOP
    t_in->SetBranchStatus("*", 1);
    TFile* f_out = new TFile(in_file_root.ReplaceAll("_tmp.root", 9, ".root", 5), "RECREATE");
    TTree* t_out = t_in->CloneTree(0); //new TTree("nn_tree", "nn_tree");
    TString curr_var_name;

    for (int i = 0; i < n_float_vars; i++) { // we clone ALL float the variables
        curr_var_name = var_names[i];
        t_in ->SetBranchAddress(curr_var_name, &float_addresses[i]);
        t_out ->SetBranchAddress(curr_var_name, &float_addresses[i]);
    }

    Int_t NJets;
    t_in->SetBranchAddress("NJets", &NJets);
    t_out->SetBranchAddress("NJets", &NJets);
    Int_t type;
    t_in->SetBranchAddress("type", &type);
    t_out->SetBranchAddress("type", &type);
    Float_t Weight;
    t_in->SetBranchAddress("Weight", &Weight);
    t_out->SetBranchAddress("Weight", &Weight);
    Int_t train;
    t_in->SetBranchAddress("train", &train);
    t_out->SetBranchAddress("train", &train);
    Int_t val;
    t_in->SetBranchAddress("val", &val);
    t_out->SetBranchAddress("val", &val);

    Long64_t n_entries = t_in->GetEntries();
    std::cout << "Entries: " << n_entries << std::endl;
    for (Long64_t ii = 0; ii < n_entries; ii++) {
        t_in->GetEntry(ii);
        for (int j = 0; j < n_changing; j++) { // over 2 or 7 changing vars
            if (j < n_changing - 1) { // for NANs (variables before JetM)
                if (float_addresses[changing_vars[j]] == myNAN) {
                    float_addresses[changing_vars[j]] = newNAN;
                }
            } else { // jetm := -INF;
                float_addresses[changing_vars[j]] = newINF;
            }
        }
        t_out->Fill();
    }
    t_out->Print();
    f_out->Write();
    f_in->Close();
    f_out->Close();
    return 0;
}


#ifndef __CINT__

int main(int argc, char** argv) {
    const TString in_path = argv[1];
    const TString out_path = argv[2];
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " input_text_file_path output_root_file_path" << std::endl;
        exit(1);
    }
    TString lepton;
    Txt2Root_tt_leptonjets(in_path, out_path);
    if (in_path.Contains("ele") && out_path.Contains("ele")) {
        lepton = "ele";
    } else if (in_path.Contains("muo") && out_path.Contains("muo")) {
        lepton = "muo";
    }
    restoreNans(out_path, lepton);
    return 0;
}
#endif
