#!/bin/bash

#void estimateFR_opttest_v20190918_ptincl(
#  TString indir="20190906v62_60ptincl_iso0_0p2_nominalfixed",								//	directory for template fitting (L66,67)
#  TString rmk="opt1050_QCDopt2_reltrkisoR03muptlt10_L3Mu12",								// file name for template fitting (L66,67)
#		TFile* fin_fitNch_barrel = new TFile(Form("histogramsFit_%s/histFRFit_barrel_%s.root",indir.Data(),rmk.Data()),"read");

#  TString rmk1="reltrkisoR03muptlt10",															// file name before "channel tag" for ntuples (L99,102,103,...)
#  TString rmk2="L3Mu12",																				// file nameafter "channel tag" for ntuples (L99,102,103...)
#		file[i] = new TFile(Form("histograms_%s/histFR_%s_%s_%s.root",indir2.Data(),rmk1.Data(),Name(tag),rmk2.Data()));

#  int QCDopt=2,																							// QCDoption for template fitting, QCDopt=3 is nominal
#  TString indir2="20190901v50_iso0_1_nominal",													// directory for ntuples with SS(Same-signed)
#		fileSS1 = new TFile(Form("histograms_%s/histFR_SS%s_Data1_%s.root",indir2.Data(),rmk1.Data(),rmk2.Data()));

#  TString indir3="20190906v62_60ptincl_iso0_0p2_nominalfixed",							// directory for output ...,L776,L778)
#  TString rmkf="reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt2_L3Mu12_FRopt101",		// file name for output FR
#		TFile* g = new TFile(Form("resultEst_%s/fakerate_%s.root",indir3.Data(),rmkf.Data()),"RECREATE");

#	int FRopt=101																							// FR formula option, 205 is nominal
#) {

mkdir printEst_20190926v103r2_ptincl_iso0_0p2
mkdir resultEst_20190926v103r2_ptincl_iso0_0p2

## nominal
#root -l -b -q 'estimateFR_opttest_v20190918_ptincl.cc++("20190917v103r2_iso0_0p2_ptincl","opt1050_QCDopt3_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",3,"20190901v50_iso0_1_nominal","20190926v103r2_ptincl_iso0_0p2","reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt3_L3Mu12_FRopt205",205)'

## option 1 : different FRs
#root -l -b -q 'estimateFR_opttest_v20190918_ptincl.cc++("20190917v103r2_iso0_0p2_ptincl","opt1050_QCDopt3_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",3,"20190901v50_iso0_1_nominal","20190926v103r2_ptincl_iso0_0p2","reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt3_L3Mu12_FRopt102",102)'
#root -l -b -q 'estimateFR_opttest_v20190918_ptincl.cc++("20190917v103r2_iso0_0p2_ptincl","opt1050_QCDopt3_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",3,"20190901v50_iso0_1_nominal","20190926v103r2_ptincl_iso0_0p2","reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt3_L3Mu12_FRopt200",200)'
#root -l -b -q 'estimateFR_opttest_v20190918_ptincl.cc++("20190917v103r2_iso0_0p2_ptincl","opt1050_QCDopt3_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",3,"20190901v50_iso0_1_nominal","20190926v103r2_ptincl_iso0_0p2","reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt3_L3Mu12_FRopt201",201)'
#root -l -b -q 'estimateFR_opttest_v20190918_ptincl.cc++("20190917v103r2_iso0_0p2_ptincl","opt1050_QCDopt3_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",3,"20190901v50_iso0_1_nominal","20190926v103r2_ptincl_iso0_0p2","reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt3_L3Mu12_FRopt202",202)'
#root -l -b -q 'estimateFR_opttest_v20190918_ptincl.cc++("20190917v103r2_iso0_0p2_ptincl","opt1050_QCDopt3_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",3,"20190901v50_iso0_1_nominal","20190926v103r2_ptincl_iso0_0p2","reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt3_L3Mu12_FRopt203",203)'
#root -l -b -q 'estimateFR_opttest_v20190918_ptincl.cc++("20190917v103r2_iso0_0p2_ptincl","opt1050_QCDopt3_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",3,"20190901v50_iso0_1_nominal","20190926v103r2_ptincl_iso0_0p2","reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt3_L3Mu12_FRopt204",204)'

## option 2 : different QCD templates

root -l -b -q 'estimateFR_opttest_v20190918_ptincl.cc++("20190906v62_60ptincl_iso0_0p2_nominalfixed","opt1050_QCDopt1_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",1,"20190901v50_iso0_1_nominal","20190926v103r2_ptincl_iso0_0p2","reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt1_L3Mu12_FRopt205",205)'
root -l -b -q 'estimateFR_opttest_v20190918_ptincl.cc++("20190906v62_60ptincl_iso0_0p2_nominalfixed","opt1050_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",2,"20190901v50_iso0_1_nominal","20190926v103r2_ptincl_iso0_0p2","reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt2_L3Mu12_FRopt205",205)'



exit 0

