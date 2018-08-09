#include "../BkgEst/interface/defs.h"
#include "KDEProducer.cc"

#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TDirectory.h"

#include <iostream>
#include <fstream>

using namespace DYana;
using namespace std;

// flag to apply tkiso, dxy, dz cuts
const bool docuts = true;
const bool mergeSStemplates = true;
const double dxycut = 0.01;
const double dzcut = 0.1;
const double tkisocut = 5;
const TString addlcuts = (docuts) ? Form("&&pt1*trkiso1<%f&&pt2*trkiso2<%f&&abs(dxyVTX1)<%f&&abs(dxyVTX2)<%f&&abs(dzVTX1)<%f&&abs(dzVTX2)<%f",tkisocut,tkisocut,dxycut,dxycut,dzcut,dzcut) : "";

// declarations
void fillHisto(TFile *fdata, TFile *fmc, 
      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double phistarbin1, double phistarbin2,
      bool doKDE=false, double hSF=1, double trimFactor=5, bool doSigmaScaling=false, bool doAdaptive=true);
void printCombineDatacard(const char* filename, const char* histfilename, const char* dirname, 
      TH1D *data_obs, TH1D *DYMuMu, TH1D *DYTauTau, TH1D *htt, TH1D *hww, TH1D *hwz, TH1D *hzz, TH1D *DataSS1, TH1D *DataSS2);
TH1D* fillHistoKDE(TFile *file, TString treename, TString histname,
      bool doOS,
      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double phistarbin1, double phistarbin2,
      int dxymode,
      bool doweights, double hSF,
      unsigned int nXBins, double xMin, double xMax, double trimFactor, bool doSigmaScaling,
      bool doAdaptive);

// implementation
void fillHistos(const char* datafile, const char* mcfile, const char* outputfile,
      const char* expr="log(vtxnormchi2)/log(10)", int nvarbins=80, double varmin=-4, double varmax=4,
      bool doKDE=true, double hSF=1, double trimFactor=5, bool doSigmaScaling=false, bool doAdaptive=true) {
   TFile *fdata = TFile::Open(datafile);
   TFile *fmc = TFile::Open(mcfile);
   TFile *fout = TFile::Open(outputfile,"RECREATE");

   // mass
   TDirectory *tdir_mass = fout->mkdir("mass");
   tdir_mass->cd();
   for (int i=0; i<binnum; i++) {
      TDirectory *tdir = tdir_mass->mkdir(Form("%.2f_%.2f",bins[i],bins[i+1]));
      tdir->cd();

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
      for (int i=3; i<=10; i++) new TH1D(Form("DataSS%d",i),Form("DataSS%d",i),nvarbins,varmin,varmax);

      // fill histos
      cout << "mass, " << bins[i] << " -- " << bins[i+1] << endl;
      fillHisto(fdata, fmc, bins[i], bins[i+1], rapbin_60120[0], rapbin_60120[rapbinnum_60120], -1, 1e99, -1, 1e99,
            doKDE,hSF,trimFactor,doSigmaScaling,doAdaptive);
      printCombineDatacard(Form("datacard_mass_%.2f_%.2f.txt",bins[i],bins[i+1]), outputfile, Form("mass/%.2f_%.2f",bins[i],bins[i+1]), 
            data_obs, DYMuMu, DYTauTau, htt, hww, hwz, hzz, DataSS1, DataSS2);

      tdir_mass->cd();
   }

   // rap1560
   TDirectory *tdir_rap1560 = fout->mkdir("rap1560");
   tdir_rap1560->cd();
   for (int i=0; i<rapbinnum_1560; i++) {
      TDirectory *tdir = tdir_rap1560->mkdir(Form("%.2f_%.2f",rapbin_1560[i],rapbin_1560[i+1]));
      tdir->cd();

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
      for (int i=3; i<=10; i++) new TH1D(Form("DataSS%d",i),Form("DataSS%d",i),nvarbins,varmin,varmax);

      // fill histos
      cout << "rap1560, " << rapbin_1560[i] << " -- " << rapbin_1560[i+1] << endl;
      fillHisto(fdata, fmc, 11, 60, rapbin_1560[i], rapbin_1560[i+1], -1, 1e99, -1, 1e99,
            doKDE,hSF,trimFactor,doSigmaScaling,doAdaptive);
      printCombineDatacard(Form("datacard_rap1560_%.2f_%.2f.txt",rapbin_1560[i],rapbin_1560[i+1]), outputfile, Form("rap1560/%.2f_%.2f",rapbin_1560[i],rapbin_1560[i+1]), 
            data_obs, DYMuMu, DYTauTau, htt, hww, hwz, hzz, DataSS1, DataSS2);

      tdir_rap1560->cd();
   }

   // rap60120
   TDirectory *tdir_rap60120 = fout->mkdir("rap60120");
   tdir_rap60120->cd();
   for (int i=0; i<rapbinnum_60120; i++) {
      TDirectory *tdir = tdir_rap60120->mkdir(Form("%.2f_%.2f",rapbin_60120[i],rapbin_60120[i+1]));
      tdir->cd();

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
      for (int i=3; i<=10; i++) new TH1D(Form("DataSS%d",i),Form("DataSS%d",i),nvarbins,varmin,varmax);

      // fill histos
      cout << "rap60120, " << rapbin_60120[i] << " -- " << rapbin_60120[i+1] << endl;
      fillHisto(fdata, fmc, 60, 120, rapbin_60120[i], rapbin_60120[i+1], -1, 1e99, -1, 1e99,
            doKDE,hSF,trimFactor,doSigmaScaling,doAdaptive);
      printCombineDatacard(Form("datacard_rap60120_%.2f_%.2f.txt",rapbin_60120[i],rapbin_60120[i+1]), outputfile, Form("rap60120/%.2f_%.2f",rapbin_60120[i],rapbin_60120[i+1]), 
            data_obs, DYMuMu, DYTauTau, htt, hww, hwz, hzz, DataSS1, DataSS2);

      tdir_rap60120->cd();
   }

   // pt
   TDirectory *tdir_pt = fout->mkdir("pt");
   tdir_pt->cd();
   for (int i=0; i<ptbinnum_meas; i++) {
      TDirectory *tdir = tdir_pt->mkdir(Form("%.2f_%.2f",ptbin_meas[i],ptbin_meas[i+1]));
      tdir->cd();

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
      for (int i=3; i<=10; i++) new TH1D(Form("DataSS%d",i),Form("DataSS%d",i),nvarbins,varmin,varmax);

      // fill histos
      cout << "pt, " << ptbin_meas[i] << " -- " << ptbin_meas[i+1] << endl;
      fillHisto(fdata, fmc, 60, 120, rapbin_60120[0], rapbin_60120[rapbinnum_60120], ptbin_meas[i], ptbin_meas[i+1], -1, 1e99,
            doKDE,hSF,trimFactor,doSigmaScaling,doAdaptive);
      printCombineDatacard(Form("datacard_pt_%.2f_%.2f.txt",ptbin_meas[i],ptbin_meas[i+1]), outputfile, Form("pt/%.2f_%.2f",ptbin_meas[i],ptbin_meas[i+1]), 
            data_obs, DYMuMu, DYTauTau, htt, hww, hwz, hzz, DataSS1, DataSS2);

      tdir_pt->cd();
   }

   // phistar
   TDirectory *tdir_phistar = fout->mkdir("phistar");
   tdir_phistar->cd();
   for (int i=0; i<phistarnum; i++) {
      TDirectory *tdir = tdir_phistar->mkdir(Form("%.2f_%.2f",phistarbin[i],phistarbin[i+1]));
      tdir->cd();

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
      for (int i=3; i<=10; i++) new TH1D(Form("DataSS%d",i),Form("DataSS%d",i),nvarbins,varmin,varmax);

      // fill histos
      cout << "phistar, " << phistarbin[i] << " -- " << phistarbin[i+1] << endl;
      fillHisto(fdata, fmc, 60, 120, rapbin_60120[0], rapbin_60120[rapbinnum_60120], -1, 1e99, phistarbin[i], phistarbin[i+1],
            doKDE,hSF,trimFactor,doSigmaScaling,doAdaptive);
      printCombineDatacard(Form("datacard_phistar_%.2f_%.2f.txt",phistarbin[i],phistarbin[i+1]), outputfile, Form("phistar/%.2f_%.2f",phistarbin[i],phistarbin[i+1]), 
            data_obs, DYMuMu, DYTauTau, htt, hww, hwz, hzz, DataSS1, DataSS2);

      tdir_phistar->cd();
   }

   fout->Write();
   fout->Close();
}

void fillHisto(TFile *fdata, TFile *fmc, 
      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double phistarbin1, double phistarbin2,
      bool doKDE, double hSF, double trimFactor, bool doSigmaScaling, bool doAdaptive) {
   // data
   // cout << "Filling data histo" << endl;
   TTree *tr = (TTree*) fdata->Get("tr_Data1");
   tr->Draw("log(vtxnormchi2)/log(10)>>+data_obs",
         Form("isTight1+isTight2==2&&sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2) + addlcuts,
         "goff");
   tr = (TTree*) fdata->Get("tr_Data2");
   tr->Draw("log(vtxnormchi2)/log(10)>>+data_obs",
         Form("isTight1+isTight2==2&&sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2) + addlcuts,
         "goff");

   // DY
   // cout << "Filling DY histo" << endl;
   tr = (TTree*) fmc->Get("tr_DYMuMu1030");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYMuMu",
         Form("weight*(isTight1+isTight2==2&&sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2) + addlcuts + ")",
         "goff");
   tr = (TTree*) fmc->Get("tr_DYMuMu1030_PbP");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYMuMu",
         Form("weight*(isTight1+isTight2==2&&sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2) + addlcuts + ")",
         "goff");
   tr = (TTree*) fmc->Get("tr_DYMuMu30");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYMuMu",
         Form("weight*(isTight1+isTight2==2&&sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2) + addlcuts + ")",
         "goff");
   tr = (TTree*) fmc->Get("tr_DYMuMu30_PbP");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYMuMu",
         Form("weight*(isTight1+isTight2==2&&sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2) + addlcuts + ")",
         "goff");

   // DYtautau
   // cout << "Filling DYtautau histo" << endl;
   tr = (TTree*) fmc->Get("tr_DYTauTau1030");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYTauTau",
         Form("weight*(isTight1+isTight2==2&&sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2) + addlcuts + ")",
         "goff");
   tr = (TTree*) fmc->Get("tr_DYTauTau1030");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYTauTau",
         Form("weight*(isTight1+isTight2==2&&sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2) + addlcuts + ")",
         "goff");

   // ttbar
   // cout << "Filling ttbar histo" << endl;
   tr = (TTree*) fmc->Get("tr_TT");
   tr->Draw("log(vtxnormchi2)/log(10)>>+TT",
         Form("weight*(isTight1+isTight2==2&&sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2) + addlcuts + ")",
         "goff");

   // diboson
   // cout << "Filling diboson histo" << endl;
   tr = (TTree*) fmc->Get("tr_WW");
   tr->Draw("log(vtxnormchi2)/log(10)>>+WW",
         Form("weight*(isTight1+isTight2==2&&sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2) + addlcuts + ")",
         "goff");
   tr = (TTree*) fmc->Get("tr_WZ");
   tr->Draw("log(vtxnormchi2)/log(10)>>+WZ",
         Form("weight*(isTight1+isTight2==2&&sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2) + addlcuts + ")",
         "goff");
   tr = (TTree*) fmc->Get("tr_ZZ");
   tr->Draw("log(vtxnormchi2)/log(10)>>+ZZ",
         Form("weight*(isTight1+isTight2==2&&sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2) + addlcuts + ")",
         "goff");


   if (mergeSStemplates) {
      // merge some bins for SS templates, due to lack of statistics
      if (!docuts && massbin1==60 && massbin2==120) {
         if (ptbin1>-0.5) {
            if (ptbin1<13) {ptbin1 = 0; ptbin2 = 12;}
            else if (ptbin1<41) {ptbin1 = 12; ptbin2 = 40;}
            else {ptbin1 = 40; ptbin2 = 200;}
         } else if (phistarbin1>0-0.5) {
            if (phistarbin1<0.06) {phistarbin1 = 0; phistarbin2 = 0.05;}
            else if (phistarbin1<0.16) {phistarbin1 = 0.05; phistarbin2 = 0.15;}
            else {phistarbin1 = 0.15; phistarbin2 = 3;}
         } else {
            if (rapbin1<-1.15) {rapbin1 = -2.87; rapbin2 = -1.2;}
            else if (rapbin1<0.45) {rapbin1 = -1.2; rapbin2 = 0.4;}
            else {rapbin1 = 0.4; rapbin2 = 1.93;}
         }
      } else if (docuts) {
         if (massbin1>85) {massbin1=86; massbin2=600;}
         else if (phistarbin1>-0.5) {phistarbin1=-1; phistarbin2=1e9;}
         else if (ptbin1>-0.5) {ptbin1=-1; ptbin2=1e9;}
         else if (massbin1>55 && rapbin2-rapbin1<4) {rapbin1=rapbin_60120[0]; rapbin2=rapbin_60120[rapbinnum_60120];}
      }
   }

   if (!doKDE) { // no KDE, use data templates directly
      // data SS, same sign dxy
      // cout << "Filling data SS histo" << endl;
      tr = (TTree*) fdata->Get("tr_Data1");
      tr->Draw("log(vtxnormchi2)/log(10)>>+DataSS1",
            Form("isTight1+isTight2==2&&sign!=0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&dxyVTX1*dxyVTX2>0" + addlcuts,
               massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
            "goff"); // SS
      tr = (TTree*) fdata->Get("tr_Data2");
      tr->Draw("log(vtxnormchi2)/log(10)>>+DataSS1",
            Form("isTight1+isTight2==2&&sign!=0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&dxyVTX1*dxyVTX2>0" + addlcuts,
               massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
            "goff"); // SS

      // data SS, opposite sign dxy
      // cout << "Filling data SS histo" << endl;
      tr = (TTree*) fdata->Get("tr_Data1");
      tr->Draw("log(vtxnormchi2)/log(10)>>+DataSS2",
            Form("isTight1+isTight2==2&&sign!=0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&dxyVTX1*dxyVTX2<=0" + addlcuts,
               massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
            "goff");
      tr = (TTree*) fdata->Get("tr_Data2");
      tr->Draw("log(vtxnormchi2)/log(10)>>+DataSS2",
            Form("isTight1+isTight2==2&&sign!=0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&dxyVTX1*dxyVTX2<=0" + addlcuts,
               massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
            "goff");
   } else {
      for (int iss=1; iss<=10; iss++) {
         TH1D *DataSS = (TH1D*) gDirectory->Get(Form("DataSS%d",iss));
         int nvarbins = DataSS->GetNbinsX();
         double varmin = DataSS->GetBinCenter(1)-DataSS->GetBinWidth(1)/2.;
         double varmax = DataSS->GetBinCenter(nvarbins)+DataSS->GetBinWidth(nvarbins)/2.;

         TH1D *tmp = fillHistoKDE(fdata,"tr_Data1,tr_Data2","tmp",false,
               massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,iss,
               false,hSF,nvarbins,varmin,varmax,trimFactor,doSigmaScaling,doAdaptive);
         DataSS->Add(tmp);
         // DataSS1->Write();
         delete tmp;
      }
   }
}

void printCombineDatacard(const char* filename, const char* histfilename, const char* dirname, 
      TH1D *data_obs, TH1D *DYMuMu, TH1D *DYTauTau, TH1D *htt, TH1D *hww, TH1D *hwz, TH1D *hzz, TH1D *DataSS1, TH1D *DataSS2) {
   // check for each input if it is empty -- if yes, just ignore it
   bool okDYMuMu = (DYMuMu->GetEntries()>0);
   if (!okDYMuMu) {
      cout << "ERROR the signal template is empty!!!" << endl;
      return;
   }
   bool okDYTauTau = (DYTauTau->GetEntries()>0);
   bool okhtt = (htt->GetEntries()>0);
   bool okhww = (hww->GetEntries()>0);
   bool okhwz = (hwz->GetEntries()>0);
   bool okhzz = (hzz->GetEntries()>0);
   bool okDataSS1 = (DataSS1->GetEntries()>0);
   bool okDataSS2 = (DataSS2->GetEntries()>0);
   int nok = okDYTauTau + okhtt + okhww + okhwz + okhzz + okDataSS1 + okDataSS2;

   ofstream of(filename);
   of << "imax 1" << endl;
   of << "jmax " << nok << endl;
   of << "kmax *" << endl;
   of << "---------------" << endl;
   of << "shapes * * " << histfilename << " " << dirname << "/$PROCESS " << dirname << "/$PROCESS_$SYSTEMATIC" << endl;
   TString binname(dirname); binname.ReplaceAll("/","_");
   of << "bin " << binname << endl;
   of << "observation " << data_obs->Integral() << endl;
   of << "------------------------------" << endl;
   of << "bin\t" << binname << "\t" << binname << "\t" << binname << "\t" << binname << "\t" << binname << "\t" << binname << "\t" << binname << "\t" << binname << endl;
   
   of << "process\tDYMuMu";
   if (okDYTauTau) of << "\tDYTauTau";
   if (okhtt) of << "\tTT";
   if (okhww) of << "\tWW";
   if (okhwz) of << "\tWZ";
   if (okhzz) of << "\tZZ";
   if (okDataSS1) of << "\tDataSS1";
   if (okDataSS2) of << "\tDataSS2";
   of << endl;
   
   of << "process\t0";
   for (int i=0; i<nok; i++) of << "\t" << i+1;
   of << endl;
   
   of << "rate\t" << DYMuMu->Integral();
   if (okDYTauTau) of << "\t" << DYTauTau->Integral();
   if (okhtt) of << "\t" << htt->Integral();
   if (okhww) of << "\t" << hww->Integral();
   if (okhwz) of << "\t" << hwz->Integral();
   if (okhzz) of << "\t" << hzz->Integral();
   if (okDataSS1) of << "\t" << DataSS1->Integral();
   if (okDataSS2) of << "\t" << DataSS2->Integral();
   of << endl;

   of << "------------------------------" << endl;
   if (okDataSS1 && okDataSS2) {
      of << "nDataSS1 rateParam " << binname << " DataSS1 @0*@1/" << DataSS1->Integral() << " nDataSS,fracDataSS1" << endl;
      of << "nDataSS2 rateParam " << binname << " DataSS2 @0*(1-@1)/" << DataSS2->Integral() << " nDataSS,fracDataSS1" << endl;
      of << "nDataSS extArg " << 0.1*data_obs->Integral() << " [0," << 1.5*data_obs->Integral() << "]" << endl;
      of << "fracDataSS1 extArg 0.5 [0,1] " << endl;
   }
}

TH1D* fillHistoKDE(TFile *file, TString treename, TString histname,
      bool doOS,
      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double phistarbin1, double phistarbin2,
      int dxymode,
      bool doweights, double hSF,
      unsigned int nXBins, double xMin, double xMax, double trimFactor, bool doSigmaScaling,
      bool doAdaptive
      ) {
   // vectors of values
   vector<double> vals, weights;
   int nevts=0;

   // loop on the tree(s)
   TString tok;
   Ssiz_t from = 0;
   while (treename.Tokenize(tok, from, ",")) {
      TTree *tr = (TTree*) file->Get(tok);
      // variables
      int sign,isTight1,isTight2,isMedium1,isMedium2;
      float diMass, diRapidity, diPt, diPhistar, vtxnormchi2,pt1,pt2;
      float dxyVTX1,dxyVTX2,dzVTX1,dzVTX2;
      float dxyBS1,dxyBS2;
      float trkiso1,trkiso2;
      float weight=1;
      tr->SetBranchStatus("*",0);
      tr->SetBranchStatus("sign",1); tr->SetBranchAddress("sign",&sign);
      tr->SetBranchStatus("isTight1",1); tr->SetBranchAddress("isTight1",&isTight1);
      tr->SetBranchStatus("isTight2",1); tr->SetBranchAddress("isTight2",&isTight2);
      // tr->SetBranchStatus("isMedium1",1); tr->SetBranchAddress("isMedium1",&isMedium1);
      // tr->SetBranchStatus("isMedium2",1); tr->SetBranchAddress("isMedium2",&isMedium2);
      tr->SetBranchStatus("diMass",1); tr->SetBranchAddress("diMass",&diMass);
      tr->SetBranchStatus("diRapidity",1); tr->SetBranchAddress("diRapidity",&diRapidity);
      tr->SetBranchStatus("diPt",1); tr->SetBranchAddress("diPt",&diPt);
      tr->SetBranchStatus("diPhistar",1); tr->SetBranchAddress("diPhistar",&diPhistar);
      tr->SetBranchStatus("pt1",1); tr->SetBranchAddress("pt1",&pt1);
      tr->SetBranchStatus("pt2",1); tr->SetBranchAddress("pt2",&pt2);
      tr->SetBranchStatus("trkiso1",1); tr->SetBranchAddress("trkiso1",&trkiso1);
      tr->SetBranchStatus("trkiso2",1); tr->SetBranchAddress("trkiso2",&trkiso2);
      tr->SetBranchStatus("vtxnormchi2",1); tr->SetBranchAddress("vtxnormchi2",&vtxnormchi2);
      tr->SetBranchStatus("dxyVTX1",1); tr->SetBranchAddress("dxyVTX1",&dxyVTX1);
      tr->SetBranchStatus("dxyVTX2",1); tr->SetBranchAddress("dxyVTX2",&dxyVTX2);
      tr->SetBranchStatus("dzVTX1",1); tr->SetBranchAddress("dzVTX1",&dzVTX1);
      tr->SetBranchStatus("dzVTX2",1); tr->SetBranchAddress("dzVTX2",&dzVTX2);
      if (doweights) {
         tr->SetBranchStatus("weight",1); tr->SetBranchAddress("weight",&weight);
      } 

      for (int i=0; i<tr->GetEntries(); i++) {
         tr->GetEntry(i);

         if (!(diMass>massbin1 && diMass<massbin2)) continue;
         if (!(diRapidity-0.47>rapbin1 && diRapidity-0.47<rapbin2)) continue;
         if (!(diPt>ptbin1 && diPt<ptbin2)) continue;
         if (!(diPhistar>phistarbin1 && diPhistar<phistarbin2)) continue;

         // tight cuts: |dxy|<0.2, |dz|<0.5
         if (docuts) {
            if (fabs(dxyVTX1)>dxycut || fabs(dxyVTX2)>dxycut) continue;
            if (fabs(dzVTX1)>dzcut || fabs(dzVTX2)>dzcut) continue;
         }

         if (doOS) { // OS, TT, iso
            if (sign!=0) continue;
            if (isTight1+isTight2<2) continue;
            if (docuts && !(pt1*trkiso1<tkisocut&&pt2*trkiso2<tkisocut)) continue; 
         }

         if (!doOS) {
            if (dxymode==1) { // SS, TT, iso, dxy1*dxy2>0
               if (sign==0) continue;
               if (isTight1+isTight2<2) continue;
               if (docuts && !(pt1*trkiso1<tkisocut&&pt2*trkiso2<tkisocut)) continue; 
               if (dxyVTX1*dxyVTX2<=0) continue;
            } else if (dxymode==2) { // SS, TT, iso, dxy1*dxy2<=0
               if (sign==0) continue;
               if (isTight1+isTight2<2) continue;
               if (docuts && !(pt1*trkiso1<tkisocut&&pt2*trkiso2<tkisocut)) continue; 
               if (dxyVTX1*dxyVTX2>0) continue;
            } else if (dxymode==3) { // SS, TT, iso, no dxy1*dxy2 cut
               if (sign==0) continue;
               if (isTight1+isTight2<2) continue;
               if (docuts && !(pt1*trkiso1<tkisocut&&pt2*trkiso2<tkisocut)) continue; 
            } else if (dxymode==4) { // SS, TT, non iso, no dxy1*dxy2 cut
               if (sign==0) continue;
               if (isTight1+isTight2<2) continue;
               if (docuts && (pt1*trkiso1<tkisocut&&pt2*trkiso2<tkisocut)) continue; 
            } else if (dxymode==5) { // SS, TT, dxy1*dxy2>0
               if (sign==0) continue;
               if (isTight1+isTight2<2) continue;
               if (dxyVTX1*dxyVTX2<=0) continue;
            } else if (dxymode==6) { // SS, TT, dxy1*dxy2<=0
               if (sign==0) continue;
               if (isTight1+isTight2<2) continue;
               if (dxyVTX1*dxyVTX2>0) continue;
            } else if (dxymode==7) { // SS, TT, no dxy1*dxy2 cut
               if (sign==0) continue;
               if (isTight1+isTight2<2) continue;
            } else if (dxymode==8) { // OS, TT, non iso, dxy1*dxy2>0
               if (sign!=0) continue;
               if (isTight1+isTight2<2) continue;
               if (docuts && (pt1*trkiso1<tkisocut&&pt2*trkiso2<tkisocut)) continue; 
               if (dxyVTX1*dxyVTX2<=0) continue;
            } else if (dxymode==9) { // OS, TT, non iso, dxy1*dxy2<=0
               if (sign!=0) continue;
               if (isTight1+isTight2<2) continue;
               if (docuts && (pt1*trkiso1<tkisocut&&pt2*trkiso2<tkisocut)) continue; 
               if (dxyVTX1*dxyVTX2>0) continue;
            } else if (dxymode==10) { // OS, TT, non iso, no dxy1*dxy2 cut
               if (sign!=0) continue;
               if (isTight1+isTight2<2) continue;
               if (docuts && (pt1*trkiso1<tkisocut&&pt2*trkiso2<tkisocut)) continue; 
            }
         }

         double val = log(vtxnormchi2)/log(10);
         vals.push_back(val);
         weights.push_back(weight);
         if (val>xMin&&val<xMax) nevts++;
      } // end event loop
   } // end tokenize

   if (nevts==0) {
      // too few events: makes no sense to do KDE
      return new TH1D(histname,histname,nXBins,xMin,xMax);
   }

   KDEProducer kdeprod(&vals,&weights,hSF,nXBins,xMin,xMax,trimFactor,doSigmaScaling);

   TH1D *ans = NULL;
   if (doAdaptive) ans = (TH1D*) kdeprod.getAPDF(histname.Data(), "KDE density", nXBins, xMin, xMax);
   else ans = (TH1D*) kdeprod.getPDF(histname.Data(), "KDE density", nXBins, xMin, xMax);
   ans->Scale(nevts*((xMax-xMin)/nXBins));
   return ans;
}
