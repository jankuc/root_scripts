// To Compile
// g++ -Wall $(root-config --libs --cflags) -o VariablesPrint3_new VariablesPrint3_new.C

#ifndef __CINT__
// Include all needed headers here 
#include <string.h>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include <stdio.h>
#include <stdlib.h>
#endif
using namespace std;

//char *source_file_path;

void VariablesPrint3_new(const char *source_file_path) {

printf("Variables (leaves) from file: %s \n", source_file_path);

TFile     *f   = new TFile(source_file_path);
TTree     *t   = (TTree *) f->Get("nn_tree");
TObjArray *arr = t->GetListOfBranches();

//     for (int i = 0; i < arr->GetSize(); i++) {
//       char s[1000];
//       strcpy(s, arr->At(i)->GetTitle());
//       //char c = s[strlen(s)-1];
//       //s[strlen(s)-2] = '\0';
//       //printf("fprintf(dest_file,\"%%20.10f\",%s);\n", s);
//       printf("%s\n", s);
//     }

//    int j = 1;
//    for (int i = 0; i < arr->GetSize(); i++) {
//       char s[1000];
//       strcpy(s, arr->At(i)->GetTitle());
//       char c = s[strlen(s)-1];
//       s[strlen(s)-2] = '\0';
//       if (c == 'D') {
//         printf("%5i  D %s \n", j, s);
//         j++;
//       } else if (c == 'I') {
//         printf("%5i  I %s \n", j, s);
//         j++;
//       } else if (c == 'C') {
//         printf("%5i  C %s \n", j, s);
//         j++;
//       } else {};
//       }

    for (int i = 0; i < arr->GetSize(); i++) {
      char s[1000];
      strcpy(s, arr->At(i)->GetTitle());
      char c = s[strlen(s)-1];
      s[strlen(s)-2] = '\0';
      if (c == 'C')
        printf("Char_t %s; t->SetBranchAddress(\"%s\", &%s);\n", s, s, s);
       //printf("Char_t \n");
     else if (c == 'D')
        printf("Double_t %s; t->SetBranchAddress(\"%s\", &%s);\n", s, s, s);
      else if (c == 'I')
        printf("Int_t %s; t->SetBranchAddress(\"%s\", &%s);\n", s, s, s);
      else if (c == 'F')
        printf("Float_t %s; t->SetBranchAddress(\"%s\", &%s);\n", s, s, s);
      else
        printf("PROBLEM: %s, %c\n", s, c);
    }

}

#ifndef __CINT__
int main(int argc, char** argv)
{
   // source_file_path = argv;
   // sprintf(destination_path,"%s.txt",argv[2]);
   // sprintf(destination_path_variables,"%s.var_list",argv[2]);
    VariablesPrint3_new(argv[1]);
    return 0;
}
#endif
