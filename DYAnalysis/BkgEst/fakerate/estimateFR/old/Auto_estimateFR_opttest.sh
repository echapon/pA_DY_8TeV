#!/bin/bash

root -l -b -q 'estimateFR_opttest.cc++("opt16_QCDopt2_histZVETO_SMUwoJET_MuPtlt10_QCDin0p875Data","histZVETO_SMUwoJET_MuPtlt10")'
root -l -b -q 'estimateFR_opttest.cc++("opt16_QCDopt2_histZVETO_SMUwJET_MuPtlt10_QCDin0p875Data","histZVETO_SMUwJET_MuPtlt10")'
root -l -b -q 'estimateFR_opttest.cc++("opt16_QCDopt2_histZVETO_SMUwJET_MuPtlt15_QCDin0p875Data","histZVETO_SMUwJET_MuPtlt15")'
root -l -b -q 'estimateFR_opttest.cc++("opt16_QCDopt2_histFRiso0p10_QCDin0p875Data","histFRiso0p10")'
root -l -b -q 'estimateFR_opttest.cc++("opt16_QCDopt2_histFRiso0p20_QCDin0p875Data","histFRiso0p20")'
root -l -b -q 'estimateFR_opttest.cc++("opt16_QCDopt2_histFR_QCDin0p875Data","histFR")'

root -l -b -q 'estimateFR_opttest.cc++("opt16_QCDopt1_histZVETO_SMUwoJET_MuPtlt10_QCDin0p875Data","histZVETO_SMUwoJET_MuPtlt10")'
root -l -b -q 'estimateFR_opttest.cc++("opt16_QCDopt1_histZVETO_SMUwJET_MuPtlt10_QCDin0p875Data","histZVETO_SMUwJET_MuPtlt10")'
root -l -b -q 'estimateFR_opttest.cc++("opt16_QCDopt1_histZVETO_SMUwJET_MuPtlt15_QCDin0p875Data","histZVETO_SMUwJET_MuPtlt15")'
root -l -b -q 'estimateFR_opttest.cc++("opt16_QCDopt1_histFRiso0p10_QCDin0p875Data","histFRiso0p10")'
root -l -b -q 'estimateFR_opttest.cc++("opt16_QCDopt1_histFRiso0p20_QCDin0p875Data","histFRiso0p20")'
root -l -b -q 'estimateFR_opttest.cc++("opt16_QCDopt1_histFR_QCDin0p875Data","histFR")'



#
#histograms/histFRQCDZZ.root      histograms/histFRiso0p20ZZ.root               histograms/histZVETO_SMUwoJET_MuPtlt10ZZ.root
#histograms/histFRZZ.root         histograms/histZVETO_SMUwJET_MuPtlt10ZZ.root
#histograms/histFRiso0p10ZZ.root  histograms/histZVETO_SMUwJET_MuPtlt15ZZ.root

exit 0

