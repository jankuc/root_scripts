#define _GNU_SOURCE
#include <string.h>

void VariablePrint1() {
TFile *f = new TFile("/home/francji/data/subsets/small_training_sample/p17_CC_diboson_EqOneTag_EqThreeJet_zero_Topo_small_training_sample.root");
TTree *t = (TTree *) f->Get("TopologicalVariables");
TObjArray *arr = t->GetListOfBranches();
    for (int i = 0; i < arr->GetSize(); i++) {
      char s[1000];
      strcpy(s, arr->At(i)->GetTitle());
      char c = s[strlen(s)-1];
      s[strlen(s)-2] = '\0';
      printf("%i  %s \n", i, s);
    }
}