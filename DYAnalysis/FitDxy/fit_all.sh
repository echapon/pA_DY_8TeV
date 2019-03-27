#!/bin/bash

inputhist="histos_PAL3Mu12_pt1510_20190326.root"
basenameout="output_L3Mu12_pt1510_bkg_20190326_"
outputdir="fits_L3Mu12_pt1510_20190326"

mkdir -p $outputdir
for ij in "7,10" "3,13" "14,15"; do
# for ij in "1,2" "3,4" "5,6" "7,10" "8,9" "3,13" "14,15"; do
# for ij in "1,2" "3,4" "5,6"; do
   ij2=`echo $ij | sed 's/,//'`
   root -l -b -t -q fit.C+'("'${inputhist}'","'${basenameout}${ij2}'.root",'${ij}')'
   mkdir bkg${ij2}
   mv fit_mass_* fit_rap1560_* fit_rap60120_* fit_pt_* fit_phistar_* bkg${ij2}
   mv bkg${ij2} $outputdir
done
