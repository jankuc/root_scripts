// To Compile
// g++ -Wall $([route]/root-config --libs --cflags) -o top_extractor_new top_extractor_new.C
//
// To run to one file
// ./top_extractor_new [source file]  [destination file]
//
// To run all files in certain directory
// for i in /[route]/*; do ./top_extractor_new $i [route]/$(basename ${i/.root/}); done
//

#ifndef __CINT__
// Include all needed headers here e
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include <stdio.h>
#include <stdlib.h>
#endif
using namespace std;

char *source_file_path;
char destination_path[1000];
char destination_path_variables[1000];


void top_extractor_new() {
    
    TFile *f = new TFile(source_file_path);
    
    TTree *t = (TTree *) f->Get("nn_tree");

	Float_t Apla; 		t->SetBranchAddress("Apla", &Apla);
	Float_t Spher; 		t->SetBranchAddress("Spher", &Spher);
	Float_t HTL; 		t->SetBranchAddress("HTL", &HTL);
	Float_t JetMt; 		t->SetBranchAddress("JetMt", &JetMt);
	Float_t HT3; 		t->SetBranchAddress("HT3", &HT3);
	Float_t MEvent;	 	t->SetBranchAddress("MEvent", &MEvent);
	Float_t MT1NL; 		t->SetBranchAddress("MT1NL", &MT1NL);
	Float_t M01mall; 	t->SetBranchAddress("M01mall", &M01mall);
	Float_t M0nl; 		t->SetBranchAddress("M0nl", &M0nl);
	Float_t M1nl; 		t->SetBranchAddress("M1nl", &M1nl);
	Float_t MT0nl; 		t->SetBranchAddress("MT0nl", &MT0nl);
	Float_t Met; 		t->SetBranchAddress("Met", &Met);
	Float_t Mtt; 		t->SetBranchAddress("Mtt", &Mtt);
	Float_t Mva_max; 	t->SetBranchAddress("Mva_max", &Mva_max);
	Float_t Wmt; 		t->SetBranchAddress("Wmt", &Wmt);
	Float_t Wpt; 		t->SetBranchAddress("Wpt", &Wpt);
	Float_t Centr; 		t->SetBranchAddress("Centr", &Centr);
	Float_t DRminejet; 	t->SetBranchAddress("DRminejet", &DRminejet);
	Float_t DiJetDrmin; 	t->SetBranchAddress("DiJetDrmin", &DiJetDrmin);
	Float_t Ht; 		t->SetBranchAddress("Ht", &Ht);
	Float_t Ht20; 		t->SetBranchAddress("Ht20", &Ht20);
	Float_t Ktminp; 	t->SetBranchAddress("Ktminp", &Ktminp);
	Float_t Lepdphimet; 	t->SetBranchAddress("Lepdphimet", &Lepdphimet);
	Float_t Lepemv; 	t->SetBranchAddress("Lepemv", &Lepemv);
	Float_t Jetm; 		t->SetBranchAddress("Jetm", &Jetm);
	
	Int_t   NJets; 		t->SetBranchAddress("NJets", &NJets);
	Int_t   type; 		t->SetBranchAddress("type", &type);
	Float_t Weight; 	t->SetBranchAddress("Weight", &Weight);
	Int_t   train; 		t->SetBranchAddress("train", &train);
	Int_t   val; 		t->SetBranchAddress("val", &val);

	Int_t nevents = t->GetEntries();

    FILE *dest_file;
    dest_file = fopen(destination_path,"w");
	// cout << "ok 1";

    for (Int_t i = 0; i<  nevents; i++) {

	t->GetEvent(i);

	fprintf(dest_file,"%30.20f",Apla);
	fprintf(dest_file,"%30.20f",Spher);
	fprintf(dest_file,"%30.20f",HTL);
	fprintf(dest_file,"%30.20f",JetMt);
	fprintf(dest_file,"%30.20f",HT3);
	fprintf(dest_file,"%30.20f",MEvent);
	fprintf(dest_file,"%30.20f",MT1NL);

	fprintf(dest_file,"%30.20f",M01mall);
	fprintf(dest_file,"%30.20f",M0nl);
	fprintf(dest_file,"%30.20f",M1nl);
	fprintf(dest_file,"%30.20f",MT0nl);
	fprintf(dest_file,"%30.20f",Met);
	fprintf(dest_file,"%30.20f",Mtt);
	fprintf(dest_file,"%30.20f",Mva_max);
	fprintf(dest_file,"%30.20f",Wmt);
	fprintf(dest_file,"%30.20f",Wpt);
	fprintf(dest_file,"%30.20f",Centr);
	fprintf(dest_file,"%30.20f",DRminejet);
	fprintf(dest_file,"%30.20f",DiJetDrmin);
	fprintf(dest_file,"%30.20f",Ht);
	fprintf(dest_file,"%30.20f",Ht20);
	fprintf(dest_file,"%30.20f",Ktminp);
	fprintf(dest_file,"%30.20f",Lepdphimet);
	fprintf(dest_file,"%30.20f",Lepemv);
	fprintf(dest_file,"%30.20f",Jetm);

	fprintf(dest_file,"%30i",NJets);
	fprintf(dest_file,"%30i",type);
	fprintf(dest_file,"%30.20f",Weight);
	fprintf(dest_file,"%30i",train);
	fprintf(dest_file,"%30i\n",val);
    }
    fclose(dest_file);

//  dest_file = fopen(destination_path_variables,"w");    
//
//  TObjArray *arr = t->GetListOfBranches();
//  for (int i = 0; i < arr->GetSize(); i++) {
//      fprintf(dest_file,"%s\n", arr->At(i)->GetTitle() );
//  }
//  fclose(dest_file);

}


#ifndef __CINT__
int main(int argc, char** argv)
{
    source_file_path = argv[1];

    sprintf(destination_path,"%s.txt",argv[2]);
   // sprintf(destination_path_variables,"%s.var_list",argv[2]);

    top_extractor_new();

    return 0;
}
#endif


