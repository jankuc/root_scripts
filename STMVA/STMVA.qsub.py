#!/usr/bin/env python
import os, sys, re

INPUT = 'STMVA.params.in'
OUTPUT = 'STMVA.params.out'
TEMPLATE = 'STMVA.setup'
CHANNELS = [
  ('muo', '2jet'),
  ('muo', '3jet'),
  ('muo', '4jet'),
  ('ele', '2jet'),
  ('ele', '3jet'),
  ('ele', '4jet'),
]
SIGNALS = [ 'ttA_172', ]
METHODS = [ 'BNN', 'BDT', ]

q = open("qsub/qsub.sh", "w")
for method in METHODS:
    for i, channel in enumerate(CHANNELS):
        tagBin, jetBin = channel
        for signal in SIGNALS:
            output = "STMVA.%s_%s_%s.root" % (tagBin, jetBin, signal.replace(" ",""))
            data = open(TEMPLATE).read()
	    param_name = "%s_%s_%s_%s_%s" % (OUTPUT, method, tagBin, jetBin, signal.replace(' ', ''))
            #data = data.replace('XXX_METHOD_XXX', ' '.join([method]+EXTMET))
            f = open("qsub/%s.sh" % (param_name, ), "w")
            f.write(data)
	    f.write("/work/budvar-clued0/francji/scripts/STMVA/STMVA /work/budvar-clued0/francji/scripts/STMVA/params/%s\n" % param_name)
            f.close()
	    q.write("qsub -q HUGEMEM -l mem=1950mb %s.sh\n" % param_name)
q.close()

