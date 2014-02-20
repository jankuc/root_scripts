// To Compile
// g++ -Wall `$ROOTSYS/bin/root-config --libs --cflags` -o updateWeights updateWeights.C
//
// To run to one file
// ./top_extractor_new [source file]  [destination file]
//
// To run all files in certain directory
// for i in /[route]/*; do ./top_extractor_new $i [route]/$(basename ${i/.root/}); done
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
int multiY;
int multiTT;

void update_weights() {

    // path to the .root file
    TFile *f = new TFile(source_file_path,"UPDATE");
    // gets the tree with given name
    TTree *t = (TTree *) f->Get("nn_tree");

    // we change only weights
    Float_t weight;
    t->SetBranchAddress("Weight", &weight);
    // we have to know, if we are Train, Test or Yield
    Int_t train;
    t->SetBranchAddress("train",&train);

    Int_t nEvents = t->GetEntries();

    for (Int_t i = 0; i<  nEvents; i++) {
        // updates all addressed variables to the state of event i
        t->GetEvent(i);
        if (train>0){ // train==0 ... yield, train==1 ... train or test
            weight = multiTT*weight; // train and test
        } else {
            weight = multiY*weight; // yield
        }
        // writes variables with linked addresses back to the tree
        t->Fill();
    }
    // writes the tree to the .root file
    t->Write();
}

#ifndef __CINT__
int main(int argc, char** argv)
{
    multiTT = *argv[1] - '0';
    multiY = *argv[2] - '0';
    source_file_path = argv[3];
    cout << "multiplier Yield = " << multiY << endl;
    cout << "multiplier TrTes = " << multiTT << endl;
    cout << "file.root = " << source_file_path << endl;
    update_weights();
    return 0;
}
#endif


