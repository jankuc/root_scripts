#define _GNU_SOURCE
#include <string.h>

void VariablesPrint() {
TFile *f = new TFile("/work/budvar-clued0/francji/subsets/small_training_sample/p17_CC_diboson_EqOneTag_EqThreeJet_zero_Topo_small_training_sample.root");
TTree *t = (TTree *) f->Get("TopologicalVariables");
TObjArray *arr = t->GetListOfBranches();
int j = 1;
for (int i = 0; i < arr->GetSize(); i++) {
      char s[1000];
      strcpy(s, arr->At(i)->GetTitle());
      char c = s[strlen(s)-1];
      s[strlen(s)-2] = '\0';
      if (c == 'D') {
        printf("%5i  D %s \n", j, s);
        j++;
      } else if (c == 'I') {
        printf("%5i  I %s \n", j, s);
        j++;
      } else {};
      }
}