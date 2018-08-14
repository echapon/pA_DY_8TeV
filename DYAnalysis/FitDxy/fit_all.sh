#!/bin/bash

inputhist="histos_L1DoubleMu0_kde_tkiso2p5_dxy1e-2_dz1e-1_pt0707_morehistos.root"
basenameout="output_L1DoubleMu0_kde_tkiso2p5_dxy1e-2_dz1e-1_pt0707_bkg"
outputdir="fits_L1DoubleMu0_kde_tkiso2p5_dxy1e-2_dz1e-1_pt0707"

mkdir -p $outputdir
for ij in "1,2" "3,4" "5,6" "7,10" "8,9" "3,13" "14,15"; do
   ij2=`echo $ij | sed 's/,//'`
   root -l -b -q fit.C+'("'${inputhist}'","'${basenameout}${ij2}'.root",'${ij}')'
   mkdir bkg${ij2}
   mv fit_mass_* fit_rap1560_* fit_rap60120_* fit_pt_* fit_phistar_* bkg${ij2}
   mv bkg${ij2} $outputdir
done
