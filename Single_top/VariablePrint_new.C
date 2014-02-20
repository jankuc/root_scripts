#define _GNU_SOURCE
#include <string.h>


void VariablePrint1() {
TFile *f = new TFile("/work/budvar-clued0/francji/tt_leptonjets/samples/split_trees_3samples_ele_1119/ttAll_172_miniTree.root");
TTree *t = (TTree *) f->Get("TopologicalVariables");
TObjArray *arr = t->GetListOfBranches();
    for (int i = 0; i < arr->GetSize(); i++) {
      char s[1000];
      strcpy(s, arr->At(i)->GetTitle());
      char c = s[strlen(s)-1];
      s[strlen(s)-2] = '\0';
      if (c == 'C')
        printf("Char_t %s; t->SetBranchAddress(\"%s\", &%s);\n", s, s, s);
      else if (c == 'D')
        printf("Double_t %s; t->SetBranchAddress(\"%s\", &%s);\n", s, s, s);
      else if (c == 'I')
        printf("Int_t %s; t->SetBranchAddress(\"%s\", &%s);\n", s, s, s);
      else
        printf("PROBLEM: %s, %c\n", s, c);
    }
}