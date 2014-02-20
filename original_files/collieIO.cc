
#include "/prj_root/2671/top_write/savitsky/collie/xsec_ljets/input_libs.h"

using namespace std;

void writeCanvasFiles(TCanvas *canv, const char *pre, const char *post) {
  char file[512];
  sprintf(file,"%s%s.eps",pre, post);
  canv->Print(file);
  //sprintf(file,"%s%s.root",pre, post);
  //canv->Print(file);
  sprintf(file,"%s%s.png",pre, post);
  canv->Print(file);
  return;
}

double average_calculator(TH1D * hist, TH1D * central_hist, Int_t start_bin) {
 Int_t nbins = hist->GetNbinsX();
 Double_t hist_integral = hist->Integral();
 Double_t sum = 0.0;
 for (Int_t i=start_bin; i<=nbins; i++) {
	if(central_hist->GetBinContent(i) != 0) sum = sum + (hist->GetBinContent(i)/central_hist->GetBinContent(i) - 1.0)*(hist->GetBinContent(i)/hist_integral);
 }
 return sum;
}

double average_calculator_for_mcstat(TH1D * hist, TH1D * central_hist, Int_t start_bin) {
 Int_t nbins = hist->GetNbinsX();
 Double_t hist_integral = hist->Integral();
 Double_t sum = 0.0;
 for (Int_t i=start_bin; i<=nbins; i++) {
	if(central_hist->GetBinContent(i) != 0) sum = sum + (hist->GetBinContent(i)/central_hist->GetBinContent(i) - 1.0)*(hist->GetBinContent(i)/(hist_integral*central_hist->GetEntries()));
 }
 return sum;
}

void Usage() {
     printf("./collieIO.exe => Arguments order: <treeType> <run_period> <cut_set> <syst_setup> <channel> <tag_bin> <working_point> <mva: 'max' or 'max_next'> <jet_bin> \n");
}

int main(int argc, char* argv[]) {

   gROOT->Reset();
   if(argc != 10) { Usage();
		   exit(1);
		 }

   TString treeType = argv[1];
   TString runPeriod = argv[2];
   TString cutSet = argv[3];
   TString sysSet = argv[4];
   TString lepType = argv[5];
   TString tagBin =  argv[6];
   TString WP = argv[7];
   TString max_next_string = argv[8];//max_next, max
   TString jetBin = argv[9];

  string channel = Form("%s_%s_%s_%s", lepType.Data(), tagBin.Data(), max_next_string.Data(), jetBin.Data());
  string period_folder = Form("%s", runPeriod.Data());

  bool account4syst = true;

///Create IO file with input parameters

  CollieIOFile* cfile = new CollieIOFile();
  if (account4syst) cfile->initFile("rootfiles_IO/"+period_folder+"/xsec_"+channel+"_syst.root", channel);
  else cfile->initFile("rootfiles_IO/"+period_folder+"/xsec_"+channel+"_stat.root", channel);
  //cfile->setNoviceFlag(false);

  int rebin_factor = 1;//for stacking histograms
  int top_mass_point = 172;

  double Xmin = 0.0; 
  double Xmax = 1.0;
  int Nbins = 20;
  Int_t start_bin_for_syst = 0; //beforeTag = 1; inc1Tag = 4;
  if(tagBin.Sizeof() == 8 && max_next_string.Sizeof() == 4) start_bin_for_syst = 4;
  if(tagBin.Sizeof() == 8 && max_next_string.Sizeof() == 9) start_bin_for_syst = 1;
  if(tagBin.Sizeof() == 10) start_bin_for_syst = 1;

  cfile->setInputHist(Xmin,Xmax,Nbins);
  //cfile->setCutoffs(0.0,1.0);//Option to define physical cutoffs where events should not exist (in terms of your historam range)
  cfile->setRebin(1);// Option to rebin histograms to a coarser binning
  //cfile->setSmooth(false);  // Option to smooth histograms, don't smooth unless you know what you're doing
  //cfile->setHistNorm(50000); // Average number of histogram entries for smoothing purposes (default=50k), the value doesn't matter if you're not smoothing.

///Definition of background types
  vector<string> bckg_names;

  Int_t Ntypes = 18;

  TString Type_l[18];

  TString Type_mu[18] = {"Data","QCD","WJet0","WJetC","WJetB","ZbbMuMu","ZbbTauTau","ZccMuMu","ZccTauTau","ZlpMuMu","ZlpTauTau","tb","tbq","WW","WZ","ZZ","ttll","TTbar"};// for read out from my histominitrees

  TString Type_e[18] = {"Data","QCD","WJet0","WJetC","WJetB","ZbbEE","ZbbTauTau","ZccEE","ZccTauTau","ZlpEE","ZlpTauTau","tb","tbq","WW","WZ","ZZ","ttll","TTbar"};// for read out from my histominitrees

  TString TypeLeg[18] = {"Data","QCD","Wlp","Wcc","Wbb","ZbbLL","ZbbTauTau","ZccLL","ZccTauTau","ZlpLL","ZlpTauTau","tb","tbq","WW","WZ","ZZ","ttll","t#bar{t}(ljet)"};// for proper names in collie root files

  TString TypePlotLeg[18];

  TString TypePlotLeg_mu[18] = {"Data","QCD","Wlp","Wcc","Wbb","ZbbMuMu","ZbbTauTau","ZccMuMu","ZccTauTau","ZlpMuMu","ZlpTauTau","tb","tbq","WW","WZ","ZZ","ttbar (dilepton)","ttbar (l+jets)"};// for nice names in legends:)

  TString TypePlotLeg_e[18] = {"Data","QCD","Wlp","Wcc","Wbb","ZbbEE","ZbbTauTau","ZccEE","ZccTauTau","ZlpEE","ZlpTauTau","tb","tbq","WW","WZ","ZZ","ttbar (dilepton)","ttbar (l+jets)"};// for nice names in legends:)

  for (Int_t i = 1; i <(Ntypes-1); i++) {
      bckg_names.push_back(TypeLeg[i].Data()) ;//!!
  }

  cfile->createChannel(bckg_names);

///Redout of input histograms

TH1D** allInputComb = new TH1D*[19];

// Int_t addHistoID[14] = {2,5,6,7,8,9,10,11,12,13,14,15}; //excludes ttbar dilepton in allOther
// Int_t nAdd = 12;

Int_t addHistoID[14] = {2,5,6,7,8,9,10,11,12,13,14,15,16}; //includes ttbar dilepton in allOther
Int_t nAdd = 13;

Bool_t addDileptonToSignal = kFALSE;

  // add dilepton to signal
  if(addDileptonToSignal)nAdd=12;

  TString PATH = Form("/prj_root/2671/top_write/savitsky/for_%s/final_tt13_dIbad/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  THStack *hstCom=new THStack("testStackCom", "sCom");//for drawing

    for (Int_t i =0; i <Ntypes; i++) {

     if (lepType.Sizeof() == 7) {//mujets - 7, ejets - 6
      Type_l[i] = Type_mu[i];
      TypePlotLeg[i] = TypePlotLeg_mu[i];
     } else if (lepType.Sizeof() == 6) {
      Type_l[i] = Type_e[i];
      TypePlotLeg[i] = TypePlotLeg_e[i];
     } else {
      std::cout<<"Wrong channel name: 'mujets' or 'ejets' only!"<<std::endl;
     }

     TString FileName = Form("%s/ff_myhist_%s.root", PATH.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     allInputComb[i] = hist;
     allInputComb[i] -> Rebin(rebin_factor);
     //allInputComb[i]->Sumw2();
     allInputComb[i] ->SetDirectory(gROOT);
     if(i>0)hstCom->Add(((TH1D*) allInputComb[i]));
     f->Close();
    }

   TH1D *mc0Comb = (TH1D*)(allInputComb[1])->Clone("mc0Comb");
   TH1D *mc2Comb = (TH1D*)(allInputComb[3])->Clone("mc2Comb");
   mc2Comb->Add( (TH1D*) allInputComb[4]);

   for (Int_t i =0; i <nAdd; i++) {
     int id = (int) addHistoID[i];
     mc0Comb->Add( ((TH1D*) allInputComb[id]));
   }

// DRAWING part *********************************************************************************************
  gROOT->SetStyle("Plain");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

    Int_t color=8;
    for(Int_t i=11;i<13;i++) {
        allInputComb[i]->SetLineColor(color);
        allInputComb[i]->SetFillColor(color);
        --color;
    }
    color=15;
    for(Int_t i=5;i<11;i++) {
        if(color==10)--color;
        allInputComb[i]->SetLineColor(color);
        allInputComb[i]->SetFillColor(color);
        --color;
    }
    color=29;
    for(Int_t i=13;i<16;i++) {
        allInputComb[i]->SetLineColor(color);
        allInputComb[i]->SetFillColor(color);
        ++color;
    }

    allInputComb[17]->SetLineColor(2);
    allInputComb[17]->SetFillColor(2);
    allInputComb[16]->SetLineColor(6);
    allInputComb[16]->SetFillColor(6);
    allInputComb[2]->SetLineColor(3);
    allInputComb[2]->SetFillColor(3);
    allInputComb[3]->SetLineColor(8);
    allInputComb[3]->SetFillColor(8);
    allInputComb[4]->SetLineColor(kGreen+1);
    allInputComb[4]->SetFillColor(kGreen+1);
    allInputComb[1]->SetLineColor(28);
    allInputComb[1]->SetFillColor(28);

    allInputComb[0]->SetMarkerStyle(20);
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  TCanvas *c2 = new TCanvas("c2", "Unfolded Cross Sections", 0,0,2800,1800);
  c2->Divide(1,1);
  c2->cd(1);
    Double_t maxJ=allInputComb[0]->GetMaximum();
    if(maxJ<1.0) maxJ=2.0;
    if(hstCom->GetMaximum() > maxJ) maxJ=hstCom->GetMaximum();
    hstCom->SetMaximum(maxJ*1.2);
    hstCom->SetMinimum(0.8*allInputComb[1]->GetBinContent(12));
    if(0.8*allInputComb[1]->GetBinContent(12) < 1.0) hstCom->SetMinimum(1.0);

    TPad *pad1J = new TPad("pad1J","histos",0.,0.25,1.,1.);
    pad1J->Draw();
    TPad *pad2J = new TPad("pad2J","ratio",0.,0.,1.,0.25);
    pad2J->Draw();
    pad1J->cd()->SetLogy();
    pad1J->SetTopMargin(0.08);
    pad1J->SetBottomMargin(0.0);
    pad2J->SetTopMargin(0.0);
    pad2J->SetBottomMargin(0.1);

    gPad->SetLeftMargin(0.045);
    gPad->SetRightMargin(0.25);

    allInputComb[0]->SetMarkerSize(5.1);
    allInputComb[0]->SetLineWidth(1);
    hstCom->Draw("HIST");
    hstCom-> GetYaxis()->SetLabelSize(0.05);
    hstCom-> GetYaxis()->SetTickLength(0.03);
    hstCom-> GetXaxis()->SetLabelSize(0.05);
    hstCom-> GetXaxis()->SetTickLength(0.03);
    hstCom-> GetXaxis()->SetNdivisions(405);
    allInputComb[0]->Draw("samePE1X0");

    TLegend *legJ=new TLegend(0.76, 0.0, 0.95, 0.9);
    legJ->SetLineColor(0);
    legJ->SetFillColor(0);
    legJ->SetTextSize(0.04);
    legJ->SetLineWidth(0);
    legJ->AddEntry(allInputComb[0], Form("%1.0f Data",allInputComb[0]->Integral()), "PL");

    for(Int_t i=0;i<17;i++) {
        TString label = TypePlotLeg[17-i];
        legJ->AddEntry(allInputComb[17-i], Form("%2.2f %s",allInputComb[17-i]->Integral(),label.Data()), "F");
    }
   
    legJ->AddEntry(allInputComb[0], Form("%2.2f Wcc+Wbb",mc2Comb->Integral()), "");
    legJ->AddEntry(allInputComb[0], Form("%2.2f otherMC",mc0Comb->Integral()), "");
    legJ->Draw("same");

    TLatex *tex0 = new TLatex();
    tex0->SetNDC();
    tex0->SetTextSize(0.06);
    char buf0[265];
    TString max_next_label;
    if (max_next_string.Sizeof() == 4) max_next_label = "mva_max";
    if (max_next_string.Sizeof() == 9) max_next_label = "mva_next";
    sprintf(buf0, "Prefit: %s %s %s %s %s %s %s %s\n", runPeriod.Data(), cutSet.Data(), sysSet.Data(), lepType.Data(), tagBin.Data(), WP.Data(), max_next_label.Data(), jetBin.Data());
    tex0->DrawLatex(0.022,0.945,buf0);

// calculate a ratio data/mc and draw it
    pad2J->cd();
    gROOT->ForceStyle();
    gROOT->SetStyle("Plain");
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gPad->SetLeftMargin(0.045);
    gPad->SetRightMargin(0.25);
    pad2J->SetBottomMargin(0.2);

    TH1D* dummy_dataComb = (TH1D*) allInputComb[0]->Clone("dummy_dataComb");dummy_dataComb->SetTitle("");
    TH1D* dummy_mcComb = (TH1D*) hstCom->GetStack()->Last();
    //dummy_dataComb->Sumw2();
    //dummy_mcComb->Sumw2();

    dummy_dataComb->Divide(dummy_mcComb);
    //dummy_dataComb->Sumw2();
    dummy_dataComb->SetMarkerSize(5.1);
    dummy_dataComb->SetMarkerColor(2);
    dummy_dataComb->GetYaxis()->SetTitle("Data/MC(sum)");
    dummy_dataComb->GetXaxis()->SetTitleOffset(5.5);
    dummy_dataComb->GetXaxis()->SetLabelSize(0.16);
    dummy_dataComb-> GetXaxis()->SetLabelOffset(0.05);
    dummy_dataComb-> GetXaxis()->SetTickLength(0.15);
    dummy_dataComb->GetXaxis()->SetNdivisions(405);
    dummy_dataComb-> GetYaxis()->SetLabelSize(0.16);
    dummy_dataComb-> GetYaxis()->SetLabelOffset(0.011);
    dummy_dataComb-> GetYaxis()->SetTickLength(0.015);
    dummy_dataComb->GetYaxis()->SetNdivisions(203);
    double minYJ =0;
    double maxYJ =dummy_dataComb->GetBinContent(dummy_dataComb->GetMaximumBin());
    if(dummy_dataComb->GetBinContent(dummy_dataComb->GetMinimumBin())==0)minYJ = 0.6;
    minYJ = dummy_dataComb->GetBinContent(dummy_dataComb->GetMinimumBin());
    if (minYJ < 0.6) minYJ = 0.6;
    if (maxYJ > 2.)maxYJ=2.;
    dummy_dataComb->GetYaxis()->SetRangeUser(0.75*minYJ, 1.25*maxYJ);
    dummy_dataComb->Draw("PE1X0");
    TLine* line_J = new TLine();
    line_J->DrawLine(dummy_dataComb->GetXaxis()->GetXmin(),1,dummy_dataComb->GetXaxis()->GetXmax(),1);
    dummy_dataComb->Draw("PE1X0same");

    TLegend *legJrat=new TLegend(0.765, 0.5, 0.9, 0.9);
    legJrat->SetLineColor(0);
    legJrat->SetFillColor(0);
    legJrat->SetTextSize(0.16);
    legJrat->SetLineWidth(0);
    legJrat->AddEntry(dummy_dataComb, Form("Data/MC - Prefit"), "PL");
    legJrat->Draw("same");

    TLatex *tex1 = new TLatex();
    tex1->SetNDC();
    tex1->SetTextSize(0.2);
    char buf1[265];
    sprintf(buf1, "mva output");
    tex1->DrawLatex(0.765,0.21,buf1);

    TString add = Form("prefit_%s_%s_%s_%s_%s_%s_%s_%s", runPeriod.Data(), cutSet.Data(), sysSet.Data(), lepType.Data(), tagBin.Data(), WP.Data(), max_next_label.Data(), jetBin.Data());
    TString period_prefit_folder = Form("/prj_root/2671/top_write/savitsky/collie/xsec_ljets/xsec_output/controlplots/prefit/%s/", runPeriod.Data());
    writeCanvasFiles(c2, period_prefit_folder.Data(), add.Data());

  ///Definition of input histograms to Collie
  //Each parameter point has a signal histo, data histo, and an array of backgrounds...
  //  Smoothing parameters are also passed in.

  TH1D* data = (TH1D*) allInputComb[0]->Clone("data");
  data->SetDirectory(0);
  TH1D* signal = (TH1D*) allInputComb[17]->Clone("signal");
  signal->SetDirectory(0);

  //Backgrounds are passed in via vector
  vector<TH1D*> vbkgd;

  //Alpha parameters only matter when smoothing is utilized
  //  Input values don't matter if you're not smoothing.
  //  Don't smooth unless you know what you're doing.
  vector<double> valpha;

for (Int_t i = 1; i <(Ntypes-1); i++) {
     TH1D* h_bkg = (TH1D*) allInputComb[i]->Clone("h_bkg");
     h_bkg->SetDirectory(gROOT);
     vbkgd.push_back(h_bkg);
     valpha.push_back(-1);
    }

  cfile->createMassPoint(top_mass_point, data, signal, -1, vbkgd,valpha);

  // Add systematics...either flat or by shape (ie, function of final variable)
  //   if by shape, must supply a histogram of the values in percent(%) fluctuations...!!!FALSE
  //   Signal requires no index, but backgrounds must be specifically indexed (0->N bkgds)
  //   Read the instructions in collie/io/include/CollieIOFile.hh if you're in doubt

  if (!account4syst) {
    ///store and output channel information
    cfile->storeFile();
    exit(0);
  }

  ///=====Systematics Section=====
  ///Flat systematics

  //W flat syst
  cfile->createFlatBkgdSystematic(1,"Wlp_sys",0.5,0.5,top_mass_point);
  cfile->createFlatBkgdSystematic(2,"Whf_sys",0.08,0.08,top_mass_point);
  cfile->createFlatBkgdSystematic(3,"Whf_sys",0.08,0.08,top_mass_point);

  //Lumi flat syst (signal included)
  cfile->createFlatSigSystematic("Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(0,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(1,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(2,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(3,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(4,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(5,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(6,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(7,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(8,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(9,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(10,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(11,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(12,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(13,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(14,"Lumi_sys",0.061,0.061,top_mass_point);
  cfile->createFlatBkgdSystematic(15,"Lumi_sys",0.061,0.061,top_mass_point);

  //MC bkgd xsec flat syst (include Z 4-9)
  cfile->createFlatBkgdSystematic(4,"MCbkgdxsec_sys",0.25,0.25,top_mass_point);
  cfile->createFlatBkgdSystematic(5,"MCbkgdxsec_sys",0.25,0.25,top_mass_point);
  cfile->createFlatBkgdSystematic(6,"MCbkgdxsec_sys",0.25,0.25,top_mass_point);
  cfile->createFlatBkgdSystematic(7,"MCbkgdxsec_sys",0.25,0.25,top_mass_point);
  cfile->createFlatBkgdSystematic(8,"MCbkgdxsec_sys",0.25,0.25,top_mass_point);
  cfile->createFlatBkgdSystematic(9,"MCbkgdxsec_sys",0.25,0.25,top_mass_point);
  cfile->createFlatBkgdSystematic(10,"MCbkgdxsec_sys",0.126,0.126,top_mass_point);
  cfile->createFlatBkgdSystematic(11,"MCbkgdxsec_sys",0.126,0.126,top_mass_point);
  cfile->createFlatBkgdSystematic(12,"MCbkgdxsec_sys",0.07,0.07,top_mass_point);
  cfile->createFlatBkgdSystematic(13,"MCbkgdxsec_sys",0.07,0.07,top_mass_point);
  cfile->createFlatBkgdSystematic(14,"MCbkgdxsec_sys",0.07,0.07,top_mass_point);

  //Trigger efficiency flat syst (signal included)
  cfile->createFlatSigSystematic("TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(0,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(1,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(2,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(3,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(4,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(5,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(6,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(7,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(8,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(9,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(10,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(11,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(12,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(13,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(14,"TriggEff_sys",0.05,0.05,top_mass_point);
  cfile->createFlatBkgdSystematic(15,"TriggEff_sys",0.05,0.05,top_mass_point);

  //epsilon_QCD flat syst
  if (lepType.Sizeof() == 7) cfile->createFlatBkgdSystematic(0,"epsQCDsig_sys",0.086,0.086,top_mass_point);//mujets - 7
  if (lepType.Sizeof() == 6) cfile->createFlatBkgdSystematic(0,"epsQCDsig_sys",0.023,0.023,top_mass_point);//ejets - 6

  //MC signal and background BR (signal included)
  cfile->createFlatSigSystematic("MCBR_sys",0.008,0.008,top_mass_point);//ttbar->ljets
  cfile->createFlatBkgdSystematic(15,"MCBR_sys",0.017,0.017,top_mass_point);//ttbar->dilepton

  //MC bkgd sf syst
  /*cfile->createFlatBkgdSystematic(4,"MCbkgdsf_sys",0.2,0.2,top_mass_point);
  cfile->createFlatBkgdSystematic(5,"MCbkgdsf_sys",0.2,0.2,top_mass_point);
  cfile->createFlatBkgdSystematic(6,"MCbkgdsf_sys",0.2,0.2,top_mass_point);
  cfile->createFlatBkgdSystematic(7,"MCbkgdsf_sys",0.2,0.2,top_mass_point);
  cfile->createFlatBkgdSystematic(8,"MCbkgdsf_sys",0.2,0.2,top_mass_point);
  cfile->createFlatBkgdSystematic(9,"MCbkgdsf_sys",0.2,0.2,top_mass_point);*/

  ///Shape dependent systematics

  //bTag_shape_systematics

  TH1D** btagUPInputComb = new TH1D*[19];

  TString PATH_btagUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_btagUP/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t btagUP_9b = 0.0;
  Double_t btagUP_10b = 0.0;
  Double_t btagUP_8b_ratio = 0.0;
  Double_t btagUP_9b_ratio = 0.0;
  Double_t btagUP_10b_ratio = 0.0;
  Double_t btagUP_11b_ratio = 0.0;
  Double_t btagUP_step = 0.0;

  Double_t btagUP_avg_histo = 0.0;
  Double_t btagUP_avg_sample_stat = 0.0;
  Double_t btagUP_avg = 0.0;
  Double_t btagUP_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString FileName = Form("%s/ff_myhist_%s.root", PATH_btagUP.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     btagUPInputComb[i] = hist;
     btagUPInputComb[i] -> Rebin(rebin_factor);
     btagUPInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(allInputComb[i]->GetBinContent(8)!=0) btagUP_8b_ratio = btagUPInputComb[i]->GetBinContent(8)/allInputComb[i]->GetBinContent(8);
     if(allInputComb[i]->GetBinContent(11)!=0) btagUP_11b_ratio = btagUPInputComb[i]->GetBinContent(11)/allInputComb[i]->GetBinContent(11);
     btagUP_step = (btagUP_11b_ratio - btagUP_8b_ratio)/3;
     btagUP_9b = btagUPInputComb[i]->GetBinContent(9);
     btagUP_10b = btagUPInputComb[i]->GetBinContent(10);
     btagUPInputComb[i]->SetBinContent(9, btagUP_9b*(btagUP_8b_ratio + btagUP_step));
     btagUPInputComb[i]->SetBinContent(10, btagUP_10b*(btagUP_8b_ratio + 2*btagUP_step));

     if(allInputComb[i]->GetBinContent(9)!=0) btagUP_9b_ratio = btagUPInputComb[i]->GetBinContent(9)/allInputComb[i]->GetBinContent(9);
     if(allInputComb[i]->GetBinContent(10)!=0) btagUP_10b_ratio = btagUPInputComb[i]->GetBinContent(10)/allInputComb[i]->GetBinContent(10);
     if(btagUP_9b_ratio < 1.0 || btagUP_10b_ratio < 1.0) {
		btagUPInputComb[i]->SetBinContent(9, (allInputComb[i]->GetBinContent(9))*(btagUP_11b_ratio - 2*btagUP_step));
		btagUPInputComb[i]->SetBinContent(10, (allInputComb[i]->GetBinContent(10))*(btagUP_11b_ratio - btagUP_step));
     }
     if(i!=0 && i!=1 && i!=17) {
     btagUP_avg_histo = average_calculator(btagUPInputComb[i], allInputComb[i], start_bin_for_syst);
     if(btagUPInputComb[i]->Integral()!=0) btagUP_avg = btagUP_avg + btagUP_avg_histo*btagUPInputComb[i]->Integral();
     if(btagUPInputComb[i]->Integral()!=0) btagUP_avg_sample_stat = btagUP_avg_sample_stat + btagUPInputComb[i]->Integral();
     }
     if(i==17) btagUP_avg_sig = average_calculator(btagUPInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    btagUP_avg = btagUP_avg/btagUP_avg_sample_stat;

  TH1D** btagDOWNInputComb = new TH1D*[19];

  TString PATH_btagDOWN = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_btagDOWN/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t btagDOWN_9b = 0.0;
  Double_t btagDOWN_10b = 0.0;
  Double_t btagDOWN_8b_ratio = 0.0;
  Double_t btagDOWN_9b_ratio = 0.0;
  Double_t btagDOWN_10b_ratio = 0.0;
  Double_t btagDOWN_11b_ratio = 0.0;
  Double_t btagDOWN_step = 0.0;

  Double_t btagDOWN_avg_histo = 0.0;
  Double_t btagDOWN_avg_sample_stat = 0.0;
  Double_t btagDOWN_avg = 0.0;
  Double_t btagDOWN_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString FileName = Form("%s/ff_myhist_%s.root", PATH_btagDOWN.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     btagDOWNInputComb[i] = hist;
     btagDOWNInputComb[i] -> Rebin(rebin_factor);
     btagDOWNInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(allInputComb[i]->GetBinContent(8)!=0) btagDOWN_8b_ratio = btagDOWNInputComb[i]->GetBinContent(8)/allInputComb[i]->GetBinContent(8);
     if(allInputComb[i]->GetBinContent(11)!=0) btagDOWN_11b_ratio = btagDOWNInputComb[i]->GetBinContent(11)/allInputComb[i]->GetBinContent(11);
     btagDOWN_step = (btagDOWN_11b_ratio - btagDOWN_8b_ratio)/3;
     btagDOWN_9b = btagDOWNInputComb[i]->GetBinContent(9);
     btagDOWN_10b = btagDOWNInputComb[i]->GetBinContent(10);
     btagDOWNInputComb[i]->SetBinContent(9, btagDOWN_9b*(btagDOWN_8b_ratio + btagDOWN_step));
     btagDOWNInputComb[i]->SetBinContent(10, btagDOWN_10b*(btagDOWN_8b_ratio + 2*btagDOWN_step));

     if(allInputComb[i]->GetBinContent(9)!=0) btagDOWN_9b_ratio = btagDOWNInputComb[i]->GetBinContent(9)/allInputComb[i]->GetBinContent(9);
     if(allInputComb[i]->GetBinContent(10)!=0) btagDOWN_10b_ratio = btagDOWNInputComb[i]->GetBinContent(10)/allInputComb[i]->GetBinContent(10);
     if(btagDOWN_9b_ratio > 1.0 || btagDOWN_10b_ratio > 1.0) {
		btagDOWNInputComb[i]->SetBinContent(9, (allInputComb[i]->GetBinContent(9))*(btagDOWN_11b_ratio - 2*btagDOWN_step));
		btagDOWNInputComb[i]->SetBinContent(10, (allInputComb[i]->GetBinContent(10))*(btagDOWN_11b_ratio - btagDOWN_step));
     }
     if(i!=0 && i!=1 && i!=17) {
     btagDOWN_avg_histo = average_calculator(btagDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
     if(btagDOWNInputComb[i]->Integral()!=0) btagDOWN_avg = btagDOWN_avg + btagDOWN_avg_histo*btagDOWNInputComb[i]->Integral();
     if(btagDOWNInputComb[i]->Integral()!=0) btagDOWN_avg_sample_stat = btagDOWN_avg_sample_stat + btagDOWNInputComb[i]->Integral();
     }
     if(i==17) btagDOWN_avg_sig = average_calculator(btagDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    btagDOWN_avg = btagDOWN_avg/btagDOWN_avg_sample_stat;

  // signal systematics
	cfile->createShapeSigSystematic("bTag_sys", btagUPInputComb[17], btagDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "bTag_sys" << " for  signal "
	//<< " [" << btagUPInputComb[17]->GetEntries() << "," <<  btagDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics
    for (Int_t i = 2; i <(Ntypes-1); i++) {

     	cfile->createShapeBkgdSystematic(i-1, "bTag_sys", btagUPInputComb[i], btagDOWNInputComb[i], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "bTag_sys" << " for  "  << TypeLeg[i]
	//<< " [" << btagUPInputComb[i]->GetEntries()  << "," <<  btagDOWNInputComb[i]->GetEntries() << "]" << endl ;
    }

  //cTag_shape_systematics

  TH1D** ctagUPInputComb = new TH1D*[19];

  TString PATH_ctagUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_ctagUP/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t ctagUP_avg_histo = 0.0;
  Double_t ctagUP_avg_sample_stat = 0.0;
  Double_t ctagUP_avg = 0.0;
  Double_t ctagUP_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {


     TString FileName = Form("%s/ff_myhist_%s.root", PATH_ctagUP.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     ctagUPInputComb[i] = hist;
     ctagUPInputComb[i] -> Rebin(rebin_factor);
     ctagUPInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     ctagUP_avg_histo = average_calculator(ctagUPInputComb[i], allInputComb[i], start_bin_for_syst);
     if(ctagUPInputComb[i]->Integral()!=0) ctagUP_avg = ctagUP_avg + ctagUP_avg_histo*ctagUPInputComb[i]->Integral();
     if(ctagUPInputComb[i]->Integral()!=0) ctagUP_avg_sample_stat = ctagUP_avg_sample_stat + ctagUPInputComb[i]->Integral();
     }     
     if(i==17) ctagUP_avg_sig = average_calculator(ctagUPInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    ctagUP_avg = ctagUP_avg/ctagUP_avg_sample_stat;

  TH1D** ctagDOWNInputComb = new TH1D*[19];

  TString PATH_ctagDOWN = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_ctagDOWN/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t ctagDOWN_avg_histo = 0.0;
  Double_t ctagDOWN_avg_sample_stat = 0.0;
  Double_t ctagDOWN_avg = 0.0;
  Double_t ctagDOWN_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString FileName = Form("%s/ff_myhist_%s.root", PATH_ctagDOWN.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     ctagDOWNInputComb[i] = hist;
     ctagDOWNInputComb[i] -> Rebin(rebin_factor);
     ctagDOWNInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     ctagDOWN_avg_histo = average_calculator(ctagDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
     if(ctagDOWNInputComb[i]->Integral()!=0) ctagDOWN_avg = ctagDOWN_avg + ctagDOWN_avg_histo*ctagDOWNInputComb[i]->Integral();
     if(ctagDOWNInputComb[i]->Integral()!=0) ctagDOWN_avg_sample_stat = ctagDOWN_avg_sample_stat + ctagDOWNInputComb[i]->Integral();
     }
     if(i==17) ctagDOWN_avg_sig = average_calculator(ctagDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    ctagDOWN_avg = ctagDOWN_avg/ctagDOWN_avg_sample_stat;

  // signal systematics
	cfile->createShapeSigSystematic("cTag_sys", ctagUPInputComb[17], ctagDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "cTag_sys" << " for  signal "
	//<< " [" << ctagUPInputComb[17]->GetEntries() << "," <<  ctagDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics
    for (Int_t i = 2; i <(Ntypes-1); i++) {

     	cfile->createShapeBkgdSystematic(i-1, "cTag_sys", ctagUPInputComb[i], ctagDOWNInputComb[i], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "cTag_sys" << " for  "  << TypeLeg[i]
	//<< " [" << ctagUPInputComb[i]->GetEntries()  << "," <<  ctagDOWNInputComb[i]->GetEntries() << "]" << endl ;
    }

  //lTag_shape_systematics

  TH1D** ltagUPInputComb = new TH1D*[19];

  TString PATH_ltagUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_ltagUP/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t ltagUP_avg_histo = 0.0;
  Double_t ltagUP_avg_sample_stat = 0.0;
  Double_t ltagUP_avg = 0.0;
  Double_t ltagUP_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {


     TString FileName = Form("%s/ff_myhist_%s.root", PATH_ltagUP.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     ltagUPInputComb[i] = hist;
     ltagUPInputComb[i] -> Rebin(rebin_factor);
     ltagUPInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     ltagUP_avg_histo = average_calculator(ltagUPInputComb[i], allInputComb[i], start_bin_for_syst);
     if(ltagUPInputComb[i]->Integral()!=0) ltagUP_avg = ltagUP_avg + ltagUP_avg_histo*ltagUPInputComb[i]->Integral();
     if(ltagUPInputComb[i]->Integral()!=0) ltagUP_avg_sample_stat = ltagUP_avg_sample_stat + ltagUPInputComb[i]->Integral();
     }
     if(i==17) ltagUP_avg_sig = average_calculator(ltagUPInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    ltagUP_avg = ltagUP_avg/ltagUP_avg_sample_stat;

  TH1D** ltagDOWNInputComb = new TH1D*[19];

  TString PATH_ltagDOWN = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_ltagDOWN/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t ltagDOWN_avg_histo = 0.0;
  Double_t ltagDOWN_avg_sample_stat = 0.0;
  Double_t ltagDOWN_avg = 0.0;
  Double_t ltagDOWN_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString FileName = Form("%s/ff_myhist_%s.root", PATH_ltagDOWN.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     ltagDOWNInputComb[i] = hist;
     ltagDOWNInputComb[i] -> Rebin(rebin_factor);
     ltagDOWNInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     ltagDOWN_avg_histo = average_calculator(ltagDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
     if(ltagDOWNInputComb[i]->Integral()!=0) ltagDOWN_avg = ltagDOWN_avg + ltagDOWN_avg_histo*ltagDOWNInputComb[i]->Integral();
     if(ltagDOWNInputComb[i]->Integral()!=0) ltagDOWN_avg_sample_stat = ltagDOWN_avg_sample_stat + ltagDOWNInputComb[i]->Integral();
     }
     if(i==17) ltagDOWN_avg_sig = average_calculator(ltagDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    ltagDOWN_avg = ltagDOWN_avg/ltagDOWN_avg_sample_stat;

  // signal systematics
	cfile->createShapeSigSystematic("lTag_sys", ltagUPInputComb[17], ltagDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "lTag_sys" << " for  signal "
	//<< " [" << ltagUPInputComb[17]->GetEntries() << "," <<  ltagDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics
    for (Int_t i = 2; i <(Ntypes-1); i++) {

     	cfile->createShapeBkgdSystematic(i-1, "lTag_sys", ltagUPInputComb[i], ltagDOWNInputComb[i], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "lTag_sys" << " for  "  << TypeLeg[i]
	//<< " [" << ltagUPInputComb[i]->GetEntries()  << "," <<  ltagDOWNInputComb[i]->GetEntries() << "]" << endl ;
    }

//vc_shape_systematics

  TH1D** vcUPInputComb = new TH1D*[19];

  TString PATH_vcUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_vcUP/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t vcUP_avg_histo = 0.0;
  Double_t vcUP_avg_sample_stat = 0.0;
  Double_t vcUP_avg = 0.0;
  Double_t vcUP_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {


     TString FileName = Form("%s/ff_myhist_%s.root", PATH_vcUP.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     vcUPInputComb[i] = hist;
     vcUPInputComb[i] -> Rebin(rebin_factor);
     vcUPInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     vcUP_avg_histo = average_calculator(vcUPInputComb[i], allInputComb[i], start_bin_for_syst);
     if(vcUPInputComb[i]->Integral()!=0) vcUP_avg = vcUP_avg + vcUP_avg_histo*vcUPInputComb[i]->Integral();
     if(vcUPInputComb[i]->Integral()!=0) vcUP_avg_sample_stat = vcUP_avg_sample_stat + vcUPInputComb[i]->Integral();
     }
     if(i==17) vcUP_avg_sig = average_calculator(vcUPInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    vcUP_avg = vcUP_avg/vcUP_avg_sample_stat;

  TH1D** vcDOWNInputComb = new TH1D*[19];

  TString PATH_vcDOWN = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_vcDOWN/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t vcDOWN_avg_histo = 0.0;
  Double_t vcDOWN_avg_sample_stat = 0.0;
  Double_t vcDOWN_avg = 0.0;
  Double_t vcDOWN_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString FileName = Form("%s/ff_myhist_%s.root", PATH_vcDOWN.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     vcDOWNInputComb[i] = hist;
     vcDOWNInputComb[i] -> Rebin(rebin_factor);
     vcDOWNInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     vcDOWN_avg_histo = average_calculator(vcDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
     if(vcDOWNInputComb[i]->Integral()!=0) vcDOWN_avg = vcDOWN_avg + vcDOWN_avg_histo*vcDOWNInputComb[i]->Integral();
     if(vcDOWNInputComb[i]->Integral()!=0) vcDOWN_avg_sample_stat = vcDOWN_avg_sample_stat + vcDOWNInputComb[i]->Integral();
     }
     if(i==17) vcDOWN_avg_sig = average_calculator(vcDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    vcDOWN_avg = vcDOWN_avg/vcDOWN_avg_sample_stat;

  // signal systematics
	cfile->createShapeSigSystematic("vc_sys", vcUPInputComb[17], vcDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "vc_sys" << " for  signal "
	//<< " [" << vcUPInputComb[17]->GetEntries() << "," <<  vcDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics
    for (Int_t i = 2; i <(Ntypes-1); i++) {

     	cfile->createShapeBkgdSystematic(i-1, "vc_sys", vcUPInputComb[i], vcDOWNInputComb[i], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "vc_sys" << " for  "  << TypeLeg[i]
	//<< " [" << vcUPInputComb[i]->GetEntries()  << "," <<  vcDOWNInputComb[i]->GetEntries() << "]" << endl ;
    }

//lepid_shape_systematics

  TH1D** lepidUPInputComb = new TH1D*[19];

  TString PATH_lepidUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_lepidUP/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t lepidUP_avg_histo = 0.0;
  Double_t lepidUP_avg_sample_stat = 0.0;
  Double_t lepidUP_avg = 0.0;
  Double_t lepidUP_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {


     TString FileName = Form("%s/ff_myhist_%s.root", PATH_lepidUP.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     lepidUPInputComb[i] = hist;
     lepidUPInputComb[i] -> Rebin(rebin_factor);
     lepidUPInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     lepidUP_avg_histo = average_calculator(lepidUPInputComb[i], allInputComb[i], start_bin_for_syst);
     if(lepidUPInputComb[i]->Integral()!=0) lepidUP_avg = lepidUP_avg + lepidUP_avg_histo*lepidUPInputComb[i]->Integral();
     if(lepidUPInputComb[i]->Integral()!=0) lepidUP_avg_sample_stat = lepidUP_avg_sample_stat + lepidUPInputComb[i]->Integral();
     }
     if(i==17) lepidUP_avg_sig = average_calculator(lepidUPInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    lepidUP_avg = lepidUP_avg/lepidUP_avg_sample_stat;

  TH1D** lepidDOWNInputComb = new TH1D*[19];

  TString PATH_lepidDOWN = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_lepidDOWN/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t lepidDOWN_avg_histo = 0.0;
  Double_t lepidDOWN_avg_sample_stat = 0.0;
  Double_t lepidDOWN_avg = 0.0;
  Double_t lepidDOWN_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString FileName = Form("%s/ff_myhist_%s.root", PATH_lepidDOWN.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     lepidDOWNInputComb[i] = hist;
     lepidDOWNInputComb[i] -> Rebin(rebin_factor);
     lepidDOWNInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     lepidDOWN_avg_histo = average_calculator(lepidDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
     if(lepidDOWNInputComb[i]->Integral()!=0) lepidDOWN_avg = lepidDOWN_avg + lepidDOWN_avg_histo*lepidDOWNInputComb[i]->Integral();
     if(lepidDOWNInputComb[i]->Integral()!=0) lepidDOWN_avg_sample_stat = lepidDOWN_avg_sample_stat + lepidDOWNInputComb[i]->Integral();
     }
     if(i==17) lepidDOWN_avg_sig = average_calculator(lepidDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    lepidDOWN_avg = lepidDOWN_avg/lepidDOWN_avg_sample_stat;

  // signal systematics
	cfile->createShapeSigSystematic("lepid_sys", lepidUPInputComb[17], lepidDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "lepid_sys" << " for  signal "
	//<< " [" << lepidUPInputComb[17]->GetEntries() << "," <<  lepidDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics
    for (Int_t i = 2; i <(Ntypes-1); i++) {

     	cfile->createShapeBkgdSystematic(i-1, "lepid_sys", lepidUPInputComb[i], lepidDOWNInputComb[i], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "lepid_sys" << " for  "  << TypeLeg[i]
	//<< " [" << lepidUPInputComb[i]->GetEntries()  << "," <<  lepidDOWNInputComb[i]->GetEntries() << "]" << endl ;
    }

//pdf_shape_systematics

 /* TH1D** pdfUPInputComb = new TH1D*[19];

  TString PATH_pdfUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_PDFUP/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t pdfUP_avg_histo = 0.0;
  Double_t pdfUP_avg_sample_stat = 0.0;
  Double_t pdfUP_avg = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {


     TString FileName = Form("%s/ff_myhist_%s.root", PATH_pdfUP.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     pdfUPInputComb[i] = hist;
     pdfUPInputComb[i] -> Rebin(rebin_factor);
     pdfUPInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1) {
     pdfUP_avg_histo = average_calculator(pdfUPInputComb[i], allInputComb[i], start_bin_for_syst);
     if(pdfUPInputComb[i]->Integral()!=0) pdfUP_avg = pdfUP_avg + pdfUP_avg_histo*pdfUPInputComb[i]->Integral();
     if(pdfUPInputComb[i]->Integral()!=0) pdfUP_avg_sample_stat = pdfUP_avg_sample_stat + pdfUPInputComb[i]->Integral();
     }
    }
    pdfUP_avg = pdfUP_avg/pdfUP_avg_sample_stat;

  TH1D** pdfDOWNInputComb = new TH1D*[19];

  TString PATH_pdfDOWN = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_PDFDOWN/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t pdfDOWN_avg_histo = 0.0;
  Double_t pdfDOWN_avg_sample_stat = 0.0;
  Double_t pdfDOWN_avg = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString FileName = Form("%s/ff_myhist_%s.root", PATH_pdfDOWN.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     pdfDOWNInputComb[i] = hist;
     pdfDOWNInputComb[i] -> Rebin(rebin_factor);
     pdfDOWNInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1) {
     pdfDOWN_avg_histo = average_calculator(pdfDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
     if(pdfDOWNInputComb[i]->Integral()!=0) pdfDOWN_avg = pdfDOWN_avg + pdfDOWN_avg_histo*pdfDOWNInputComb[i]->Integral();
     if(pdfDOWNInputComb[i]->Integral()!=0) pdfDOWN_avg_sample_stat = pdfDOWN_avg_sample_stat + pdfDOWNInputComb[i]->Integral();
     }
    }
    pdfDOWN_avg = pdfDOWN_avg/pdfDOWN_avg_sample_stat;

  // signal systematics
	cfile->createShapeSigSystematic("pdf_sys", pdfUPInputComb[17], pdfDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "pdf_sys" << " for  signal "
	//<< " [" << pdfUPInputComb[17]->GetEntries() << "," <<  pdfDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics
    for (Int_t i = 2; i <(Ntypes-1); i++) {

     	cfile->createShapeBkgdSystematic(i-1, "pdf_sys", pdfUPInputComb[i], pdfDOWNInputComb[i], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "pdf_sys" << " for  "  << TypeLeg[i]
	//<< " [" << pdfUPInputComb[i]->GetEntries()  << "," <<  pdfDOWNInputComb[i]->GetEntries() << "]" << endl ;
    }*/

//JER_shape_systematics

  TH1D** JERUPInputComb = new TH1D*[19];

  TString PATH_JERUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_JERUP/controlplots/final/%s/%s/JERUP/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t JERUP_avg_histo = 0.0;
  Double_t JERUP_avg_sample_stat = 0.0;
  Double_t JERUP_avg = 0.0;
  Double_t JERUP_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {


     TString FileName = Form("%s/ff_myhist_%s.root", PATH_JERUP.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     JERUPInputComb[i] = hist;
     JERUPInputComb[i] -> Rebin(rebin_factor);
     JERUPInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     JERUP_avg_histo = average_calculator(JERUPInputComb[i], allInputComb[i], start_bin_for_syst);
     if(JERUPInputComb[i]->Integral()!=0) JERUP_avg = JERUP_avg + JERUP_avg_histo*JERUPInputComb[i]->Integral();
     if(JERUPInputComb[i]->Integral()!=0) JERUP_avg_sample_stat = JERUP_avg_sample_stat + JERUPInputComb[i]->Integral();
     }
     if(i==17) JERUP_avg_sig = average_calculator(JERUPInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    JERUP_avg = JERUP_avg/JERUP_avg_sample_stat;

  TH1D** JERDOWNInputComb = new TH1D*[19];

  TString PATH_JERDOWN = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_JERDOWN/controlplots/final/%s/%s/JERDOWN/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t JERDOWN_avg_histo = 0.0;
  Double_t JERDOWN_avg_sample_stat = 0.0;
  Double_t JERDOWN_avg = 0.0;
  Double_t JERDOWN_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString FileName = Form("%s/ff_myhist_%s.root", PATH_JERDOWN.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     JERDOWNInputComb[i] = hist;
     JERDOWNInputComb[i] -> Rebin(rebin_factor);
     JERDOWNInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     JERDOWN_avg_histo = average_calculator(JERDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
     if(JERDOWNInputComb[i]->Integral()!=0) JERDOWN_avg = JERDOWN_avg + JERDOWN_avg_histo*JERDOWNInputComb[i]->Integral();
     if(JERDOWNInputComb[i]->Integral()!=0) JERDOWN_avg_sample_stat = JERDOWN_avg_sample_stat + JERDOWNInputComb[i]->Integral();
     }
     if(i==17) JERDOWN_avg_sig = average_calculator(JERDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    JERDOWN_avg = JERDOWN_avg/JERDOWN_avg_sample_stat;

  // signal systematics
	cfile->createShapeSigSystematic("JER_sys", JERUPInputComb[17], JERDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "JER_sys" << " for  signal "
	//<< " [" << JERUPInputComb[17]->GetEntries() << "," <<  JERDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics
    for (Int_t i = 2; i <(Ntypes-1); i++) {

     	cfile->createShapeBkgdSystematic(i-1, "JER_sys", JERUPInputComb[i], JERDOWNInputComb[i], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "JER_sys" << " for  "  << TypeLeg[i]
	//<< " [" << JERUPInputComb[i]->GetEntries()  << "," <<  JERDOWNInputComb[i]->GetEntries() << "]" << endl ;
    }

//tagga_shape_systematics

  TH1D** taggaUPInputComb = new TH1D*[19];

  TString PATH_taggaUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_taggaUP/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t taggaUP_avg_histo = 0.0;
  Double_t taggaUP_avg_sample_stat = 0.0;
  Double_t taggaUP_avg = 0.0;
  Double_t taggaUP_avg_sig = 0.0;

    for (Int_t i =1; i <Ntypes; i++) {


     TString FileName = Form("%s/ff_myhist_%s.root", PATH_taggaUP.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     taggaUPInputComb[i] = hist;
     taggaUPInputComb[i] -> Rebin(rebin_factor);
     taggaUPInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     taggaUP_avg_histo = average_calculator(taggaUPInputComb[i], allInputComb[i], start_bin_for_syst);
     if(taggaUPInputComb[i]->Integral()!=0) taggaUP_avg = taggaUP_avg + taggaUP_avg_histo*taggaUPInputComb[i]->Integral();
     if(taggaUPInputComb[i]->Integral()!=0) taggaUP_avg_sample_stat = taggaUP_avg_sample_stat + taggaUPInputComb[i]->Integral();
     }
     if(i==17) taggaUP_avg_sig = average_calculator(taggaUPInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    taggaUP_avg = taggaUP_avg/taggaUP_avg_sample_stat;

  TH1D** taggaDOWNInputComb = new TH1D*[19];

  TString PATH_taggaDOWN = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_taggaDOWN/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t taggaDOWN_avg_histo = 0.0;
  Double_t taggaDOWN_avg_sample_stat = 0.0;
  Double_t taggaDOWN_avg = 0.0;
  Double_t taggaDOWN_avg_sig = 0.0;

    for (Int_t i =1; i <Ntypes; i++) {
     TString FileName = Form("%s/ff_myhist_%s.root", PATH_taggaDOWN.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     taggaDOWNInputComb[i] = hist;
     taggaDOWNInputComb[i] -> Rebin(rebin_factor);
     taggaDOWNInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     taggaDOWN_avg_histo = average_calculator(taggaDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
     if(taggaDOWNInputComb[i]->Integral()!=0) taggaDOWN_avg = taggaDOWN_avg + taggaDOWN_avg_histo*taggaDOWNInputComb[i]->Integral();
     if(taggaDOWNInputComb[i]->Integral()!=0) taggaDOWN_avg_sample_stat = taggaDOWN_avg_sample_stat + taggaDOWNInputComb[i]->Integral();
     }
     if(i==17) taggaDOWN_avg_sig = average_calculator(taggaDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    taggaDOWN_avg = taggaDOWN_avg/taggaDOWN_avg_sample_stat;

  // signal systematics
	cfile->createShapeSigSystematic("tagga_sys", taggaUPInputComb[17], taggaDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "tagga_sys" << " for  signal "
	//<< " [" << taggaUPInputComb[17]->GetEntries() << "," <<  taggaDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics
    for (Int_t i = 2; i <(Ntypes-1); i++) {

     	cfile->createShapeBkgdSystematic(i-1, "tagga_sys", taggaUPInputComb[i], taggaDOWNInputComb[i], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "tagga_sys" << " for  "  << TypeLeg[i]
	//<< " [" << taggaUPInputComb[i]->GetEntries()  << "," <<  taggaDOWNInputComb[i]->GetEntries() << "]" << endl ;
    }
/*
//bfrag_shape_systematics

  TH1D** bfragUPInputComb = new TH1D*[19];
  TH1D** bfragDOWNInputComb = new TH1D*[19];

  TString PATH_bfragUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_bfragUP/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t bfragUP_avg_histo = 0.0;
  Double_t bfragUP_avg_sample_stat = 0.0;
  Double_t bfragUP_avg = 0.0;

  TH1D** bfrag_divided_sf = new TH1D*[19];

    for (Int_t i =0; i <Ntypes; i++) {
     TString FileName = Form("%s/ff_myhist_%s.root", PATH_bfragUP.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     bfragUPInputComb[i] = hist;
     bfragUPInputComb[i] -> Rebin(rebin_factor);
     bfragUPInputComb[i] ->SetDirectory(gROOT);
     bfragDOWNInputComb[i] = allInputComb[i];
     bfrag_divided_sf[i] = allInputComb[i];
     for (Int_t j = 1; j<21; j++) {
     bfrag_divided_sf[i]->SetBinContent(j, 1.0 + (1.0 - (bfragUPInputComb[i]->GetBinContent(j)/allInputComb[i]->GetBinContent(j))));
     bfragDOWNInputComb[i]->SetBinContent(j, bfrag_divided_sf[i]->GetBinContent(j)*allInputComb[i]->GetBinContent(j));
     }
     bfrag_divided_sf[i] ->SetDirectory(gROOT);
     bfragDOWNInputComb[i] ->SetDirectory(gROOT);
     std::cout<<"qwe qwe"<<bfragUPInputComb[i]->GetNbinsX()<<std::endl;
     f->Close();
     if(i!=0 && i!=1) {
     bfragUP_avg_histo = average_calculator(bfragUPInputComb[i], allInputComb[i], start_bin_for_syst);
     if(bfragUPInputComb[i]->Integral()!=0) bfragUP_avg = bfragUP_avg + bfragUP_avg_histo*bfragUPInputComb[i]->Integral();
     if(bfragUPInputComb[i]->Integral()!=0) bfragUP_avg_sample_stat = bfragUP_avg_sample_stat + bfragUPInputComb[i]->Integral();
     }
    }
    bfragUP_avg = bfragUP_avg/bfragUP_avg_sample_stat;

  // signal systematics
	cfile->createShapeSigSystematic("bfrag_sys", bfragUPInputComb[17], bfragDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "bfrag_sys" << " for  signal "
	//<< " [" << bfragUPInputComb[17]->GetEntries() << "," <<  bfragDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics
    for (Int_t i = 2; i <(Ntypes-1); i++) {

     	cfile->createShapeBkgdSystematic(i-1, "bfrag_sys", bfragUPInputComb[i], bfragDOWNInputComb[i], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "bfrag_sys" << " for  "  << TypeLeg[i]
	//<< " [" << bfragUPInputComb[i]->GetEntries()  << "," <<  bfragDOWNInputComb[i]->Integral() << "]"<< allInputComb[i]->Integral() << endl ;
    }*/

//zwpt_shape_systematics

  TH1D** zwptUPInputComb = new TH1D*[19];

  TString PATH_zwptUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_zwptREW/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t zwptUP_avg_histo = 0.0;
  Double_t zwptUP_avg_sample_stat = 0.0;
  Double_t zwptUP_avg = 0.0;
  Double_t zwptUP_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString FileName = Form("%s/ff_myhist_%s.root", PATH_zwptUP.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     zwptUPInputComb[i] = hist;
     zwptUPInputComb[i] -> Rebin(rebin_factor);
     zwptUPInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i<11) {
     zwptUP_avg_histo = average_calculator(zwptUPInputComb[i], allInputComb[i], start_bin_for_syst);
     if(zwptUPInputComb[i]->Integral()!=0) zwptUP_avg = zwptUP_avg + zwptUP_avg_histo*zwptUPInputComb[i]->Integral();
     if(zwptUPInputComb[i]->Integral()!=0) zwptUP_avg_sample_stat = zwptUP_avg_sample_stat + zwptUPInputComb[i]->Integral();
     }
    }
    zwptUP_avg = zwptUP_avg/zwptUP_avg_sample_stat;

  TH1D** zwptDOWNInputComb = new TH1D*[19];

  Double_t zwptDOWN_avg_histo = 0.0;
  Double_t zwptDOWN_avg_sample_stat = 0.0;
  Double_t zwptDOWN_avg = 0.0;
  Double_t zwptDOWN_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString zwptDOWN_hist = Form("h_zwptDOWNInputComb_%d",i);
     zwptDOWNInputComb[i] = new TH1D(zwptDOWN_hist, "zwptDOWNInputComb",20,0.,1.);
     zwptDOWNInputComb[i]->Sumw2();
     	for (Int_t j = start_bin_for_syst; j<21; j++) {
     		 if(allInputComb[i]->GetBinContent(j)!=0) zwptDOWNInputComb[i]->SetBinContent(j, (allInputComb[i]->GetBinContent(j)*(1.0 + (1.0 - (zwptUPInputComb[i]->GetBinContent(j)/allInputComb[i]->GetBinContent(j))))));
	}
     	zwptDOWNInputComb[i] ->SetDirectory(gROOT);
     if(i!=0 && i!=1 && i<11) {
     zwptDOWN_avg_histo = average_calculator(zwptDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
     if(zwptDOWNInputComb[i]->Integral()!=0) zwptDOWN_avg = zwptDOWN_avg + zwptDOWN_avg_histo*zwptDOWNInputComb[i]->Integral();
     if(zwptDOWNInputComb[i]->Integral()!=0) zwptDOWN_avg_sample_stat = zwptDOWN_avg_sample_stat + zwptDOWNInputComb[i]->Integral();
     }
    }
    zwptDOWN_avg = zwptDOWN_avg/zwptDOWN_avg_sample_stat;

  // background systematics
    for (Int_t i = 2; i < 11; i++) {//only for all Z and W

     	cfile->createShapeBkgdSystematic(i-1, "zwpt_sys", zwptUPInputComb[i], zwptDOWNInputComb[i], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "zwpt_sys" << " for  "  << TypeLeg[i]
	//<< " [" << zwptUPInputComb[i]->GetEntries()  << "," <<  zwptDOWNInputComb[i]->GetEntries() << "]" << endl ;
    }

//lumiREW_shape_systematics
/*
  TH1D** lumiREWUPInputComb = new TH1D*[19];

  TString PATH_lumiREWUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_lumiREW/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t lumiREWUP_avg_histo = 0.0;
  Double_t lumiREWUP_avg_sample_stat = 0.0;
  Double_t lumiREWUP_avg = 0.0;

  TH1D** lumiREW_divided_sf = new TH1D*[19];

    for (Int_t i =0; i <Ntypes; i++) {


     TString FileName = Form("%s/ff_myhist_%s.root", PATH_lumiREWUP.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     lumiREWUPInputComb[i] = hist;
     lumiREWUPInputComb[i] -> Rebin(rebin_factor);
     lumiREWUPInputComb[i] ->SetDirectory(gROOT);
     lumiREW_divided_sf[i] = allInputComb[i];
     for (Int_t j = 1; j<21; j++) {
     lumiREW_divided_sf[i]->SetBinContent(j, 1.0 + (1.0 - (lumiREWUPInputComb[i]->GetBinContent(j)/allInputComb[i]->GetBinContent(j))));
     }
     lumiREW_divided_sf[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1) {
     lumiREWUP_avg_histo = average_calculator(lumiREWUPInputComb[i], allInputComb[i], start_bin_for_syst);
     if(lumiREWUPInputComb[i]->Integral()!=0) lumiREWUP_avg = lumiREWUP_avg + lumiREWUP_avg_histo*lumiREWUPInputComb[i]->Integral();
     if(lumiREWUPInputComb[i]->Integral()!=0) lumiREWUP_avg_sample_stat = lumiREWUP_avg_sample_stat + lumiREWUPInputComb[i]->Integral();
     }
    }
    lumiREWUP_avg = lumiREWUP_avg/lumiREWUP_avg_sample_stat;

  TH1D** lumiREWDOWNInputComb = new TH1D*[19];

  Double_t lumiREWDOWN_avg_histo = 0.0;
  Double_t lumiREWDOWN_avg_sample_stat = 0.0;
  Double_t lumiREWDOWN_avg = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     lumiREWDOWNInputComb[i] ->Sumw2();
     lumiREWDOWNInputComb[i] -> Multiply(lumiREW_divided_sf[i], allInputComb[i]);
     lumiREWDOWNInputComb[i] ->SetDirectory(gROOT);
     if(i!=0 && i!=1) {
     lumiREWDOWN_avg_histo = average_calculator(lumiREWDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
     if(lumiREWDOWNInputComb[i]->Integral()!=0) lumiREWDOWN_avg = lumiREWDOWN_avg + lumiREWDOWN_avg_histo*lumiREWDOWNInputComb[i]->Integral();
     if(lumiREWDOWNInputComb[i]->Integral()!=0) lumiREWDOWN_avg_sample_stat = lumiREWDOWN_avg_sample_stat + lumiREWDOWNInputComb[i]->Integral();
     }
    }
    lumiREWDOWN_avg = lumiREWDOWN_avg/lumiREWDOWN_avg_sample_stat;

  // signal systematics
	cfile->createShapeSigSystematic("lumiREW_sys", lumiREWUPInputComb[17], lumiREWDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "lumiREW_sys" << " for  signal "
	//<< " [" << lumiREWUPInputComb[17]->GetEntries() << "," <<  lumiREWDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics
    for (Int_t i = 2; i <(Ntypes-1); i++) {

     	cfile->createShapeBkgdSystematic(i-1, "lumiREW_sys", lumiREWUPInputComb[i], lumiREWDOWNInputComb[i], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "lumiREW_sys" << " for  "  << TypeLeg[i]
	//<< " [" << lumiREWUPInputComb[i]->GetEntries()  << "," <<  lumiREWDOWNInputComb[i]->GetEntries() << "]" << endl ;
    }*/

//zvtxREW_shape_systematics

  TH1D** zvtxREWUPInputComb = new TH1D*[19];

  TString PATH_zvtxREWUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_zvtxREW/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t zvtxREWUP_avg_histo = 0.0;
  Double_t zvtxREWUP_avg_sample_stat = 0.0;
  Double_t zvtxREWUP_avg = 0.0;
  Double_t zvtxREWUP_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString FileName = Form("%s/ff_myhist_%s.root", PATH_zvtxREWUP.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     zvtxREWUPInputComb[i] = hist;
     zvtxREWUPInputComb[i] -> Rebin(rebin_factor);
     zvtxREWUPInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     zvtxREWUP_avg_histo = average_calculator(zvtxREWUPInputComb[i], allInputComb[i], start_bin_for_syst);
     if(zvtxREWUPInputComb[i]->Integral()!=0) zvtxREWUP_avg = zvtxREWUP_avg + zvtxREWUP_avg_histo*zvtxREWUPInputComb[i]->Integral();
     if(zvtxREWUPInputComb[i]->Integral()!=0) zvtxREWUP_avg_sample_stat = zvtxREWUP_avg_sample_stat + zvtxREWUPInputComb[i]->Integral();
     }
     if(i==17) zvtxREWUP_avg_sig = average_calculator(zvtxREWUPInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    zvtxREWUP_avg = zvtxREWUP_avg/zvtxREWUP_avg_sample_stat;

  TH1D** zvtxREWDOWNInputComb = new TH1D*[19];

  Double_t zvtxREWDOWN_avg_histo = 0.0;
  Double_t zvtxREWDOWN_avg_sample_stat = 0.0;
  Double_t zvtxREWDOWN_avg = 0.0;
  Double_t zvtxREWDOWN_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString zvtxREWDOWN_hist = Form("h_zvtxREWDOWNInputComb_%d",i);
     zvtxREWDOWNInputComb[i] = new TH1D(zvtxREWDOWN_hist, "zvtxREWDOWNInputComb",20,0.,1.);
     zvtxREWDOWNInputComb[i] ->Sumw2();
     	for (Int_t j = start_bin_for_syst; j<21; j++) {
     		 if(allInputComb[i]->GetBinContent(j)!=0) zvtxREWDOWNInputComb[i]->SetBinContent(j, (allInputComb[i]->GetBinContent(j)*(1.0 + (1.0 - (zvtxREWUPInputComb[i]->GetBinContent(j)/allInputComb[i]->GetBinContent(j))))));
	}
     zvtxREWDOWNInputComb[i] ->SetDirectory(gROOT);
     if(i!=0 && i!=1 && i!=17) {
     zvtxREWDOWN_avg_histo = average_calculator(zvtxREWDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
     if(zvtxREWDOWNInputComb[i]->Integral()!=0) zvtxREWDOWN_avg = zvtxREWDOWN_avg + zvtxREWDOWN_avg_histo*zvtxREWDOWNInputComb[i]->Integral();
     if(zvtxREWDOWNInputComb[i]->Integral()!=0) zvtxREWDOWN_avg_sample_stat = zvtxREWDOWN_avg_sample_stat + zvtxREWDOWNInputComb[i]->Integral();
     }
     if(i==17) zvtxREWDOWN_avg_sig = average_calculator(zvtxREWDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    zvtxREWDOWN_avg = zvtxREWDOWN_avg/zvtxREWDOWN_avg_sample_stat;

  // signal systematics
	cfile->createShapeSigSystematic("zvtxREW_sys", zvtxREWUPInputComb[17], zvtxREWDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "zvtxREW_sys" << " for  signal "
	//<< " [" << zvtxREWUPInputComb[17]->GetEntries() << "," <<  zvtxREWDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics
    for (Int_t i = 2; i <(Ntypes-1); i++) {

     	cfile->createShapeBkgdSystematic(i-1, "zvtxREW_sys", zvtxREWUPInputComb[i], zvtxREWDOWNInputComb[i], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "zvtxREW_sys" << " for  "  << TypeLeg[i]
	//<< " [" << zvtxREWUPInputComb[i]->GetEntries()  << "," <<  zvtxREWDOWNInputComb[i]->GetEntries() << "]" << endl ;
    }

//MC statistics systematics

  TH1D** mcUPInputComb = new TH1D*[19];

  Double_t mcUP_avg_histo = 0.0;
  Double_t mcUP_avg_sample_stat = 0.0;
  Double_t mcUP_avg = 0.0;
  Double_t mcUP_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString mcUP_hist = Form("h_mcUPInputComb_%d",i);
     mcUPInputComb[i] = new TH1D(mcUP_hist, "mcUPInputComb",20,0.,1.);
     mcUPInputComb[i]->Sumw2();
     	for (Int_t j = start_bin_for_syst; j<21; j++) {
     		mcUPInputComb[i]->SetBinContent(j, (allInputComb[i]->GetBinContent(j) + allInputComb[i]->GetBinError(j)));
	}
     	mcUPInputComb[i] ->SetDirectory(gROOT);
     if(i!=0 && i!=1 && i!=17) {
     mcUP_avg_histo = average_calculator(mcUPInputComb[i], allInputComb[i], start_bin_for_syst);
     if(mcUPInputComb[i]->Integral()!=0) mcUP_avg = mcUP_avg + mcUP_avg_histo*mcUPInputComb[i]->Integral()*mcUPInputComb[i]->Integral()/allInputComb[i]->GetEntries();
     //cout<<"!!!!!!!!!!!!!!!!!!!   "<<i<<"     "<<allInputComb[i]->GetEffectiveEntries()<<endl;
     if(mcUPInputComb[i]->Integral()!=0) mcUP_avg_sample_stat = mcUP_avg_sample_stat + mcUPInputComb[i]->Integral();
     }
     if(i==17) mcUP_avg_sig = average_calculator_for_mcstat(mcUPInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    mcUP_avg = mcUP_avg/mcUP_avg_sample_stat;

  TH1D** mcDOWNInputComb = new TH1D*[19];

  Double_t mcDOWN_avg_histo = 0.0;
  Double_t mcDOWN_avg_sample_stat = 0.0;
  Double_t mcDOWN_avg = 0.0;
  Double_t mcDOWN_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString mcDOWN_hist = Form("h_mcDOWNInputComb_%d",i);
     mcDOWNInputComb[i] = new TH1D(mcDOWN_hist, "mcDOWNInputComb",20,0.,1.);
     mcDOWNInputComb[i]->Sumw2();
     	for (Int_t j = start_bin_for_syst; j<21; j++) {
     		mcDOWNInputComb[i]->SetBinContent(j, (allInputComb[i]->GetBinContent(j) - allInputComb[i]->GetBinError(j)));
	}
     	mcDOWNInputComb[i] ->SetDirectory(gROOT);
     if(i!=0 && i!=1 && i!=17) {
     mcDOWN_avg_histo = average_calculator(mcDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
     if(mcDOWNInputComb[i]->Integral()!=0) mcDOWN_avg = mcDOWN_avg + mcDOWN_avg_histo*mcDOWNInputComb[i]->Integral()*mcDOWNInputComb[i]->Integral()/allInputComb[i]->GetEntries();
     if(mcDOWNInputComb[i]->Integral()!=0) mcDOWN_avg_sample_stat = mcDOWN_avg_sample_stat + mcDOWNInputComb[i]->Integral();
     }
     if(i==17) mcDOWN_avg_sig = average_calculator_for_mcstat(mcDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    mcDOWN_avg = mcDOWN_avg/mcDOWN_avg_sample_stat;

  // signal systematics
	cfile->createShapeSigSystematic("mc_stat_sys", mcUPInputComb[17], mcDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "mc_stat_sys" << " for  signal "
	//<< " [" << mcUPInputComb[17]->GetEntries() << "," <<  mcDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics
    for (Int_t i = 1; i <(Ntypes-1); i++) {

     	cfile->createShapeBkgdSystematic(i-1, "mc_stat_sys", mcUPInputComb[i], mcDOWNInputComb[i], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "mc_stat_sys" << " for  "  << TypeLeg[i]
	//<< " [" << mcUPInputComb[i]->GetEntries()  << "," <<  mcDOWNInputComb[i]->GetEntries() << "]" << endl ;
    }

//!ttbar shape dependent systematics
//top mass shape systematics
/*
//cTag_shape_systematics

  TH1D** topmassUPInputComb = new TH1D*[19];

  TString PATH_topmassUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_ttbar/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t topmassUP_avg_histo = 0.0;
  Double_t topmassUP_avg_sample_stat = 0.0;
  Double_t topmassUP_avg = 0.0;
  Double_t topmassUP_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {


     TString FileName = Form("%s/ff_myhist_%s.root", PATH_topmassUP.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     topmassUPInputComb[i] = hist;
     topmassUPInputComb[i] -> Rebin(rebin_factor);
     topmassUPInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     topmassUP_avg_histo = average_calculator(topmassUPInputComb[i], allInputComb[i], start_bin_for_syst);
     if(topmassUPInputComb[i]->Integral()!=0) topmassUP_avg = topmassUP_avg + topmassUP_avg_histo*topmassUPInputComb[i]->Integral();
     if(topmassUPInputComb[i]->Integral()!=0) topmassUP_avg_sample_stat = topmassUP_avg_sample_stat + topmassUPInputComb[i]->Integral();
     }     
     if(i==17) topmassUP_avg_sig = average_calculator(topmassUPInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    topmassUP_avg = topmassUP_avg/topmassUP_avg_sample_stat;

  TH1D** topmassDOWNInputComb = new TH1D*[19];

  TString PATH_topmassDOWN = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_topmassDOWN/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

  Double_t topmassDOWN_avg_histo = 0.0;
  Double_t topmassDOWN_avg_sample_stat = 0.0;
  Double_t topmassDOWN_avg = 0.0;
  Double_t topmassDOWN_avg_sig = 0.0;

    for (Int_t i =0; i <Ntypes; i++) {
     TString FileName = Form("%s/ff_myhist_%s.root", PATH_topmassDOWN.Data(), Type_l[i].Data());
     TFile *f = new TFile(FileName.Data(),"READ");
     if(f==NULL) return 0; f->cd();
     TH1D *hist = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     topmassDOWNInputComb[i] = hist;
     topmassDOWNInputComb[i] -> Rebin(rebin_factor);
     topmassDOWNInputComb[i] ->SetDirectory(gROOT);
     f->Close();
     if(i!=0 && i!=1 && i!=17) {
     topmassDOWN_avg_histo = average_calculator(topmassDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
     if(topmassDOWNInputComb[i]->Integral()!=0) topmassDOWN_avg = topmassDOWN_avg + topmassDOWN_avg_histo*topmassDOWNInputComb[i]->Integral();
     if(topmassDOWNInputComb[i]->Integral()!=0) topmassDOWN_avg_sample_stat = topmassDOWN_avg_sample_stat + topmassDOWNInputComb[i]->Integral();
     }
     if(i==17) topmassDOWN_avg_sig = average_calculator(topmassDOWNInputComb[i], allInputComb[i], start_bin_for_syst);
    }
    topmassDOWN_avg = topmassDOWN_avg/topmassDOWN_avg_sample_stat;

  // signal systematics
	cfile->createShapeSigSystematic("cTag_sys", topmassUPInputComb[17], topmassDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "cTag_sys" << " for  signal "
	//<< " [" << topmassUPInputComb[17]->GetEntries() << "," <<  topmassDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics
    for (Int_t i = 2; i <(Ntypes-1); i++) {

     	cfile->createShapeBkgdSystematic(i-1, "cTag_sys", topmassUPInputComb[i], topmassDOWNInputComb[i], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "cTag_sys" << " for  "  << TypeLeg[i]
	//<< " [" << topmassUPInputComb[i]->GetEntries()  << "," <<  topmassDOWNInputComb[i]->GetEntries() << "]" << endl ;
    }


/*
  TH1D** topmassUPInputComb = new TH1D*[19];
  TH1D** tm175_divided_sf = new TH1D*[19];

  TString PATH_topmassUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_ttbar/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

     TString FileName_tmA175 = Form("%s/ff_myhist_ttA_175.root", PATH_topmassUP.Data());
     TFile *f_tmA175 = new TFile(FileName_tmA175.Data(),"READ");
     if(f_tmA175==NULL) return 0; f_tmA175->cd();
     TH1D *hist_tmA175 = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     topmassUPInputComb[17] = hist_tmA175;
     topmassUPInputComb[17] -> Rebin(rebin_factor);
     topmassUPInputComb[17] ->SetDirectory(gROOT);
     tm175_divided_sf[17] ->SetDirectory(gROOT);
     f_tmA175->Close();
     tm175_divided_sf[17] ->Sumw2();
     tm175_divided_sf[17] = topmassUPInputComb[17];
     for (Int_t j = 1; j<21; j++) {
     tm175_divided_sf[17]->SetBinContent(j, (topmassUPInputComb[17]->GetBinContent(j)/allInputComb[17]->GetBinContent(j) - 1.0)*0.4);//need to scale by 0.4 in order to remain within +-1 GeV error (172.5pm1)
     tm175_divided_sf[17]->SetBinContent(j, (tm175_divided_sf[17]->GetBinContent(j) + 1.0)*allInputComb[17]->GetBinContent(j));
     tm175_divided_sf[17]->SetBinContent(j, topmassUPInputComb[17]->GetBinContent(j)/tm175_divided_sf[17]->GetBinContent(j));
     }
     topmassUPInputComb[17] -> Multiply(topmassUPInputComb[17], tm175_divided_sf[17]);

     TString FileName_tmAll175 = Form("%s/ff_myhist_ttAll_175.root", PATH_topmassUP.Data());
     TFile *f_tmAll175 = new TFile(FileName_tmAll175.Data(),"READ");
     if(f_tmAll175==NULL) return 0; f_tmAll175->cd();
     TH1D *hist_tmAll175 = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     topmassUPInputComb[16] = hist_tmAll175;
     topmassUPInputComb[16] -> Rebin(rebin_factor);
     topmassUPInputComb[16] ->SetDirectory(gROOT);
     tm175_divided_sf[16] ->SetDirectory(gROOT);
     f_tmAll175->Close();
     tm175_divided_sf[16] ->Sumw2();
     tm175_divided_sf[16] = topmassUPInputComb[16];
     for (Int_t j = 1; j<21; j++) {
     tm175_divided_sf[16]->SetBinContent(j, (topmassUPInputComb[16]->GetBinContent(j)/allInputComb[16]->GetBinContent(j) - 1.0)*0.4);//need to scale by 0.4 in order to remain within +-1 GeV error (172.5pm1)
     tm175_divided_sf[16]->SetBinContent(j, (tm175_divided_sf[16]->GetBinContent(j) + 1.0)*allInputComb[16]->GetBinContent(j));
     tm175_divided_sf[16]->SetBinContent(j, topmassUPInputComb[16]->GetBinContent(j)/tm175_divided_sf[16]->GetBinContent(j));
     }
     topmassUPInputComb[16] -> Multiply(topmassUPInputComb[16], tm175_divided_sf[16]);

  TH1D** topmassDOWNInputComb = new TH1D*[19];
  TH1D** tm170_divided_sf = new TH1D*[19];

  TString PATH_topmassDOWN = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_ttbar/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

     TString FileName_tmA170 = Form("%s/ff_myhist_ttA_170.root", PATH_topmassDOWN.Data());
     TFile *f_tmA170 = new TFile(FileName_tmA170.Data(),"READ");
     if(f_tmA170==NULL) return 0; f_tmA170->cd();
     TH1D *hist_tmA170 = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     topmassDOWNInputComb[17] = hist_tmA170;
     topmassDOWNInputComb[17] -> Rebin(rebin_factor);
     topmassDOWNInputComb[17] ->SetDirectory(gROOT);
     tm170_divided_sf[17] ->SetDirectory(gROOT);
     f_tmA170->Close();
     tm170_divided_sf[17] ->Sumw2();
     tm170_divided_sf[17] = topmassDOWNInputComb[17];
     for (Int_t j = 1; j<21; j++) {
     tm170_divided_sf[17]->SetBinContent(j, (topmassDOWNInputComb[17]->GetBinContent(j)/allInputComb[17]->GetBinContent(j) - 1.0)*0.4);//need to scale by 0.4 in order to remain within +-1 GeV error (172.5pm1)
     tm170_divided_sf[17]->SetBinContent(j, (tm170_divided_sf[17]->GetBinContent(j) + 1.0)*allInputComb[17]->GetBinContent(j));
     tm170_divided_sf[17]->SetBinContent(j, topmassDOWNInputComb[17]->GetBinContent(j)/tm170_divided_sf[17]->GetBinContent(j));
     }
     topmassDOWNInputComb[17] -> Multiply(topmassDOWNInputComb[17], tm170_divided_sf[17]);

     TString FileName_tmAll170 = Form("%s/ff_myhist_ttAll_170.root", PATH_topmassDOWN.Data());
     TFile *f_tmAll170 = new TFile(FileName_tmAll170.Data(),"READ");
     if(f_tmAll170==NULL) return 0; f_tmAll170->cd();
     TH1D *hist_tmAll170 = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     topmassDOWNInputComb[16] = hist_tmAll170;
     topmassDOWNInputComb[16] -> Rebin(rebin_factor);
     topmassDOWNInputComb[16] ->SetDirectory(gROOT);
     tm170_divided_sf[16] ->SetDirectory(gROOT);
     f_tmAll170->Close();
     tm170_divided_sf[16] ->Sumw2();
     tm170_divided_sf[16] = topmassDOWNInputComb[16];
     for (Int_t j = 1; j<21; j++) {
     tm170_divided_sf[16]->SetBinContent(j, (topmassDOWNInputComb[16]->GetBinContent(j)/allInputComb[16]->GetBinContent(j) - 1.0)*0.4);//need to scale by 0.4 in order to remain within +-1 GeV error (172.5pm1)
     tm170_divided_sf[16]->SetBinContent(j, (tm170_divided_sf[16]->GetBinContent(j) + 1.0)*allInputComb[16]->GetBinContent(j));
     tm170_divided_sf[16]->SetBinContent(j, topmassDOWNInputComb[16]->GetBinContent(j)/tm170_divided_sf[16]->GetBinContent(j));
     }
     topmassDOWNInputComb[16] -> Multiply(topmassDOWNInputComb[16], tm170_divided_sf[16]);

  // signal systematics
	cfile->createShapeSigSystematic("topmass_sys", topmassUPInputComb[17], topmassDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "topmass_sys" << " for  signal "
	//<< " [" << topmassUPInputComb[17]->GetEntries() << "," <<  topmassDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics - ttbar->dilepton only
     	cfile->createShapeBkgdSystematic(15, "topmass_sys", topmassUPInputComb[16], topmassDOWNInputComb[16], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "topmass_sys" << " for  "  << TypeLeg[16]
	//<< " [" << topmassUPInputComb[16]->GetEntries()  << "," <<  topmassDOWNInputComb[16]->GetEntries() << "]" << endl;

//ISR/FSR variation

  TH1D** ifsrvarUPInputComb = new TH1D*[19];
  TH1D** ifsrvarDOWNInputComb = new TH1D*[19];
  TH1D* ifsrvarNominal = new TH1D;
  TH1D* ifsrvarUP = new TH1D;
  TH1D* ifsrvarDOWN = new TH1D;
  TH1D* ifsrvarUP_divided_sf = new TH1D;
  TH1D* ifsrvarDOWN_divided_sf = new TH1D;

  TString PATH_ifsrvarUP = Form("/prj_root/2671/top_write/savitsky/for_%s/%s_ttbar/controlplots/final/%s/%s/Central/%s/%s/%s/tight", lepType.Data(), treeType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data());

     TString FileName_isrn = Form("%s/ff_myhist_ttAisrn.root", PATH_ifsrvarUP.Data());
     TFile *f_isrn = new TFile(FileName_isrn.Data(),"READ");
     if(f_isrn==NULL) return 0; f_isrn->cd();
     TH1D *hist_isrn = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     ifsrvarNominal = hist_isrn;
     ifsrvarNominal -> Rebin(rebin_factor);
     ifsrvarNominal -> SetDirectory(gROOT);
     f_isrn->Close();

     TString FileName_isru = Form("%s/ff_myhist_ttAisru.root", PATH_ifsrvarUP.Data());
     TFile *f_isru = new TFile(FileName_isru.Data(),"READ");
     if(f_isru==NULL) return 0; f_isru->cd();
     TH1D *hist_isru = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     ifsrvarUP = hist_isru;
     ifsrvarUP -> Rebin(rebin_factor);
     ifsrvarUP -> SetDirectory(gROOT);
     f_isru->Close();

     TString FileName_isrd = Form("%s/ff_myhist_ttAisrd.root", PATH_ifsrvarUP.Data());
     TFile *f_isrd = new TFile(FileName_isrd.Data(),"READ");
     if(f_isrd==NULL) return 0; f_isrd->cd();
     TH1D *hist_isrd = (TH1D*) gDirectory->Get(Form("h_mva_%s_%s;1", max_next_string.Data(), jetBin.Data()));
     ifsrvarDOWN = hist_isrd;
     ifsrvarDOWN -> Rebin(rebin_factor);
     ifsrvarDOWN -> SetDirectory(gROOT);
     f_isrd->Close();

     ifsrvarUP_divided_sf->Sumw2();
     ifsrvarDOWN_divided_sf->Sumw2();
     ifsrvarUP_divided_sf =  ifsrvarNominal;
     ifsrvarDOWN_divided_sf =  ifsrvarNominal;

     ifsrvarUP_divided_sf->Divide(ifsrvarUP, ifsrvarNominal);
     ifsrvarDOWN_divided_sf->Divide(ifsrvarDOWN, ifsrvarNominal);

     //go to systematics
     ifsrvarUPInputComb[17] = allInputComb[17];
     ifsrvarUPInputComb[17] -> Multiply(allInputComb[17], ifsrvarUP_divided_sf);
     ifsrvarUPInputComb[16] = allInputComb[16];
     ifsrvarUPInputComb[16] -> Multiply(allInputComb[16], ifsrvarUP_divided_sf);

     ifsrvarDOWNInputComb[17] = allInputComb[17];
     ifsrvarDOWNInputComb[17] -> Multiply(allInputComb[17], ifsrvarDOWN_divided_sf);
     ifsrvarDOWNInputComb[16] = allInputComb[16];
     ifsrvarDOWNInputComb[16] -> Multiply(allInputComb[16], ifsrvarDOWN_divided_sf);


  // signal systematics
	cfile->createShapeSigSystematic("ifsrvar_sys", ifsrvarUPInputComb[17], ifsrvarDOWNInputComb[17], top_mass_point, -1,  -1,
					1.0, false, false, true);
  //Parameters -------> double scaleFactor=1.0, bool norm = false, bool flatten = false, bool testShape = true);
	//cout << " Create shape syst  " << "ifsrvar_sys" << " for  signal "
	//<< " [" << ifsrvarUPInputComb[17]->GetEntries() << "," <<  ifsrvarDOWNInputComb[17]->GetEntries() << "]" << endl;

  // background systematics - ttbar->dilepton only
     	cfile->createShapeBkgdSystematic(15, "ifsrvar_sys", ifsrvarUPInputComb[16], ifsrvarDOWNInputComb[16], top_mass_point, -1,  -1,
     					1.0, false, false, true);
     	//cout << " Create shape syst  " << "ifsrvar_sys" << " for  "  << TypeLeg[16]
	//<< " [" << ifsrvarUPInputComb[16]->GetEntries()  << "," <<  ifsrvarDOWNInputComb[16]->GetEntries() << "]" << endl;

*/
  ///store and output channel information
  cfile->storeFile();

cout<<"Correct average deviations in bkgd:"<<endl;
cout<<"bTag_sys: pos = "<<btagUP_avg<<" neg = "<<btagDOWN_avg<<endl;
cout<<"cTag_sys: pos = "<<ctagUP_avg<<" neg = "<<ctagDOWN_avg<<endl;
cout<<"lTag_sys: pos = "<<ltagUP_avg<<" neg = "<<ltagDOWN_avg<<endl;
cout<<"vc_sys: pos = "<<vcUP_avg<<" neg = "<<vcDOWN_avg<<endl;
cout<<"lepid_sys: pos = "<<lepidUP_avg<<" neg = "<<lepidDOWN_avg<<endl;
//cout<<"pdf_sys: pos = "<<pdfUP_avg<<" neg = "<<pdfDOWN_avg<<endl;
cout<<"JER_sys: pos = "<<JERUP_avg<<" neg = "<<JERDOWN_avg<<endl;
cout<<"tagga_sys: pos = "<<taggaUP_avg<<" neg = "<<taggaDOWN_avg<<endl;
//cout<<"bfrag_sys: pos = "<<bfragUP_avg<<endl;
cout<<"zwpt_sys: pos = "<<zwptUP_avg<<" neg = "<<zwptDOWN_avg<<endl;
cout<<"zvtxREW_sys: pos = "<<zvtxREWUP_avg<<" neg = "<<zvtxREWDOWN_avg<<endl;
cout<<"mc_stat_sys: pos = "<<mcUP_avg<<" neg = "<<mcDOWN_avg<<endl;

cout<<"Correct average deviations in signal:"<<endl;
cout<<"bTag_sys: pos = "<<btagUP_avg_sig<<" neg = "<<btagDOWN_avg_sig<<endl;
cout<<"cTag_sys: pos = "<<ctagUP_avg_sig<<" neg = "<<ctagDOWN_avg_sig<<endl;
cout<<"lTag_sys: pos = "<<ltagUP_avg_sig<<" neg = "<<ltagDOWN_avg_sig<<endl;
cout<<"vc_sys: pos = "<<vcUP_avg_sig<<" neg = "<<vcDOWN_avg_sig<<endl;
cout<<"lepid_sys: pos = "<<lepidUP_avg_sig<<" neg = "<<lepidDOWN_avg_sig<<endl;
//cout<<"pdf_sys: pos = "<<pdfUP_avg_sig<<" neg = "<<pdfDOWN_avg_sig<<endl;
cout<<"JER_sys: pos = "<<JERUP_avg_sig<<" neg = "<<JERDOWN_avg_sig<<endl;
cout<<"tagga_sys: pos = "<<taggaUP_avg_sig<<" neg = "<<taggaDOWN_avg_sig<<endl;
//cout<<"bfrag_sys: pos = "<<bfragUP_avg_sig<<endl;
cout<<"zwpt_sys: pos = "<<zwptUP_avg_sig<<" neg = "<<zwptDOWN_avg_sig<<endl;
cout<<"zvtxREW_sys: pos = "<<zvtxREWUP_avg_sig<<" neg = "<<zvtxREWDOWN_avg_sig<<endl;
cout<<"mc_stat_sys: pos = "<<mcUP_avg_sig<<" neg = "<<mcDOWN_avg_sig<<endl;

TString period_prefit_folder_txt = Form("/prj_root/2671/top_write/savitsky/collie/xsec_ljets/xsec_output/controlplots/txt_sys/%s", runPeriod.Data());
TString add_txt = Form("%s/sysavdev_%s_%s_%s_%s_%s_%s_%s_%s.txt", period_prefit_folder_txt.Data(), runPeriod.Data(), cutSet.Data(), sysSet.Data(), lepType.Data(), tagBin.Data(), WP.Data(), max_next_label.Data(), jetBin.Data());
ofstream myfile;
myfile.open(add_txt);
myfile<<"Correct average deviations in bkgd:"<<endl;
myfile<<"bTag_sys: pos = "<<btagUP_avg<<" neg = "<<btagDOWN_avg<<endl;
myfile<<"cTag_sys: pos = "<<ctagUP_avg<<" neg = "<<ctagDOWN_avg<<endl;
myfile<<"lTag_sys: pos = "<<ltagUP_avg<<" neg = "<<ltagDOWN_avg<<endl;
myfile<<"vc_sys: pos = "<<vcUP_avg<<" neg = "<<vcDOWN_avg<<endl;
myfile<<"lepid_sys: pos = "<<lepidUP_avg<<" neg = "<<lepidDOWN_avg<<endl;
//myfile<<"pdf_sys: pos = "<<pdfUP_avg<<" neg = "<<pdfDOWN_avg<<endl;
myfile<<"JER_sys: pos = "<<JERUP_avg<<" neg = "<<JERDOWN_avg<<endl;
myfile<<"tagga_sys: pos = "<<taggaUP_avg<<" neg = "<<taggaDOWN_avg<<endl;
//myfile<<"bfrag_sys: pos = "<<bfragUP_avg<<endl;
myfile<<"zwpt_sys: pos = "<<zwptUP_avg<<" neg = "<<zwptDOWN_avg<<endl;
myfile<<"zvtxREW_sys: pos = "<<zvtxREWUP_avg<<" neg = "<<zvtxREWDOWN_avg<<endl;
myfile<<"mc_stat_sys: pos = "<<mcUP_avg<<" neg = "<<mcDOWN_avg<<endl;


myfile<<"Correct average deviations in signal:"<<endl;
myfile<<"bTag_sys: pos = "<<btagUP_avg_sig<<" neg = "<<btagDOWN_avg_sig<<endl;
myfile<<"cTag_sys: pos = "<<ctagUP_avg_sig<<" neg = "<<ctagDOWN_avg_sig<<endl;
myfile<<"lTag_sys: pos = "<<ltagUP_avg_sig<<" neg = "<<ltagDOWN_avg_sig<<endl;
myfile<<"vc_sys: pos = "<<vcUP_avg_sig<<" neg = "<<vcDOWN_avg_sig<<endl;
myfile<<"lepid_sys: pos = "<<lepidUP_avg_sig<<" neg = "<<lepidDOWN_avg_sig<<endl;
//myfile<<"pdf_sys: pos = "<<pdfUP_avg_sig<<" neg = "<<pdfDOWN_avg_sig<<endl;
myfile<<"JER_sys: pos = "<<JERUP_avg_sig<<" neg = "<<JERDOWN_avg_sig<<endl;
myfile<<"tagga_sys: pos = "<<taggaUP_avg_sig<<" neg = "<<taggaDOWN_avg_sig<<endl;
//myfile<<"bfrag_sys: pos = "<<bfragUP_avg_sig<<endl;
myfile<<"zwpt_sys: pos = "<<zwptUP_avg_sig<<" neg = "<<zwptDOWN_avg_sig<<endl;
myfile<<"zvtxREW_sys: pos = "<<zvtxREWUP_avg_sig<<" neg = "<<zvtxREWDOWN_avg_sig<<endl;
myfile<<"mc_stat_sys: pos = "<<mcUP_avg_sig<<" neg = "<<mcDOWN_avg_sig<<endl;
myfile.close();
cout<<"...Stored to: "<<add_txt<<std::endl<<std::endl;

}
