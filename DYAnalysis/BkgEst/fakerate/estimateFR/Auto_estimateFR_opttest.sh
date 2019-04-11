#!/bin/bash


mkdir printEst_20190411v11_iso0_1_HFminus
mkdir resultEst_20190411v11_iso0_1_HFminus

### For nominal
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1050_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",2,"20190306v9_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p2_opt1050_QCDopt2_L3Mu12")'

### For systematics
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1005_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p5","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p5_opt1005_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1005_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p4","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p4_opt1005_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1005_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p3","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p3_opt1005_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1005_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p1","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p1_opt1005_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1005_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",2,"20190306v9_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p2_opt1005_QCDopt2_L3Mu12")'


root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1010_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p5","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p5_opt1010_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1010_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p4","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p4_opt1010_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1010_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p3","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p3_opt1010_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1010_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p1","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p1_opt1010_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1010_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",2,"20190306v9_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p2_opt1010_QCDopt2_L3Mu12")'


root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1020_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p5","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p5_opt1020_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1020_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p4","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p4_opt1020_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1020_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p3","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p3_opt1020_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1020_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p1","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p1_opt1020_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1020_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",2,"20190306v9_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p2_opt1020_QCDopt2_L3Mu12")'


root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1030_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p5","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p5_opt1030_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1030_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p4","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p4_opt1030_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1030_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p3","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p3_opt1030_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1030_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p1","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p1_opt1030_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1030_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",2,"20190306v9_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p2_opt1030_QCDopt2_L3Mu12")'


root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1040_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p5","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p5_opt1040_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1040_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p4","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p4_opt1040_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1040_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p3","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p3_opt1040_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1040_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p1","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p1_opt1040_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1040_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10","L3Mu12",2,"20190306v9_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p2_opt1040_QCDopt2_L3Mu12")'


root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1050_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p5","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p5_opt1050_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1050_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p4","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p4_opt1050_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1050_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p3","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p3_opt1050_QCDopt2_L3Mu12")'
root -l -b -q 'estimateFR_opttest.cc++("20190411v9_iso0_0p2_HFminus","opt1050_QCDopt2_reltrkisoR03muptlt10_L3Mu12","reltrkisoR03muptlt10_isomax0p1","L3Mu12",2,"20190327v10_iso0_1_HFminus","20190411v11_iso0_1_HFminus","reltrkisoR03muptlt10_isomax0p1_opt1050_QCDopt2_L3Mu12")'

exit 0

