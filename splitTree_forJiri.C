// RUN IT FROM ROOT via .L splitTree_forJiri

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TChain.h"
#include "TF1.h"
#include "TVector2.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TLegend.h"
#include <iostream>
#include <TROOT.h>

using namespace std;

TTree *curtree;
TH1D *curHisto;
TH1D *dummyHisto = new TH1D("dummyHisto", "no addSF for data", 8, 0, 8);

TDirectory *curdir;

Float_t Weight;
Float_t weight;

Float_t Apla, Spher, HTL, JetMt, HT3;
Float_t MEvent, MT1NL;

// newly added vars: 
Float_t M01mall, M0nl, M1nl, MT0nl, Met, Mtt, Mva_max, Wmt, Wpt, Jetm; //, Mva_max_next;
Float_t Centr, DRminejet, DiJetDrmin, Ht, Ht20, Ktminp, Lepdphimet, Lepemv;

Double_t apla, spher, htl, jetmt, ht3;
Double_t mevent, mT1nl;

// newly added vars: 
Double_t m01mall, m0nl, m1nl, mT0nl, met, mtt, mva_max, wmt, wpt, jetm; //, mva_max_next;
Double_t centr, dRminejet, diJetDrmin, ht, ht20, ktminp, lepdphimet, lepemv;

Int_t njets, NJets;

void splitTree(bool useEle) {

    // nominal way to run: HWWNNTrain(14,200,1,6,1,160)


    //---------------------------------------------------------------------------------
    //---------------file which saves data --------------------------------------------
    //-----uses the format DATE_TRAININGTYPE_NODES_HIDDENLAYERS_EPOCHS.root------------





    //-------------- variables from Shannon -------------------------------------------------------------------
    // double PI=TMath::Pi();//3.14159265;
    //  cout << PI << " " << TMath::Pi() <<endl;

    //---------------file which saves data --------------------------------------------
    //-----uses the format DATE_TRAININGTYPE_NODES_HIDDENLAYERS_EPOCHS.root------------

    // char fileName[100];

    // Some random definitions
    //  char inDir[500];
    Int_t n_bkg = 15;
    Int_t nfile = 3 + n_bkg; //nfile= 1(for data)+2(for lj and ll signals)+n(number of different backgrounds)
    float data_weight = 0;
    float bkg_weight = 0;
    Float_t sample_weight[20];
    Float_t _weight[20]; // promeni posle gore za bck
    Int_t type = -1;
    Int_t sample = -1;
    Float_t weight_signal = 1.;
    Int_t train = 2;
    Int_t val = 0;

    Float_t lumi = 9.7; // 9.7 fb-1
    Float_t topxs = 7420; // 7.42 pb = 7420 fb
    Float_t ljets_br = 0.1721; // Branching ratios (should be confirmed)
    Float_t dilep_br = 0.06627;

    //  ljets_br=1.;
    //  dilep_br=1.;


    //Define all backgrounds
    vector<string> bkg_samples;
    bkg_samples.push_back("Wlp"); //0
    bkg_samples.push_back("Wbb"); //1
    bkg_samples.push_back("Wcc"); //2
    if (useEle)
        bkg_samples.push_back("ZlpEE"); //3
    else
        bkg_samples.push_back("ZlpMuMu"); //3
    bkg_samples.push_back("ZlpTauTau"); //4
    if (useEle)
        bkg_samples.push_back("ZbbEE"); //5
    else
        bkg_samples.push_back("ZbbMuMu"); //5
    bkg_samples.push_back("ZbbTauTau"); //6
    if (useEle)
        bkg_samples.push_back("ZccEE"); //7
    else
        bkg_samples.push_back("ZccMuMu"); //7
    bkg_samples.push_back("ZccTauTau"); //8
    bkg_samples.push_back("tb"); //9
    bkg_samples.push_back("tqb"); //10
    bkg_samples.push_back("WW"); //11
    bkg_samples.push_back("WZ"); //12
    bkg_samples.push_back("ZZ"); //13
    bkg_samples.push_back("QCD"); //14

    TString dat_made = "1119";

    TString lchan = "ele";
    if (!useEle)
        lchan = "muo";

    //   TString inDir="inputs_test_"+lchan+"/";
    TString inDir = "/work/dude-clued0/ajung/D0/TopStuff/Lidiaz_TMVA/inputs_" + lchan + "_" + dat_made + "/";
    TString outDir = "/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples/split_trees_3samples_" + lchan + "_" + dat_made + "/";
    cout << "New *.root files are in: " + outDir << endl;

    // new for Jiri
    //  TString outDir="train_tree/";
    //  TString outDir="test_tree/";
    //  TString outDir="eval_tree/";


    TString rfile = "_miniTree.root";
    TString ftype;
    TString htype;


    for (int j = 0; j < 15; ++j)
        cout << j << " : " << bkg_samples[j] << endl;


    TList tree_list;
    TList histo_list;
    TFile * input(0);

    //   TString fname = "inputs_test/ttA_172_miniTree.root";
    //Data
    ftype = "Data";
    TString fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* data_tree = (TTree*) input->Get("events");
    TH1D* data_hist = dummyHisto; // need a dummy here

    // tt l+jets
    ftype = "ttA_172";
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* ttA_tree = (TTree*) input->Get("events");
    TH1D* ttA_hist = (TH1D*) input->Get("addSF");
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;

    // tt ll
    ftype = "ttAll_172";
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* ttAll_tree = (TTree*) input->Get("events");
    TH1D* ttAll_hist = (TH1D*) input->Get("addSF");
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;

    //   bkg_samples.push_back ("Wlp"); //0
    ftype = bkg_samples[0];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* Wlp_tree = (TTree*) input->Get("events");
    tree_list.Add(Wlp_tree);
    TH1D* Wlp_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(Wlp_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;


    //   bkg_samples.push_back ("Wbb"); //1
    ftype = bkg_samples[1];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* Wbb_tree = (TTree*) input->Get("events");
    tree_list.Add(Wbb_tree);
    TH1D* Wbb_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(Wbb_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;


    //   bkg_samples.push_back ("Wcc"); //2
    ftype = bkg_samples[2];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* Wcc_tree = (TTree*) input->Get("events");
    tree_list.Add(Wcc_tree);
    TH1D* Wcc_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(Wcc_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;


    //   bkg_samples.push_back ("ZlpLL"); //3
    ftype = bkg_samples[3];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* ZlpLL_tree = (TTree*) input->Get("events");
    tree_list.Add(ZlpLL_tree);
    TH1D* ZlpLL_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(ZlpLL_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;


    //   bkg_samples.push_back ("ZlpTauTau"); //4
    ftype = bkg_samples[4];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* ZlpTauTau_tree = (TTree*) input->Get("events");
    tree_list.Add(ZlpTauTau_tree);
    TH1D* ZlpTauTau_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(ZlpTauTau_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;


    //   bkg_samples.push_back ("ZbbLL"); //5
    ftype = bkg_samples[5];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* ZbbLL_tree = (TTree*) input->Get("events");
    tree_list.Add(ZbbLL_tree);
    TH1D* ZbbLL_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(ZbbLL_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;


    //   bkg_samples.push_back ("ZbbTauTau"); //6
    ftype = bkg_samples[6];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* ZbbTauTau_tree = (TTree*) input->Get("events");
    tree_list.Add(ZbbTauTau_tree);
    TH1D* ZbbTauTau_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(ZbbTauTau_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;


    //   bkg_samples.push_back ("ZccLL"); //7
    ftype = bkg_samples[7];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* ZccLL_tree = (TTree*) input->Get("events");
    tree_list.Add(ZccLL_tree);
    TH1D* ZccLL_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(ZccLL_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;


    //   bkg_samples.push_back ("ZccTauTau"); //8
    ftype = bkg_samples[8];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* ZccTauTau_tree = (TTree*) input->Get("events");
    tree_list.Add(ZccTauTau_tree);
    TH1D* ZccTauTau_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(ZccTauTau_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;


    //   bkg_samples.push_back ("tb"); //9
    ftype = bkg_samples[9];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* tb_tree = (TTree*) input->Get("events");
    tree_list.Add(tb_tree);
    TH1D* tb_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(tb_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;


    //   bkg_samples.push_back ("tqb"); //10
    ftype = bkg_samples[10];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* tqb_tree = (TTree*) input->Get("events");
    tree_list.Add(tqb_tree);
    TH1D* tqb_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(tqb_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;


    //  bkg_samples.push_back ("WW"); //11
    ftype = bkg_samples[11];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* WW_tree = (TTree*) input->Get("events");
    tree_list.Add(WW_tree);
    TH1D* WW_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(WW_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;


    // bkg_samples.push_back ("WZ"); //12
    ftype = bkg_samples[12];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* WZ_tree = (TTree*) input->Get("events");
    tree_list.Add(WZ_tree);
    TH1D* WZ_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(WZ_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;

    //   bkg_samples.push_back ("ZZ"); //13
    ftype = bkg_samples[13];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* ZZ_tree = (TTree*) input->Get("events");
    tree_list.Add(ZZ_tree);
    TH1D* ZZ_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(ZZ_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;


    //   bkg_samples.push_back ("QCD"); //14
    ftype = bkg_samples[14];
    fname = inDir + ftype + rfile;
    input = TFile::Open(fname); // check if file in local directory exists
    cout << " Using input file: " << input->GetName() << endl;
    TTree* QCD_tree = (TTree*) input->Get("events");
    tree_list.Add(QCD_tree);
    TH1D* QCD_hist = (TH1D*) input->Get("addSF");
    histo_list.Add(QCD_hist);
    cout << "   addSF " << addSF->GetBinContent(1) << " " << addSF->GetBinContent(2) << " " << addSF->GetBinContent(3) << " " << addSF->GetBinContent(4) << endl;




    curtree = ttA_tree;
    curHisto = ttA_hist;
    gROOT->ProcessLine(".x SetBranchAddresses.C");

    curtree = ttAll_tree;
    curHisto = ttAll_hist;
    gROOT->ProcessLine(".x SetBranchAddresses.C");
    //
    curtree = data_tree;
    curHisto = data_hist;
    gROOT->ProcessLine(".x SetBranchAddresses.C");

    TTree *First_tree = (TTree*) tree_list.First();
    curtree = First_tree;

    TH1D *First_histo = (TH1D*) histo_list.First();
    curHisto = First_histo;
    gROOT->ProcessLine(".x SetBranchAddresses.C");

    cout << "   addSF " << curHisto->GetBinContent(1) << " " << curHisto->GetBinContent(2) << " " << curHisto->GetBinContent(3) << " " << curHisto->GetBinContent(4) << endl;

    cout << "First " << First_tree << " " << tree_list.At(0) << endl;
    cout << "Histo " << First_histo << " " << histo_list.At(0) << endl;
    TTree *next_tree = (TTree*) tree_list.After(First_tree);
    TH1D *next_histo = (TH1D*) histo_list.After(First_histo);

    Int_t ibg = 0;
    while (next_tree) {
        ++ibg;
        cout << "next " << next_tree << " " << tree_list.At(ibg) << " i " << ibg << endl;
        cout << "histo " << next_histo << " " << histo_list.At(ibg) << " i " << ibg << endl;
        curtree = next_tree;
        curHisto = next_histo;
        gROOT->ProcessLine(".x SetBranchAddresses.C");

        next_tree = (TTree*) tree_list.After(next_tree);
        next_histo = (TH1D*) histo_list.After(next_histo);

    }



    //------------------------------------
    for (int ifile = 0; ifile < nfile; ++ifile)
        sample_weight[ifile] = 0;
    for (int ifile = 0; ifile < nfile; ++ifile) {
        if (ifile == 0) {//data case
            type = 2; //data
            sample = 0;
            curtree = data_tree;
            _weight[ifile] = 1;
            ftype = "data"; // ovde dodaj da je to ustvari sting
            curHisto = data_hist;
        }
        if (ifile == 1) {//signal case
            type = 1; //signal
            sample = 101;
            curtree = ttA_tree;
            //  _weight[ifile]=
            weight_signal = lumi * topxs*ljets_br;
            ftype = "ttA_172"; // ovde dodaj da je to ustvari sting
            curHisto = ttA_hist;
        }
        if (ifile == 2) {//signal case
            type = 1; //signal
            sample = 102;
            curtree = ttAll_tree;
            //      _weight[ifile]=weight_signal;
            weight_signal = lumi * topxs*dilep_br;
            ftype = "ttAll_172"; // ovde dodaj da je to ustvari sting
            curHisto = ttAll_hist;
        }
        if (ifile > 2) {//W2lp case
            type = 0; //bck
            sample = 200 + (ifile - 3);
            cout << ifile << " " << (TTree*) tree_list.At(ifile - 3) << endl;
            curtree = (TTree*) tree_list.At(ifile - 3); //bkg_Wjj_2lp;
            ftype = bkg_samples[ifile - 3]; // ovde dodaj da je to ustvari sting
            curHisto = (TH1D*) histo_list.At(ifile - 3);
        }

        cout << "file " << ftype << endl;
        cout << "addSF " << curHisto->GetBinContent(1) << " " << curHisto->GetBinContent(2) << " " << curHisto->GetBinContent(3) << " " << curHisto->GetBinContent(4) << endl;




        TString fileName = outDir + ftype + rfile;

        //   sprintf(fileName,"%s_test.root",ftype);
        TFile nn_tree_file(fileName, "recreate");
        TTree *nn_tree = new TTree("nn_tree", "Input Tree");

        int bsize = 256000; //1000000;  //arbitrary
        //  Ovde treba inputi u NN
        nn_tree->Branch("Apla", &Apla, "Apla/F", bsize);
        nn_tree->Branch("Spher", &Spher, "Spher/F", bsize);
        nn_tree->Branch("HTL", &HTL, "HTL/F", bsize);
        nn_tree->Branch("JetMt", &JetMt, "JetMt/F", bsize);
        nn_tree->Branch("HT3", &HT3, "HT3/F", bsize);
        nn_tree->Branch("MEvent", &MEvent, "MEvent/F", bsize);
        nn_tree->Branch("MT1NL", &MT1NL, "MT1NL/F", bsize);

        // newly added
        nn_tree->Branch("M01mall", &M01mall, "M01mall/F", bsize);
        nn_tree->Branch("M0nl", &M0nl, "M0nl/F", bsize);
        nn_tree->Branch("M1nl", &M1nl, "M1nl/F", bsize);
        nn_tree->Branch("MT0nl", &MT0nl, "MT0nl/F", bsize);
        nn_tree->Branch("Met", &Met, "Met/F", bsize);
        nn_tree->Branch("Mtt", &Mtt, "Mtt/F", bsize);
        nn_tree->Branch("Mva_max", &Mva_max, "Mva_max/F", bsize);

        //nn_tree->Branch("Mva_max_next",&Mva_max_next,"Mva_max_next/F",bsize);

        nn_tree->Branch("Wmt", &Wmt, "Wmt/F", bsize);
        nn_tree->Branch("Wpt", &Wpt, "Wpt/F", bsize);
        nn_tree->Branch("Centr", &Centr, "Centr/F", bsize);
        nn_tree->Branch("DRminejet", &DRminejet, "DRminejet/F", bsize);
        nn_tree->Branch("DiJetDrmin", &DiJetDrmin, "DiJetDrmin/F", bsize);
        nn_tree->Branch("Ht", &Ht, "Ht/F", bsize);
        nn_tree->Branch("Ht20", &Ht20, "Ht20/F", bsize);
        nn_tree->Branch("Ktminp", &Ktminp, "Ktminp/F", bsize);
        nn_tree->Branch("Lepdphimet", &Lepdphimet, "Lepdphimet/F", bsize);
        nn_tree->Branch("Lepemv", &Lepemv, "Lepemv/F", bsize);
        nn_tree->Branch("Jetm", &Jetm, "Jetm/F", bsize);

        nn_tree->Branch("NJets", &NJets, "NJets/I", bsize);

        nn_tree->Branch("type", &type, "type/I", bsize); //signal or background
        nn_tree->Branch("Weight", &Weight, "Weight/F", bsize);
        nn_tree->Branch("train", &train, "train/I", bsize);
        nn_tree->Branch("val", &val, "val/I", bsize);



        //   cout << ifile << " ws: " << weight_signal << " wb " << _weight[ifile] << endl;


        nn_tree_file.cd();
        //    nn_tree_file.mkdir(ftype);

        for (Long64_t ievt = 0; ievt < curtree->GetEntries(); ievt++) { // begin
            //      for (Long64_t ievt=0; ievt<100;ievt++) { // begin

            // if (ievt%1000 == 0) cout << "--- ... Processing event: " << ievt << endl;

            curtree->GetEntry(ievt);

            if (ifile == 0) // data and all bck except W+jets for test only
                train = 0;
            else
                train = ievt % 2;

            if (train == 0)
                val = 0;

            if (train == 1)
                val = ievt % 4;

            if (val == 3)
                val = 2;

            //     cout << "train " << train << " val " << val << endl;

            ///------------------------
            // Some cuts to remove strange events
            // 	if(mevent<0)continue;
            // 	if(mT1nl<0)continue;

            //	if(mevent<0)mevent=0;
            //	if(mT1nl<0)mT1nl=0;

            //	if(mevent>900)mevent=900;
            //	if(mT1nl>600)mT1nl=600;

            Apla = apla;
            Spher = spher;
            HTL = htl;
            JetMt = jetmt;
            HT3 = ht3;
            MEvent = mevent;
            MT1NL = mT1nl;

            NJets = njets;

            // newly added
            M01mall = m01mall;
            M0nl = m0nl;
            M1nl = m1nl;
            MT0nl = mT0nl;
            Met = met;
            Mtt = mtt;
            Mva_max = mva_max;

            //Mva_max_next=mva_max_next;

            Wmt = wmt;
            Wpt = wpt;
            Centr = centr;
            DRminejet = dRminejet;
            DiJetDrmin = diJetDrmin;
            Ht = ht;
            Ht20 = ht20;
            Ktminp = ktminp;
            Lepdphimet = lepdphimet;
            Lepemv = lepemv;
            Jetm = jetm;
            // cout << "wmt " << wmt << " met " << Wmt << endl;

            // this corrects samples train, test and validation to the same yields as if it were
            // not splitted - only data is not weighted
            if (ifile == 0)
                Weight = weight;
            else {
                if (val == 0) // yield
                    Weight = weight * 2.;
                else // train  + test
                    Weight = weight * 4.;


            }

            // have to apply the additional weights from the alpgen reweightings
            if (ifile > 0) {
                if (NJets == 2)
                    Weight *= curHisto->GetBinContent(2);
                if (NJets == 3)
                    Weight *= curHisto->GetBinContent(3);
                if (NJets > 3)
                    Weight *= curHisto->GetBinContent(4);
            }

            // values taken from these folders:
            // /prj_root/2671/top_write/savitsky/for_mujets/final_tt13_dIbad/stacking/final/RunII/VCJetTCS10/Central/mujets/beforeTag/Medium/_hht_4Data.eps
            // /prj_root/2671/top_write/savitsky/for_ejets/final_tt13_dIbad/stacking/final/RunII/VCJetDef/Central/ejets/beforeTag/Medium/_hht_4Data.eps
            //
            if (useEle) {
                if (ifile == 17) { // QCD
                    Weight = 0.366 * weight;
                    if (val == 0)
                        Weight = Weight * 2.;
                    else
                        Weight = Weight * 4.;
                }

                if (ifile == 3 || ifile == 4 || ifile == 5) {
                    if (NJets == 2)
                        Weight *= 0.901; //0.945;
                    if (NJets == 3)
                        Weight *= 0.999; //1.046;
                    if (NJets > 3)
                        Weight *= 1.328; //1.287;
                }
            } else {
                if (ifile == 17) {// QCD
                    Weight = 0.156 * weight;
                    if (val == 0)
                        Weight = Weight * 2.;
                    else
                        Weight = Weight * 4.;
                }

                if (ifile == 3 || ifile == 4 || ifile == 5) {
                    if (NJets == 2)
                        Weight *= 0.930; //1.025;
                    if (NJets == 3)
                        Weight *= 1.091; //1.202;
                    if (NJets > 3)
                        Weight *= 1.421; //1.525;
                }
            }

            if (ifile == 1 || ifile == 2)
                Weight *= weight_signal;

            //	nn_tree_file.cd(ftype);
            nn_tree->Fill();
        }
        nn_tree_file.Write();
        nn_tree_file.Close();
        //nn_tree_file.Delete();
    }
}
