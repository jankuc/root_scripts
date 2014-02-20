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

void update_weights() {

    // path to the .root file
    TFile *f = new TFile(source_file_path);
    // gets the tree with given name
    TTree *t = (TTree *) f->Get("nn_tree");

    // we read only weights
    Float_t weight;
    t->SetBranchAddress("Weight", &weight);
    Int_t train;
    t->SetBranchAddress("train",&train);

    Int_t nEvents = t->GetEntries();
    Float_t sumY = 0, sumT=0;
    for (Int_t i = 0; i<  nEvents; i++) {
        // updates all addressed variables to the state of event i
        t->GetEvent(i);
        if(train>0){
            sumT += weight;
        } else {
            sumY += weight;
        }
    }
    cout << "sum(weigths Yield) = " << sumY << endl;
    cout << "sum(weigths TT) = " << sumT << endl;
}


#ifndef __CINT__
int main(int argc, char** argv)
{
    source_file_path = argv[1];
    update_weights();
    return 0;
}
#endif



