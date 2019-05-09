#!/bin/bash

dir=/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR

cd $dir
# source /afs/cern.ch/sw/lcg/external/gcc/4.9/x86_64-slc6-gcc49-opt/setup.sh
# source /afs/cern.ch/sw/lcg/app/releases/ROOT/6.04.02/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh

# # for SLC6
# source /cvmfs/sft.cern.ch/lcg/external/gcc/4.9.1/x86_64-slc6-gcc48-opt/setup.sh /cvmfs/sft.cern.ch/lcg/external
# source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.06.08/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh

# for CentOS7
source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.16.00/x86_64-centos7-gcc48-opt/bin/thisroot.sh
uname -a
which root


echo root -l -b -q applyFR.cc+'('${1}')'
root -l -b -q applyFR.cc+'('${1}')'
