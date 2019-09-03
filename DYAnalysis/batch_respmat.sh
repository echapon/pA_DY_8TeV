#!/bin/bash

# first argument, from 0 to 10: variation
# second argument: trigger

trigger=PAL3Mu12
# trigger=PAL1DoubleMu0
# trigger=$2

dir=/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis

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

# nominal
if [ $1 -eq 0 ]; then
   arg='(kTRUE,"Powheg","'$trigger'",0)'

# others
elif [ $1 -eq 1 ]; then
   arg='(kTRUE,"Pyquen","'$trigger'",0)'
elif [ $1 -eq 2 ]; then
   arg='(kFALSE,"Powheg","'$trigger'",0)'
elif [ $1 -eq 3 ]; then
   arg='(kTRUE,"Powheg","'$trigger'",1)'
elif [ $1 -eq 4 ]; then
   arg='(kTRUE,"Powheg","'$trigger'",2)'

echo root -l -b -q ResponseMatrix/MuonResponseMatrix_1D.C+${arg}
root -l -b -q ResponseMatrix/MuonResponseMatrix_1D.C+${arg}
