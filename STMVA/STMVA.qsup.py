#!/usr/bin/env python
import os, sys, re

INPUT = 'STMVA.params.in'
OUTPUT = 'STMVA.params.out'
TEMPLATE = 'STMVA.params.template'
CHANNELS = [
  ('EqOneTag', 'EqTwoJet'),
  ('EqOneTag', 'EqThreeJet'),
  ('EqTwoTag', 'EqTwoJet'),
  ('EqTwoTag', 'EqThreeJet'),
]
SIGNALS = [ 'tb', 'tqb', 'tb tqb', ]
METHODS = [ 'BNN', 'BDT', ]
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

for method in METHODS:
    for i, channel in enumerate(CHANNELS):
        tagBin, jetBin = channel
        variables = [ x[0] for x in filter(lambda x: method in x[1][i], varList.items()) ]
        for signal in SIGNALS:
            output = "STMVA.%s_%s_%s.root" % (tagBin, jetBin, signal.replace(" ",""))
            data = open(TEMPLATE).read()
            data = data.replace('XXX_OUTPUT_XXX', output)
            data = data.replace('XXX_TAG_XXX', tagBin)
            data = data.replace('XXX_JET_XXX', jetBin)
            data = data.replace('XXX_SIGNAL_XXX', signal)
            data = data.replace('XXX_SIGNAL2_XXX', signal.replace("_",""))
            data = data.replace('XXX_VARIABLE_XXX', ' '.join(variables))
            data = data.replace('XXX_METHOD_XXX', method)
            #data = data.replace('XXX_METHOD_XXX', ' '.join([method]+EXTMET))
            f = open("params/%s_%s_%s_%s_%s" % (OUTPUT, method, tagBin, jetBin, signal.replace(' ', '')), "w")
            f.write(data)
            f.close()
