#!/bin/sh

source /usr/local/bin/clued0_setup.sh

setup ld v2_16_1
setup gcc v3_4_3_glibc_232_dzero
setup python v2_6_4
setup gsl v1_11_lts3
#setup cern 2000_eh -q +gcc342
#setup TMVA v04-01-00

#export ROOTSYS=/rooms/prague/vokac/apps/root_v5.30.01.lts4.python26
#export ROOTSYS=/rooms/prague/vokac/apps/root_v5_30_01-GCC_3_4_3-python-2_6-TMVA-lts4
#export ROOTSYS=/rooms/prague/vokac/apps/root_v5_30_06-GCC_3_4_3-python-2_6-TMVA-slf5
#export ROOTSYS=/rooms/prague/vokac/apps/root_v5_32_03-GCC_3_4_3-python-2_6-TMVA-slf5
#export ROOTSYS=/rooms/prague/vokac/apps/root_v5_32_00-GCC_3_4_3-python-2_6-TMVA-lts4
#export ROOTSYS=/rooms/prague/vokac/apps/root_v5_34_00-GCC_3_4_3-python-2_6-TMVA-slf5
export ROOTSYS=/rooms/prague/vokac/apps/root_v5_34_03-GCC_3_4_3-python-2_6-TMVA-slf5
export PATH=$ROOTSYS/bin:$PATH
export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/rooms/prague/vokac/mj/work/vjets_cafe-v05-07-23-devel-201303.current/top_multijet/scripts:$ROOTSYS/lib:/rooms/prague/vokac/apps/pyminuit2-1.1.0/lib-root_v5_34_03-GCC_3_4_3-python-2_6-TMVA-slf5:${PYTHONPATH}
