#include "../BkgEst/interface/defs.h"

using namespace DYana;

void fillHisto(TFile *fdata, TFile *fmc, 
      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double phistarbin1, double phistarbin2);

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
      fillHisto(fdata, fmc, bins[i], bins[i+1], rapbin_60120[0], rapbin_60120[rapbinnum_60120], ptbin_meas[0], ptbin_meas[ptbinnum_meas], phistarbin[0], phistarbin[phistarnum]);

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
      fillHisto(fdata, fmc, 15, 60, rapbin_60120[i], rapbin_60120[i+1], ptbin_meas[0], ptbin_meas[ptbinnum_meas], phistarbin[0], phistarbin[phistarnum]);

      tdir_rap1560->cd();
   }

   // similar instructions will follow for rap1560, rap60120, pt, phistar

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
   tr->Draw("0.95*log(vtxnormchi2)/log(10)>>+DataSS1",
         Form("sign!=0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&dxyVTX1*dxyVTX2/abs(dxyVTX1*dxyVTX2)>0",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");
   tr = (TTree*) fdata->Get("tr_Data2");
   tr->Draw("0.95*log(vtxnormchi2)/log(10)>>+DataSS1",
         Form("sign!=0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&dxyVTX1*dxyVTX2/abs(dxyVTX1*dxyVTX2)>0",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");

   // data SS, opposite sign dxy
   // cout << "Filling data SS histo" << endl;
   tr = (TTree*) fdata->Get("tr_Data1");
   tr->Draw("0.95*log(vtxnormchi2)/log(10)>>+DataSS2",
         Form("sign!=0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&dxyVTX1*dxyVTX2/abs(dxyVTX1*dxyVTX2)<=0",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");
   tr = (TTree*) fdata->Get("tr_Data2");
   tr->Draw("0.95*log(vtxnormchi2)/log(10)>>+DataSS2",
         Form("sign!=0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&dxyVTX1*dxyVTX2/abs(dxyVTX1*dxyVTX2)<=0",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2),
         "goff");
}
