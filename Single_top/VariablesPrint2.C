// g++ -Wall $(root-config --libs --cflags) -o VariablesPrint2 VariablesPrint2.C

#define _GNU_SOURCE
#include <string.h>

void VariablesPrint2() {
TFile *f = new TFile("/home/francji/data/subsets/yield_sample/p17_CC_diboson_EqOneTag_EqThreeJet_zero_yield_sample_sample.root");
TTree *t = (TTree *) f->Get("TopologicalVariables");
TObjArray *arr = t->GetListOfBranches();
    for (int i = 0; i < arr->GetSize(); i++) {
      char s[1000];
      strcpy(s, arr->At(i)->GetTitle());
      char c = s[strlen(s)-1];
      s[strlen(s)-2] = '\0';
        printf("fprintf(dest_file,\"%%25.10f\",%s);", s);
    }
}


