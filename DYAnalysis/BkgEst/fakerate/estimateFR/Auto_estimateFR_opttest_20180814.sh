#!/bin/bash

#root -l -b -q 'estimateFR_opttest_v2.cc++("nominal","opt20_QCDopt1_histnominal","highPtMuonID")'
#root -l -b -q 'estimateFR_opttest_v2.cc++("nominal","opt20_QCDopt1_histnominal","tighthighPtMuonIDCommon")'
#root -l -b -q 'estimateFR_opttest_v2.cc++("nominal","opt20_QCDopt1_histnominal","noMuonID")'


root -l -b -q 'estimateFR_opttest_v2.cc++("new","opt20_QCDopt1_histnew","")'
#root -l -b -q 'estimateFR_opttest_v2.cc++("new","opt20_QCDopt2_histnew","")'
#root -l -b -q 'estimateFR_opttest_v2.cc++("new","opt20_QCDopt1_histnominal","")'
#root -l -b -q 'estimateFR_opttest_v2.cc++("new","opt20_QCDopt2_histnominal","")'








#
#histograms/histFRQCDZZ.root      histograms/histFRiso0p20ZZ.root               histograms/histZVETO_SMUwJET_MuPtlt10ZZ.root
#histograms/histFRZZ.root         histograms/histZVETO_SMUwJET_MuPtlt10ZZ.root
#histograms/histFRiso0p10ZZ.root  histograms/histZVETO_SMUwJET_MuPtlt15ZZ.root

exit 0

