#!/bin/bash

dir=/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis

cd $dir
source /afs/cern.ch/sw/lcg/external/gcc/4.9/x86_64-slc6-gcc49-opt/setup.sh
source /afs/cern.ch/sw/lcg/app/releases/ROOT/6.04.02/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh

# nominal
if [ $1 -eq 0 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",true,HFweight::HFside::both,true)'

# alternative reweighting
elif [ $1 -eq 1 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",true,HFweight::HFside::both,false)'
elif [ $1 -eq 2 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",true,HFweight::HFside::Ntracks,true)'
elif [ $1 -eq 3 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",true,HFweight::HFside::plus,true)'
elif [ $1 -eq 4 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",true,HFweight::HFside::minus,true)'
elif [ $1 -eq 5 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",false)'

# Pyquen
elif [ $1 -eq 6 ]; then
   arg='(kFALSE,"Pyquen","PAL3Mu12",true,HFweight::HFside::both,true)'

# MomCor
elif [ $1 -eq 7 ]; then
   arg='(kTRUE,"Powheg","PAL3Mu12",true,HFweight::HFside::both,true,0,0)'
elif [ $1 -eq 8 ]; then
   arg='(kTRUE,"Powheg","PAL3Mu12",true,HFweight::HFside::both,true,7,6)'

# no Zpt reweighting
elif [ $1 -eq 10 ]; then
   arg='(kTRUE,"Powheg","PAL3Mu12",true,HFweight::HFside::both,true,0,0,false)'

# Data
elif [ $1 -eq 10 ]; then
   arg='(kFALSE,"Data")'
elif [ $1 -eq 11 ]; then
   arg='(kTRUE,"Data","PAL3Mu12",false,HFweight::HFside::both,false,0,0)'
elif [ $1 -eq 12 ]; then
   arg='(kTRUE,"Data","PAL3Mu12",false,HFweight::HFside::both,false,7,6)'
fi

echo root -l -b -q ControlPlots/MuonPlots.C+${arg}
root -l -b -q ControlPlots/MuonPlots.C+${arg}
