#!/bin/bash

# nominal
root -l  -b -q ControlPlots/DrawControlPlot.C+'("None",kTRUE,"Lumi","MomCorr00","rewboth",true)'
root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomCorr00_rewboth_tnprew.root","ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root","Plots/results/xsec_nom.root",false)'

# no muon momentum correction
root -l  -b -q ControlPlots/DrawControlPlot.C+'("None",kTRUE,"Lumi","MomUnCorr","rewboth",true)'
root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomUnCorr_rewboth_tnprew.root","ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_0_rewboth.root","Plots/results/xsec_MomUnCorr.root",true)'

# Ntrk rewt
root -l  -b -q ControlPlots/DrawControlPlot.C+'("None",kTRUE,"Lumi","MomCorr00","rewNtracks",true)'
root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomCorr00_rewNtracks_tnprew.root","ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewNtracks.root","Plots/results/xsec_rewNtracks.root",false)'

# no tnp rew in histo filling
root -l  -b -q ControlPlots/DrawControlPlot.C+'("None",kTRUE,"Lumi","MomCorr00","rewboth",false)'
root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomCorr00_rewboth_notnprew.root","ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root","Plots/results/xsec_notnprew.root",false)'

# # part 1
# root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomUnCorr_rewboth_tnprew.root","ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_1_rewboth.root","Plots/results/xsec_part1.root",true)'

# # part 2
# root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomUnCorr_rewboth_tnprew.root","ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_2_rewboth.root","Plots/results/xsec_part2.root",true)'
