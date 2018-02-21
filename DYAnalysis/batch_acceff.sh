#!/bin/bash

dir=/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis

cd $dir
source /afs/cern.ch/sw/lcg/external/gcc/4.9/x86_64-slc6-gcc49-opt/setup.sh
source /afs/cern.ch/sw/lcg/app/releases/ROOT/6.04.02/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh

# nominal
if [ $1 -eq 0 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",0,true,HFweight::HFside::both)'

# alternative reweighting
elif [ $1 -eq 1 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",0,true,HFweight::HFside::Ntracks)'
elif [ $1 -eq 2 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",0,true,HFweight::HFside::plus)'
elif [ $1 -eq 3 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",0,true,HFweight::HFside::minus)'
elif [ $1 -eq 4 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",0,false)'

# Pyquen
elif [ $1 -eq 5 ]; then
   arg='(kFALSE,"Pyquen","PAL3Mu12",0,true,HFweight::HFside::both)'

# pPb vs PbP
elif [ $1 -eq 6 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",1,true,HFweight::HFside::both)'
elif [ $1 -eq 7 ]; then
   arg='(kFALSE,"Powheg","PAL3Mu12",2,true,HFweight::HFside::both)'

# MomCor
elif [ $1 -eq 8 ]; then
   arg='(kTRUE,"Powheg","PAL3Mu12",0,true,HFweight::HFside::both,0,0)'
elif [ $1 -eq 9 ]; then
   arg='(kTRUE,"Powheg","PAL3Mu12",0,true,HFweight::HFside::both,7,6)'
fi

echo root -l -b -q AccEff/Acc_Eff.C+${arg}
root -l -b -q AccEff/Acc_Eff.C+${arg}
