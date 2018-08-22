#!/bin/bash

# fill histos
root -l -b -q fillHistograms.C'("Data","PAL1DoubleMu0")' 
root -l -b -q fillHistograms.C'("Powheg","PAL1DoubleMu0")' 
root -l -b -q fillHistograms.C'("Data","PAL3Mu12")' 
root -l -b -q fillHistograms.C'("Powheg","PAL3Mu12")' 

# run ABCD method
root -l -b -q runABCD.C'("PAL1DoubleMu0","DataMinusMC")'
root -l -b -q runABCD.C'("PAL1DoubleMu0","Wjets")'
root -l -b -q runABCD.C'("PAL1DoubleMu0","QCD")'
root -l -b -q runABCD.C'("PAL3Mu12","DataMinusMC")'
root -l -b -q runABCD.C'("PAL3Mu12","Wjets")'
root -l -b -q runABCD.C'("PAL3Mu12","QCD")'

# make plots
root -l -b -q plotABCD.C'("PAL1DoubleMu0","DataMinusMC")'
root -l -b -q plotABCD.C'("PAL1DoubleMu0","Wjets")'
root -l -b -q plotABCD.C'("PAL1DoubleMu0","QCD")'
root -l -b -q plotABCD.C'("PAL3Mu12","DataMinusMC")'
root -l -b -q plotABCD.C'("PAL3Mu12","Wjets")'
root -l -b -q plotABCD.C'("PAL3Mu12","QCD")'
