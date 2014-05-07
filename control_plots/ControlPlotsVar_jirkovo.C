
// gcc -Wall `root-config --libs --cflags` -g -I /io/include -o ControlPlotsVar ControlPlotsVar.C
// Outpath has to be craeated prior to executing this 

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
#include "TString.h"
#include "TFrame.h"
#include "TPaveText.h"
//#include <TRandom.h>
#include <stdio.h>
#include <stdlib.h>
#include <THStack.h>
#include <TLegend.h>
#include <TROOT.h>
#include <TStyle.h>

#endif

void setColorsForHistograms(TH1F** allHists) {
    Int_t color = 8;
    for (Int_t i = 11; i < 13; i++) {
        allHists[i]->SetLineColor(color);
        allHists[i]->SetFillColor(color);
        --color;
    }
    color = 15;
    for (Int_t i = 5; i < 11; i++) {
        if (color == 10)--color;
        allHists[i]->SetLineColor(color);
        allHists[i]->SetFillColor(color);
        --color;
    }
    color = 29;
    for (Int_t i = 13; i < 16; i++) {
        allHists[i]->SetLineColor(color);
        allHists[i]->SetFillColor(color);
        ++color;
    }

    allHists[17]->SetLineColor(6);
    allHists[17]->SetFillColor(6);
    allHists[16]->SetLineColor(2);
    allHists[16]->SetFillColor(2);
    allHists[2]->SetLineColor(3);
    allHists[2]->SetFillColor(3);
    allHists[3]->SetLineColor(8);
    allHists[3]->SetFillColor(8);
    allHists[4]->SetLineColor(kGreen + 1);
    allHists[4]->SetFillColor(kGreen + 1);
    allHists[1]->SetLineColor(28);
    allHists[1]->SetFillColor(28);

    allHists[0]->SetMarkerStyle(21); // 20
    //allHists[0]->Sumw2();
}

void writeCanvasFiles(TCanvas *canv, const char *fileName) {
    char file[512];
    sprintf(file, "%s.eps", fileName);
    canv->Print(file);
    sprintf(file, "%s.png", fileName);
    canv->Print(file);
    return;
    gROOT->SetStyle("Plain");
}

void ControlPlotsVar(
        char* setC, // yield
        char* leptonC, // muo or ele
        char* jetbinC, // 2jet or 3jet or 4jet
        char* treeNameC, // nn_tree or 'MBC_1_1_noTrans' 
	char* outputC, // "/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_plots"
        char* varNameC, // Apla or ttA_172
	Int_t  nBin, // 25
        Double_t intervalStart = 0.0,
        Double_t intervalEnd = 1.0) {
    
    TString inPath = "/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_root_sets";
 		     //"/work/budvar-clued0/fjfi-D0/tt_leptonjets/history_made_2013_03_23/results_root_matched_sets";
                     //"/work/budvar-clued0/fjfi-D0/tt_leptonjets/samples_root_sets";
                    // "/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_root_matched_sets";
    TString lepton = leptonC;
    TString jetbin = jetbinC;
    TString set = setC;
    
    TString treeName = treeNameC; //BDT MBC_1_1_noTrans MBC_30_30_noTrans MBC_100_100_noTrans NNSU_no_gain_var_all_nonzero_weights NNSU_no_gain_var_first_5
    TString varName = varNameC; //ttA_172
    
    TString niceName; // we will use this for printing titles and such.
    if (treeName.CompareTo("nn_tree") == 0) { // because we are loading FLOAT variable from nn_tree and need to cast it to double
        niceName = varName;
    } else {
        niceName = treeName;
    }
    
    Double_t eps = 10e-5;
    
    const Int_t nTypes = 18;
    TString TypePlotLeg_mu[] = {"data", "QCD", "Wlp", "Wcc", "Wbb", "ZbbMuMu", "ZbbTauTau",
    "ZccMuMu", "ZccTauTau", "ZlpMuMu", "ZlpTauTau", "tb", "tqb", "WW", "WZ", "ZZ", "ttA_172", "ttAll_172"}; // for nice names in legends:)
    TString TypePlotLeg_e[] = {"data", "QCD", "Wlp", "Wcc", "Wbb", "ZbbEE", "ZbbTauTau",
    "ZccEE", "ZccTauTau", "ZlpEE", "ZlpTauTau", "tb", "tqb", "WW", "WZ", "ZZ", "ttA_172", "ttAll_172"}; // for nice names in legends:)
    TString Type_l[nTypes];
    TH1F* allHists[19];
    
    THStack *hStack = new THStack("testStackCom", Form("%s %s %s %s", niceName.Data(), lepton.Data(), jetbin.Data(), set.Data()));
    Int_t lines[nTypes]; // just for validation of Integral()
    Double_t weights[nTypes]; // just for validation of Integral()
    
    for (Int_t i = 0; i < nTypes ; i++) { // over all channels
        
        //  Defining Channels (muo/ele))
        
        if (lepton.CompareTo("muo") == 0) { // muo
            Type_l[i] = TypePlotLeg_mu[i];
        } else if (lepton.CompareTo("ele") == 0) { // ele
            Type_l[i] = TypePlotLeg_e[i];
        } else {
            std::cout << "Wrong channel name: 'muo' or 'ele' only! You've used: '" << lepton << "'." << std::endl;
        }
        
        // Loading Files
        
        TString filePath;
        if (i==0) { // data (we have to load it from other directory)
            std::cout << "Loading " << Type_l[i] << "." << std::endl;
            filePath = Form("%s/%s/%s/%s", inPath.Data(), "data", lepton.Data(), jetbin.Data());
        } else {
            std::cout << "Loading " << Type_l[i] << "." << std::endl;
            filePath = Form("%s/%s/%s/%s", inPath.Data(), set.Data(), lepton.Data(), jetbin.Data());
        }
        TString fileName = Form("%s/%s_miniTree.root", filePath.Data(), Type_l[i].Data());
        TFile *f = new TFile(fileName.Data(), "READ");
        if (f == NULL) { 
            return;
        }
        f->cd();
        
        // Loading Trees and variables
        // Creates histogram for current channel
        TH1F *hist = new TH1F("h", Form("%s %s %s %s", niceName.Data(), lepton.Data(), jetbin.Data(), set.Data()), nBin, intervalStart - eps, intervalEnd + eps);
        Float_t weightF, valF;  // weight is Float in both nn_tree and method_trees
        Double_t weightD, valD; // value of discriminant. Is Double_t
        TTree *t = (TTree*) f->Get(treeName);
        t->SetBranchStatus("*", 1);
        if (treeName.CompareTo("nn_tree")==0) { // because we are loading FLOAT variable from nn_tree and need to cast it to double
           t->SetBranchAddress(varName.Data(), &valF);
        } else { // we are loading DOUBLE variable 'ttA_172' from tree named 'method'
           t->SetBranchAddress( varName.Data(), &valD);
        }

        //t->SetBranchAddress("Weight", &weightD); 
         t->SetBranchAddress("Weight", &weightF); 
        // ADDING events to HISTOGRAM
        

        lines[i] = 0;
        weights[i] = 0;
        for (Long64_t j = 0; j < t->GetEntries(); ++j) {
            t->GetEntry(j);
             weightD = (Double_t) weightF;
            if (treeName.CompareTo("nn_tree") == 0) { // because we are loading float variable and need to cast it to double
                valD = (Double_t) valF;
            }
            

         // Change sign in ele-QCD-Lepemv from negative to positiv
            if ((varName.CompareTo("Lepemv") == 0) && (i == 1)  && (lepton.CompareTo("ele") == 0)) {
       std::cout<<"varName: "  << varName <<", "<< varName.CompareTo("Lepemv")
                <<", channel: "<< i 
                <<", lepton: " << lepton << ", "<< lepton.CompareTo("ele") <<std::endl;
               valD = -valD;
            }

            // setting all datapoints to the histogram interval
            if (valD > intervalEnd) {
                valD = intervalEnd;
            } else if (valD < intervalStart) {
                valD = intervalStart;
            }

            
            hist->Fill(valD,weightD);
            lines[i]++; // just for validating allHists[i]->Integral()
            weights[i] += weightD; // just for validating allHists[i]->Integral()
        }
        //hist->Rebin(1);
        allHists[i]=hist; // allHists[0] ... histogram of data.
        //allHists[i]->ComputeIntegral();
        allHists[i]->SetDirectory(gROOT);
        
        // CREATING STACK
        
        if (i > 0) { // We stack all but Data
            std::cout<< "Adding " << Type_l[i] << " to stack."<<std::endl;
            hStack->Add(((TH1F*) allHists[i]));
        } else {
            std::cout << "Adding " << Type_l[i] << "." << std::endl;
        }
        f->Close();
    }
    
    
    gROOT->SetStyle("Plain");
    //gStyle->SetOptTitle(0); // Wouldn't print out TITLE
    gStyle->SetOptStat(0); // 0==Doesn't print stats like MEAN, RMS.

    // COLORS for the Stack.
    setColorsForHistograms(allHists);

   // Compute Statistical tests

   TH1F *hist_data = (TH1F*)allHists[0]->Clone();
   TH1F *hist_MC   = (TH1F*)allHists[1]->Clone();
   for (Int_t i = 2; i < nTypes - 1; i++) { 
     hist_MC->Add(allHists[i]);
      //std::cout << "Control of runnig: " << i << std::endl;
    }
    Double_t KS_value_prob = hist_MC->KolmogorovTest(hist_data);
    Double_t KS_value_dist = hist_MC->KolmogorovTest(hist_data, "M");
    std::cout << "KS value probability: " << KS_value_prob << ", KS statistics (distance): " << KS_value_dist << std::endl;

    TH1F *hist_data_rebin    = (TH1F*)hist_data->Rebin(5,"hist_data_rebin");
    TH1F *hist_MC_rebin      = (TH1F*)hist_MC->Rebin(5,"hist_MC_rebin");
    Double_t Chi2Test_pvalue = hist_MC_rebin->Chi2Test(hist_data_rebin, "UU");
    Double_t Chi2Test_Chi2   = hist_MC_rebin->Chi2Test(hist_data_rebin, "CHI2");
    std::cout << "Chi2 test p-value: " << Chi2Test_pvalue << ", Chi2 test statistics " << Chi2Test_Chi2 << std::endl;


    // PRINT 
    TCanvas *canvas    = new TCanvas("canvas", "stacked hists", 0, 0, 1200, 800);
//     canvas->Divide(2,1); // Number of subplots in canvas
//     canvas->cd(1);
//     canvas->cd(2);  // cd to specific subplot 
     TPad*    LeftUpperPad  = new TPad("LeftUpperPad",  "LeftUpperPad", .005, .2,    .8,    .995);
     TPad*    LeftLowerPad  = new TPad("LeftLowerPad",  "LeftLowerPad", .005, .01,   .8,    .23);
     TPad*    RightUpperPad = new TPad("RightUpperPad", "RightUpperPad",.75,  .22,   .995,  .99);
     TPad*    RightLowerPad = new TPad("RightLowerPad", "RightLowerPad",.75,  .01,  .995,  .22);
     LeftUpperPad->Draw();
     LeftLowerPad->Draw();
     RightUpperPad->Draw();
     RightLowerPad->Draw();

    // DRAW MAIN Histograms
    LeftUpperPad->cd();
    allHists[0]->Draw("PE1X0"); // first plot data to have propper scale of canvas
    hStack->Draw("same"); // plot MC in stack
    allHists[0]->Draw("samePE1X0"); // plot data to cover the stack
    
    // DRAW LEGEND
    RightUpperPad->cd();
    TLegend *legend = new TLegend(0, 0, 1, 0.9, "legend title"); //x1, y1, x2, y2
    legend->SetFillColor(42);
    legend->SetTextSize(0.05);
  //  legend->AddEntry(allHists[0], Form("%10.f %10i Data", allHists[0]->Integral(), lines[0] )); // data legend line
    legend->AddEntry(allHists[0], Form("%10.f Data", allHists[0]->Integral() )); // data legend line
    // Stack  legend lines
    for (Int_t i = 0; i < nTypes - 1; i++) { 
        Int_t histLine = nTypes - 1 - i;
        TString label = Type_l[histLine];
        //legend->AddEntry(allHists[histLine], Form("%2.2f %2.2f   %s %1.0i",
        //TString legendLine = Form("%10.2f %10i \t %s", allHists[histLine]->Integral(), lines[histLine], label.Data());
        TString legendLine = Form("%10.2f \t %s", allHists[histLine]->Integral(), label.Data());
        legend->AddEntry(allHists[histLine], legendLine);
    }
    legend->Draw();

    // DRAW division of histograms

    TH1F *hist_div  = (TH1F*)hist_data->Clone();
    //hist_div->SetName("Hist_div");
    hist_div->Sumw2();
    hist_div->Divide(hist_MC);
    LeftLowerPad->cd();
    //LeftLowerPad->SetGrid(0,1);
    hist_div->SetMarkerSize(1);
    hist_div->SetMarkerStyle(21);
    hist_div->SetMarkerColor(kBlack);
    //hist_div->GetYaxis()->SetTitle("Data/MC(sum)");
    //hist_div->GetXaxis()->SetTitle("");
    hist_div->SetTitle("");
    hist_div->GetYaxis()->SetTitle();
    hist_div->GetXaxis()->SetTitleOffset(5.5);
    hist_div->GetXaxis()->SetLabelSize(0.1);
    hist_div->GetXaxis()->SetLabelOffset(0.05);
    hist_div->GetXaxis()->SetTickLength(0.15);
    hist_div->GetXaxis()->SetNdivisions(506); //405
    hist_div->GetYaxis()->SetLabelSize(0.1);
    hist_div->GetYaxis()->SetLabelOffset(0.011);
    hist_div->GetYaxis()->SetTickLength(0.015);
    hist_div->GetYaxis()->SetNdivisions(506);
    //double maxYJ =hist_div->GetBinContent(hist_div->GetMaximumBin());
    //double minYJ = hist_div->GetBinContent(hist_div->GetMinimumBin());
    //if (minYJ < 0.6) minYJ = 0.6;
    //if (maxYJ > 2.0) maxYJ = 2.0;
    double minYJ = -1;
    double maxYJ =  3;
    hist_div->GetYaxis()->SetRangeUser(minYJ, maxYJ);
    //hist_div->GetYaxis()->SetRangeUser(0.0, 3.0);

    hist_div->Draw("PE1X0");
    TLine* line_J = new TLine();
    line_J->DrawLine(hist_div->GetXaxis()->GetXmin(),1,hist_div->GetXaxis()->GetXmax(),1);
    hist_div->Draw("PE1X0same");




    // Draw STATISTICS - HYPOTHESIS TESTING
    RightLowerPad->cd();
    //RightLowerPad->GetFrame()->SetFillColor(18);
   TPaveText *pave = new TPaveText(0, 0, 1, .99);
   pave->SetFillColor(42);
   pave->SetShadowColor(kWhite);
   TText *t1=pave->AddText("Statistical tests");
   t1->SetTextColor(4);
   t1->SetTextSize(0.12);
   pave->AddText(TString::Format("KS test p-value: %.04f", KS_value_prob));
   pave->AddText(TString::Format("KS test statistic: %.04f", KS_value_dist));
   pave->AddText(TString::Format("Chi2 test p-value: %.04f", Chi2Test_pvalue));
   pave->AddText(TString::Format("Chi2 test statistic: %.04f", Chi2Test_Chi2));
   TText *t2=pave->AddText(TString::Format("Number of bins: %i", nBin));
   TText *t3=pave->AddText("Chi2 statistic is based on nBin/5 intervals");
   t2->SetTextSize(0.07);
   t3->SetTextSize(0.06);
   pave->Draw();
   canvas->Update();

    // Difference between MC and data 
    Double_t sumW = 0; 
    sumW = 0;
    for (int bin = 1; bin < nBin + 1; bin++) {
        sumW += allHists[0]->GetArray()[bin];
        for (int chan = 1; chan < nTypes; chan++) { // over channels
            sumW -= allHists[chan]->GetArray()[bin];
        }
    }
    std::cout << "Data - MC: " << sumW << std::endl;

    // SAVING PLOTS
    
   // if nput is main directory
  // TString outputFileName = Form("%s/%s/%s/%s", outputC, lepton.Data(), jetbin.Data(), niceName.Data());
   TString outputFileName = Form("%s/%s_%s_%s", outputC, lepton.Data(), jetbin.Data(), niceName.Data());
   writeCanvasFiles(canvas, outputFileName.Data());

    return;
}

#ifndef __CINT__


int main(int argc, char** argv) {

    float a, b;
    int nbin;
    
    if (argc == 7 ) {
    // ControlPlotsVar yield ele 4jet nn_tree ./ Apla 25 0 0.5 transform
    ControlPlotsVar(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], 25, 0, 1);
    } else if (argc == 8 ) {
        sscanf(argv[7], "%i", &nbin);
        ControlPlotsVar(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], nbin, 0, 1);
    } else if (argc == 10) {
        sscanf(argv[7], "%i", &nbin);
        sscanf(argv[8], "%f", &a);
        sscanf(argv[9], "%f", &b);
        ControlPlotsVar(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], nbin, (double) a, (double) b);
    }else {
        std::cout << "Usage: " << argv[0] << " yield ele 4jet nn_tree ./ Apla 25 0 0.5 0" << std::endl;
        exit(1);
    }
    return 0;
}
#endif
 
