#ifndef ROOT_STMVAConfig
#define  ROOT_STMVAConfig

//#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include <vector>
#include <map>

#include "TMVA/Types.h"
//#endif

class STMVAConfig {
  public:
    STMVAConfig(const char *filename);
    ~STMVAConfig() {};
    //const std::string &trainingPath() { return _trainingPath; };
    //const std::string &testingPath() { return _testingPath; };
    //const std::string &yieldsPath() { return _yieldsPath; };
    //const std::string &dataPath() { return _yieldsPath; };
    const std::string &outputPath() const { return _outputPath; };
    const std::string &outputName() const { return _outputName; };
    const std::string &treeName() const { return _treeName; };
    const std::string &lepton() { return _lepton; };
    const std::string &jetBin() { return _jetBin; };
    const std::string &eventWeight() const { return _eventWeight; };
    const std::vector<std::string> &samples() { return _samples; };
    const std::vector<std::string> &signals() { return _signals; };
    const std::vector<std::string> &variables() const { return _variables; };
    const std::vector<std::string> &spectators() const { return _spectators; };
    const std::vector<std::string> &methods() const { return _methods; };
    const std::vector<std::string> &trainingSignals() const { return _trainingSignals; };
    const std::vector<std::string> &trainingBackgrounds() const { return _trainingBackgrounds; };
    const std::vector<std::string> &testingSignals() const { return _testingSignals; };
    const std::vector<std::string> &testingBackgrounds() const { return _testingBackgrounds; };
    const std::vector<std::string> &yieldsSignals() const { return _yieldsSignals; };
    const std::vector<std::string> &yieldsBackgrounds() const { return _yieldsBackgrounds; };
    const std::string &data() const { return _data; };
    const TMVA::Types::EMVA methodType(std::string &method) { return _methodsType[method]; };
    const std::string &methodParams(std::string &method) { return _methodsParams[method]; };
    const std::string &methodTreeName(std::string &method) { return _methodsTreeName[method]; };
    const std::string &methodLeaveName(std::string &method) { return _methodsLeaveName[method]; };
    const std::vector<std::string> &methodTrainingSignals(std::string &method) { return _methodsTrainingSignals[method]; };
    const std::vector<std::string> &methodTrainingBackgrounds(std::string &method) { return _methodsTrainingBackgrounds[method]; };
    const std::vector<std::string> &methodTestingSignals(std::string &method) { return _methodsTestingSignals[method]; };
    const std::vector<std::string> &methodTestingBackgrounds(std::string &method) { return _methodsTestingBackgrounds[method]; };
    const std::vector<std::string> &methodYieldsSignals(std::string &method) { return _methodsYieldsSignals[method]; };
    const std::vector<std::string> &methodYieldsBackgrounds(std::string &method) { return _methodsYieldsBackgrounds[method]; };
    void dump();
  private:
    std::vector<std::string> getInputPaths(std::string cfgName, std::string path, std::string prefix, std::string suffix, bool signal);
    void checkMethodsInputs(std::string cfgName, std::vector<std::string> &paths1, std::vector<std::string> &paths2, std::string tname1, std::string tname2, std::string lname);
    std::string _configFile;
    std::string _trainingPath;
    std::string _testingPath;
    std::string _yieldsPath;
    std::string _dataPath;
    std::string _outputPath;
    std::string _outputName;
    std::string _treeName;
    std::string _lepton;
    std::string _jetBin;
    std::string _eventWeight;
    std::vector<std::string> _samples;
    std::vector<std::string> _signals;
    std::vector<std::string> _variables;
    std::vector<std::string> _spectators;
    std::vector<std::string> _methods;
    std::vector<std::string> _trainingSignals;
    std::vector<std::string> _trainingBackgrounds;
    std::vector<std::string> _testingSignals;
    std::vector<std::string> _testingBackgrounds;
    std::vector<std::string> _yieldsSignals;
    std::vector<std::string> _yieldsBackgrounds;
    std::string _data;
#if not defined(__CINT__) || defined(__MAKECINT__)
    std::map<std::string, TMVA::Types::EMVA> _methodsType;
#else
    std::map<std::string, Int_t> _methodsType;
#endif
    std::map<std::string, std::string> _methodsParams;
    std::map<std::string, std::string> _methodsTreeName;
    std::map<std::string, std::string> _methodsLeaveName;
    std::map<std::string, std::vector<std::string> > _methodsTrainingSignals;
    std::map<std::string, std::vector<std::string> > _methodsTrainingBackgrounds;
    std::map<std::string, std::vector<std::string> > _methodsTestingSignals;
    std::map<std::string, std::vector<std::string> > _methodsTestingBackgrounds;
    std::map<std::string, std::vector<std::string> > _methodsYieldsSignals;
    std::map<std::string, std::vector<std::string> > _methodsYieldsBackgrounds;
    std::map<std::string, std::string> _methodsData;
};

#endif
