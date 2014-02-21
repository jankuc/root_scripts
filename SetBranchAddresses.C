{

  curtree->SetBranchAddress( "njets", &njets );

  curtree->SetBranchAddress( "apla", &apla );
  curtree->SetBranchAddress( "spher", &spher );
  curtree->SetBranchAddress( "htl",  &htl );
  curtree->SetBranchAddress( "jetmt",  &jetmt );
  curtree->SetBranchAddress( "ht3",  &ht3 );
  curtree->SetBranchAddress( "mevent",  &mevent );
  curtree->SetBranchAddress( "mT1nl",  &mT1nl );

// newly added
  curtree->SetBranchAddress( "m01mall",  &m01mall );
  curtree->SetBranchAddress( "m0nl",  &m0nl );
  curtree->SetBranchAddress( "m1nl",  &m1nl );
  curtree->SetBranchAddress( "mT0nl",  &mT0nl );
  curtree->SetBranchAddress( "met",  &met );
  curtree->SetBranchAddress( "mtt",  &mtt );
  curtree->SetBranchAddress( "mva_max",  &mva_max );

  //curtree->SetBranchAddress( "mva_max_next",  &mva_max_next );

  curtree->SetBranchAddress( "wmt",  &wmt );
  curtree->SetBranchAddress( "wpt",  &wpt );
  curtree->SetBranchAddress( "centr",  &centr );
  curtree->SetBranchAddress( "dRminejet",  &dRminejet );
  curtree->SetBranchAddress( "diJetDrmin",  &diJetDrmin );
  curtree->SetBranchAddress( "ht",  &ht );
  curtree->SetBranchAddress( "ht20",  &ht20 );
  curtree->SetBranchAddress( "ktminp",  &ktminp );
  curtree->SetBranchAddress( "lepdphimet",  &lepdphimet );
  curtree->SetBranchAddress( "lepemv",  &lepemv );
  curtree->SetBranchAddress( "jetm",  &jetm );

  curtree->SetBranchAddress( "weight",  &weight );

}
