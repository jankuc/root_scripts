
/*
  * To compile use command:
  * gcc -g -O2 -Wall `$ROOTSYS/root/bin/root-config --cflags --libs` -lTMVA -lTreePlayer -lMLP -lMinuit -lXMLIO -lstdc++ root_tmva.cpp -o root_tmva
  *
  * or
  *
  * gcc -g -O2 -Wall `root-config --cflags --libs` -lTMVA -lTreePlayer -lMLP -lMinuit -lXMLIO -lstdc++ root_tmva.cpp -o root_tmva
  *
  *
  * To run with learning phase - use command:
  *
  * ./root_tmva 0 pocet_sloupcu_dat <absolutni_cesta_k_datum>/ learn_data_file test_data_file <absolutni_cesta_pro_vysledky>/
  *
  * To apply learned BDT and MLP - use command:
  *
  * ./root_tmva 1 pocet_sloupcu_dat <absolutni_cesta_k_datum>/ test_data_file <absolutni_cesta_pro_vysledky>/ <absolutni_cesta_ke_xml_vaham>/ vahy_bdt_xml_data vahy_mlp_xml_data
*/

#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TCanvas.h"

#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/DecisionTree.h"
#include "TMVA/Reader.h"

// Define variables used for analysis
/*#define VARIABLES {"var1", "var2", "var3", "var4", "var5", "var6", \
//      "var8", "var9", "var10", "var11", "var12", "var13", \
//      "var15", "var16", "var17", "var18", "var20", \
//      "var21", "var22", "var23", "var25", "var26"}*/

const TString VARS[] =  {"Apla", "Spher", "HTL", "JetMt", "HT3", "MEvent", "MT1NL", "M01mall", "M0nl", "M1nl", "Mt0nl", "Met", "Mtt", "Mva_max", "Wmt", "Wpt", "Centr", "DRminejet", "DiJetDrmin", "Ht", "Ht20", "Ktminp", "Lepdphimet", "Lepemv"};

#define VARNAME "var"


using namespace std;

/**
 * Should return size of arbitrary array
 * @param N Array
 * @return Array element number
 */
template <typename T, size_t N>
inline
size_t sizeOfArray( const T(&)[ N ] )
{
  return N;
}
// ------------------------------------------------------------------

/**
 * @brief Converts int to string
 * @param number
 * @return string instead of number
 */
string intToStr(int number) {
  stringstream ss;//create a stringstream
  ss << number;//add number to the stream
  return ss.str();//return a string with the contents of the stream
}
// ------------------------------------------------------------------

/**
 * @brief Converts double to string
 * @param number
 * @return string instead of number
 */
string dblToStr(double number) {
  stringstream ss;//create a stringstream
  ss << number;//add number to the stream
  return ss.str();//return a string with the contents of the stream
}
// ------------------------------------------------------------------

/**
 * This method adds variables to the ROOT factory
 * @param factory The ROOT factory
 * @param vars Variable names
 * @param l Length of the array vars
 */
void addVariables(TMVA::Factory *factory, const char* vars[], size_t l) {
  for (size_t i = 0; i < l; i++) {
      factory->AddVariable(TString(vars[i]), 'F');
    }
}
// ------------------------------------------------------------------

/**
 * Returns complete file name composed of path and file name
 * @param absPath path to the file
 * @param fName name of the file
 * @return complete file name containing path to it
 */
string getCompleteFileName(const char* absPath, const char* fName) {
  string completeFName(absPath);
  completeFName.append(fName);
  return completeFName;
}
// ------------------------------------------------------------------

/**
 * @brief Creates string which should be taken into stringstream to initialize
 * required TTree
 * @param varNumber column number of file
 * @param fName input file name
 * @return string representation of data with variable specification
 */
string createInputString(int varNumber, const char* absPath, const char* fName) {

  // Create variable description
  TString varDesc;
  string varName(VARNAME);
  for (int i = 1; i < varNumber; i++) {
      varDesc += varName + intToStr(i) + "/F:";
    }
  varDesc += varName + intToStr(varNumber) + "/F\n";
      // Create stream with variable description and all data from file
      stringstream stream;
  stream << varDesc.Data();
  ifstream fb;
  string completeFName = getCompleteFileName(absPath, fName);
  fb.open(completeFName.c_str(), fstream::in);

  // Check if the input file was successfully opened
  if ( (fb.rdstate() & std::ifstream::failbit ) != 0 ) {
      std::cerr << "!!!! Error while opening " << completeFName << "\n";
    }

  // Copy whole content of input file into stream
  copy(istreambuf_iterator<char>(fb),
       istreambuf_iterator<char>(),
       ostreambuf_iterator<char>(stream));

  // Return resulting stringstream converted to string
  return stream.str();
}
// ------------------------------------------------------------------

/**
 * Performs the classification training
 * @param absPath Path to input data file
 * @param dataFile Name of file containing background and signal data
 * @param outPath Path of output files
 * @param varNumber Number of columns in the data file
 */
void createClassificator(const char* absPath, const char* dataFile,
                         const char* outPath, int varNumber) {

  // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
  TString outfileName(getCompleteFileName(outPath, "TMVA.root"));
  TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

  // Create the factory object. Later you can choose the methods
  // whose performance you'd like to investigate. The factory is
  // the only TMVA object you have to interact with
  //
  // The first argument is the base of the name of all the
  // weightfiles in the directory weight/
  //
  // The second argument is the output file for the training results
  // All TMVA output can be suppressed by removing the "!" (not) in
  // front of the "Silent" argument in the option string
  TString options = "!V:!Silent:Color:DrawProgressBar:"
      "Transformations=I;D;P;G,D:AnalysisType=Classification";
  TMVA::Factory *factory = new TMVA::Factory( dataFile,
                                              outputFile, options);
  // Set signal and background trees
  // We have all data in one .txt file. It can be read into TTree by
  // method TTree::ReadFile(...). Then signal and background cuts (TCut)
  // have to be created for distinguishing S and B in the tree. Now we
  // can call Factory::SetInputTrees(tree, SCut, BCut)
  //TString sigFile(trainF); // text file for signal
  //TString bkgFile(trainF); // text file for background
  //Double_t sigWeight = 1.0; // overall weight for all signal events
  //Double_t bkgWeight = 1.0; // overall weight for all background events
  //factory->SetInputTrees( sigFile, bkgFile, sigWeight, bkgWeight );
  //
  TTree *treeBS = new TTree("our_input_tree", "data");
  stringstream stream;
  stream << createInputString(varNumber, absPath, dataFile);
  treeBS->ReadStream(stream);

  // Add variables to factory if it is not constant (min == max)
  for (int i = 1; i < varNumber; i++) {
      string name = VARNAME + intToStr(i);
      if (treeBS->GetMaximum(name.c_str()) != treeBS->GetMinimum(name.c_str())) {
          factory->AddVariable(TString(name.c_str()), 'F');
        }
    }

  // Create signal and background cuts according to the last variable
  string signalCut, backgroundCut, vName("var" + intToStr(varNumber));
  signalCut = vName + ">0";
  backgroundCut = vName + "<1";
  TCut cutS = signalCut.c_str();
  TCut cutB = backgroundCut.c_str();
  factory->SetInputTrees(treeBS, cutS, cutB);
  
  // Set individual event weights (the variables must exist in the original TTree)
  //    for signal    : factory->SetSignalWeightExpression    ("weight1*weight2");
  //    for background: factory->SetBackgroundWeightExpression("weight1*weight2");
  //factory->SetBackgroundWeightExpression( "var5" );
  
  // Apply additional cuts on the signal and background samples (can be different)
  TCut mycuts = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
  TCut mycutb = ""; // for example: TCut mycutb = "abs(var1)<0.5";

  // Tell the factory how to use the training and testing events
  //
  // If no numbers of events are given, half of the events in the tree are used
  // for training, and the other half for testing:
  //    factory->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
  // To also specify the number of testing events, use:
  //    factory->PrepareTrainingAndTestTree( mycut,
  //                                         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );
  // This ("nTrain_Signal=0:nTrain_Background=0") splits data in two halves for training and testing
  // Use SplitMode=Alternate for odd and even events splitting
  options = "nTrain_Signal=0:nTrain_Background=0:SplitMode=Alternate:"
      "NormMode=NumEvents:!V";
  factory->PrepareTrainingAndTestTree(mycuts, mycutb, options);


  // Book method BDT
  options = "!H:!V:NTrees=850:nEventsMin=150:MaxDepth=3:"
      "BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:"
      "nCuts=20:PruneMethod=NoPruning";
  factory->BookMethod( TMVA::Types::kBDT, "BDT", options);

  // Book method MLP
  options = "H:!V:NeuronType=tanh:VarTransform=N:NCycles=500:"
      "HiddenLayers=N+1:TestRate=5:!UseRegulator";
  factory->BookMethod( TMVA::Types::kMLP, "MLP", options);
  
  // Book method MLP BNN
  options = "H:!V:NeuronType=tanh:VarTransform=N:NCycles=500:"
      "HiddenLayers=N+1:TestRate=5:TrainingMethod=BFGS:UseRegulator";
  //factory->BookMethod( TMVA::Types::kMLP, "MLPBNN", options);

  // Train MVAs using the set of training events
  factory->TrainAllMethods();

  // ---- Evaluate all MVAs using the set of test events
  factory->TestAllMethods();

  // ----- Evaluate and compare performance of all configured MVAs
  factory->EvaluateAllMethods();

  //outputFile->Write();
  outputFile->Close();
  delete factory;
}
// ------------------------------------------------------------------

/**
 * Performs trained classificators on some data sample
 * @param absPath Absolute path to the data file
 * @param dataFile Name of input data file
 * @param learnFile Name of learn data file
 * @param varNumber Number of columns in the data file
 * @param outputPath Path for output files
 */
void performClassifiers(const vector<string> &methods, 
			const vector<TString> &weightFiles,
			const char* absPath, const char* dataFile,
			const char* learnFile, int varNumber,
			const char* outputPath) {
  TMVA::Reader *reader = new TMVA::Reader();
  
  bool generateWeightfileNames = weightFiles.empty();

  // Create histograms
  size_t binsCount = 100;
  map<string, TH1F*> histos;
  histos.insert(pair<string, TH1F*>(methods[0],
                new TH1F("BDT_histo", "BDT_histo", binsCount, -1, 1)));
  histos.insert(pair<string, TH1F*>(methods[1],
                new TH1F("MLP_histo", "MLP_histo", binsCount, -1, 1)));

  // Read tree from ascii file
  string pref = getCompleteFileName(outputPath, "result_");
  TString outFName = pref + TString(dataFile) + ".root";
  TFile *outputFile = new TFile(outFName, "recreate");
  TTree *tree = new TTree("our_input_tree", "data");
  stringstream stream;
  stream << createInputString(varNumber, absPath, dataFile);
  tree->ReadStream(stream);

  // Create variables, and declare them to the reader
  vector<string> varNames;
  map<const char*, Float_t> vars;
  for (size_t i = 0; (int)i < varNumber; i++) {
      string n = VARNAME + intToStr(i);
      const char* name = n.c_str();
      if (tree->GetMaximum(name) != tree->GetMinimum(name)) {
          varNames.push_back(n);
          vars.insert(pair<const char*, Float_t>(name, 0.0));
          reader->AddVariable(name, &vars[name]);
        }
    }

  // Book methods
  for (size_t i = 0; i < methods.size(); i++) {
      printf("bookmethod%d\n", (int)i);
      TString methodName = TString(methods[i]) + TString(" method");

      if (generateWeightfileNames) {
          TString dir    = "weights/";
          TString prefix = TString(learnFile)+"_";
          TString weightfile = dir + prefix + TString(methods[i]) +
              TString(".weights.xml");
          reader->BookMVA(methodName, weightfile);
        } else {
          reader->BookMVA(methodName, weightFiles[i]);
        }
    }
  
  // Prepare event tree
  for (size_t i = 0; i < varNames.size(); i++) {
      const char* name = varNames[i].c_str();
      tree->SetBranchAddress(name, &vars[name]);
    }

  // Prepare output strings to save MVA results in file
  map<string, string> outStrings;
  for (size_t i = 0; i < methods.size(); i++) {
      string s;
      outStrings.insert(pair<string, string>(methods[i], s));
    }

  // Get TMVA results
  cout << "--- Processing: " << tree->GetEntries() << " events" << endl;
  for (Long64_t ievt=0; ievt<tree->GetEntries();ievt++) {

      tree->GetEntry(ievt);

      for (size_t i = 0; i < methods.size(); i++) {
          TString methodName = TString(methods[i]) + TString(" method");
          Double_t mvaValue = reader->EvaluateMVA(methodName);
          //Double_t mvaError = reader->GetMVAError();
          outStrings[methods[i]] += dblToStr(mvaValue) + "\n";
          //printf("%d - mva value: %f, mva error: %f\n", (int)i, mvaValue, mvaError);
          histos[methods[i]]->Fill(mvaValue);
        }
    }
  
  for (size_t i = 0; i < methods.size(); i++) {
      TString mName = TString(methods[i]) + TString(" method");
      printf("--- Writing histogram for method: %s\n", mName.Data());
      histos[methods[i]]->Write();

      // Write output strings into files
      ofstream s;
      s.open((outputPath + methods[i] + "_" + dataFile + ".txt").c_str());
      s << outStrings[methods[i]];
      s.close();
    }

  outputFile->Write();
  outputFile->Close();
  
  delete reader;
}
// ------------------------------------------------------------------

/**
 * @brief main function
 * @param argc number of parameters
 * @param argv array of parameters
 * @return
 */
int main(int argc, char* argv[]) {
  const int reqParNum = 7;
  if(argc < reqParNum) {
      cout << "Invalid number of parameters" << endl;
      cout << "Obtained input parameters number: " << argc << endl;
      cout << "Required input parameters number is " << reqParNum << " and more" << endl;
      exit(0);
    }
  
  // argv[0]       ... name of program
  // argv[1]       ... mode - if 0 learning and testing phase of TMVA
  //                       will be performed
  // argv[2]       ... number of variables inside input file
  // argv[3]       ... absolute path to the data file
  // argv[4]       ... name of learn data file
  // argv[5]       ... name of test data file
  // argv[6]       ... path for output files
  // argv[7..argc-1] ... absolute paths to weightfiles

  // Get variable count
  int varNumber = static_cast<int>(strtod(argv[2], NULL));
  int mode = static_cast<int>(strtod(argv[1], NULL));
  
  // Prepare usefull strings
  vector<string> methods;
  methods.push_back("BDT");
  methods.push_back("MLP");
  
  
  // Create list of weithfile names
  vector<TString> weightfiles;
  
  if (mode == 0) {
      // Run ROOT methods
      createClassificator(argv[3], argv[4], argv[6], varNumber); // root can't work with std strings
      performClassifiers(methods, weightfiles, argv[3], argv[4], argv[4], varNumber, argv[6]);
      performClassifiers(methods, weightfiles, argv[3], argv[5], argv[4], varNumber, argv[6]);
    } else {
      for (int i = 7; i < argc; i++) {
          weightfiles.push_back(TString(argv[6]) + TString(argv[i]));
        }
      performClassifiers(methods, weightfiles, argv[3], argv[4], "", varNumber, argv[5]);
    }
  printf("End of the program\n");
  return 0;
}
// program 1mode,, 2varNumber, 3data_path,  4learn_data, 5test_data, , 6output_path
// program 1mode, 2varNumber,3data_path,  4test_data,  5output_path, 6weight_f_path,  7weight_f1, 8weight_f2
