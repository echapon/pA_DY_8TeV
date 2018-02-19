#!/bin/bash

dir=/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis

cd $dir
source /afs/cern.ch/sw/lcg/external/gcc/4.9/x86_64-slc6-gcc49-opt/setup.sh
source /afs/cern.ch/sw/lcg/app/releases/ROOT/6.04.02/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh

if [ $1 -eq 0 ]; then
   arg='(kTRUE,"Data","PAL3Mu12",false,HFweight::HFside::both,false,0,0)'
elif [ $1 -le 100 ]; then
   i=$(( $1 - 1 ))
   arg='(kTRUE,"Data","PAL3Mu12",false,HFweight::HFside::both,false,1,'$i')'
elif [ $1 -eq 101 ]; then
   arg='(kTRUE,"Data","PAL3Mu12",false,HFweight::HFside::both,false,2,0)'
elif [ $1 -le 106 ]; then
   i=$(( $1 - 102 ))
   arg='(kTRUE,"Data","PAL3Mu12",false,HFweight::HFside::both,false,4,'$i')'
elif [ $1 -le 111 ]; then
   i=$(( $1 - 107 ))
   arg='(kTRUE,"Data","PAL3Mu12",false,HFweight::HFside::both,false,5,'$i')'
else
   arg='(kTRUE,"Data","PAL3Mu12",false,HFweight::HFside::both,false,7,6)'
fi

echo root -l -b -q ControlPlots/MuonPlots.C+${arg}
root -l -b -q ControlPlots/MuonPlots.C+${arg}
