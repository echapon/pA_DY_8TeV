#!/bin/bash

#histograms_20190306v9_iso0_0p2_HFboth
#histograms_20190306v9_iso0_0p2_HFminus

#mkdir histogramsFit_20190306v9_iso0_0p2_HFminus
#mkdir printFit_20190306v9_iso0_0p2_HFminus

mkdir histogramsFit_20190411v9_iso0_0p2_HFminus
mkdir printFit_20190411v9_iso0_0p2_HFminus
 

####################### L3Mu12 ######################

####################################################

### Nominal (Inclusive, Barrel, Endcap)

root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","","trkIso/p_{T}",0.0,0.2,1050,2,"reltrkisoR03muptlt10","_L3Mu12")'
root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_barrel","trkIso/p_{T}",0.0,0.2,1050,2,"reltrkisoR03muptlt10","_L3Mu12")'
root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_endcap","trkIso/p_{T}",0.0,0.2,1050,2,"reltrkisoR03muptlt10","_L3Mu12")'


### SF uncertainties variation

#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","","trkIso/p_{T}",0.0,0.2,1005,2,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_barrel","trkIso/p_{T}",0.0,0.2,1005,2,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_endcap","trkIso/p_{T}",0.0,0.2,1005,2,"reltrkisoR03muptlt10","_L3Mu12")'


#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","","trkIso/p_{T}",0.0,0.2,1010,2,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_barrel","trkIso/p_{T}",0.0,0.2,1010,2,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_endcap","trkIso/p_{T}",0.0,0.2,1010,2,"reltrkisoR03muptlt10","_L3Mu12")'

#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","","trkIso/p_{T}",0.0,0.2,1020,2,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_barrel","trkIso/p_{T}",0.0,0.2,1020,2,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_endcap","trkIso/p_{T}",0.0,0.2,1020,2,"reltrkisoR03muptlt10","_L3Mu12")'

#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","","trkIso/p_{T}",0.0,0.2,1030,2,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_barrel","trkIso/p_{T}",0.0,0.2,1030,2,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_endcap","trkIso/p_{T}",0.0,0.2,1030,2,"reltrkisoR03muptlt10","_L3Mu12")'

#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","","trkIso/p_{T}",0.0,0.2,1040,2,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_barrel","trkIso/p_{T}",0.0,0.2,1040,2,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_endcap","trkIso/p_{T}",0.0,0.2,1040,2,"reltrkisoR03muptlt10","_L3Mu12")'

#################################################################

### use QCD MC as template (QCDopt1)
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","","trkIso/p_{T}",0.0,0.2,1005,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_barrel","trkIso/p_{T}",0.0,0.2,1005,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_endcap","trkIso/p_{T}",0.0,0.2,1005,1,"reltrkisoR03muptlt10","_L3Mu12")'

#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","","trkIso/p_{T}",0.0,0.2,1010,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_barrel","trkIso/p_{T}",0.0,0.2,1010,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_endcap","trkIso/p_{T}",0.0,0.2,1010,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","","trkIso/p_{T}",0.0,0.2,1020,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_barrel","trkIso/p_{T}",0.0,0.2,1020,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_endcap","trkIso/p_{T}",0.0,0.2,1020,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","","trkIso/p_{T}",0.0,0.2,1030,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_barrel","trkIso/p_{T}",0.0,0.2,1030,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_endcap","trkIso/p_{T}",0.0,0.2,1030,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","","trkIso/p_{T}",0.0,0.2,1040,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_barrel","trkIso/p_{T}",0.0,0.2,1040,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_endcap","trkIso/p_{T}",0.0,0.2,1040,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","","trkIso/p_{T}",0.0,0.2,1050,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_barrel","trkIso/p_{T}",0.0,0.2,1050,1,"reltrkisoR03muptlt10","_L3Mu12")'
#root -l -b -q 'fitTemplates_opttest.cc++("histograms_20190306v9_iso0_0p2_HFminus","20190411v9_iso0_0p2_HFminus","_endcap","trkIso/p_{T}",0.0,0.2,1050,1,"reltrkisoR03muptlt10","_L3Mu12")'

####################################################


exit 0

