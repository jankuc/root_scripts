
// To Compile
// g++ -g -O2 -Wall `root-config --cflags --libs` CreateMethodsTree.C -o  CreateMethodsTree
//
// To run use CreateMethodsTree.sh (sh CreateMethodsTree.sh)


#ifndef __CINT__
// Include all needed headers here 
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <TH1.h>
#include "TCanvas.h"
//#include <TRandom.h>
#include <stdio.h>
#include <stdlib.h>
#include "TSystem.h"


#endif 

void getNEntries(char* fileName, char* treeName) {
    TFile *f = new TFile(fileName);
    TTree *t = (TTree*) f->Get(treeName);
    TSystem s;
    std::cout<< s.BaseName(f->GetName())<< ": " << t->GetEntries()<<std::endl;
}


int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage of " << argv[0] << ": " << "file_name, tree_name"<<std::endl;
    }
    getNEntries(argv[1], argv[2]);
    
    return 0;
}
