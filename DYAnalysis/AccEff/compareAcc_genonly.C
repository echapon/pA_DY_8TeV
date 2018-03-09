#include "TFile.h"
#include "Include/MyCanvas.C"
#include "BkgEst/interface/defs.h"
#include "TGraphAsymmErrors.h"

void setStyle(TGraphAsymmErrors *e, Color_t color=kBlack, Style_t markerstyle=kFullCircle);

void compareAcc_genonly(TString variable="mass") { // mass | pt | rap1560 | rap60120 | phistar
   TFile *fnom = TFile::Open("ROOTFile_Histogram_Acc_weights_genonly_EPPS16.root");
   TFile *fCT14 = TFile::Open("ROOTFile_Histogram_Acc_weights_genonly_CT14.root");
   TFile *fnegweights = TFile::Open("ROOTFile_Histogram_Acc_weights_genonly_negweights.root");
   TFile *fnoEWK = TFile::Open("ROOTFile_Histogram_Acc_weights_genonly_noEWK.root");

   TH1D *hnom = (TH1D*) fnom->Get("h_" + variable + "_AccTotal0");
   TH1D *hCT14 = (TH1D*) fCT14->Get("h_" + variable + "_AccTotal0");
   TH1D *hnegweights = (TH1D*) fnegweights->Get("h_" + variable + "_AccTotal0");
   TH1D *hnoEWK = (TH1D*) fnoEWK->Get("h_" + variable + "_AccTotal0");
   TGraphAsymmErrors *acc_nom = new TGraphAsymmErrors((TH1D*) fnom->Get("h_" + variable + "_AccPass0"),hnom);
   TGraphAsymmErrors *acc_CT14 = new TGraphAsymmErrors((TH1D*) fCT14->Get("h_" + variable + "_AccPass0"),hCT14);
   TGraphAsymmErrors *acc_negweights = new TGraphAsymmErrors((TH1D*) fnegweights->Get("h_" + variable + "_AccPass0"),hnegweights);
   TGraphAsymmErrors *acc_noEWK = new TGraphAsymmErrors((TH1D*) fnoEWK->Get("h_" + variable + "_AccPass0"),hnoEWK);

   // axes
   TString xtitle = DYana::xaxistitle(variable);

   MyCanvas cnom("AccEff/comp_acc_PDF_" + variable, xtitle, "Acceptance");
   cnom.SetYRange(0,1);
   cnom.SetRatioRange(0.84,1.16);
   cnom.SetLegendPosition(0.65,0.7,0.95,0.8);
   if (variable=="mass" || variable=="pt" || variable=="phistar") cnom.SetLogx();
   if (variable=="mass") {cnom.SetLogy(); cnom.SetYRange(0.0015,1.5);}
   cnom.CanvasWithGraphRatioPlot(acc_nom, acc_CT14, "CT14+EPPS16","CT14","CT14/(CT14+EPPS16)");
   cnom.PrintCanvas();

   MyCanvas cnw("AccEff/comp_acc_negweights_" + variable, xtitle, "Acceptance");
   cnw.SetYRange(0,1);
   cnw.SetRatioRange(0.84,1.16);
   cnw.SetLegendPosition(0.65,0.7,0.95,0.8);
   if (variable=="mass" || variable=="pt" || variable=="phistar") cnw.SetLogx();
   if (variable=="mass") {cnw.SetLogy(); cnw.SetYRange(0.0015,1.5);}
   cnw.CanvasWithGraphRatioPlot(acc_nom, acc_negweights, "w/o neg weights","w/ neg weights","with / without");
   cnw.PrintCanvas();

   MyCanvas cnoEWK("AccEff/comp_acc_noEWK_" + variable, xtitle, "Acceptance");
   cnoEWK.SetYRange(0,1);
   cnoEWK.SetRatioRange(0.84,1.16);
   cnoEWK.SetLegendPosition(0.65,0.7,0.95,0.8);
   if (variable=="mass" || variable=="pt" || variable=="phistar") cnoEWK.SetLogx();
   if (variable=="mass") {cnoEWK.SetLogy(); cnoEWK.SetYRange(0.0015,1.5);}
   cnoEWK.CanvasWithGraphRatioPlot(acc_nom, acc_noEWK, "w/ EWK corr","w/o EWK corr","without / with");
   cnoEWK.PrintCanvas();

   // compare gen-level distributions
   hnom->Scale(1./hnom->Integral());
   hCT14->Scale(1./hCT14->Integral());
   hnegweights->Scale(1./hnegweights->Integral());
   hnoEWK->Scale(1./hnoEWK->Integral());

   MyCanvas c2nom("AccEff/comp_gen_PDF_" + variable, xtitle, "Norm. yields");
   c2nom.SetRatioRange(0.84,1.16);
   c2nom.SetLegendPosition(0.65,0.7,0.95,0.8);
   if (variable=="mass" || variable=="pt" || variable=="phistar") c2nom.SetLogx();
   if (variable=="mass") {c2nom.SetLogy(); c2nom.SetYRange(0.0015,1.5);}
   c2nom.CanvasWithHistogramsRatioPlot(hnom, hCT14, "CT14+EPPS16","CT14","CT14/(CT14+EPPS16)");
   c2nom.PrintCanvas();

   MyCanvas c2nw("AccEff/comp_gen_negweights_" + variable, xtitle, "Norm. yields");
   c2nw.SetRatioRange(0.84,1.16);
   c2nw.SetLegendPosition(0.65,0.7,0.95,0.8);
   if (variable=="mass" || variable=="pt" || variable=="phistar") c2nw.SetLogx();
   if (variable=="mass") {c2nw.SetLogy(); c2nw.SetYRange(0.0015,1.5);}
   c2nw.CanvasWithHistogramsRatioPlot(hnom, hnegweights, "w/o neg weights","w/ neg weights","with / without");
   c2nw.PrintCanvas();

   MyCanvas c2noEWK("AccEff/comp_gen_noEWK_" + variable, xtitle, "Norm. yields");
   c2noEWK.SetRatioRange(0.84,1.16);
   c2noEWK.SetLegendPosition(0.65,0.7,0.95,0.8);
   if (variable=="mass" || variable=="pt" || variable=="phistar") c2noEWK.SetLogx();
   if (variable=="mass") {c2noEWK.SetLogy(); c2noEWK.SetYRange(0.0015,1.5);}
   c2noEWK.CanvasWithHistogramsRatioPlot(hnom, hnoEWK, "w/ EWK corr","w/o EWK corr","without / with");
   c2noEWK.PrintCanvas();
}
