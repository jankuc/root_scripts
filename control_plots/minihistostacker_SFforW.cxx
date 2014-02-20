//*******************************************//
//               minihistostacker            //
//*******************************************//

#include "TROOT.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TSelector.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TEventList.h"
#include "TEntryList.h"
#include "TString.h"
#include "TBranch.h"
#include "TSystem.h"
#include "TFunction.h"
#include "TVector3.h"
#include "TMath.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TProfile.h"


#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <math.h>
#include <string.h>
#include <math.h>
#include "TMath.h"
#include <iomanip>

using namespace std;

TString outputDir = "/work/budvar-clued0/fjfi-D0/tt_leptonjets/results_hist/";
// /work/budvar-clued0/kuceraja/tt_leptonjets/samples/incl_mva_max_next/WSscaleFactors

//Scale Factors Definition Begins/////////////////////////////////////////////////////////

Int_t m_ltf;//1-ej, 2-mj
Int_t m_rpf;//0-IIb, 1-IIb1, 2-IIb2, 3-IIb34, 4-IIa, 5-II
Int_t m_jbf;//0-2jb, 1-3jb, 2-4jb
Int_t m_tbf;//0-bt, 1-0t, 2-1t, 3-2t, 4-i1t

Double_t m_ttbar_sf_lj = 1.0;
Double_t m_ttbar_sf_ll = 1.0;
Double_t m_mjets_sf = 1.0;
Double_t m_wjets_sf_2 = 1.0;
Double_t m_wjets_sf_3 = 1.0;
Double_t m_wjets_sf_4 = 1.0;
Double_t stat_for_wjets_sf_2[25] = {0.0};
Double_t stat_for_wjets_sf_3[25] = {0.0};
Double_t stat_for_wjets_sf_4[25] = {0.0};
Double_t ov_stat_for_wjets_sf_2 = 0.0;
Double_t ov_stat_for_wjets_sf_3 = 0.0;
Double_t ov_stat_for_wjets_sf_4 = 0.0;
Double_t ov_wstat_for_wjets_sf_2 = 0.0;
Double_t ov_wstat_for_wjets_sf_3 = 0.0;
Double_t ov_wstat_for_wjets_sf_4 = 0.0;
Double_t lumi_RunIIb = 8655.69;
Double_t lumi_RunIIb1 = 1217.67;
Double_t lumi_RunIIb2 = 3039.84;
Double_t lumi_RunIIb34 = 4398.18;
Double_t lumi_RunII = 9736.90;
Double_t lumi_RunIIa = 1081.21;
Double_t BR_mujets = 0.17137;
Double_t BR_ejets = 0.1721;
Double_t BR_ljets = 0.34347;
Double_t llBR_mujets = 0.06607;
Double_t llBR_ejets = 0.06627;
Double_t llBR_ljets = 0.13234;
Double_t fake_xsec = 7.48;

Int_t pdf_flag = 0; //0-central, 1-positive, 2-negative

void sf_processor(Double_t &ttbar_sf_lj, Double_t &ttbar_sf_ll, Double_t &mjets_sf, Int_t ltf, Int_t rpf, Int_t jbf, Int_t tbf) {
  switch (rpf) {
    case 0: switch(ltf) {//RunIIb
        case 0: ttbar_sf_lj = lumi_RunIIb*fake_xsec*BR_ljets;//ljets
          ttbar_sf_ll = lumi_RunIIb*fake_xsec*llBR_ljets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 1: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 2: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 3: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 4: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 1: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 2: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 3: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 4: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 1: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 2: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 3: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 4: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        case 1: ttbar_sf_lj = lumi_RunIIb*fake_xsec*BR_ejets;//ejets
          ttbar_sf_ll = lumi_RunIIb*fake_xsec*llBR_ejets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 0.34615;//!
                  //wjets_sf = 0.94679;//!
                  break;
                case 1: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 2: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 3: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 4: mjets_sf = 0.32412;//!
                  //wjets_sf = 0.94679;//!
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 0.37513;
                  //wjets_sf = 1.07323;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.31801;
                  //wjets_sf = 1.07323;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 0.35138;
                  //wjets_sf = 1.47389;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.20830;
                  //wjets_sf = 1.47389;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        case 2: ttbar_sf_lj = lumi_RunIIb*fake_xsec*BR_mujets;//mujets
          ttbar_sf_ll = lumi_RunIIb*fake_xsec*llBR_mujets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 0.12884;
                  //wjets_sf = 1.00253;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.15174;
                  //wjets_sf = 1.00253;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 0.16489;
                  //wjets_sf = 1.20713;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.15669;
                  //wjets_sf = 1.20713;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 0.16200;
                  //wjets_sf = 1.65860;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.13041;
                  //wjets_sf = 1.65860;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        default: cout<<"Wrong lepton type!"<<endl;
          break;
        }
      break;
    case 1: switch(ltf) {//RunIIb1
        case 0: ttbar_sf_lj = lumi_RunIIb1*fake_xsec*BR_ljets;//ljets
          ttbar_sf_ll = lumi_RunIIb1*fake_xsec*llBR_ljets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        case 1: ttbar_sf_lj = lumi_RunIIb1*fake_xsec*BR_ejets;//ejets
          ttbar_sf_ll = lumi_RunIIb1*fake_xsec*llBR_ejets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 0.31268;
                  //wjets_sf = 0.94457;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.29801;
                  //wjets_sf = 0.94457;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 0.34772;
                  //wjets_sf = 1.03662;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.27361;
                  //wjets_sf = 1.03662;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 0.31346;
                  //wjets_sf = 1.40493;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.18941;
                  //wjets_sf = 1.40493;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        case 2: ttbar_sf_lj = lumi_RunIIb1*fake_xsec*BR_mujets;//mujets
          ttbar_sf_ll = lumi_RunIIb1*fake_xsec*llBR_mujets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 0.12180;
                  //wjets_sf = 0.93789;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.14093;
                  //wjets_sf = 0.93789;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 0.15721;
                  //wjets_sf = 1.12717;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.13562;
                  //wjets_sf = 1.12717;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 0.16487;
                  //wjets_sf = 1.35876;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.14757;
                  //wjets_sf = 1.35876;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        default: cout<<"Wrong lepton type!"<<endl;
          break;
        }
      break;
    case 2: switch(ltf) {//RunIIb2
        case 0: ttbar_sf_lj = lumi_RunIIb2*fake_xsec*BR_ljets;//ljets
          ttbar_sf_ll = lumi_RunIIb2*fake_xsec*llBR_ljets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                case 1: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                case 2: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                case 3: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                case 4: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                case 1: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                case 2: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                case 3: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                case 4: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                case 1: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                case 2: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                case 3: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                case 4: mjets_sf = 1.00;//!
                  //wjets_sf = 1.00;//!
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        case 1: ttbar_sf_lj = lumi_RunIIb2*fake_xsec*BR_ejets;//ejets
          ttbar_sf_ll = lumi_RunIIb2*fake_xsec*llBR_ejets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 0.35287;//
                  //wjets_sf = 0.96280;//
                  break;
                case 1: mjets_sf = 1.0;//!
                  //wjets_sf = 1.0;//!
                  break;
                case 2: mjets_sf = 1.0;//
                  //wjets_sf = 1.0;//
                  break;
                case 3: mjets_sf = 1.0;//
                  //wjets_sf = 1.0;//
                  break;
                case 4: mjets_sf = 0.32273;//
                  //wjets_sf = 0.96280;//
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 0.37831;//
                  //wjets_sf = 1.16554;//
                  break;
                case 1: mjets_sf = 1.0;//!!
                  //wjets_sf = 1.0;//!!
                  break;
                case 2: mjets_sf = 1.0;//
                  //wjets_sf = 1.0;//
                  break;
                case 3: mjets_sf = 1.0;//
                  //wjets_sf = 1.0;//
                  break;
                case 4: mjets_sf = 0.30920;//
                  //wjets_sf = 1.16554;//
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 0.35353;//
                  //wjets_sf = 1.44927;//
                  break;
                case 1: mjets_sf = 1.0;//!!
                  //wjets_sf = 1.0;//!!
                  break;
                case 2: mjets_sf = 1.0;//
                  //wjets_sf = 1.0;//
                  break;
                case 3: mjets_sf = 1.0;//
                  //wjets_sf = 1.0;//
                  break;
                case 4: mjets_sf = 0.22831;//
                  //wjets_sf = 1.44927;//
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        case 2: ttbar_sf_lj = lumi_RunIIb2*fake_xsec*BR_mujets;//mujets
          ttbar_sf_ll = lumi_RunIIb2*fake_xsec*llBR_mujets;//mujets
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 0.12974;
                  //wjets_sf = 1.04436;
                  break;
                case 1: mjets_sf = 1.0;//!!
                  //wjets_sf = 1.0;//!!
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.15394;
                  //wjets_sf = 1.04436;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 0.16585;
                  //wjets_sf = 1.25117;
                  break;
                case 1: mjets_sf = 1.0;//!!
                  //wjets_sf = 1.0;//!!
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.14904;
                  //wjets_sf = 1.25117;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 0.14343;
                  //wjets_sf = 1.99087;
                  break;
                case 1: mjets_sf = 1.0;//!!
                  //wjets_sf = 1.0;//!!
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.11919;
                  //wjets_sf = 1.99087;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        default: cout<<"Wrong lepton type!"<<endl;
          break;
        }
      break;
    case 3: switch(ltf) {//RunIIb34
        case 0: ttbar_sf_lj = lumi_RunIIb34*fake_xsec*BR_ljets;//ljets
          ttbar_sf_ll = lumi_RunIIb34*fake_xsec*llBR_ljets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        case 1: ttbar_sf_lj = lumi_RunIIb34*fake_xsec*BR_ejets;//ejets
          ttbar_sf_ll = lumi_RunIIb34*fake_xsec*llBR_ejets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 0.31728;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.30064;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 0.34306;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.29995;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 0.32310;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.18412;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        case 2: ttbar_sf_lj = lumi_RunIIb34*fake_xsec*BR_mujets;//mujets
          ttbar_sf_ll = lumi_RunIIb34*fake_xsec*llBR_mujets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 0.13556;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.15882;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 0.17260;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.17219;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 0.17721;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.13700;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        default: cout<<"Wrong lepton type!"<<endl;
          break;
        }
      break;
    case 4: switch(ltf) {//RunIIa
        case 0: ttbar_sf_lj = lumi_RunIIa*fake_xsec*BR_ljets;//ljets
          ttbar_sf_ll = lumi_RunIIa*fake_xsec*llBR_ljets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        case 1: ttbar_sf_lj = lumi_RunIIa*fake_xsec*BR_ejets;//ejets
          ttbar_sf_ll = lumi_RunIIa*fake_xsec*llBR_ejets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 0.35479;
                  //wjets_sf = 0.86523;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.28912;
                  //wjets_sf = 0.86523;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 0.42307;
                  //wjets_sf = 0.94709;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.34348;
                  //wjets_sf = 0.94709;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 0.41964;
                  //wjets_sf = 1.12231;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.35166;
                  //wjets_sf = 1.12231;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        case 2: ttbar_sf_lj = lumi_RunIIa*fake_xsec*BR_mujets;//mujets
          ttbar_sf_ll = lumi_RunIIa*fake_xsec*llBR_mujets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 0.29029;
                  //wjets_sf = 0.71332;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.31634;
                  //wjets_sf = 0.71332;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 0.33963;
                  //wjets_sf = 0.80768;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.34690;
                  //wjets_sf = 0.80768;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 0.33100;
                  //wjets_sf = 0.93209;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.26440;
                  //wjets_sf = 0.93209;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        default: cout<<"Wrong lepton type!"<<endl;
          break;
        }
      break;
    case 5: switch(ltf) {//RunII
        case 0: ttbar_sf_lj = lumi_RunII*fake_xsec*BR_ljets;//ljets
          ttbar_sf_ll = lumi_RunII*fake_xsec*llBR_ljets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        case 1: ttbar_sf_lj = lumi_RunII*fake_xsec*BR_ejets;//ejets
          ttbar_sf_ll = lumi_RunII*fake_xsec*llBR_ejets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 0.36342;
                  //wjets_sf = 0.92439;//0.92434
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.34004;
                  //wjets_sf = 0.92439;//0.92434
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 0.39607;
                  //wjets_sf = 1.03362;//1.03361
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.33554;
                  //wjets_sf = 1.03362;//1.03361
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 0.37233;
                  //wjets_sf = 1.37612;//1.37610
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.23442;
                  //wjets_sf = 1.37612;//1.37610
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        case 2: ttbar_sf_lj = lumi_RunII*fake_xsec*BR_mujets;//mujets
          ttbar_sf_ll = lumi_RunII*fake_xsec*llBR_mujets;
          switch(jbf) {
            case 0: switch(tbf) {//2jb
                case 0: mjets_sf = 0.15416;
                  //wjets_sf = 0.94920;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.17987;
                  //wjets_sf = 0.94920;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 1: switch(tbf) {//3jb
                case 0: mjets_sf = 0.19517;
                  //wjets_sf = 1.12237;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.18877;
                  //wjets_sf = 1.12237;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            case 2: switch(tbf) {//4jb
                case 0: mjets_sf = 0.19095;
                  //wjets_sf = 1.47320;
                  break;
                case 1: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 2: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 3: mjets_sf = 1.0;
                  //wjets_sf = 1.0;
                  break;
                case 4: mjets_sf = 0.15263;
                  //wjets_sf = 1.47320;
                  break;
                default: cout<<"Wrong tag bin!"<<endl;
                  break;
                }
              break;
            default: cout<<"Wrong jet bin!"<<endl;
              break;
            }
          break;
        default: cout<<"Wrong lepton type!"<<endl;
          break;
        }
      break;
    default: cout<<"Wrong run period!"<<endl;
      break;
    }
}

//Scale Factors Definition Ends///////////////////////////////////////////////////////////

void Usage() {
  printf("./mhst.exe => User's input: customize  'inPATH' TString to suit actual path to your folder\n");
  printf("Arguments order: <treeType> <run_period> <cut_set> <syst_setup> <channel> <tag_bin> <working_point> \n");
}

int main(int argc, char* argv[])
{

  gROOT->Reset();
  if(argc != 8) { Usage();
      exit(1);
    }

  TString treeType = argv[1];
  TString runPeriod = argv[2];
  TString cutSet = argv[3];
  TString sysSet = argv[4];
  TString lepType = argv[5];
  TString tagBin =  argv[6];
  TString WP = argv[7];
  TString looseTight;

  std::cout<<std::endl<<"Epoch: "<<runPeriod<<"     TreeType:"<<treeType<<std::endl;

  if (!strcmp(argv[2], "RunIIb")) m_rpf = 0;
  if (!strcmp(argv[2], "RunIIb1")) m_rpf = 1;
  if (!strcmp(argv[2], "RunIIb2")) m_rpf = 2;
  if (!strcmp(argv[2], "RunIIb34")) m_rpf = 3;
  if (!strcmp(argv[2], "RunIIa")) m_rpf = 4;
  if (!strcmp(argv[2], "RunII")) m_rpf = 5;

  if (!strcmp(argv[5], "ejets")) m_ltf = 1;
  if (!strcmp(argv[5], "mujets")) m_ltf = 2;

  if (!strcmp(argv[6], "beforeTag")) m_tbf = 0;
  if (!strcmp(argv[6], "0Tag")) m_tbf = 1;
  if (!strcmp(argv[6], "1Tag")) m_tbf = 2;
  if (!strcmp(argv[6], "2Tag")) m_tbf = 3;
  if (!strcmp(argv[6], "inc1Tag")) m_tbf = 4;

  /*Int_t ltf;//1-ej, 2-mj
    Int_t rpf;//0-IIb, 1-IIb1, 2-IIb2, 3-IIb34, 4-IIa, 5-II
    Int_t jbf;//0-2jb, 1-3jb, 2-4jb
    Int_t tbf;//0-bt, 1-0t, 2-1t, 3-2t, 4-i1t*/

  const Int_t Ntypes = 18;//!all - 18, ttbar - 12, topmass - 20

  TString Type_CP;
  TString Type_HF;

  Int_t mass_point = 172;

  TString Type_CPmu[Ntypes] = {"data","QCD","Wlp","Wcc","Wbb","ZbbMuMu","ZbbTauTau","ZccMuMu","ZccTauTau","ZlpMuMu","ZlpTauTau","tb","tqb","WW","WZ","ZZ","ttAll_172","ttA_172"};
  TString Type_CPe[Ntypes] = {"data","QCD","Wlp","Wcc","Wbb","ZbbEE","ZbbTauTau","ZccEE","ZccTauTau","ZlpEE","ZlpTauTau","tb","tqb","WW","WZ","ZZ","ttAll_172","ttA_172"};
  TString Type_HFmu[Ntypes] = {"data","QCD","WJet0","WJetC","WJetB","ZbbMuMu","ZbbTauTau","ZccMuMu","ZccTauTau","ZlpMuMu","ZlpTauTau","tb","tbq","WW","WZ","ZZ","ttll","TTbar"};
  TString Type_HFe[Ntypes] = {"data","QCD","WJet0","WJetC","WJetB","ZbbEE","ZbbTauTau","ZccEE","ZccTauTau","ZlpEE","ZlpTauTau","tb","tbq","WW","WZ","ZZ","ttll","TTbar"};


  /*TString Type_CPmu[Ntypes] = {"ttAHer_172", "ttAisrd", "ttAisrn", "ttAisru", "ttA_170", "ttA_175", "ttAll_170", "ttAll_175", "ttMCNLO_172", "ttMCNLO_LJETS_172", "ttA_P2011", "ttA_P2011NOCR"};
    TString Type_CPe[Ntypes] = {"ttAHer_172", "ttAisrd", "ttAisrn", "ttAisru", "ttA_170", "ttA_175", "ttAll_170", "ttAll_175", "ttMCNLO_172", "ttMCNLO_LJETS_172", "ttA_P2011", "ttA_P2011NOCR"};
    TString Type_HFmu[Ntypes] = {"ttAHer_172", "ttAisrd", "ttAisrn", "ttAisru", "ttA_170", "ttA_175", "ttAll_170", "ttAll_175", "ttMCNLO_172", "ttMCNLO_LJETS_172", "ttA_P2011", "ttA_P2011NOCR"};
    TString Type_HFe[Ntypes] = {"ttAHer_172", "ttAisrd", "ttAisrn", "ttAisru", "ttA_170", "ttA_175", "ttAll_170", "ttAll_175", "ttMCNLO_172", "ttMCNLO_LJETS_172", "ttA_P2011", "ttA_P2011NOCR"};*/

  /*TString Type_CPmu[Ntypes] = {"ttA_140", "ttA_150", "ttA_160", "ttA_165", "ttA_170", "ttA_172", "ttA_175", "ttA_180", "ttA_185", "ttA_190", "ttAll_140", "ttAll_150", "ttAll_160", "ttAll_165", "ttAll_170", "ttAll_172", "ttAll_175", "ttAll_180", "ttAll_185", "ttAll_190"};
    TString Type_CPe[Ntypes] = {"ttA_140", "ttA_150", "ttA_160", "ttA_165", "ttA_170", "ttA_172", "ttA_175", "ttA_180", "ttA_185", "ttA_190", "ttAll_140", "ttAll_150", "ttAll_160", "ttAll_165", "ttAll_170", "ttAll_172", "ttAll_175", "ttAll_180", "ttAll_185", "ttAll_190"};
    TString Type_HFmu[Ntypes] = {"ttA_140", "ttA_150", "ttA_160", "ttA_165", "ttA_170", "ttA_172", "ttA_175", "ttA_180", "ttA_185", "ttA_190", "ttAll_140", "ttAll_150", "ttAll_160", "ttAll_165", "ttAll_170", "ttAll_172", "ttAll_175", "ttAll_180", "ttAll_185", "ttAll_190"};
    TString Type_HFe[Ntypes] = {"ttA_140", "ttA_150", "ttA_160", "ttA_165", "ttA_170", "ttA_172", "ttA_175", "ttA_180", "ttA_185", "ttA_190", "ttAll_140", "ttAll_150", "ttAll_160", "ttAll_165", "ttAll_170", "ttAll_172", "ttAll_175", "ttAll_180", "ttAll_185", "ttAll_190"};*/

  for(Int_t type_index = 0; type_index < Ntypes; type_index++) {

      //if (type_index == 0 || type_index == 4 || type_index == 5 || type_index == 6 || type_index == 7) continue;
      //if (type_index == 0 || type_index == 1 || type_index == 2 || type_index == 3 || type_index == 8) continue;

      if (type_index == 1) looseTight = "loose-tight"; // for QCD
      else looseTight = "tight";

      //looseTight = "tight"; //!for ttbar and topmass
      TString lepNNTrees;
      if (lepType.CompareTo("mujets")==0) {
          Type_CP = Type_CPmu[type_index];
          Type_HF = Type_HFmu[type_index];
          lepNNTrees = "muo";
        } else if (lepType.CompareTo("ejets")==0) {
          Type_CP = Type_CPe[type_index];
          Type_HF = Type_HFe[type_index];
          lepNNTrees = "ele";
        } else {
          std::cout<<"Wrong channel name: 'mujets' or 'ejets' only! You've used: '"<<lepType<<"'."<<std::endl;
        }

      //    TString inPATH = Form("/prj_root/2671/top_write/savitsky/%s/controlplots/final/%s/%s/%s/%s/%s/%s/%s/%s_miniTree.root", treeType.Data(), runPeriod.Data(), cutSet.Data(), sysSet.Data(), lepType.Data(), tagBin.Data(), WP.Data(), looseTight.Data(), Type_CP.Data());
      //    if(type_index == 0) inPATH = Form("/prj_root/2671/top_write/savitsky/for_%s/final_tt13_dIbad/controlplots/final/%s/%s/Central/%s/%s/%s/%s/%s_miniTree.root", lepType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data(), looseTight.Data(), Type_CP.Data());
      //    if(type_index == 1) inPATH = Form("/prj_root/2671/top_write/savitsky/for_%s/final_tt13_dIbad/controlplots/final/%s/%s/Central/%s/%s/%s/%s/%s_miniTree.root", lepType.Data(), runPeriod.Data(), cutSet.Data(), lepType.Data(), tagBin.Data(), WP.Data(), looseTight.Data(), Type_CP.Data());

      TString inPATH = Form("/work/budvar-clued0/kuceraja/tt_leptonjets/samples/split_trees_3samples_%s_1119/%s_miniTree.root", lepNNTrees.Data(), Type_CP.Data());
      //TString inPATH = Form("/work/dude-clued0/ajung/D0/TopStuff/Lidiaz_TMVA/split_trees_3samples_%s_1119/%s_miniTree.root", lepNNTrees.Data(), Type_CP.Data());
      if(type_index == 0) inPATH = Form("/work/budvar-clued0/kuceraja/tt_leptonjets/samples/incl_mva_max_next/split_trees_3samples_%s_1119/%s_miniTree.root", lepNNTrees.Data(), Type_CP.Data());
      if(type_index == 1) inPATH = Form("/work/budvar-clued0/kuceraja/tt_leptonjets/samples/incl_mva_max_next/split_trees_3samples_%s_1119/%s_miniTree.root", lepNNTrees.Data(), Type_CP.Data());

      //TString outPATH = Form("/prj_root/2671/top_write/savitsky/%s/controlplots/final/%s/%s/%s/%s/%s/%s/tight/ff_myhist_%s.root", treeType.Data(), runPeriod.Data(), cutSet.Data(), sysSet.Data(), lepType.Data(), tagBin.Data(), WP.Data(), Type_HF.Data());

      //TFile *fadd = new TFile(inPATH.Data(),"READ");
      //	std::cout<<"Data added."<<std::endl;

      //std::cout<<gDirectory->GetName()<<endl;

      //TH1F *addsfhist = (TH1F*) gDirectory->Get("addSF;1");

      //addsfhist->SetDirectory(gROOT);

      //fadd->Close();

      std::cout<<"Run Period:  '"<<runPeriod<<"'   Tag Bin:  '"<<tagBin<<"'   Sample:  '"<<Type_CP<<std::endl<<"'";
      TChain *fChain= new TChain("nn_tree");
      fChain->Add(inPATH);
      fChain->SetBranchStatus("*", 0);

      //TFile *mini_file = new TFile(outPATH,"RECREATE");
      //if(!mini_file) { std::cout<<"\t Error"<<std::endl; return 0;}
      //Declaration of leaf types

      Int_t	_njt = 999;
      Double_t	_mva_max;
      Double_t	_mva_max_next;
      Float_t	_weight;
      //Float_t _weight_pdf[41] = {0.0};
      Float_t _weight_twoJet = 1.0; // =0
      Float_t _weight_threeJet = 1.0; // =0
      Float_t _weight_fourJet = 1.0; // =0
      Float_t _weight_pdf_Shape = 1.0;

      //        _weight_twoJet = addsfhist->GetBinContent(6)*addsfhist->GetBinContent(2);
      //        _weight_threeJet = addsfhist->GetBinContent(7)*addsfhist->GetBinContent(3);
      //        _weight_fourJet = addsfhist->GetBinContent(8)*addsfhist->GetBinContent(4);

      //        if(type_index == 0 || type_index == 1 ) { //jet weights are empty for data and QCD
      //            _weight_twoJet = 1.0;
      //            _weight_threeJet = 1.0;
      //            _weight_fourJet = 1.0;
      //            _weight_pdf_Shape = 1.0;
      //        }

      fChain->SetBranchStatus("NJets", 1);  // Process these branches
      fChain->SetBranchStatus("Mva_max", 1);
      fChain->SetBranchStatus("Mva_max_next", 1);  // COMMENTED OUT, BECAUSE WE DONT HAVE THESE VARIABLES  IN OUR *.root FILES
      fChain->SetBranchStatus("Weight", 1);
      //fChain->SetBranchStatus("weight_pdf", 1);

      fChain->SetBranchAddress("NJets", &_njt);
      fChain->SetBranchAddress("Mva_max", &_mva_max);
      fChain->SetBranchAddress("Mva_max", &_mva_max_next);
      fChain->SetBranchAddress("Weight", &_weight);
      //fChain->SetBranchAddress("Weight", &_weight_pdf);

      TH1D *h_mva_max_2jb = new TH1D("h_mva_max_2jb","mva_max_2jb",20,0.,1.);
      TH1D *h_mva_max_3jb = new TH1D("h_mva_max_3jb","mva_max_3jb",20,0.,1.);
      TH1D *h_mva_max_4jb = new TH1D("h_mva_max_4jb","mva_max_4jb",20,0.,1.);

      TH1D *h_mva_max_next_2jb = new TH1D("h_mva_max_next_2jb","mva_max_next_2jb",20,0.,1.);
      TH1D *h_mva_max_next_3jb = new TH1D("h_mva_max_next_3jb","mva_max_next_3jb",20,0.,1.);
      TH1D *h_mva_max_next_4jb = new TH1D("h_mva_max_next_4jb","mva_max_next_4jb",20,0.,1.);

      TH2D *h_mva_2D_inc2jb = new TH2D("h_mva_2D_inc2jb", "mva_2D (x-max, y-next)", 20, 0., 1., 20, 0., 1.);// (2jb,3jb,4jb (it is actually inc) filled)
      TH2D *h_mva_2D_2jb = new TH2D("h_mva_2D_2jb", "mva_2D (x-max, y-next)", 20, 0., 1., 20, 0., 1.);
      TH2D *h_mva_2D_3jb = new TH2D("h_mva_2D_3jb", "mva_2D (x-max, y-next)", 20, 0., 1., 20, 0., 1.);
      TH2D *h_mva_2D_4jb = new TH2D("h_mva_2D_4jb", "mva_2D (x-max, y-next)", 20, 0., 1., 20, 0., 1.);

      TH1F *h_NJets= new TH1F("h_NJets","NJets",10,0.,10.);

      h_mva_max_2jb->Sumw2();  //SHOULDNT BE COMMENTED OUT.
      h_mva_max_3jb->Sumw2();
      h_mva_max_4jb->Sumw2();
      h_mva_max_next_2jb->Sumw2();
      h_mva_max_next_3jb->Sumw2();
      h_mva_max_next_4jb->Sumw2();
      h_mva_2D_2jb->Sumw2();
      h_mva_2D_3jb->Sumw2();
      h_mva_2D_4jb->Sumw2();
      h_mva_2D_inc2jb->Sumw2();
      h_NJets->Sumw2();

      Long64_t nentries = fChain->GetEntries();
      Float_t overall_stat = 0.0;

      for(Long64_t nEvt=0; nEvt<nentries; nEvt++){

          fChain->GetEntry(nEvt);
          // mva_max has to be in <0,1>
          if(_mva_max < 0.0) _mva_max = 0.0;
          if(_mva_max > 1.0) _mva_max = 1.0;
          // mva_max_next has to be in <0,1>
          if(_mva_max_next < 0.0) _mva_max_next = 0.0;
          if(_mva_max_next > 1.0) _mva_max_next = 1.0;

          // this won't happen, because we have pdf_flag == 0
//          if(type_index != 0 && type_index != 1 && pdf_flag !=0 ) {
//              _weight_pdf_Shape = 1.0;
//              if(pdf_flag == 1) {
//                  for(Int_t kk=1; kk<40; kk+=2) {//0 - central, 1-20 - pos, 21-40 - neg
//                      _weight_pdf_Shape = _weight_pdf_Shape + ((_weight_pdf[0] - _weight_pdf[kk])/_weight_pdf[0])*((_weight_pdf[0] - _weight_pdf[kk])/_weight_pdf[0]);
//                    }
//                  _weight_pdf_Shape = sqrt(_weight_pdf_Shape) + 1.0;
//                }
//              if(pdf_flag == 2) {
//                  for(Int_t kk=2; kk<41; kk+=2) {//0 - central, 1-20 - pos, 21-40 - neg
//                      _weight_pdf_Shape = _weight_pdf_Shape + ((_weight_pdf[0] - _weight_pdf[kk])/_weight_pdf[0])*((_weight_pdf[0] - _weight_pdf[kk])/_weight_pdf[0]);
//                    }
//                  _weight_pdf_Shape = 1.0 - sqrt(_weight_pdf_Shape);
//                }
//            }


          //if(nEvt == 100) std::cout<<type_index<<" Shape = "<<_weight_pdf_Shape<<std::endl;
          //if(nEvt == 200) std::cout<<type_index<<" Shape = "<<_weight_pdf_Shape<<std::endl;
          _weight_pdf_Shape = 1.0;
          //std::cout<<"NJets = "<<_njt<<endl;
          if(_njt == 2) {
              h_mva_max_2jb->Fill(_mva_max, _weight*_weight_pdf_Shape);
              h_mva_max_next_2jb->Fill(_mva_max_next, _weight*_weight_pdf_Shape);
              h_mva_2D_2jb->Fill(_mva_max, _mva_max_next, _weight*_weight_pdf_Shape);
              overall_stat = overall_stat + _weight;
            }
          if(_njt == 3) {
              h_mva_max_3jb->Fill(_mva_max, _weight*_weight_pdf_Shape);
              h_mva_max_next_3jb->Fill(_mva_max_next, _weight*_weight_pdf_Shape);
              h_mva_2D_3jb->Fill(_mva_max, _mva_max_next, _weight*_weight_pdf_Shape);
              overall_stat = overall_stat + _weight;
            }
          if(_njt >= 4) {
              h_mva_max_4jb->Fill(_mva_max, _weight*_weight_pdf_Shape);
              h_mva_max_next_4jb->Fill(_mva_max_next, _weight*_weight_pdf_Shape);
              h_mva_2D_4jb->Fill(_mva_max, _mva_max_next, _weight*_weight_pdf_Shape);
              overall_stat = overall_stat + _weight;
            }
          h_NJets->Fill(_njt);
        }

      if (type_index != 0 && type_index != 1 ) { //jet weights are empty for data and QCD
          h_mva_max_2jb->Scale(_weight_twoJet/h_mva_max_2jb->Integral());
          h_mva_max_3jb->Scale(_weight_threeJet/h_mva_max_3jb->Integral());
          h_mva_max_4jb->Scale(_weight_fourJet/h_mva_max_4jb->Integral());
          h_mva_max_next_2jb->Scale(_weight_twoJet/h_mva_max_next_2jb->Integral());
          h_mva_max_next_3jb->Scale(_weight_threeJet/h_mva_max_next_3jb->Integral());
          h_mva_max_next_4jb->Scale(_weight_fourJet/h_mva_max_next_4jb->Integral());
          h_mva_2D_2jb->Scale(_weight_twoJet/h_mva_2D_2jb->Integral());
          h_mva_2D_3jb->Scale(_weight_threeJet/h_mva_2D_3jb->Integral());
          h_mva_2D_4jb->Scale(_weight_fourJet/h_mva_2D_4jb->Integral());
        }
      // 		std::cout<<"after if"<<endl;
      //2jb scaling
      m_jbf = 0;
      sf_processor(m_ttbar_sf_lj, m_ttbar_sf_ll, m_mjets_sf, m_ltf, m_rpf, m_jbf, m_tbf);
      if (type_index == 1) {
          h_mva_max_2jb->Scale(m_mjets_sf);
          h_mva_max_next_2jb->Scale(m_mjets_sf);
          h_mva_2D_2jb->Scale(m_mjets_sf);
        } else if (type_index == 16) {
          h_mva_max_2jb->Scale(m_ttbar_sf_ll);
          h_mva_max_next_2jb->Scale(m_ttbar_sf_ll);
          h_mva_2D_2jb->Scale(m_ttbar_sf_ll);
        } else if (type_index == 17) {
          h_mva_max_2jb->Scale(m_ttbar_sf_lj);
          h_mva_max_next_2jb->Scale(m_ttbar_sf_lj);
          h_mva_2D_2jb->Scale(m_ttbar_sf_lj);
        }

      //3jb scaling
      m_jbf = 1;
      sf_processor(m_ttbar_sf_lj, m_ttbar_sf_ll, m_mjets_sf, m_ltf, m_rpf, m_jbf, m_tbf);
      if (type_index == 1) {
          h_mva_max_3jb->Scale(m_mjets_sf);
          h_mva_max_next_3jb->Scale(m_mjets_sf);
          h_mva_2D_3jb->Scale(m_mjets_sf);
        } else if (type_index == 16) {
          h_mva_max_3jb->Scale(m_ttbar_sf_ll);
          h_mva_max_next_3jb->Scale(m_ttbar_sf_ll);
          h_mva_2D_3jb->Scale(m_ttbar_sf_ll);
        } else if (type_index == 17) {
          h_mva_max_3jb->Scale(m_ttbar_sf_lj);
          h_mva_max_next_3jb->Scale(m_ttbar_sf_lj);
          h_mva_2D_3jb->Scale(m_ttbar_sf_lj);
        }

      //4jb scaling
      m_jbf = 2;
      sf_processor(m_ttbar_sf_lj, m_ttbar_sf_ll, m_mjets_sf, m_ltf, m_rpf, m_jbf, m_tbf);
      if (type_index == 1) {
          h_mva_max_4jb->Scale(m_mjets_sf);
          h_mva_max_next_4jb->Scale(m_mjets_sf);
          h_mva_2D_4jb->Scale(m_mjets_sf);
        } else if (type_index == 16) {
          h_mva_max_4jb->Scale(m_ttbar_sf_ll);
          h_mva_max_next_4jb->Scale(m_ttbar_sf_ll);
          h_mva_2D_4jb->Scale(m_ttbar_sf_ll);
        } else if (type_index == 17) {
          h_mva_max_4jb->Scale(m_ttbar_sf_lj);
          h_mva_max_next_4jb->Scale(m_ttbar_sf_lj);
          h_mva_2D_4jb->Scale(m_ttbar_sf_lj);
        }

      h_mva_2D_inc2jb->Add(h_mva_2D_2jb, h_mva_2D_3jb);
      h_mva_2D_inc2jb->Add(h_mva_2D_4jb);

      /* if (type_index == 0 || type_index == 11 || type_index == 16 || type_index == 10  || type_index == 17) {
        std::cout<<"Type: "<<Type_HF<<" Int = "<<h_mva_max_2jb->Integral()<<"   IntO = "<<h_mva_max_2jb->Integral(0,21)<<"    EffEnt =  "<<h_mva_max_2jb->GetEffectiveEntries()<<std::endl;
    }*/

      stat_for_wjets_sf_2[type_index] = h_mva_max_2jb->Integral();
      stat_for_wjets_sf_3[type_index] = h_mva_max_3jb->Integral();
      stat_for_wjets_sf_4[type_index] = h_mva_max_4jb->Integral();

      if (type_index != 0 && type_index != 2 && type_index != 3 && type_index != 4){
          ov_stat_for_wjets_sf_2+=stat_for_wjets_sf_2[type_index];
          ov_stat_for_wjets_sf_3+=stat_for_wjets_sf_3[type_index];
          ov_stat_for_wjets_sf_4+=stat_for_wjets_sf_4[type_index];
        }

      if (type_index == 2 || type_index == 3 || type_index == 4){
          ov_wstat_for_wjets_sf_2+=stat_for_wjets_sf_2[type_index];
          ov_wstat_for_wjets_sf_3+=stat_for_wjets_sf_3[type_index];
          ov_wstat_for_wjets_sf_4+=stat_for_wjets_sf_4[type_index];
        }

      std::cout<<"RP:  '"<<runPeriod<<"'   TB:  '"<<tagBin<<"'   Sample:  '"<<Type_CP<<"'   N_events = "<<nentries<<"   N_stat = "<<overall_stat<<std::endl;

      //mini_file->Write();
      //mini_file->Close();

      delete fChain;
      //delete mini_file;

      delete  h_mva_max_2jb;
      delete  h_mva_max_3jb;
      delete  h_mva_max_4jb;
      delete  h_mva_max_next_2jb;
      delete  h_mva_max_next_3jb;
      delete  h_mva_max_next_4jb;
      delete  h_mva_2D_2jb;
      delete  h_mva_2D_3jb;
      delete  h_mva_2D_4jb;
      delete  h_mva_2D_inc2jb;
      delete  h_NJets;

    }

  //!Calculating W+jets SFs

  m_wjets_sf_2 = (stat_for_wjets_sf_2[0] - ov_stat_for_wjets_sf_2)/ov_wstat_for_wjets_sf_2;
  m_wjets_sf_3 = (stat_for_wjets_sf_3[0] - ov_stat_for_wjets_sf_3)/ov_wstat_for_wjets_sf_3;
  m_wjets_sf_4 = (stat_for_wjets_sf_4[0] - ov_stat_for_wjets_sf_4)/ov_wstat_for_wjets_sf_4;

  TString txt_outPATH = Form("%s%s_%s_%s_%s_%d.txt", outputDir.Data(), treeType.Data(), runPeriod.Data(), sysSet.Data(), lepType.Data(), mass_point);
  ofstream myfile;
  myfile.open(txt_outPATH);
  myfile<<m_wjets_sf_2<<std::endl<<m_wjets_sf_3<<std::endl<<m_wjets_sf_4<<std::endl;
  myfile.close();

  std::cout<<"Wsf_2jb = "<<m_wjets_sf_2<<"    Wsf_3jb = "<<m_wjets_sf_3<<"    Wsf_4jb = "<<m_wjets_sf_4<<std::endl;
  std::cout<<"Scale factors are written to a file: "<<txt_outPATH<<std::endl;
  std::cout<<"Done!"<<std::endl;

  return 0;
}
