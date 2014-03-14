#!/usr/bin/env python
import os, sys, re

INPUT = 'STMVA.params.in'
OUTPUT = 'STMVA.params.out'
TEMPLATE = 'STMVA.params.template.tt_leptonjets.ext'
CHANNELS = [
  ('muo', '2jet'),
  ('muo', '3jet'),
  ('muo', '4jet'),
  ('ele', '2jet'),
  ('ele', '3jet'),
  ('ele', '4jet'),
]

SIGNALS = [ 'ttA_172', ]
#METHODS = [ 'BNN', 'BDT', ]
METHODS = [ 'MBC_and_BDT' ]
SAMPLES_MU = ["QCD","Wlp","Wcc","Wbb","ZbbMuMu","ZbbTauTau","ZccMuMu","ZccTauTau","ZlpMuMu","ZlpTauTau","tb","tqb","WW","WZ","ZZ","ttAll_172","ttA_172"] 
SAMPLES_ELE = ["QCD","Wlp","Wcc","Wbb","ZbbEE","ZbbTauTau","ZccEE","ZccTauTau","ZlpEE","ZlpTauTau","tb","tqb","WW","WZ","ZZ","ttAll_172","ttA_172"]
#EXTMET = [ 'GLM' ]

reLine = re.compile('^(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)$')

varList = {}
for line in open(INPUT).readlines():
    if len(line.strip()) == 0: # skip empty lines
        continue
    if line.lstrip()[0] == '#': # skip comments
        continue
    res = reLine.match(line)
    if res == None:
        raise Exception("unknown line format: %s" % line.rstrip())
    varName = res.group(1)
    varList[varName] = (res.group(2).split(','), res.group(3).split(','), res.group(4).split(','), res.group(5).split(','))

if not os.path.exists('params'):
    os.mkdir('params')

VARIABLES_2 = ["Apla", "Spher", "HTL", "JetMt", "MEvent", "MT1NL", "M01mall", "M0nl", "M1nl", "MT0nl", "Met", "Mtt", "Mva_max", "Wmt", "Wpt", "Centr", "DRminejet", "DiJetDrmin", "Ht", "Ht20", "Ktminp", "Lepdphimet", "Lepemv"]
VARIABLES_34 = ["Apla", "Spher", "HTL", "JetMt", "HT3", "MEvent", "MT1NL", "M01mall", "M0nl", "M1nl", "MT0nl", "Met", "Mtt", "Mva_max", "Wmt", "Wpt", "Centr", "DRminejet", "DiJetDrmin", "Ht", "Ht20", "Ktminp", "Lepdphimet", "Lepemv"]



for method in METHODS:
    for i, channel in enumerate(CHANNELS):
        lepton, jetBin = channel
        #variables = [ x[0] for x in filter(lambda x: method in x[1][i], varList.items()) ]
        for signal in SIGNALS:
            output = "STMVA.%s_%s_%s.root" % (lepton, jetBin, signal.replace(" ",""))
            data = open(TEMPLATE).read()
            data = data.replace('XXX_OUTPUT_XXX', output)
            data = data.replace('XXX_LEPTON_XXX', lepton)
            data = data.replace('XXX_JET_XXX', jetBin)
            
            if lepton == "ele":
						    samples = SAMPLES_ELE
            if lepton == "muo":
						    samples = SAMPLES_MU
            data = data.replace('XXX_SAMPLES_XXX', ' '.join(samples))
            
            data = data.replace('XXX_SIGNAL_XXX', signal)
            data = data.replace('XXX_SIGNAL2_XXX', signal.replace(" ",""))
            
            if jetBin == "2jet":
						    variables = VARIABLES_2
            else:
							  variables = VARIABLES_34
            if lepton == "muo":
							  variables = variables[:-1]
            variables = map(lambda x: x+'/F', variables)
            data = data.replace('XXX_VARIABLE_XXX', ' '.join(variables))
            
            data = data.replace('XXX_METHOD_XXX', method)
            #data = data.replace('XXX_METHOD_XXX', ' '.join([method]+EXTMET))
            f = open("params/%s_%s_%s_%s_%s" % (OUTPUT, method, lepton, jetBin, signal.replace(' ', '')), "w")
            f.write(data)
            f.close()
