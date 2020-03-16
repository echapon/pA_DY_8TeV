#!/bin/bash

######### PRE-UNFOLDING PLOTS #################
# nominal
root -l  -b -q ControlPlots/DrawControlPlot.C+'("PAL3Mu12",kTRUE,"Lumi","MomCorr00","rewboth",true)'
root -l  -b -q ControlPlots/DrawControlPlot.C+'("PAL3Mu12",kTRUE,"Lumi","MomCorr00","rewboth",true,false)' # do not apply Zpt reweighting
root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomCorr00_rewboth_tnprew_noZptrew.root","ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root","Plots/results/xsec_nom.root",false)'
root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomCorr00_rewboth_tnprew_noZptrew.root","ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root","Plots/results/xsec_noacc_nom.root",true,false,false)'

# no muon momentum correction
root -l  -b -q ControlPlots/DrawControlPlot.C+'("PAL3Mu12",kTRUE,"Lumi","MomUnCorr","rewboth",true)'
root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomUnCorr_rewboth_tnprew.root","ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_0_rewboth.root","Plots/results/xsec_MomUnCorr.root",true,false)'
root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomUnCorr_rewboth_tnprew.root","ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_0_rewboth.root","Plots/results/xsec_noacc_MomUnCorr.root",true,false,false)'

# Ntrk rewt
root -l  -b -q ControlPlots/DrawControlPlot.C+'("PAL3Mu12",kTRUE,"Lumi","MomCorr00","rewNtracks",true)'
root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomCorr00_rewNtracks_tnprew.root","ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewNtracks.root","Plots/results/xsec_rewNtracks.root",true,false)'
root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomCorr00_rewNtracks_tnprew.root","ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewNtracks.root","Plots/results/xsec_noacc_rewNtracks.root",true,false,false)'

# no tnp rew in histo filling
root -l  -b -q ControlPlots/DrawControlPlot.C+'("PAL3Mu12",kTRUE,"Lumi","MomCorr00","rewboth",false)'
root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomCorr00_rewboth_notnprew.root","ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root","Plots/results/xsec_notnprew.root",true,false)'
root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomCorr00_rewboth_notnprew.root","ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root","Plots/results/xsec_noacc_notnprew.root",true,false,false)'

# # part 1
# root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomUnCorr_rewboth_tnprew.root","ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_1_rewboth.root","Plots/results/xsec_part1.root",true)'

# # part 2
# root -l -b -q Plots/myXsec.C+'("ROOTFile_YieldHistogram_MomUnCorr_rewboth_tnprew.root","ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_2_rewboth.root","Plots/results/xsec_part2.root",true)'


###########################################
## DO DETECTOR RESOLUTION UNFOLDING HERE ##
###########################################

########### POST - DET RES UNFOLD PLOTS ###########
# nom
root -l -b -q Plots/myXsec.C+'("ResponseMatrix/yields_detcor_Powheg_MomCorr00_0.root","ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root","Plots/results/xsec_nom_detcor.root",true,false,true,false)'
root -l -b -q Plots/myXsec.C+'("ResponseMatrix/yields_detcor_Powheg_MomCorr00_0.root","ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root","Plots/results/xsec_noacc_nom_detcor.root",true,false,false,false)'


###########################################
##          DO FSR UNFOLDING HERE        ##
###########################################

########### POST - FSR UNFOLD PLOTS ###########

# nom
root -l -b -q Plots/myXsec.C+'("FSRCorrection/xsec_FSRcor_Powheg_MomCorr00_0.root","ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root","Plots/results/xsec_nom_detcor_FSR.root",false,true,true)'
root -l -b -q Plots/myXsec.C+'("FSRCorrection/xsec_noacc_FSRcor_Powheg_MomCorr00_0.root","ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root","Plots/results/xsec_noacc_nom_detcor_FSR.root",false,true,false)'
