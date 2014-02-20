// source [root setup]
//
// To Compile
// g++ -Wall $(root-config --libs --cflags) -o top_extractor_BNNBDT_EqOneTag_EqThreeJet top_extractor_BNNBDT_EqOneTag_EqThreeJet.C
//
// To run to one file
// ./top_extractor2 [source file]  [destination file]
//
// To run all files in certain directory
// for i in /[route]/*; do ./top_extractor2 $i [route]/$(basename ${i/.root/}); done
//

#ifndef __CINT__
// Include all needed headers here 
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"
#include <TH1.h>
#include "TCanvas.h"
#include <TRandom.h>

#include <TROOT.h>
#include <TChain.h>
#include <TSelector.h>

#endif 

using namespace std;




char *source_file_path;
char destination_path[1000];
char destination_path_variables[1000];


void top_extractor_BNNBDT_EqOneTag_EqThreeJet() {
    
    TFile *f = new TFile(source_file_path);
    
    TTree *t = (TTree *) f->Get("TopologicalVariables");

	Double_t Centrality_AllJets;		t->SetBranchAddress("Centrality_AllJets", &Centrality_AllJets);
	Double_t Cos_Jet1AllJets_AllJets; 	t->SetBranchAddress("Cos_Jet1AllJets_AllJets", &Cos_Jet1AllJets_AllJets);
	Double_t Cos_Jet1Lepton_Lab; 		t->SetBranchAddress("Cos_Jet1Lepton_Lab", &Cos_Jet1Lepton_Lab);
	Double_t Cos_Jet2AllJets_AllJets; 	t->SetBranchAddress("Cos_Jet2AllJets_AllJets", &Cos_Jet2AllJets_AllJets);
	Double_t Cos_Jet2Lepton_Lab; 		t->SetBranchAddress("Cos_Jet2Lepton_Lab", &Cos_Jet2Lepton_Lab);
	Double_t DeltaPhiJet1Jet2; 		t->SetBranchAddress("DeltaPhiJet1Jet2", &DeltaPhiJet1Jet2);
	Double_t DeltaPhiJet1MET; 		t->SetBranchAddress("DeltaPhiJet1MET", &DeltaPhiJet1MET);
	Double_t DeltaPhiJet2MET; 		t->SetBranchAddress("DeltaPhiJet2MET", &DeltaPhiJet2MET);
	Double_t DeltaPhiLeptonMET; 		t->SetBranchAddress("DeltaPhiLeptonMET", &DeltaPhiLeptonMET);
	Double_t DeltaRJet1Jet2; 		t->SetBranchAddress("DeltaRJet1Jet2", &DeltaRJet1Jet2);
	Double_t DeltaRMinLeptonJet; 		t->SetBranchAddress("DeltaRMinLeptonJet", &DeltaRMinLeptonJet);
	Double_t EventWeight; 			t->SetBranchAddress("EventWeight", &EventWeight);
	Double_t H_AllJets; 			t->SetBranchAddress("H_AllJets", &H_AllJets);
	Double_t H_Jet1Jet2; 			t->SetBranchAddress("H_Jet1Jet2", &H_Jet1Jet2);
	Double_t HT_AllJets; 			t->SetBranchAddress("HT_AllJets", &HT_AllJets);
	Double_t HT_AllJetsLeptonMET; 		t->SetBranchAddress("HT_AllJetsLeptonMET", &HT_AllJetsLeptonMET);
	Double_t HT_Jet1Jet2; 			t->SetBranchAddress("HT_Jet1Jet2", &HT_Jet1Jet2);
	Double_t HT_METlep; 			t->SetBranchAddress("HT_METlep", &HT_METlep);
	Double_t InstLumi; 			t->SetBranchAddress("InstLumi", &InstLumi);
	Double_t InvariantMass_AllJets; 	t->SetBranchAddress("InvariantMass_AllJets", &InvariantMass_AllJets);
	Double_t InvariantMass_Jet1Jet2; 	t->SetBranchAddress("InvariantMass_Jet1Jet2", &InvariantMass_Jet1Jet2);
	Double_t Jet1MuonPtRel; 		t->SetBranchAddress("Jet1MuonPtRel", &Jet1MuonPtRel);
	Double_t Jet2MuonPtRel; 		t->SetBranchAddress("Jet2MuonPtRel", &Jet2MuonPtRel);
	Double_t Jet3MuonPtRel; 		t->SetBranchAddress("Jet3MuonPtRel", &Jet3MuonPtRel);
	Double_t Jet4MuonPtRel; 		t->SetBranchAddress("Jet4MuonPtRel", &Jet4MuonPtRel);
	Double_t LeptonDCA; 			t->SetBranchAddress("LeptonDCA", &LeptonDCA);
	Double_t LeptonDCASig; 			t->SetBranchAddress("LeptonDCASig", &LeptonDCASig);
	Double_t LeptonDetEta; 			t->SetBranchAddress("LeptonDetEta", &LeptonDetEta);
	Double_t LeptonDetEtaCFT; 		t->SetBranchAddress("LeptonDetEtaCFT", &LeptonDetEtaCFT);
	Double_t LeptonE; 			t->SetBranchAddress("LeptonE", &LeptonE);
	Double_t LeptonEta; 			t->SetBranchAddress("LeptonEta", &LeptonEta);
	Double_t LeptonPhi; 			t->SetBranchAddress("LeptonPhi", &LeptonPhi);
	Double_t LeptonPt; 			t->SetBranchAddress("LeptonPt", &LeptonPt);
	Double_t METPhi; 			t->SetBranchAddress("METPhi", &METPhi);
	Double_t METPt; 			t->SetBranchAddress("METPt", &METPt);
	Double_t METPx; 			t->SetBranchAddress("METPx", &METPx);
	Double_t METPy; 			t->SetBranchAddress("METPy", &METPy);
	Double_t MetSig; 			t->SetBranchAddress("MetSig", &MetSig);
	Double_t Pt_Jet1Jet2; 			t->SetBranchAddress("Pt_Jet1Jet2", &Pt_Jet1Jet2);
	Double_t PVz; 				t->SetBranchAddress("PVz", &PVz);
	Double_t QTimesEta; 			t->SetBranchAddress("QTimesEta", &QTimesEta);
	Double_t weight_btag; 			t->SetBranchAddress("weight_btag", &weight_btag);
	Double_t weight_btagSF; 		t->SetBranchAddress("weight_btagSF", &weight_btagSF);
	Double_t weight_btagTaga; 		t->SetBranchAddress("weight_btagTaga", &weight_btagTaga);
	Double_t weight_Lumi; 			t->SetBranchAddress("weight_Lumi", &weight_Lumi);
	Double_t weight_Lumi2; 			t->SetBranchAddress("weight_Lumi2", &weight_Lumi2);
	Double_t weight_LeptonDetEta; 		t->SetBranchAddress("weight_LeptonDetEta", &weight_LeptonDetEta);
	Double_t weight_WPt; 			t->SetBranchAddress("weight_WPt", &weight_WPt);
	Double_t weight_VJets; 			t->SetBranchAddress("weight_VJets", &weight_VJets);
	Double_t weight_QCDshape; 		t->SetBranchAddress("weight_QCDshape", &weight_QCDshape);
	Double_t scale_HF; 			t->SetBranchAddress("scale_HF", &scale_HF);
	Double_t scale_MC; 			t->SetBranchAddress("scale_MC", &scale_MC);
	Double_t scale_MC2; 			t->SetBranchAddress("scale_MC2", &scale_MC2);
	Double_t scale_norm; 			t->SetBranchAddress("scale_norm", &scale_norm);
	Double_t weight_EMcorr; 		t->SetBranchAddress("weight_EMcorr", &weight_EMcorr);
	Double_t weight_Muon_ID; 		t->SetBranchAddress("weight_Muon_ID", &weight_Muon_ID);
	Double_t WPt; 				t->SetBranchAddress("WPt", &WPt);
	Double_t WTransverseMass; 		t->SetBranchAddress("WTransverseMass", &WTransverseMass);
	Double_t KS_LeptonPt; 			t->SetBranchAddress("KS_LeptonPt", &KS_LeptonPt);
	Double_t KS_METPt; 			t->SetBranchAddress("KS_METPt", &KS_METPt);
	Double_t KS_WTransverseMass; 		t->SetBranchAddress("KS_WTransverseMass", &KS_WTransverseMass);
	Double_t TrackQPt; 			t->SetBranchAddress("TrackQPt", &TrackQPt);
	Double_t TrackCurvSig; 			t->SetBranchAddress("TrackCurvSig", &TrackCurvSig);
	Double_t QTimesLeptonEta; 		t->SetBranchAddress("QTimesLeptonEta", &QTimesLeptonEta);
	Double_t w_superOR_trig; 		t->SetBranchAddress("w_superOR_trig", &w_superOR_trig);
	Double_t w_singlelep_trig; 		t->SetBranchAddress("w_singlelep_trig", &w_singlelep_trig);
	Double_t w_ljets_trig; 			t->SetBranchAddress("w_ljets_trig", &w_ljets_trig);
	Int_t    Is_W1c; 			t->SetBranchAddress("Is_W1c", &Is_W1c);
	Int_t    Analysis; 			t->SetBranchAddress("Analysis", &Analysis);
	Int_t    EventNumber; 			t->SetBranchAddress("EventNumber", &EventNumber);
	Int_t    MCindex; 			t->SetBranchAddress("MCindex", &MCindex);
	Int_t    FiredLepJetsTrigger; 		t->SetBranchAddress("FiredLepJetsTrigger", &FiredLepJetsTrigger);
	Int_t    FiredSingleLepTrigger; 	t->SetBranchAddress("FiredSingleLepTrigger", &FiredSingleLepTrigger);
	Int_t    Jet1MuonTagCharge; 		t->SetBranchAddress("Jet1MuonTagCharge", &Jet1MuonTagCharge);
	Int_t    Jet1PartonMatch; 		t->SetBranchAddress("Jet1PartonMatch", &Jet1PartonMatch);
	Int_t    Jet2MuonTagCharge; 		t->SetBranchAddress("Jet2MuonTagCharge", &Jet2MuonTagCharge);
	Int_t    Jet2PartonMatch; 		t->SetBranchAddress("Jet2PartonMatch", &Jet2PartonMatch);
	Int_t    Jet3MuonTagCharge; 		t->SetBranchAddress("Jet3MuonTagCharge", &Jet3MuonTagCharge);
	Int_t    Jet3PartonMatch; 		t->SetBranchAddress("Jet3PartonMatch", &Jet3PartonMatch);
	Int_t    Jet4MuonTagCharge; 		t->SetBranchAddress("Jet4MuonTagCharge", &Jet4MuonTagCharge);
	Int_t    Jet4PartonMatch; 		t->SetBranchAddress("Jet4PartonMatch", &Jet4PartonMatch);
	Int_t    LeptonCharge; 			t->SetBranchAddress("LeptonCharge", &LeptonCharge);
	Int_t    LeptonNCFTHits; 		t->SetBranchAddress("LeptonNCFTHits", &LeptonNCFTHits);
	Int_t    LeptonNSMTHits; 		t->SetBranchAddress("LeptonNSMTHits", &LeptonNSMTHits);
	Int_t    NGoodJets; 			t->SetBranchAddress("NGoodJets", &NGoodJets);
	Int_t    NAllPrimaryVertex; 		t->SetBranchAddress("NAllPrimaryVertex", &NAllPrimaryVertex);
	Int_t    NTaggedJets; 			t->SetBranchAddress("NTaggedJets", &NTaggedJets);
	Int_t    RunNumber; 			t->SetBranchAddress("RunNumber", &RunNumber);
	Double_t Jet1E; 			t->SetBranchAddress("Jet1E", &Jet1E);
	Double_t Jet1Pt; 			t->SetBranchAddress("Jet1Pt", &Jet1Pt);
	Double_t Jet1Phi; 			t->SetBranchAddress("Jet1Phi", &Jet1Phi);
	Double_t Jet1Eta; 			t->SetBranchAddress("Jet1Eta", &Jet1Eta);
	Double_t Jet1DetEta; 			t->SetBranchAddress("Jet1DetEta", &Jet1DetEta);
	Double_t Jet1Mass; 			t->SetBranchAddress("Jet1Mass", &Jet1Mass);
	Double_t Jet1BTagNN; 			t->SetBranchAddress("Jet1BTagNN", &Jet1BTagNN);
	Double_t Jet1QTimesEta; 		t->SetBranchAddress("Jet1QTimesEta", &Jet1QTimesEta);
	Double_t Jet1Taggability; 		t->SetBranchAddress("Jet1Taggability", &Jet1Taggability);
	Double_t Jet1Taggability_Error; 	t->SetBranchAddress("Jet1Taggability_Error", &Jet1Taggability_Error);
	Int_t    Jet1HasMU; 			t->SetBranchAddress("Jet1HasMU", &Jet1HasMU);
	Int_t    Jet1Flavor; 			t->SetBranchAddress("Jet1Flavor", &Jet1Flavor);
	Double_t Jet2E; 			t->SetBranchAddress("Jet2E", &Jet2E);
	Double_t Jet2Pt; 			t->SetBranchAddress("Jet2Pt", &Jet2Pt);
	Double_t Jet2Phi; 			t->SetBranchAddress("Jet2Phi", &Jet2Phi);
	Double_t Jet2Eta; 			t->SetBranchAddress("Jet2Eta", &Jet2Eta);
	Double_t Jet2DetEta; 			t->SetBranchAddress("Jet2DetEta", &Jet2DetEta);
	Double_t Jet2Mass; 			t->SetBranchAddress("Jet2Mass", &Jet2Mass);
	Double_t Jet2BTagNN; 			t->SetBranchAddress("Jet2BTagNN", &Jet2BTagNN);
	Double_t Jet2QTimesEta; 		t->SetBranchAddress("Jet2QTimesEta", &Jet2QTimesEta);
	Double_t Jet2Taggability; 		t->SetBranchAddress("Jet2Taggability", &Jet2Taggability);
	Double_t Jet2Taggability_Error; 	t->SetBranchAddress("Jet2Taggability_Error", &Jet2Taggability_Error);
	Int_t    Jet2HasMU; 			t->SetBranchAddress("Jet2HasMU", &Jet2HasMU);
	Int_t    Jet2Flavor; 			t->SetBranchAddress("Jet2Flavor", &Jet2Flavor);
	Double_t Jet3E; 			t->SetBranchAddress("Jet3E", &Jet3E);
	Double_t Jet3Pt; 			t->SetBranchAddress("Jet3Pt", &Jet3Pt);
	Double_t Jet3Phi; 			t->SetBranchAddress("Jet3Phi", &Jet3Phi);
	Double_t Jet3Eta; 			t->SetBranchAddress("Jet3Eta", &Jet3Eta);
	Double_t Jet3DetEta; 			t->SetBranchAddress("Jet3DetEta", &Jet3DetEta);
	Double_t Jet3Mass; 			t->SetBranchAddress("Jet3Mass", &Jet3Mass);
	Double_t Jet3BTagNN; 			t->SetBranchAddress("Jet3BTagNN", &Jet3BTagNN);
	Double_t Jet3Taggability; 		t->SetBranchAddress("Jet3Taggability", &Jet3Taggability);
	Double_t Jet3Taggability_Error; 	t->SetBranchAddress("Jet3Taggability_Error", &Jet3Taggability_Error);
	Int_t    Jet3HasMU; 			t->SetBranchAddress("Jet3HasMU", &Jet3HasMU);
	Int_t    Jet3Flavor; 			t->SetBranchAddress("Jet3Flavor", &Jet3Flavor);
	Double_t Jet4E; 			t->SetBranchAddress("Jet4E", &Jet4E);
	Double_t Jet4Pt; 			t->SetBranchAddress("Jet4Pt", &Jet4Pt);
	Double_t Jet4Phi; 			t->SetBranchAddress("Jet4Phi", &Jet4Phi);
	Double_t Jet4Eta; 			t->SetBranchAddress("Jet4Eta", &Jet4Eta);
	Double_t Jet4DetEta; 			t->SetBranchAddress("Jet4DetEta", &Jet4DetEta);
	Double_t Jet4Mass; 			t->SetBranchAddress("Jet4Mass", &Jet4Mass);
	Double_t Jet4BTagNN; 			t->SetBranchAddress("Jet4BTagNN", &Jet4BTagNN);
	Double_t Jet4Taggability; 		t->SetBranchAddress("Jet4Taggability", &Jet4Taggability);
	Double_t Jet4Taggability_Error; 	t->SetBranchAddress("Jet4Taggability_Error", &Jet4Taggability_Error);
	Int_t    Jet4HasMU; 			t->SetBranchAddress("Jet4HasMU", &Jet4HasMU);
	Int_t    Jet4Flavor; 			t->SetBranchAddress("Jet4Flavor", &Jet4Flavor);
	Double_t MostBLikeJetE; 		t->SetBranchAddress("MostBLikeJetE", &MostBLikeJetE);
	Double_t MostBLikeJetPt; 		t->SetBranchAddress("MostBLikeJetPt", &MostBLikeJetPt);
	Double_t MostBLikeJetPhi; 		t->SetBranchAddress("MostBLikeJetPhi", &MostBLikeJetPhi);
	Double_t MostBLikeJetEta; 		t->SetBranchAddress("MostBLikeJetEta", &MostBLikeJetEta);
	Double_t MostBLikeJetDetEta; 		t->SetBranchAddress("MostBLikeJetDetEta", &MostBLikeJetDetEta);
	Double_t MostBLikeJetBTagNN; 		t->SetBranchAddress("MostBLikeJetBTagNN", &MostBLikeJetBTagNN);
	Double_t MostBLikeJetLeptonDeltaPhi; 	t->SetBranchAddress("MostBLikeJetLeptonDeltaPhi", &MostBLikeJetLeptonDeltaPhi);
	Double_t MostBLikeJetQTimesEta; 	t->SetBranchAddress("MostBLikeJetQTimesEta", &MostBLikeJetQTimesEta);
// 	Double_t MostBLikeJetTaggability; 	t->SetBranchAddress("MostBLikeJetTaggability", &MostBLikeJetTaggability);
// 	Double_t MostBLikeJetTaggability_Error; t->SetBranchAddress("MostBLikeJetTaggability_Error", &MostBLikeJetTaggability_Error);
// 	Int_t    MostBLikeJetHasMU; 		t->SetBranchAddress("MostBLikeJetHasMU", &MostBLikeJetHasMU);
// 	Int_t    MostBLikeJetFlavor; 		t->SetBranchAddress("MostBLikeJetFlavor", &MostBLikeJetFlavor);
	Double_t LeadingBTaggedJetPt; 		t->SetBranchAddress("LeadingBTaggedJetPt", &LeadingBTaggedJetPt);
	Double_t LeadingBTaggedJetEta; 		t->SetBranchAddress("LeadingBTaggedJetEta", &LeadingBTaggedJetEta);
	Double_t LeadingBTaggedJetBTagNN; 	t->SetBranchAddress("LeadingBTaggedJetBTagNN", &LeadingBTaggedJetBTagNN);
	Double_t LeadingBTaggedJetLeptonDeltaPhi; t->SetBranchAddress("LeadingBTaggedJetLeptonDeltaPhi", &LeadingBTaggedJetLeptonDeltaPhi);
	Double_t SecondBTaggedJetPt; 		t->SetBranchAddress("SecondBTaggedJetPt", &SecondBTaggedJetPt);
	Double_t SecondBTaggedJetEta; 		t->SetBranchAddress("SecondBTaggedJetEta", &SecondBTaggedJetEta);
	Double_t SecondBTaggedJetBTagNN; 	t->SetBranchAddress("SecondBTaggedJetBTagNN", &SecondBTaggedJetBTagNN);
	Double_t SecondBTaggedJetLeptonDeltaPhi; t->SetBranchAddress("SecondBTaggedJetLeptonDeltaPhi", &SecondBTaggedJetLeptonDeltaPhi);
	Double_t LeadingLightQuarkJetE; 	t->SetBranchAddress("LeadingLightQuarkJetE", &LeadingLightQuarkJetE);
	Double_t LeadingLightQuarkJetPt;	t->SetBranchAddress("LeadingLightQuarkJetPt", &LeadingLightQuarkJetPt);
	Double_t LeadingLightQuarkJetPhi;	t->SetBranchAddress("LeadingLightQuarkJetPhi", &LeadingLightQuarkJetPhi);
	Double_t LeadingLightQuarkJetEta; 	t->SetBranchAddress("LeadingLightQuarkJetEta", &LeadingLightQuarkJetEta);
	Double_t LeadingLightQuarkJetMass; 	t->SetBranchAddress("LeadingLightQuarkJetMass", &LeadingLightQuarkJetMass);
	Double_t LeadingLightQuarkJetBTagNN; 	t->SetBranchAddress("LeadingLightQuarkJetBTagNN", &LeadingLightQuarkJetBTagNN);
	Double_t LeadingLightQuarkJetLeptonDeltaPhi; t->SetBranchAddress("LeadingLightQuarkJetLeptonDeltaPhi", &LeadingLightQuarkJetLeptonDeltaPhi);
	Double_t SecondLightQuarkJetPt; 	t->SetBranchAddress("SecondLightQuarkJetPt", &SecondLightQuarkJetPt);
	Double_t SecondLightQuarkJetPhi; 	t->SetBranchAddress("SecondLightQuarkJetPhi", &SecondLightQuarkJetPhi);
	Double_t SecondLightQuarkJetEta; 	t->SetBranchAddress("SecondLightQuarkJetEta", &SecondLightQuarkJetEta);
	Double_t SecondLightQuarkJetMass; 	t->SetBranchAddress("SecondLightQuarkJetMass", &SecondLightQuarkJetMass);
	Double_t SecondLightQuarkJetBTagNN; 	t->SetBranchAddress("SecondLightQuarkJetBTagNN", &SecondLightQuarkJetBTagNN);
	Double_t SecondLightQuarkJetLeptonDeltaPhi; t->SetBranchAddress("SecondLightQuarkJetLeptonDeltaPhi", &SecondLightQuarkJetLeptonDeltaPhi);
	Int_t    MCkeylength; 			t->SetBranchAddress("MCkeylength", &MCkeylength);
	//Char_t   *MCkey;		 	t->SetBranchAddress("MCkey", &MCkey);
        //Char_t   *MCkey; 		t->SetBranchAddress("MCkey", &MCkey);
        //std::string MCkey;       		t->SetBranchAddress("MCkey", &MCkey);
	Double_t Jet1BTagMVA; 			t->SetBranchAddress("Jet1BTagMVA", &Jet1BTagMVA);
	Double_t Jet2BTagMVA; 			t->SetBranchAddress("Jet2BTagMVA", &Jet2BTagMVA);
	Double_t Jet3BTagMVA; 			t->SetBranchAddress("Jet3BTagMVA", &Jet3BTagMVA);
	Double_t Jet4BTagMVA; 			t->SetBranchAddress("Jet4BTagMVA", &Jet4BTagMVA);
	Double_t LeadingLightQuarkJetBTagMVA; 	t->SetBranchAddress("LeadingLightQuarkJetBTagMVA", &LeadingLightQuarkJetBTagMVA);
	Double_t SecondLightQuarkJetBTagMVA; 	t->SetBranchAddress("SecondLightQuarkJetBTagMVA", &SecondLightQuarkJetBTagMVA);
	Double_t LeadingBTaggedJetBTagMVA; 	t->SetBranchAddress("LeadingBTaggedJetBTagMVA", &LeadingBTaggedJetBTagMVA);
	Double_t SecondBTaggedJetBTagMVA; 	t->SetBranchAddress("SecondBTaggedJetBTagMVA", &SecondBTaggedJetBTagMVA);
	Double_t MostBLikeJetBTagMVA; 		t->SetBranchAddress("MostBLikeJetBTagMVA", &MostBLikeJetBTagMVA);
    
        Int_t nevents = t->GetEntries();
    
    FILE *dest_file;
    dest_file = fopen(destination_path,"w");

	// cout << "ok 1";

    for (Int_t i = 0; i<  nevents; i++) {

	t->GetEvent(i);
        // cout << "MCkey:" <<  MCkey << endl;
        // throw std::runtime_error("MCkey:"+ std::string(MCkey));
        // t->Scan("MCkey");
        // t->Draw("MCkey");
        // t->Show(i);
        //printf("MCkey: %s \n", MCkey);

// 1	LeadingBTaggedJetEta			BNN
// 2	LeadingBTaggedJetLeptonDeltaPhi		BNN
// 3	LeadingBTaggedJetPt			BNN
// 4	LeadingLightQuarkJetEta			BNN
// 5	LeadingLightQuarkJetLeptonDeltaPhi	BNN
// 6	SecondLightQuarkJetBTagMVA		BNN
// 7	SecondLightQuarkJetEta			BNN
// 8	SecondLightQuarkJetPt			BNN
	fprintf(dest_file,"%30.20f",LeadingBTaggedJetEta);
	fprintf(dest_file,"%30.20f",LeadingBTaggedJetLeptonDeltaPhi);
	fprintf(dest_file,"%30.20f",LeadingBTaggedJetPt);
	fprintf(dest_file,"%30.20f",LeadingLightQuarkJetEta);
	fprintf(dest_file,"%30.20f",LeadingLightQuarkJetLeptonDeltaPhi);
	fprintf(dest_file,"%30.20f",SecondLightQuarkJetBTagMVA);
	fprintf(dest_file,"%30.20f",SecondLightQuarkJetEta);
	fprintf(dest_file,"%30.20f",SecondLightQuarkJetPt);
// 9	DeltaPhiLeptonMET			BDT+BNN
// 10	LeadingBTaggedJetBTagMVA		BDT+BNN
// 11	LeadingLightQuarkJetBTagMVA		BDT+BNN
// 12	LeadingLightQuarkJetPt			BDT+BNN
// 13	LeptonEta				BDT+BNN
// 14	LeptonPt				BDT+BNN
// 15	METPt					BDT+BNN
// 16	QTimesEta				BDT+BNN
// 17	WTransverseMass				BDT+BNN
	fprintf(dest_file,"%30.20f",DeltaPhiLeptonMET);
	fprintf(dest_file,"%30.20f",LeadingBTaggedJetBTagMVA);
	fprintf(dest_file,"%30.20f",LeadingLightQuarkJetBTagMVA);
	fprintf(dest_file,"%30.20f",LeadingLightQuarkJetPt);
	fprintf(dest_file,"%30.20f",LeptonEta);
	fprintf(dest_file,"%30.20f",LeptonPt);
	fprintf(dest_file,"%30.20f",METPt);
	fprintf(dest_file,"%30.20f",QTimesEta);
	fprintf(dest_file,"%30.20f",WTransverseMass);
// 18	Centrality_AllJets			BDT
// 19	Cos_Jet1Lepton_Lab			BDT
// 20	Cos_Jet2Lepton_Lab			BDT
// 21	DeltaPhiJet1MET				BDT
// 22	DeltaPhiJet2MET				BDT
// 23	DeltaRJet1Jet2				BDT
// 24	DeltaRMinLeptonJet			BDT
// 25	HT_AllJets				BDT
// 26	HT_AllJetsLeptonMET			BDT
// 27	HT_METlep				BDT
// 28	InvariantMass_Jet1Jet2			BDT
// 29	Jet1Mass				BDT
// 30	Jet1QTimesEta				BDT
// 31	Jet2Pt					BDT
// 32	Jet2QTimesEta				BDT
// 33	LeadingLightQuarkJetE			BDT
// 34	LeadingLightQuarkJetMass		BDT
// 35	MostBLikeJetQTimesEta			BDT
// 36	Pt_Jet1Jet2				BDT
// 37	QTimesLeptonEta				BDT
// 38	WPt					BDT
	fprintf(dest_file,"%30.20f",Centrality_AllJets);
	fprintf(dest_file,"%30.20f",Cos_Jet1Lepton_Lab);
	fprintf(dest_file,"%30.20f",Cos_Jet2Lepton_Lab);
	fprintf(dest_file,"%30.20f",DeltaPhiJet1MET);
	fprintf(dest_file,"%30.20f",DeltaPhiJet2MET);
	fprintf(dest_file,"%30.20f",DeltaRJet1Jet2);
	fprintf(dest_file,"%30.20f",DeltaRMinLeptonJet);
	fprintf(dest_file,"%30.20f",HT_AllJets);
	fprintf(dest_file,"%30.20f",HT_AllJetsLeptonMET);
	fprintf(dest_file,"%30.20f",HT_METlep);
	fprintf(dest_file,"%30.20f",InvariantMass_Jet1Jet2);
	fprintf(dest_file,"%30.20f",Jet1Mass);
	fprintf(dest_file,"%30.20f",Jet1QTimesEta);
	fprintf(dest_file,"%30.20f",Jet2Pt);
	fprintf(dest_file,"%30.20f",Jet2QTimesEta);
	fprintf(dest_file,"%30.20f",LeadingLightQuarkJetE);
	fprintf(dest_file,"%30.20f",LeadingLightQuarkJetMass);
	fprintf(dest_file,"%30.20f",MostBLikeJetQTimesEta);
	fprintf(dest_file,"%30.20f",Pt_Jet1Jet2);
	fprintf(dest_file,"%30.20f",QTimesLeptonEta);
	fprintf(dest_file,"%30.20f",WPt);
	fprintf(dest_file,"%30.20f\n",EventWeight);
    }
    
    fclose(dest_file);
    
//     dest_file = fopen(destination_path_variables,"w");    
// 
//     TObjArray *arr = t->GetListOfBranches();
//     for (int i = 0; i < arr->GetSize(); i++) {
// 		fprintf(dest_file,"%s\n", arr->At(i)->GetTitle() );
//     }
//     fclose(dest_file);
    
}



#ifndef __CINT__
int main(int argc, char** argv)
{
    source_file_path = argv[1];

    sprintf(destination_path,"%s",argv[2]);
//     sprintf(destination_path_variables,"%s.var_list",argv[2]);

    top_extractor_BNNBDT_EqOneTag_EqThreeJet();

    return 0;
}
#endif


