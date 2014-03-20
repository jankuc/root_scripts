

#include "TString.h"

TCanvas *StackDiscriminant() {
    // Example of stacked histograms: class THStack
    //
    //  Author: Rene Brun
    // /work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root_matched_sets/train/ele/2jet/
    TString inPath = "/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root_matched_sets";
    TString lepton = "ele";
    TString jetbin = "2jet";
    TString set = "yield";

    
    const Int_t nTypes = 18;
    TString TypePlotLeg_mu[] = {"data", "QCD", "Wlp", "Wcc", "Wbb", "ZbbMuMu", "ZbbTauTau", "ZccMuMu", "ZccTauTau", "ZlpMuMu", "ZlpTauTau", "tb", "tqb", "WW", "WZ", "ZZ", "ttA_172", "ttAll_172"}; // for nice names in legends:)
    TString TypePlotLeg_e[] = {"data", "QCD", "Wlp", "Wcc", "Wbb", "ZbbEE", "ZbbTauTau", "ZccEE", "ZccTauTau", "ZlpEE", "ZlpTauTau", "tb", "tqb", "WW", "WZ", "ZZ", "ttA_172", "ttAll_172"}; // for nice names in legends:)
    TString Type_l[nTypes];
    TH1F** allHists = new TH1F*[19];
    
    THStack *hStack = new THStack("testStackCom", "Title of the stack"); //for drawing
    // /prj_root/2671/top_write/savitsky/for_%s/final_tt13_dIbad/controlplots/final/%s/%s/Central/%s/%s/%s/%s/%s_miniTree.root", lepType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data(), looseTight.Data(), Type_CP.Data());
    for (Int_t i = 1; i < nTypes; i++) {
         TString method = "MBC_1_1_noTrans";
        if (lepton.CompareTo("muo") == 0) {
            Type_l[i] = TypePlotLeg_mu[i];
        } else if (lepton.CompareTo("ele") == 0) {
            Type_l[i] = TypePlotLeg_e[i];
        } else {
            std::cout << "Wrong channel name: 'muo' or 'ele' only! You've used: '" << lepton << "'." << std::endl;
        }
        if (i==0) { // data
            TString filePath = Form("%s/%s/%s/%s", inPath.Data(), "data", lepton.Data(), jetbin.Data());
        } else {
            TString filePath = Form("%s/%s/%s/%s", inPath.Data(), set.Data(), lepton.Data(), jetbin.Data());
        }
        

        TString fileName = Form("%s/%s_miniTree.root", filePath.Data(), Type_l[i].Data());
        TFile *f = new TFile(fileName.Data(), "READ");
        if (f == NULL) { 
            return 0;
        }
        f->cd();
        TH1F *hist = new TH1F("h", "title h", 25, -1, 1);
        Float_t weight = 0;
        Double_t val = 0;
        TTree *t = (TTree*) f->Get(method.Data());
        t->SetBranchStatus("*", 1);
        t->SetBranchAddress( "ttA_172", &val);
        t->SetBranchAddress("Weight", &weight);

        for (Long64_t j = 0; j < t->GetEntries(); ++j) {
            t->GetEntry(j);
            hist->Fill(val,weight);
        }
        allHists[i]=hist;
        allHists[i]->Rebin(1);
        //allInputComb[i]->Sumw2();
        allHists[i] ->SetDirectory(gROOT);
        if (i > 0) {
            hStack->Add(((TH1F*) allHists[i]));
        }  
        f->Close();
    }


    THStack *hs = new THStack("hs", "Title of the 1st plot");
    //create three 1-d histograms
    TH1F *h1st = new TH1F("h1st", "test hstack", 100, -4, 4);
    h1st->FillRandom("gaus", 20000);
    h1st->SetFillColor(kRed);
    h1st->SetMarkerStyle(21);
    h1st->SetMarkerColor(kRed);
    hs->Add(h1st);
    TH1F *h2st = new TH1F("h2st", "test hstack", 100, -4, 4);
    h2st->FillRandom("gaus", 15000);
    h2st->SetFillColor(kBlue);
    h2st->SetMarkerStyle(21);
    h2st->SetMarkerColor(kBlue);
    hs->Add(h2st);
    TH1F *h3st = new TH1F("h3st", "test hstack", 100, -4, 4);
    h3st->FillRandom("gaus", 10000);
    h3st->SetFillColor(kGreen);
    h3st->SetMarkerStyle(21);
    h3st->SetMarkerColor(kGreen);
    hs->Add(h3st);

    TCanvas *cst = new TCanvas("cst", "stacked hists", 10, 10, 700, 700);
    //cst->SetFillColor(0);
    cst->Divide(1, 1);
    // in top left pad, draw the stack with defaults
    cst->cd(1);
    hs->Draw();
    // in top right pad, draw the stack in non-stack mode 
    // and errors option
    cst->cd(2);
    hStack->Draw();
    
    TLegend *legend = new TLegend(0.76, 0.0, 0.95, 0.9);
    legend->SetLineColor(0);
    legend->SetFillColor(0);
    legend->SetTextSize(0.04);
    legend->SetLineWidth(0);
    legend->AddEntry(allHists[0], Form("%1.0f Data", allHists[0]->Integral()), "PL");

    for (Int_t i = 0; i < 17; i++) {
        TString label = Type_l[17 - i];
        legend->AddEntry(allHists[17 - i], Form("%2.2f %s", allHists[17 - i]->Integral(), label.Data()), "F");
    }
    
   // legend->AddEntry(allHists[0], Form("%2.2f Wcc+Wbb", mc2Comb->Integral()), "");
    //legend->AddEntry(allHists[0], Form("%2.2f otherMC", mc0Comb->Integral()), "");
    legend->Draw("same");
    
    //hs->Draw("nostack,e1p");
    //in bottom left, draw in stack mode with "lego1" option
//    cst->cd(3);
//    gPad->SetFrameFillColor(17);
//    gPad->SetTheta(3.77);
//    gPad->SetPhi(2.9);
//    hs->Draw("lego1");
//
//    cst->cd(4);
//    //create two 2-D histograms and draw them in stack mode
//    gPad->SetFrameFillColor(17);
//    THStack *a = new THStack("a", "Title of the other plot");
//    TF2 *f1 = new TF2("f1",
//            "xygaus + xygaus(5) + xylandau(10)", -4, 4, -4, 4);
//    Double_t params[] = {130, -1.4, 1.8, 1.5, 1, 150, 2, 0.5, -2, 0.5,
//        3600, -2, 0.7, -3, 0.3};
//    f1->SetParameters(params);
//    TH2F *h2sta = new TH2F("h2sta", "h2sta", 20, -4, 4, 20, -4, 4);
//    h2sta->SetFillColor(38);
//    h2sta->FillRandom("f1", 4000);
//    TF2 *f2 = new TF2("f2", "xygaus + xygaus(5)", -4, 4, -4, 4);
//    Double_t params[] = {100, -1.4, 1.9, 1.1, 2, 80, 2, 0.7, -2, 0.5};
//    f2->SetParameters(params);
//    TH2F *h2stb = new TH2F("h2stb", "h2stb", 20, -4, 4, 20, -4, 4);
//    h2stb->SetFillColor(46);
//    h2stb->FillRandom("f2", 3000);
//    a->Add(h2sta);
//    a->Add(h2stb);
//    a->Draw();
    return cst;
}
