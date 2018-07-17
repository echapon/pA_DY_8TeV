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

void fillHisto(TFile *fdata, TFile *fmc, 
      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double phistarbin1, double phistarbin2);
void printCombineDatacard(const char* filename, const char* histfilename, const char* dirname, 
      TH1D *data_obs, TH1D *DYMuMu, TH1D *DYTauTau, TH1D *htt, TH1D *hww, TH1D *hwz, TH1D *hzz, TH1D *DataSS1, TH1D *DataSS2);
TH1D* fillHistoKDE(TFile *file, TString treename, TString histname,
      bool doOS,
      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double phistarbin1, double phistarbin2,
      bool doweights, double hSF,
      unsigned int nXBins, double xMin, double xMax, double trimFactor, bool doSigmaScaling,
      bool doAdaptive);

void fillHistos(const char* datafile, const char* mcfile, const char* outputfile,
      const char* expr="log(vtxnormchi2)/log(10)", int nvarbins=80, double varmin=-4, double varmax=4) {
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

      // fill histos
      cout << "mass, " << bins[i] << " -- " << bins[i+1] << endl;
      fillHisto(fdata, fmc, bins[i], bins[i+1], rapbin_60120[0], rapbin_60120[rapbinnum_60120], -1, 1e99, -1, 1e99);
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

      // fill histos
      cout << "rap1560, " << rapbin_1560[i] << " -- " << rapbin_1560[i+1] << endl;
      fillHisto(fdata, fmc, 15, 60, rapbin_60120[i], rapbin_60120[i+1], -1, 1e99, -1, 1e99);
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

      // fill histos
      cout << "rap60120, " << rapbin_60120[i] << " -- " << rapbin_60120[i+1] << endl;
      fillHisto(fdata, fmc, 60, 120, rapbin_60120[i], rapbin_60120[i+1], -1, 1e99, -1, 1e99);
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

      // fill histos
      cout << "pt, " << rapbin_1560[i] << " -- " << rapbin_1560[i+1] << endl;
      fillHisto(fdata, fmc, 60, 120, rapbin_60120[0], rapbin_60120[rapbinnum_60120], ptbin_meas[i], ptbin_meas[i+1], -1, 1e99);
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

      // fill histos
      cout << "phistar, " << rapbin_1560[i] << " -- " << rapbin_1560[i+1] << endl;
      fillHisto(fdata, fmc, 60, 120, rapbin_60120[0], rapbin_60120[rapbinnum_60120], -1, 1e99, phistarbin[i], phistarbin[i+1]);
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
      double phistarbin1, double phistarbin2) {
   // data
   // cout << "Filling data histo" << endl;
   TTree *tr = (TTree*) fdata->Get("tr_Data1");
   tr->Draw("log(vtxnormchi2)/log(10)>>+data_obs",
         Form("sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");
   tr = (TTree*) fdata->Get("tr_Data2");
   tr->Draw("log(vtxnormchi2)/log(10)>>+data_obs",
         Form("sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");

   // DY
   // cout << "Filling DY histo" << endl;
   tr = (TTree*) fmc->Get("tr_DYMuMu1030");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYMuMu",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");
   tr = (TTree*) fmc->Get("tr_DYMuMu1030_PbP");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYMuMu",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");
   tr = (TTree*) fmc->Get("tr_DYMuMu30");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYMuMu",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");
   tr = (TTree*) fmc->Get("tr_DYMuMu30_PbP");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYMuMu",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");

   // DYtautau
   // cout << "Filling DYtautau histo" << endl;
   tr = (TTree*) fmc->Get("tr_DYTauTau1030");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYTauTau",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");
   tr = (TTree*) fmc->Get("tr_DYTauTau1030");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYTauTau",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");

   // ttbar
   // cout << "Filling ttbar histo" << endl;
   tr = (TTree*) fmc->Get("tr_TT");
   tr->Draw("log(vtxnormchi2)/log(10)>>+TT",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");

   // diboson
   // cout << "Filling diboson histo" << endl;
   tr = (TTree*) fmc->Get("tr_WW");
   tr->Draw("log(vtxnormchi2)/log(10)>>+WW",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");
   tr = (TTree*) fmc->Get("tr_WZ");
   tr->Draw("log(vtxnormchi2)/log(10)>>+WZ",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");
   tr = (TTree*) fmc->Get("tr_ZZ");
   tr->Draw("log(vtxnormchi2)/log(10)>>+ZZ",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");

   // data SS, same sign dxy
   // cout << "Filling data SS histo" << endl;
   tr = (TTree*) fdata->Get("tr_Data1");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DataSS1",
         Form("sign!=0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&dxyVTX1*dxyVTX2/abs(dxyVTX1*dxyVTX2)>0",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");
   tr = (TTree*) fdata->Get("tr_Data2");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DataSS1",
         Form("sign!=0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&dxyVTX1*dxyVTX2/abs(dxyVTX1*dxyVTX2)>0",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");

   // data SS, opposite sign dxy
   // cout << "Filling data SS histo" << endl;
   tr = (TTree*) fdata->Get("tr_Data1");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DataSS2",
         Form("sign!=0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&dxyVTX1*dxyVTX2/abs(dxyVTX1*dxyVTX2)<=0",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");
   tr = (TTree*) fdata->Get("tr_Data2");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DataSS2",
         Form("sign!=0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&dxyVTX1*dxyVTX2/abs(dxyVTX1*dxyVTX2)<=0",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");
}

void printCombineDatacard(const char* filename, const char* histfilename, const char* dirname, 
      TH1D *data_obs, TH1D *DYMuMu, TH1D *DYTauTau, TH1D *htt, TH1D *hww, TH1D *hwz, TH1D *hzz, TH1D *DataSS1, TH1D *DataSS2) {
   ofstream of(filename);
   of << "imax 1" << endl;
   of << "jmax 7" << endl;
   of << "kmax *" << endl;
   of << "---------------" << endl;
   of << "shapes * * " << histfilename << " " << dirname << "/$PROCESS " << dirname << "/$PROCESS_$SYSTEMATIC" << endl;
   TString binname(dirname); binname.ReplaceAll("/","_");
   of << "bin " << binname << endl;
   of << "observation " << data_obs->Integral() << endl;
   of << "------------------------------" << endl;
   of << "bin\t" << binname << "\t" << binname << "\t" << binname << "\t" << binname << "\t" << binname << "\t" << binname << "\t" << binname << "\t" << binname << endl;
   of << "process\tDYMuMu\tDYTauTau\tTT\tWW\tWZ\tZZ\tDataSS1\tDataSS2" << endl;
   of << "process\t0\t1\t2\t3\t4\t5\t6\t7" << endl;
   of << "rate\t" << DYMuMu->Integral() << "\t" << DYTauTau->Integral() << "\t" << htt->Integral() << "\t" << 
      hww->Integral() << "\t" << hwz->Integral() << "\t" << hzz->Integral() << "\t" << DataSS1->Integral() << "\t" << DataSS2->Integral() << endl;
   of << "------------------------------" << endl;
   of << "nDataSS1 rateParam " << binname << " DataSS1 @0*@1/" << DataSS1->Integral() << " nDataSS,fracDataSS1" << endl;
   of << "nDataSS2 rateParam " << binname << " DataSS2 @0*(1-@1)/" << DataSS2->Integral() << " nDataSS,fracDataSS1" << endl;
   of << "nDataSS extArg " << DataSS1->Integral()+DataSS2->Integral() << " [0," << 100*(DataSS1->Integral()+DataSS2->Integral()) << "]" << endl;
   of << "fracDataSS1 extArg 0.5 [0,1] " << endl;
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
      int sign;
      float diMass, diRapidity, diPt, diPhistar, vtxnormchi2;
      float dxyVTX1,dxyVTX2;
      float weight=1;
      tr->SetBranchStatus("*",0);
      tr->SetBranchStatus("sign",1); tr->SetBranchAddress("sign",&sign);
      tr->SetBranchStatus("diMass",1); tr->SetBranchAddress("diMass",&diMass);
      tr->SetBranchStatus("diRapidity",1); tr->SetBranchAddress("diRapidity",&diRapidity);
      tr->SetBranchStatus("diPt",1); tr->SetBranchAddress("diPt",&diPt);
      tr->SetBranchStatus("diPhistar",1); tr->SetBranchAddress("diPhistar",&diPhistar);
      tr->SetBranchStatus("vtxnormchi2",1); tr->SetBranchAddress("vtxnormchi2",&vtxnormchi2);
      if (dxymode>0) {
         tr->SetBranchStatus("dxyVTX1",1); tr->SetBranchAddress("dxyVTX1",&dxyVTX1);
         tr->SetBranchStatus("dxyVTX2",1); tr->SetBranchAddress("dxyVTX2",&dxyVTX2);
      }
      if (doweights) {
         tr->SetBranchStatus("weight",1); tr->SetBranchAddress("weight",&weight);
      }

      for (int i=0; i<tr->GetEntries(); i++) {
         tr->GetEntry(i);
         if (doOS && sign!=0) continue;
         if (!doOS && sign==0) continue;
         if (!(diMass>massbin1 && diMass<massbin2)) continue;
         if (!(diRapidity>rapbin1 && diRapidity<rapbin2)) continue;
         if (!(diPt>ptbin1 && diPt<ptbin2)) continue;
         if (!(diPhistar>phistarbin1 && diPhistar<phistarbin2)) continue;
         if (dxymode==1 && !(dxyVTX1*dxyVTX2/abs(dxyVTX1*dxyVTX2)>0)) continue;
         if (dxymode==2 && !(dxyVTX1*dxyVTX2/abs(dxyVTX1*dxyVTX2)<=0)) continue;
         double val = log(vtxnormchi2)/log(10);
         vals.push_back(val);
         weights.push_back(weight);
         if (val>xMin&&val<xMax) nevts++;
      } // end event loop
   } // end tokenize

   KDEProducer kdeprod(&vals,&weights,hSF,nXBins,xMin,xMax,trimFactor,doSigmaScaling);

   TH1D *ans = NULL;
   if (doAdaptive) ans = (TH1D*) kdeprod.getAPDF(histname.Data(), "KDE density", nXBins, xMin, xMax);
   else ans = (TH1D*) kdeprod.getPDF(histname.Data(), "KDE density", nXBins, xMin, xMax);
   ans->Scale(nevts*((xMax-xMin)/nXBins));
   return ans;
}
