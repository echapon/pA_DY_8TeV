#include "fillHistos.C"
#include "TCanvas.h"

void testKDE() {
   double massmin=20;
   double massmax=30;
   double rapmin=-3;
   double rapmax=3;
   double ptmin=-1;
   double ptmax=1e99;
   double phistarmin=-1;
   double phistarmax=1e99;

   int nvarbins=80;
   double varmin=-4;
   double varmax=4;

   TFile *fdata = TFile::Open("ROOTFile_Histogram_InvMass_PAL1DoubleMu0_Data_MomCorr00_noHFrew_notnprew.root");
   TFile *fmc = TFile::Open("ROOTFile_Histogram_InvMass_PAL1DoubleMu0_Powheg_MomCorr00_rewboth_tnprew.root");

      // create histos
      TH1D *data_obs = new TH1D("data_obs","Data",nvarbins,varmin,varmax);
      TH1D *DYMuMu = new TH1D("DYMuMu","DY #mumu",nvarbins,varmin,varmax);
      TH1D *DYTauTau = new TH1D("DYTauTau","DY #tautau",nvarbins,varmin,varmax);
      TH1D *htt = new TH1D("TT","t#bar{t}",nvarbins,varmin,varmax);
      TH1D *hww = new TH1D("WW","WW",nvarbins,varmin,varmax);
      TH1D *hwz = new TH1D("WZ","WZ",nvarbins,varmin,varmax);
      TH1D *hzz = new TH1D("ZZ","ZZ",nvarbins,varmin,varmax);
      TH1D *DataSS1 = new TH1D("DataSS1","DataSS1",nvarbins,varmin,varmax);
      TH1D *DataSS2 = new TH1D("DataSS2","DataSS2",nvarbins,varmin,varmax);

      // no KDE
      fillHisto(fdata, fmc, massmin, massmax, rapmin, rapmax, -1, 1e99, -1, 1e99);
   
      // default KDE
      TH1D *hAKDE_1_5_false = fillHistoKDE(fdata,"tr_Data1,tr_Data2","hAKDE_1_5_false",false,
            massmin,massmax,rapmin,rapmax,ptmin,ptmax,phistarmin,phistarmax,1,
            false,1,nvarbins,varmin,varmax,5,false,true);

      // some variations
      TH1D *hKDE_1_5_false = fillHistoKDE(fdata,"tr_Data1,tr_Data2","hKDE_1_5_false",false,
            massmin,massmax,rapmin,rapmax,ptmin,ptmax,phistarmin,phistarmax,1,
            false,1,nvarbins,varmin,varmax,5,false,false);

      TH1D *hAKDE_1_no_false = fillHistoKDE(fdata,"tr_Data1,tr_Data2","hAKDE_1_no_false",false,
            massmin,massmax,rapmin,rapmax,ptmin,ptmax,phistarmin,phistarmax,1,
            false,1,nvarbins,varmin,varmax,-1,false,true);
      TH1D *hAKDE_1_2_false = fillHistoKDE(fdata,"tr_Data1,tr_Data2","hAKDE_1_2_false",false,
            massmin,massmax,rapmin,rapmax,ptmin,ptmax,phistarmin,phistarmax,1,
            false,1,nvarbins,varmin,varmax,2,false,true);
      TH1D *hAKDE_1_10_false = fillHistoKDE(fdata,"tr_Data1,tr_Data2","hAKDE_1_10_false",false,
            massmin,massmax,rapmin,rapmax,ptmin,ptmax,phistarmin,phistarmax,1,
            false,1,nvarbins,varmin,varmax,10,false,true);

      TH1D *hAKDE_0p25_5_false = fillHistoKDE(fdata,"tr_Data1,tr_Data2","hAKDE_0p25_5_false",false,
            massmin,massmax,rapmin,rapmax,ptmin,ptmax,phistarmin,phistarmax,1,
            false,0.25,nvarbins,varmin,varmax,5,false,true);
      TH1D *hAKDE_0p5_5_false = fillHistoKDE(fdata,"tr_Data1,tr_Data2","hAKDE_0p5_5_false",false,
            massmin,massmax,rapmin,rapmax,ptmin,ptmax,phistarmin,phistarmax,1,
            false,0.5,nvarbins,varmin,varmax,5,false,true);
      TH1D *hAKDE_2_5_false = fillHistoKDE(fdata,"tr_Data1,tr_Data2","hAKDE_2_5_false",false,
            massmin,massmax,rapmin,rapmax,ptmin,ptmax,phistarmin,phistarmax,1,
            false,2,nvarbins,varmin,varmax,5,false,true);
      TH1D *hAKDE_4_5_false = fillHistoKDE(fdata,"tr_Data1,tr_Data2","hAKDE_4_5_false",false,
            massmin,massmax,rapmin,rapmax,ptmin,ptmax,phistarmin,phistarmax,1,
            false,4,nvarbins,varmin,varmax,5,false,true);


      // make plots
      DataSS1->DrawNormalized();
      gPad->SetLogy();
      DataSS1->DrawNormalized();
      // variated KDEs
      hKDE_1_5_false->SetLineColor(kBlue);
      hKDE_1_5_false->DrawNormalized("hist same");
      hAKDE_1_no_false->SetLineColor(kGreen+1);
      hAKDE_1_no_false->DrawNormalized("hist same");
      hAKDE_1_2_false->SetLineColor(kGreen+2);
      hAKDE_1_2_false->DrawNormalized("hist same");
      hAKDE_1_10_false->SetLineColor(kGreen+3);
      hAKDE_1_10_false->DrawNormalized("hist same");
      // nominal KDE
      hAKDE_1_5_false->SetLineColor(kRed);
      hAKDE_1_5_false->DrawNormalized("hist same");
}
