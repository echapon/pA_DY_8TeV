#!/bin/bash

dir=/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis

cd $dir
source /afs/cern.ch/sw/lcg/external/gcc/4.9/x86_64-slc6-gcc49-opt/setup.sh
source /afs/cern.ch/sw/lcg/app/releases/ROOT/6.04.02/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh

if [ $1 -eq 0 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",true,HFweight::HFside::both)'
elif [ $1 -eq 1 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",true,HFweight::HFside::plus)'
elif [ $1 -eq 2 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",true,HFweight::HFside::minus)'
elif [ $1 -eq 3 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",false)'
elif [ $1 -eq 4 ]; then
   arg='(kFALSE,"Pyquen","PAL3Mu12",true,HFweight::HFside::both)'
elif [ $1 -eq 5 ]; then
   arg='(kTRUE,"Powheg","PAL3Mu12",true,HFweight::HFside::both)'
elif [ $1 -eq 6 ]; then
   arg='(kFALSE,"Data")'
elif [ $1 -eq 7 ]; then
   arg='(kTRUE,"Data")'
fi

echo root -l -b -q ControlPlots/MuonPlots.C+${arg}
root -l -b -q ControlPlots/MuonPlots.C+${arg}
