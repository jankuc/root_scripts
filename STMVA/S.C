{
  TChain chain1("TopologicalVariables");
  TChain chain2("GLM");
  TChain chain3("MBC");
  chain1.Add("/work/budvar-clued0/francji/subsets/small_training_sample/p17_CC_diboson_EqOneTag_EqThreeJet_zero_Topo_small_training_sample.root");
  chain1.Add("/work/budvar-clued0/francji/subsets/small_training_sample/p17_CC_QCD_EqOneTag_EqThreeJet_zero_Topo_small_training_sample.root");
  chain2.Add("/work/budvar-clued0/francji/results/small_training_sample/p17_CC_diboson_EqOneTag_EqThreeJet_zero_small_training_sample.root");
  chain2.Add("/work/budvar-clued0/francji/results/small_training_sample/p17_CC_QCD_EqOneTag_EqThreeJet_zero_small_training_sample.root");
  chain3.Add("/work/budvar-clued0/francji/results/small_training_sample/p17_CC_diboson_EqOneTag_EqThreeJet_zero_small_training_sample.root");
  chain3.Add("/work/budvar-clued0/francji/results/small_training_sample/p17_CC_QCD_EqOneTag_EqThreeJet_zero_small_training_sample.root");

  chain1.AddFriend("GLM");
  chain1.AddFriend("MBC");

  Double_t x1, x2, x3;
  chain1.SetBranchAddress("EventWeight", &x1);
  chain1.SetBranchAddress("GLM.tb", &x2);
  chain1.SetBranchAddress("MBC.tqb", &x3);

  for (Long64_t ievt=0; ievt < chain1.GetEntries(); ievt++) {
    chain1.GetEntry(ievt);
    std::cout << x1 << "\t" << x2 << "\t" << x3 << std::endl;
  }

  //TTreeFormula *ttf = new TTreeFormula("ttf", "EventWeight", &chain1);
  //TString s("GLM.TB");
  TString s("GLM.tb");
  TTreeFormula *ttf = new TTreeFormula(Form("Formula%s", s), s, &chain1);
  std::cout << ttf->GetNdim() << std::endl;
}
