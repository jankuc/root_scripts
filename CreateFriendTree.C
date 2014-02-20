TFile *f  = TFile::Open("singletop.root");
TTree *t  = (TTree*)f->Get("TopologicalVariables");
t->AddFriend("col","colTree.root");


Double_t Jet1Pt, Jet2Pt;
Float_t disc;
t->SetBranchAddress("Jet1Pt", &Jet1Pt);
t->SetBranchAddress("Jet2Pt", &Jet2Pt);
// ...
t->SetBranchAddress("col.disc", &disc);
// ...


Int_t nevents = t->GetEntriesFast();
for (Int_t i = 0; i<  nevents; i++){
  std::cout << "event(" << i << "): " << disc << std::endl;
  // ...
}

f->Close()