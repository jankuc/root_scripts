// g++ -Wall $(root-config --libs --cflags) -o VariablesPrint1 VariablesPrint1.C

#define _GNU_SOURCE
#include <string.h>


void VariablesPrint1() {
TFile *f = new TFile("/home/francji/data/subsets/yield_sample/p17_CC_diboson_EqOneTag_EqThreeJet_zero_yield_sample_sample.root");
TTree *t = (TTree *) f->Get("TopologicalVariables");
TObjArray *arr = t->GetListOfBranches();
    for (int i = 0; i < arr->GetSize(); i++) {
      char s[1000];
      strcpy(s, arr->At(i)->GetTitle());
      char c = s[strlen(s)-1];
      s[strlen(s)-2] = '\0';
      if (c == 'C')
        printf("Char_t %s;   t->SetBranchAddress(\"%s\", &%s);\n", s, s, s);
      else if (c == 'D')
        printf("Double_t %s; t->SetBranchAddress(\"%s\", &%s);\n", s, s, s);
      else if (c == 'I')
        printf("Int_t %s;    t->SetBranchAddress(\"%s\", &%s);\n", s, s, s);
      else if (c == 'F')
        printf("Float_t %s;  t->SetBranchAddress(\"%s\", &%s);\n", s, s, s);
      else
        printf("PROBLEM: %s, %c\n", s, c);
    }
}