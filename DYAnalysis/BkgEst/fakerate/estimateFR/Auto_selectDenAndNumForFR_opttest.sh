#!/bin/bash

mkdir histograms_20190327v11_iso0_0p2_HFminus
mkdir histograms_20190327v11_iso0_1_HFminus

## nominal (needed all cases)
root -l -b -q 'selectDenAndNumForFR_opttest.cc++("20190327v11_iso0_1_HFminus", 100, 0.0, 1.0, TT, QCD, "reltrkisoR03muptlt10", "", "_L3Mu12")'
root -l -b -q 'selectDenAndNumForFR_opttest.cc++("20190327v11_iso0_0p2_HFminus", 20, 0.0, 0.2, TT, QCD, "reltrkisoR03muptlt10", "", "_L3Mu12")'
root -l -b -q 'selectDenAndNumForFR_opttest.cc++("20190327v11_iso0_1_HFminus", 100, 0.0, 1.0, Data1, Data2, "SSreltrkisoR03muptlt10", "", "_L3Mu12")'
root -l -b -q 'selectDenAndNumForFR_opttest.cc++("20190327v11_iso0_0p2_HFminus", 20, 0.0, 0.2, Data1, Data2, "SSreltrkisoR03muptlt10", "", "_L3Mu12")'

#root -l -b -q 'selectDenAndNumForFR_opttest.cc++("20190327v11_iso0_1_HFminus", 100, 0.0, 1.0, TT, QCD, "reltrkisoR03muptlt10isomax0p5", "", "_L3Mu12")'
#root -l -b -q 'selectDenAndNumForFR_opttest.cc++("20190327v11_iso0_1_HFminus", 100, 0.0, 1.0, TT, QCD, "reltrkisoR03muptlt10isomax0p4", "", "_L3Mu12")'
#root -l -b -q 'selectDenAndNumForFR_opttest.cc++("20190327v11_iso0_1_HFminus", 100, 0.0, 1.0, TT, QCD, "reltrkisoR03muptlt10isomax0p3", "", "_L3Mu12")'
#root -l -b -q 'selectDenAndNumForFR_opttest.cc++("20190327v11_iso0_1_HFminus", 100, 0.0, 1.0, TT, QCD, "reltrkisoR03muptlt10isomax0p1", "", "_L3Mu12")'





