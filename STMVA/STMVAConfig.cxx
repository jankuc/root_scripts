#include "STMVAConfig.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>

#include "TEnv.h"
#include "TFile.h"
#include "TTree.h"
#endif

STMVAConfig::STMVAConfig(const char *filename) {
  _configFile = std::string(filename);

  std::ifstream ifile(filename);
  if (!ifile) // file doesn't exists
    throw std::runtime_error("can't open params file \""+_configFile+"\"");

  TEnv env(filename);

  _trainingPath = env.GetValue("TrainingPath", "");
  _testingPath = env.GetValue("TestingPath", "");
  _yieldsPath = env.GetValue("YieldsPath", "");
  _dataPath = env.GetValue("DataPath", "");
  _outputPath = env.GetValue("OutputPath", ".");
  _outputName = env.GetValue("OutputName", "");
  std::string trainingFilePrefix = env.GetValue("TrainingFilePrefix", "");
  std::string trainingFileSuffix = env.GetValue("TrainingFileSuffix", "_miniTree.root");
  std::string testingFilePrefix = env.GetValue("TestingFilePrefix", "");
  std::string testingFileSuffix = env.GetValue("TestingFileSuffix", "_miniTree.root");
  std::string yieldsFilePrefix = env.GetValue("YieldsFilePrefix", "");
  std::string yieldsFileSuffix = env.GetValue("YieldsFileSuffix", "_miniTree.root");
  std::string dataFilePrefix = env.GetValue("DataFilePrefix", "");
  std::string dataFileSuffix = env.GetValue("DataFileSuffix", "_miniTree.root");
  _treeName = env.GetValue("TreeName", "");
  _lepton = env.GetValue("Lepton", "");
  _jetBin = env.GetValue("JetBin", "");
  _eventWeight = env.GetValue("EventWeight", "");
  std::istringstream samples(env.GetValue("Samples", ""));
  std::istringstream signals(env.GetValue("Signals", ""));
  std::istringstream variables(env.GetValue("Variable", ""));
  std::istringstream spectators(env.GetValue("Spectator", ""));
  std::istringstream methods(env.GetValue("Method", ""));

  if (_lepton != "muo" && _lepton != "ele")
    throw std::runtime_error("invalid Lepton = \""+_lepton+"\" in "+_configFile);
  if (_jetBin != "2jet" && _jetBin != "3jet" && _jetBin != "4jet")
    throw std::runtime_error("invalid JetBin = \""+_jetBin+"\" in "+_configFile);
  if (_eventWeight == "")
    throw std::runtime_error("invalid Weight = \""+_eventWeight+"\" in "+_configFile);
  if (_outputPath == "")
    throw std::runtime_error("invalid OutputPath = \""+_outputPath+"\" in "+_configFile);
  if (_outputName == "")
    throw std::runtime_error("invalid OutputName = \""+_outputName+"\" in "+_configFile);
  if (_treeName == "")
    throw std::runtime_error("invalid TreeName = \""+_treeName+"\" in "+_configFile);

  copy(std::istream_iterator<std::string>(samples),
       std::istream_iterator<std::string>(),
       std::back_inserter<std::vector<std::string> >(_samples));
  copy(std::istream_iterator<std::string>(signals),
       std::istream_iterator<std::string>(),
       std::back_inserter<std::vector<std::string> >(_signals));
  copy(std::istream_iterator<std::string>(variables),
       std::istream_iterator<std::string>(),
       std::back_inserter<std::vector<std::string> >(_variables));
  copy(std::istream_iterator<std::string>(spectators),
       std::istream_iterator<std::string>(),
       std::back_inserter<std::vector<std::string> >(_spectators));
  copy(std::istream_iterator<std::string>(methods),
       std::istream_iterator<std::string>(),
       std::back_inserter<std::vector<std::string> >(_methods));

  _trainingSignals = getInputPaths("TrainingPath", _trainingPath, trainingFilePrefix, trainingFileSuffix, true);
  _trainingBackgrounds = getInputPaths("TrainingPath", _trainingPath, trainingFilePrefix, trainingFileSuffix, false);
  _testingSignals = getInputPaths("TestingPath", _testingPath, testingFilePrefix, testingFileSuffix, true);
  _testingBackgrounds = getInputPaths("TestingPath", _testingPath, testingFilePrefix, testingFileSuffix, false);
  _yieldsSignals = getInputPaths("YieldsPath", _yieldsPath, yieldsFilePrefix, yieldsFileSuffix, true);
  _yieldsBackgrounds = getInputPaths("YieldsPath", _yieldsPath, yieldsFilePrefix, yieldsFileSuffix, false);
  std::vector<std::string> dataPaths = getInputPaths("DataPath", _dataPath, dataFilePrefix, dataFileSuffix, false);

  if (_trainingSignals.size() == 0)
    throw std::runtime_error("invalid training samples or training signals (no signal files) in "+_configFile);
  if (_trainingBackgrounds.size() == 0)
    throw std::runtime_error("invalid training samples or training signals (no background files) in "+_configFile);
  if (_testingSignals.size() == 0)
    throw std::runtime_error("invalid testing samples or testing signals (no signal files) in "+_configFile);
  if (_testingBackgrounds.size() == 0)
    throw std::runtime_error("invalid testing samples or testing signals (no background files) in "+_configFile);
  if (_yieldsSignals.size() == 0)
    throw std::runtime_error("invalid yields samples or yields signals (no signal files) in "+_configFile);
  if (_yieldsBackgrounds.size() == 0)
    throw std::runtime_error("invalid yields samples or yields signals (no background files) in "+_configFile);
  if (dataPaths.size() == 0)
    throw std::runtime_error("invalid data path \""+_dataPath+"\" (no files) in "+_configFile);
  _data = dataPaths[0];

  for (std::vector<std::string>::const_iterator it = _methods.begin();
       it != _methods.end(); ++it) {
    std::string method(*it);
    std::string methodType(env.GetValue(std::string(method+".Type").c_str(), ""));
    std::string methodParams(env.GetValue(std::string(method+".Params").c_str(), ""));

    if (_methodsType.find(method) != _methodsType.end())
      throw std::runtime_error("duplicate method \""+method+"\" in "+_configFile);

    TMVA::Types::EMVA methodEMVA;
    if (methodType == "TMVA::Types::kExternal") {
      methodEMVA = TMVA::Types::kExternal;
      std::string methodTreeName = std::string(env.GetValue(std::string(method+".TreeName").c_str(), ""));
      std::string methodLeaveName = std::string(env.GetValue(std::string(method+".LeaveName").c_str(), ""));
      std::string methodTrainingFilePrefix = env.GetValue(std::string(method+".TrainingFilePrefix").c_str(), "");
      std::string methodTrainingFileSuffix = env.GetValue(std::string(method+".TrainingFileSuffix").c_str(), "_miniTree.root");
      std::string methodTestingFilePrefix = env.GetValue(std::string(method+".TestingFilePrefix").c_str(), "");
      std::string methodTestingFileSuffix = env.GetValue(std::string(method+".TestingFileSuffix").c_str(), "_miniTree.root");
      std::string methodYieldsFilePrefix = env.GetValue(std::string(method+".YieldsFilePrefix").c_str(), "");
      std::string methodYieldsFileSuffix = env.GetValue(std::string(method+".YieldsFileSuffix").c_str(), "_miniTree.root");
      std::string methodDataFilePrefix = env.GetValue(std::string(method+".DataFilePrefix").c_str(), "");
      std::string methodDataFileSuffix = env.GetValue(std::string(method+".DataFileSuffix").c_str(), "_miniTree.root");

      if (methodTreeName == "")
        throw std::runtime_error("invalid "+method+".TreeName = \""+methodTreeName+"\" in "+_configFile);
      if (methodLeaveName == "")
        throw std::runtime_error("invalid "+method+".LeaveName = \""+methodLeaveName+"\" in "+_configFile);
      if (std::find(_spectators.begin(), _spectators.end(), methodTreeName+"."+methodLeaveName) == _spectators.end())
        _spectators.push_back(methodTreeName+"."+methodLeaveName);

      _methodsTreeName[method] = methodTreeName;
      _methodsLeaveName[method] = methodLeaveName;

      std::string trainingPath(env.GetValue(std::string(method+".TrainingPath").c_str(), ""));
      std::string testingPath(env.GetValue(std::string(method+".TestingPath").c_str(), ""));
      std::string yieldsPath(env.GetValue(std::string(method+".YieldsPath").c_str(), ""));
      std::string dataPath(env.GetValue(std::string(method+".DataPath").c_str(), ""));

      _methodsTrainingSignals[method] = getInputPaths("TrainingPath["+method+"]", trainingPath, methodTrainingFilePrefix, methodTrainingFileSuffix, true);
      _methodsTrainingBackgrounds[method] = getInputPaths("TrainingPath["+method+"]", trainingPath, methodTrainingFilePrefix, methodTrainingFileSuffix, false);
      _methodsTestingSignals[method] = getInputPaths("TestingPath["+method+"]", testingPath, methodTestingFilePrefix, methodTestingFileSuffix, true);
      _methodsTestingBackgrounds[method] = getInputPaths("TestingPath["+method+"]", testingPath, methodTestingFilePrefix, methodTestingFileSuffix, false);
      _methodsYieldsSignals[method] = getInputPaths("YieldsPath["+method+"]", yieldsPath, methodYieldsFilePrefix, methodYieldsFileSuffix, true);
      _methodsYieldsBackgrounds[method] = getInputPaths("YieldsPath["+method+"]", yieldsPath, methodYieldsFilePrefix, methodYieldsFileSuffix, false);
      std::vector<std::string> methodsDataPaths = getInputPaths("DataPath["+method+"]", dataPath, methodDataFilePrefix, methodDataFileSuffix, false);
      
      
      checkMethodsInputs("TrainingSignals["+method+"]", _trainingSignals, _methodsTrainingSignals[method], _treeName, methodTreeName, methodLeaveName);
      checkMethodsInputs("TrainingBackgrounds["+method+"]", _trainingBackgrounds, _methodsTrainingBackgrounds[method], _treeName, methodTreeName, methodLeaveName);
      checkMethodsInputs("TestingSignals["+method+"]", _testingSignals, _methodsTestingSignals[method], _treeName, methodTreeName, methodLeaveName);
      checkMethodsInputs("TestingBackgrounds["+method+"]", _testingBackgrounds, _methodsTestingBackgrounds[method], _treeName, methodTreeName, methodLeaveName);
      checkMethodsInputs("YieldsSignals["+method+"]", _yieldsSignals, _methodsYieldsSignals[method], _treeName, methodTreeName, methodLeaveName);
      checkMethodsInputs("YieldsBackgrounds["+method+"]", _yieldsBackgrounds, _methodsYieldsBackgrounds[method], _treeName, methodTreeName, methodLeaveName);
      checkMethodsInputs("YieldsBackgrounds["+method+"]", _yieldsBackgrounds, _methodsYieldsBackgrounds[method], _treeName, methodTreeName, methodLeaveName);
      checkMethodsInputs("Data["+method+"]", dataPaths, methodsDataPaths, _treeName, methodTreeName, methodLeaveName);

      _methodsData[method] = dataPaths[0];
      methodParams += ":DiscName="+methodTreeName+"."+methodLeaveName;

    } else if (methodType == "TMVA::Types::kVariable") methodEMVA = TMVA::Types::kVariable;
    else if (methodType == "TMVA::Types::kCuts") methodEMVA = TMVA::Types::kCuts;
    else if (methodType == "TMVA::Types::kLikelihood") methodEMVA = TMVA::Types::kLikelihood;
    else if (methodType == "TMVA::Types::kPDERS") methodEMVA = TMVA::Types::kPDERS;
    else if (methodType == "TMVA::Types::kHMatrix") methodEMVA = TMVA::Types::kHMatrix;
    else if (methodType == "TMVA::Types::kFisher") methodEMVA = TMVA::Types::kFisher;
    else if (methodType == "TMVA::Types::kKNN") methodEMVA = TMVA::Types::kKNN;
    else if (methodType == "TMVA::Types::kCFMlpANN") methodEMVA = TMVA::Types::kCFMlpANN;
    else if (methodType == "TMVA::Types::kTMlpANN") methodEMVA = TMVA::Types::kTMlpANN;
    else if (methodType == "TMVA::Types::kBDT") methodEMVA = TMVA::Types::kBDT;
    else if (methodType == "TMVA::Types::kDT") methodEMVA = TMVA::Types::kDT;
    else if (methodType == "TMVA::Types::kRuleFit") methodEMVA = TMVA::Types::kRuleFit;
    else if (methodType == "TMVA::Types::kSVM") methodEMVA = TMVA::Types::kSVM;
    else if (methodType == "TMVA::Types::kMLP") methodEMVA = TMVA::Types::kMLP;
    else if (methodType == "TMVA::Types::kBayesClassifier") methodEMVA = TMVA::Types::kBayesClassifier;
    else if (methodType == "TMVA::Types::kFDA") methodEMVA = TMVA::Types::kFDA;
    //else if (methodType == "TMVA::Types::kCommittee") methodEMVA = TMVA::Types::kCommittee;
    else if (methodType == "TMVA::Types::kBoost") methodEMVA = TMVA::Types::kBoost;
    else if (methodType == "TMVA::Types::kPDEFoam") methodEMVA = TMVA::Types::kPDEFoam;
    else if (methodType == "TMVA::Types::kLD") methodEMVA = TMVA::Types::kLD;
    else if (methodType == "TMVA::Types::kPlugins") methodEMVA = TMVA::Types::kPlugins;
    else if (methodType == "TMVA::Types::kCategory") methodEMVA = TMVA::Types::kCategory;
    else
      throw std::runtime_error("invalid TMVA::Types::EMVA = \""+methodType+"\" in "+_configFile);

    _methodsType[method] = methodEMVA;
    _methodsParams[method] = methodParams;
  }
}



void STMVAConfig::dump() {
  std::cout << "STMVAConfig.ConfigFile: " << _configFile << std::endl;
  std::cout << "STMVAConfig.OutputPath: " << _outputPath << std::endl;
  std::cout << "STMVAConfig.OutputName: " << _outputName << std::endl;
  std::cout << "STMVAConfig.TreeName: " << _treeName << std::endl;
  std::cout << "STMVAConfig.Lepton: " << _lepton << std::endl;
  std::cout << "STMVAConfig.JetBin: " << _jetBin << std::endl;
  std::cout << "STMVAConfig.EventWeight: " << _eventWeight << std::endl;
  std::cout << "STMVAConfig.Variables: "; std::copy(_variables.begin(), _variables.end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;
  std::cout << "STMVAConfig.Spectators: "; std::copy(_spectators.begin(), _spectators.end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;
  std::cout << "STMVAConfig.TrainingSignal: "; std::copy(_trainingSignals.begin(), _trainingSignals.end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;
  std::cout << "STMVAConfig.TrainingBackground: "; std::copy(_trainingBackgrounds.begin(), _trainingBackgrounds.end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;
  std::cout << "STMVAConfig.TestingSignal: "; std::copy(_testingSignals.begin(), _testingSignals.end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;
  std::cout << "STMVAConfig.TestingBackground: "; std::copy(_testingBackgrounds.begin(), _testingBackgrounds.end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;
  std::cout << "STMVAConfig.YieldsSignal: "; std::copy(_yieldsSignals.begin(), _yieldsSignals.end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;
  std::cout << "STMVAConfig.YieldsBackground: "; std::copy(_yieldsBackgrounds.begin(), _yieldsBackgrounds.end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;
  std::cout << "STMVAConfig.Data: " << _data << std::endl;
  std::cout << "STMVAConfig.Methods: "; std::copy(_methods.begin(), _methods.end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;

  //std::cout << "STMVAConfig." << std::endl;
  for (std::vector<std::string>::const_iterator it = _methods.begin();
       it != _methods.end(); ++it) {
    std::string method(*it);
    std::cout << "STMVAConfig." << method << ".Type: ";
    switch (_methodsType[method]) {
      case TMVA::Types::kVariable: std::cout << "TMVA::Types::kVariable"; break;
      case TMVA::Types::kCuts: std::cout << "TMVA::Types::kCuts"; break;
      case TMVA::Types::kLikelihood: std::cout << "TMVA::Types::kLikelihood"; break;
      case TMVA::Types::kPDERS: std::cout << "TMVA::Types::kPDERS"; break;
      case TMVA::Types::kHMatrix: std::cout << "TMVA::Types::kHMatrix"; break;
      case TMVA::Types::kFisher: std::cout << "TMVA::Types::kFisher"; break;
      case TMVA::Types::kKNN: std::cout << "TMVA::Types::kKNN"; break;
      case TMVA::Types::kCFMlpANN: std::cout << "TMVA::Types::kCFMlpANN"; break;
      case TMVA::Types::kTMlpANN: std::cout << "TMVA::Types::kTMlpANN"; break;
      case TMVA::Types::kBDT: std::cout << "TMVA::Types::kBDT"; break;
      case TMVA::Types::kDT: std::cout << "TMVA::Types::kDT"; break;
      case TMVA::Types::kRuleFit: std::cout << "TMVA::Types::kRuleFit"; break;
      case TMVA::Types::kSVM: std::cout << "TMVA::Types::kSVM"; break;
      case TMVA::Types::kMLP: std::cout << "TMVA::Types::kMLP"; break;
      case TMVA::Types::kBayesClassifier: std::cout << "TMVA::Types::kBayesClassifier"; break;
      case TMVA::Types::kFDA: std::cout << "TMVA::Types::kFDA"; break;
     // case TMVA::Types::kCommittee: std::cout << "TMVA::Types::kCommittee"; break;
      case TMVA::Types::kBoost: std::cout << "TMVA::Types::kBoost"; break;
      case TMVA::Types::kPDEFoam: std::cout << "TMVA::Types::kPDEFoam"; break;
      case TMVA::Types::kLD: std::cout << "TMVA::Types::kLD"; break;
      case TMVA::Types::kPlugins: std::cout << "TMVA::Types::kPlugins"; break;
      case TMVA::Types::kCategory: std::cout << "TMVA::Types::kCategory"; break;
      case TMVA::Types::kExternal: std::cout << "TMVA::Types::kExternal"; break;
      default: std::cout << "UNKNOWN"; break;
    }
    std::cout << " (" << _methodsType[method] << ")" << std::endl;
    std::cout << "STMVAConfig." << method << ".Params: " << _methodsParams[method] << std::endl;
    if (_methodsType[method] == TMVA::Types::kExternal) {
      std::cout << "STMVAConfig." << method << ".TreeName: " << _methodsTreeName[method] << std::endl;
      std::cout << "STMVAConfig." << method << ".LeaveName: " << _methodsLeaveName[method] << std::endl;
      std::cout << "STMVAConfig." << method << ".TrainingSignal: "; std::copy(_methodsTrainingSignals[method].begin(), _methodsTrainingSignals[method].end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;
      std::cout << "STMVAConfig." << method << ".TrainingBackground: "; std::copy(_methodsTrainingBackgrounds[method].begin(), _methodsTrainingBackgrounds[method].end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;
      std::cout << "STMVAConfig." << method << ".TestingSignal: "; std::copy(_methodsTestingSignals[method].begin(), _methodsTestingSignals[method].end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;
      std::cout << "STMVAConfig." << method << ".TestingBackground: "; std::copy(_methodsTestingBackgrounds[method].begin(), _methodsTestingBackgrounds[method].end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;
      std::cout << "STMVAConfig." << method << ".YieldsSignal: "; std::copy(_methodsYieldsSignals[method].begin(), _methodsYieldsSignals[method].end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;
      std::cout << "STMVAConfig." << method << ".YieldsBackground: "; std::copy(_methodsYieldsBackgrounds[method].begin(), _methodsYieldsBackgrounds[method].end(), std::ostream_iterator<std::string>(std::cout, " ")); std::cout << std::endl;
    } else {
        std::cout << "STMVAConfig." << method << ".TrainingSignal: ";
            std::copy(_methodsTrainingSignals[method].begin(), _methodsTrainingSignals[method].end(), std::ostream_iterator<std::string>(std::cout, " "));
            std::cout << std::endl;
    }
  }
}



std::vector<std::string> STMVAConfig::getInputPaths(std::string cfgName, std::string path, std::string prefix, std::string suffix, bool signal) {
  if (path == "")
    throw std::runtime_error("undeclared "+cfgName+" in config file "+_configFile);

  std::vector<std::string> ret;
  for (std::vector<std::string>::const_iterator it = _samples.begin();
       it != _samples.end(); ++it) {
    if (signal) {
      if (std::find(_signals.begin(), _signals.end(), *it) == _signals.end())
        continue; // skip background samples
    } else {
      if (std::find(_signals.begin(), _signals.end(), *it) != _signals.end())
        continue; // skip signal samples
    }

    //std::string filename(path+"/"+prefix+(*it)+"_"+_lepton+"_"+_jetBin+suffix); //sets/train/ /ttA_172_ele_2jet_zero_Topo_small_training_sample.root" for TrainingPath
    std::string filename(path + "/" + _lepton + "/" + _jetBin + "/" + prefix + (*it) + suffix);
    std::ifstream ifile(filename.c_str());
    if (!ifile) // file doesn't exists
      throw std::runtime_error("missing required input file \""+filename+"\" for "+cfgName);

    ret.push_back(filename);
  }

  return ret;
}



void STMVAConfig::checkMethodsInputs(std::string cfgName, std::vector<std::string> &paths1, std::vector<std::string> &paths2, std::string tname1, std::string tname2, std::string lname) {
  if (paths1.size() != paths2.size())
    throw std::runtime_error(cfgName+" size of paths list is different");

  TFile *infile1, *infile2;
  TTree *t1, *t2;
  for (size_t i = 0; i < paths1.size(); i++) {
    infile1 = new TFile(paths1[i].c_str());
    infile2 = new TFile(paths2[i].c_str());

    infile1->GetObject(tname1.c_str(), t1);
    infile2->GetObject(tname2.c_str(), t2);

    if (!t1)
      throw std::runtime_error(cfgName+" can't get tree \""+tname1+"\" from "+paths1[i]);
    if (!t2)
      throw std::runtime_error(cfgName+" can't get tree \""+tname2+"\" from "+paths2[i]);
    if (!t2->GetLeaf(lname.c_str()))
      throw std::runtime_error(cfgName+" can't get tree \""+tname2+"\" leave \""+lname+"\" from "+paths2[i]);
    if (t1->GetEntries() != t2->GetEntries())
      throw std::runtime_error(cfgName+" different tree size for "+paths1[i]+"["+tname1+"] and "+paths1[i]+"["+tname1+"]");

    //std::cout << "DEBUG: " << t1->GetEntries() << " in " << paths1[i] << ":/" << tname1 << " and " << paths2[i] << ":/" << tname2 << std::endl;

    infile1->Close();
    infile2->Close();

    delete infile1;
    delete infile2;
  }
}



//void STMVAConfig::compareTreeSize(std::string cfgName, std::string path1, std::string path2, std::string suffix) {
//  for (std::vector<std::string>::const_iterator it = _samples.begin();
//       it != _samples.end(); ++it) {
//    std::string filename1(path1+"/p17_CC_"+(*it)+"_"+_lepton+"_"+_jetBin+suffix);
//    std::string filename2(path2+"/p17_CC_"+(*it)+"_"+_lepton+"_"+_jetBin+suffix);
//  }
//};
