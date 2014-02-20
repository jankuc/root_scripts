// source [root setup]
//
// To Compile
// g++ -Wall $(root-config --libs --cflags) -o top_extractor_Discriminants top_extractor_Discriminants.C
//
// To run to one file
// ./top_extractor2 [source file]  [destination file]
//
// To run all files in certain directory
// for i in /[route]/*; do ./top_extractor2 $i [route]/$(basename ${i/.root/}); done
//

#ifndef __CINT__
// Include all needed headers here 
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


void top_extractor_Discriminants() {
    
    TFile *f = new TFile(source_file_path);
    
    TTree *t = (TTree *) f->Get("TopologicalVariables");
	
	Double_t MEBraveDisc_tb;	t->SetBranchAddress("MEBraveDisc_tb",   &MEBraveDisc_tb);
	Double_t MEBtagDisc_tqb; 	t->SetBranchAddress("MEBtagDisc_tqb",   &MEBtagDisc_tqb);
	Double_t MEBtagDisc_tbtqb; 	t->SetBranchAddress("MEBtagDisc_tbtqb", &MEBtagDisc_tbtqb);

	Double_t BNN_tb_output;		t->SetBranchAddress("BNN_tb_output",    &BNN_tb_output);
	Double_t BNN_tqb_output; 	t->SetBranchAddress("BNN_tqb_output",   &BNN_tqb_output);
	Double_t BNN_tbtqb_output; 	t->SetBranchAddress("BNN_tbtqb_output", &BNN_tbtqb_output);

	Double_t DT_tb_output;		t->SetBranchAddress("DT_tb_output",     &DT_tb_output);
	Double_t DT_tqb_output; 	t->SetBranchAddress("DT_tqb_output",    &DT_tqb_output);
	Double_t DT_tbtqb_output; 	t->SetBranchAddress("DT_tbtqb_output",  &DT_tbtqb_output);

	Double_t BNNComb_tb_output;		t->SetBranchAddress("BNNComb_tb_output",  &BNNComb_tb_output);
	Double_t BNNComb_tqb_output; 		t->SetBranchAddress("BNNComb_tqb_output", &BNNComb_tqb_output);
	Double_t BNNComb_tbtqb_BMED_output; 	t->SetBranchAddress("BNNComb_tbtqb_BMED_output", &BNNComb_tbtqb_BMED_output);

	Double_t EventWeight;	 	t->SetBranchAddress("EventWeight", &EventWeight);

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

	fprintf(dest_file,"%20.15f",MEBraveDisc_tb);
	fprintf(dest_file,"%20.15f",MEBtagDisc_tqb);
	fprintf(dest_file,"%20.15f",MEBtagDisc_tbtqb);
	
	fprintf(dest_file,"%20.15f",BNN_tb_output);
	fprintf(dest_file,"%20.15f",BNN_tqb_output);
	fprintf(dest_file,"%20.15f",BNN_tbtqb_output);
	
	fprintf(dest_file,"%20.15f",DT_tb_output);
	fprintf(dest_file,"%20.15f",DT_tqb_output);
	fprintf(dest_file,"%20.15f",DT_tbtqb_output);

	fprintf(dest_file,"%20.15f",BNNComb_tb_output);
	fprintf(dest_file,"%20.15f",BNNComb_tqb_output);
	fprintf(dest_file,"%20.15f",BNNComb_tbtqb_BMED_output);
	
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

    top_extractor_Discriminants();

    return 0;
}
#endif


