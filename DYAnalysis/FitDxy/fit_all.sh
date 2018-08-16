#!/bin/bash

inputhist="histos_L3Mu12_kde_nocuts_pt1507_morehistos.root"
basenameout="output_L3Mu12_kde_nocuts_pt1507_bkg"
outputdir="fits_L3Mu12_kde_nocuts_pt1507"

mkdir -p $outputdir
# for ij in "1,2" "3,4" "5,6" "7,10" "8,9" "3,13" "14,15"; do
for ij in "1,2" "3,4" "5,6"; do
   ij2=`echo $ij | sed 's/,//'`
   root -l -b -q fit.C+'("'${inputhist}'","'${basenameout}${ij2}'.root",'${ij}')'
   mkdir bkg${ij2}
   mv fit_mass_* fit_rap1560_* fit_rap60120_* fit_pt_* fit_phistar_* bkg${ij2}
   mv bkg${ij2} $outputdir
done
