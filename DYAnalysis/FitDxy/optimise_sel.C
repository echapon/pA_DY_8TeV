#include "../BkgEst/interface/defs.h"
#include "KDEProducer.cc"
#include "RooGoF.C"

#include "TH1.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "RooFitResult.h"
#include "RooDataHist.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooArgSet.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"
#include "TTree.h"

#include <iostream>
#include <fstream>

using namespace DYana;
using namespace std;
using namespace RooFit;

const int nrebin = 1;
const bool doKDE=true; 
const double hSF=1; 
const double trimFactor=5; 
const bool doSigmaScaling=false; 
const bool doAdaptive=false;

void fillHisto(TFile *fdata, TFile *fmc, 
      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double phistarbin1, double phistarbin2,
      double pt1cut, double pt2cut, double isocut);
TH1D* fillHistoKDE(TFile *file, TString treename, TString histname,
      bool doOS,
      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double phistarbin1, double phistarbin2,
      double pt1cut, double pt2cut, double isocut,
      unsigned int nXBins, double xMin, double xMax,
      int dxymode);
void fixhist(TH1D *hist);
RooFitResult* fit(TDirectory *tdir, double pt1cut, double pt2cut, double isocut);

void optimise_sel(const char* datafile, const char* mcfile, const char* outputfile,
      const char* expr="log(vtxnormchi2)/log(10)", int nvarbins=55, double varmin=-7, double varmax=4) {
   TFile *fdata = TFile::Open(datafile);
   TFile *fmc = TFile::Open(mcfile);
   TFile *fout = TFile::Open(outputfile,"RECREATE");

   double ptcuts[6] = {5,6,7,8,9,10};
   double isocuts[5] = {6,8,10,12,10000};

   fout->cd();
   TTree *tr = new TTree("fitres","");
   float pt1, pt2, isocut, Ntot, Nsig, Nsig_err,fracSS1;
   tr->Branch("pt1",&pt1,"pt1/F");
   tr->Branch("pt2",&pt2,"pt2/F");
   tr->Branch("isocut",&isocut,"isocut/F");
   tr->Branch("Ntot",&Ntot,"Ntot/F");
   tr->Branch("Nsig",&Nsig,"Nsig/F");
   tr->Branch("Nsig_err",&Nsig_err,"Nsig_err/F");
   tr->Branch("fracSS1",&fracSS1,"fracSS1/F");
   

   // for (int i=0; i<6; i++)
   for (int i=0; i<1; i++)
      for (int j=0; j<=i; j++)
         for (int k=0; k<4; k++) {
            TDirectory *tdir = fout->mkdir(Form("pt1_%.0f_pt2_%.0f_isocut_%.0f",ptcuts[i],ptcuts[j],isocuts[k]));
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
            cout << "pt1>" << ptcuts[i] << ", pt2>" << ptcuts[j] << ", iso<" << isocuts[k] << endl;
            fillHisto(fdata, fmc, 10, 60, -0.8, -0.4, -1, 1e99, -1, 1e99,
            // fillHisto(fdata, fmc, 86, 96, -2.87, 1.93, -1, 1e99, -1, 1e99,
                  ptcuts[i], ptcuts[j], isocuts[k]);

            Ntot = data_obs->GetEntries();
            pt1 = ptcuts[i];
            pt2 = ptcuts[j];
            isocut = isocuts[k];

            // do the fit here!
            RooFitResult *fitres = fit(tdir, ptcuts[i], ptcuts[j], isocuts[k]);
            Nsig = ((RooRealVar*) (fitres->floatParsFinal().find("ndy")))->getVal();
            Nsig_err = ((RooRealVar*) (fitres->floatParsFinal().find("ndy")))->getError();
            // fracSS1 = ((RooRealVar*) (fitres->floatParsFinal().find("fracSS1")))->getVal();
            fitres->Write();
            fout->cd();
            tr->Fill();
         }

   fout->Write();
   fout->Close();
}

void fillHisto(TFile *fdata, TFile *fmc, 
      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double phistarbin1, double phistarbin2,
      double pt1cut, double pt2cut, double isocut) {
   // data
   // cout << "Filling data histo" << endl;
   TTree *tr = (TTree*) fdata->Get("tr_Data1");
   tr->Draw("log(vtxnormchi2)/log(10)>>+data_obs",
         Form("sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&pt1>%f&&pt2>%f&&trkiso1*pt1<%f&&trkiso2*pt2<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,pt1cut,pt2cut,isocut,isocut),
         "goff");
   tr = (TTree*) fdata->Get("tr_Data2");
   tr->Draw("log(vtxnormchi2)/log(10)>>+data_obs",
         Form("sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&pt1>%f&&pt2>%f&&trkiso1*pt1<%f&&trkiso2*pt2<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,pt1cut,pt2cut,isocut,isocut),
         "goff");

   // DY
   // cout << "Filling DY histo" << endl;
   tr = (TTree*) fmc->Get("tr_DYMuMu1030");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYMuMu",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&pt1>%f&&pt2>%f&&trkiso1*pt1<%f&&trkiso2*pt2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,pt1cut,pt2cut,isocut,isocut),
         "goff");
   tr = (TTree*) fmc->Get("tr_DYMuMu1030_PbP");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYMuMu",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&pt1>%f&&pt2>%f&&trkiso1*pt1<%f&&trkiso2*pt2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,pt1cut,pt2cut,isocut,isocut),
         "goff");
   tr = (TTree*) fmc->Get("tr_DYMuMu30");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYMuMu",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&pt1>%f&&pt2>%f&&trkiso1*pt1<%f&&trkiso2*pt2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,pt1cut,pt2cut,isocut,isocut),
         "goff");
   tr = (TTree*) fmc->Get("tr_DYMuMu30");
   tr = (TTree*) fmc->Get("tr_DYMuMu30_PbP");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYMuMu",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&pt1>%f&&pt2>%f&&trkiso1*pt1<%f&&trkiso2*pt2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,pt1cut,pt2cut,isocut,isocut),
         "goff");
   tr = (TTree*) fmc->Get("tr_DYMuMu30");

   // DYtautau
   // cout << "Filling DYtautau histo" << endl;
   tr = (TTree*) fmc->Get("tr_DYTauTau1030");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYTauTau",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&pt1>%f&&pt2>%f&&trkiso1*pt1<%f&&trkiso2*pt2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,pt1cut,pt2cut,isocut,isocut),
         "goff");
   tr = (TTree*) fmc->Get("tr_DYTauTau1030");
   tr->Draw("log(vtxnormchi2)/log(10)>>+DYTauTau",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&pt1>%f&&pt2>%f&&trkiso1*pt1<%f&&trkiso2*pt2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,pt1cut,pt2cut,isocut,isocut),
         "goff");

   // ttbar
   // cout << "Filling ttbar histo" << endl;
   tr = (TTree*) fmc->Get("tr_TT");
   tr->Draw("log(vtxnormchi2)/log(10)>>+TT",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&pt1>%f&&pt2>%f&&trkiso1*pt1<%f&&trkiso2*pt2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,pt1cut,pt2cut,isocut,isocut),
         "goff");

   // diboson
   // cout << "Filling diboson histo" << endl;
   tr = (TTree*) fmc->Get("tr_WW");
   tr->Draw("log(vtxnormchi2)/log(10)>>+WW",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&pt1>%f&&pt2>%f&&trkiso1*pt1<%f&&trkiso2*pt2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,pt1cut,pt2cut,isocut,isocut),
         "goff");
   tr = (TTree*) fmc->Get("tr_WZ");
   tr->Draw("log(vtxnormchi2)/log(10)>>+WZ",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&pt1>%f&&pt2>%f&&trkiso1*pt1<%f&&trkiso2*pt2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,pt1cut,pt2cut,isocut,isocut),
         "goff");
   tr = (TTree*) fmc->Get("tr_ZZ");
   tr->Draw("log(vtxnormchi2)/log(10)>>+ZZ",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity-0.47>%f&&diRapidity-0.47<%f&&diPt>%f&&diPt<%f&&diPhistar>%f&&diPhistar<%f&&pt1>%f&&pt2>%f&&trkiso1*pt1<%f&&trkiso2*pt2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,pt1cut,pt2cut,isocut,isocut),
         "goff");

   TH1D *DataSS1 = (TH1D*) gDirectory->Get("DataSS1");
   int nvarbins = DataSS1->GetNbinsX();
   double varmin = DataSS1->GetBinCenter(1)-DataSS1->GetBinWidth(1)/2.;
   double varmax = DataSS1->GetBinCenter(nvarbins)+DataSS1->GetBinWidth(nvarbins)/2.;

   TH1D *tmp = fillHistoKDE(fdata,"tr_Data1,tr_Data2","tmp",false,
         massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,pt1cut,pt2cut,isocut,nvarbins,varmin,varmax,1);
   DataSS1->Add(tmp);
   // DataSS1->Write();
   delete tmp;

   TH1D *DataSS2 = (TH1D*) gDirectory->Get("DataSS2");
   tmp = fillHistoKDE(fdata,"tr_Data1,tr_Data2","tmp",false,
         massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,phistarbin1,phistarbin2,pt1cut,pt2cut,isocut,nvarbins,varmin,varmax,2);
   DataSS2->Add(tmp);
   // DataSS2->Write();
   delete tmp;
}

TH1D* fillHistoKDE(TFile *file, TString treename, TString histname,
      bool doOS,
      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double phistarbin1, double phistarbin2,
      double pt1cut, double pt2cut, double isocut,
      unsigned int nXBins, double xMin, double xMax,
      int dxymode
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
      float diMass, diRapidity, diPt, diPhistar, vtxnormchi2,pt1,pt2,trkiso1,trkiso2;
      float dxyVTX1,dxyVTX2;
      float dxyBS1,dxyBS2;
      float maxrelPFiso;
      float weight=1;
      tr->SetBranchStatus("*",0);
      tr->SetBranchStatus("sign",1); tr->SetBranchAddress("sign",&sign);
      tr->SetBranchStatus("diMass",1); tr->SetBranchAddress("diMass",&diMass);
      tr->SetBranchStatus("diRapidity",1); tr->SetBranchAddress("diRapidity",&diRapidity);
      tr->SetBranchStatus("diPt",1); tr->SetBranchAddress("diPt",&diPt);
      tr->SetBranchStatus("diPhistar",1); tr->SetBranchAddress("diPhistar",&diPhistar);
      tr->SetBranchStatus("maxrelPFiso",1); tr->SetBranchAddress("maxrelPFiso",&maxrelPFiso);
      tr->SetBranchStatus("vtxnormchi2",1); tr->SetBranchAddress("vtxnormchi2",&vtxnormchi2);
      tr->SetBranchStatus("pt1",1); tr->SetBranchAddress("pt1",&pt1);
      tr->SetBranchStatus("pt2",1); tr->SetBranchAddress("pt2",&pt2);
      tr->SetBranchStatus("trkiso1",1); tr->SetBranchAddress("trkiso1",&trkiso1);
      tr->SetBranchStatus("trkiso2",1); tr->SetBranchAddress("trkiso2",&trkiso2);

      for (int i=0; i<tr->GetEntries(); i++) {
         tr->GetEntry(i);
         if (doOS && sign!=0) continue;
         if (pt1<pt1cut) continue;
         if (pt2<pt2cut) continue;
         // if (!doOS && sign==0) continue;
         if (doOS && !(pt1*trkiso1<isocut && pt2*trkiso2<isocut)) continue;
         if (!doOS) {
            if (dxymode==1 && !(sign!=0)) continue; // SS
            if (dxymode==2 && !(sign==0 && (pt1*trkiso1>isocut || pt2*trkiso2>isocut))) continue; // OS non iso
         }

         if (!(diMass>massbin1 && diMass<massbin2)) continue;
         if (!(diRapidity-0.47>rapbin1 && diRapidity-0.47<rapbin2)) continue;
         if (!(diPt>ptbin1 && diPt<ptbin2)) continue;
         if (!(diPhistar>phistarbin1 && diPhistar<phistarbin2)) continue;
         // if (!(log(max(abs(dxyVTX1),abs(dxyVTX2)))/log(10)<-2)) continue;
         // if (dxymode==1 && !(dxyVTX1*dxyVTX2>0)) continue;
         // if (dxymode==2 && !(dxyVTX1*dxyVTX2<=0)) continue;
         double val = log(vtxnormchi2)/log(10);
         vals.push_back(val);
         weights.push_back(weight);
         if (val>xMin&&val<xMax) nevts++;
      } // end event loop
   } // end tokenize

   if (nevts==0) {
      // no events: makes no sense to do KDE
      return new TH1D(histname,histname,nXBins,xMin,xMax);
   }

   KDEProducer kdeprod(&vals,&weights,hSF,nXBins,xMin,xMax,trimFactor,doSigmaScaling);

   TH1D *ans = NULL;
   if (doAdaptive) ans = (TH1D*) kdeprod.getAPDF(histname.Data(), "KDE density", nXBins, xMin, xMax);
   else ans = (TH1D*) kdeprod.getPDF(histname.Data(), "KDE density", nXBins, xMin, xMax);
   ans->Scale(nevts*((xMax-xMin)/nXBins));
   return ans;
}

RooFitResult* fit(TDirectory *tdir, double pt1cut, double pt2cut, double isocut) {
   tdir->cd();

   // get histos
   TH1D *hdata = (TH1D*) gDirectory->Get("data_obs"); hdata->Rebin(nrebin);
   if (hdata->Integral()==0) return NULL;
   TH1D *hdy = (TH1D*) gDirectory->Get("DYMuMu"); hdy->Rebin(nrebin);
   TH1D *hdytautau = (TH1D*) gDirectory->Get("DYTauTau"); hdytautau->Rebin(nrebin);
   TH1D *htt = (TH1D*) gDirectory->Get("TT"); htt->Rebin(nrebin);
   TH1D *hww = (TH1D*) gDirectory->Get("WW"); hww->Rebin(nrebin);
   TH1D *hwz = (TH1D*) gDirectory->Get("WZ"); hwz->Rebin(nrebin);
   TH1D *hzz = (TH1D*) gDirectory->Get("ZZ"); hzz->Rebin(nrebin);
   TH1D *hdataSS1 = (TH1D*) gDirectory->Get("DataSS1"); hdataSS1->Rebin(nrebin);
   TH1D *hdataSS2 = (TH1D*) gDirectory->Get("DataSS2"); hdataSS2->Rebin(nrebin);

   // create variables
   int nxbins = hdata->GetNbinsX();
   double xmin = hdata->GetBinCenter(1)-hdata->GetBinWidth(1)/2.;
   double xmax = hdata->GetBinCenter(nxbins)+hdata->GetBinWidth(nxbins)/2.;
   RooRealVar var("var","log_{10}(vtx #chi^{2}/ndf)",xmin,xmax,"");
   RooRealVar ndy("ndy","N(DYMuMu)",hdy->Integral(),0,10*hdy->Integral());
   RooRealVar ndytautau("ndytautau","N(DYTauTau)",hdytautau->Integral(),0.9*hdytautau->Integral(),1.1*hdytautau->Integral()); ndytautau.setConstant(true);
   RooRealVar ntt("ntt","N(TT)",htt->Integral(),0.9*htt->Integral(),1.1*htt->Integral()); ntt.setConstant(true);
   RooRealVar nww("nww","N(WW)",hww->Integral(),0.9*hww->Integral(),1.1*hww->Integral()); nww.setConstant(true);
   RooRealVar nwz("nwz","N(WZ)",hwz->Integral(),0.9*hwz->Integral(),1.1*hwz->Integral()); nwz.setConstant(true);
   RooRealVar nzz("nzz","N(ZZ)",hzz->Integral(),0.9*hzz->Integral(),1.1*hzz->Integral()); nzz.setConstant(true);
   RooRealVar ndataSS("ndataSS","N(HF)",(hdataSS1->Integral()+hdataSS2->Integral()),0,100*hdataSS1->Integral()+100*hdataSS2->Integral()); 
   RooRealVar fracSS1("fracSS1","frac(HF type1)",hdataSS1->Integral()/(hdataSS1->Integral()+hdataSS2->Integral()),0.00001,0.99999); 
   // fracSS1.setConstant(true);
   // avoid problem if not enough events
   // if (hdataSS1->Integral()+hdataSS2->Integral()<50) {
      // fracSS1.setVal(0.);
      // fracSS1.setConstant(true);
   // }

   // convert histos to RooFit objects
   RooDataHist rhdata("rhdata","Data",RooArgList(var),hdata);
   fixhist(hdy);
   RooDataHist rhdy("rhdy","DYMuMu",RooArgList(var),hdy);
   RooHistPdf pdy("pdy","DYMuMu",RooArgList(var),rhdy);
   fixhist(hdytautau);
   RooDataHist rhdytautau("rhdytautau","DYTauTau",RooArgList(var),hdytautau);
   RooHistPdf pdytautau("pdytautau","DYTauTau",RooArgList(var),rhdytautau);
   fixhist(htt);
   RooDataHist rhtt("rhtt","TT",RooArgList(var),htt);
   RooHistPdf ptt("ptt","TT",RooArgList(var),rhtt);
   fixhist(hww);
   RooDataHist rhww("rhww","VV",RooArgList(var),hww);
   RooHistPdf pww("pww","VV",RooArgList(var),rhww);
   fixhist(hwz);
   RooDataHist rhwz("rhwz","VV",RooArgList(var),hwz);
   RooHistPdf pwz("pwz","VV",RooArgList(var),rhwz);
   fixhist(hzz);
   RooDataHist rhzz("rhzz","VV",RooArgList(var),hzz);
   RooHistPdf pzz("pzz","VV",RooArgList(var),rhzz);
   fixhist(hdataSS1);
   RooDataHist rhdataSS1("rhdataSS1","HF1",RooArgList(var),hdataSS1);
   RooHistPdf pdataSS1("pdataSS1","HF1",RooArgList(var),rhdataSS1);
   fixhist(hdataSS2);
   RooDataHist rhdataSS2("rhdataSS2","HF2",RooArgList(var),hdataSS2);
   RooHistPdf pdataSS2("pdataSS2","HF2",RooArgList(var),rhdataSS2);
   RooAddPdf pdataSS("pdataSS","HF",RooArgSet(pdataSS1,pdataSS2),fracSS1);

   // make the model
   RooAddPdf model("model","Fit model",RooArgList(pdy,pdytautau,ptt,pww,pwz,pzz,pdataSS),RooArgList(ndy,ndytautau,ntt,nww,nwz,nzz,ndataSS));
   RooAddPdf pvv("model","Fit model",RooArgList(pww,pwz,pzz),RooArgList(nww,nwz,nzz)); // for plotting

   // do the fit
   RooFitResult *result = model.fitTo(rhdata,Minos(),Extended(true),Save());

   // draw the results
   TCanvas c1;
   // c1.SetLogy();
   RooPlot* xframe = var.frame(Title("Fit to var")) ;
   rhdata.plotOn(xframe);
   model.plotOn(xframe,Components(RooArgSet(pww,pwz,pzz,ptt,pdytautau,pdy,pdataSS)),FillColor(kRed),LineColor(kRed),DrawOption("F"));
   model.plotOn(xframe,Components(RooArgSet(pww,pwz,pzz,ptt,pdytautau,pdataSS)),FillColor(kViolet),LineColor(kViolet),DrawOption("F"));
   model.plotOn(xframe,Components(RooArgSet(pww,pwz,pzz,ptt,pdytautau)),FillColor(kOrange),LineColor(kOrange),DrawOption("F"));
   model.plotOn(xframe,Components(RooArgSet(pww,pwz,pzz,ptt)),FillColor(kGreen+2),LineColor(kGreen+2),DrawOption("F"));
   model.plotOn(xframe,Components(RooArgSet(pww,pwz,pzz)),FillColor(kBlue),LineColor(kBlue),DrawOption("F"));
   model.plotOn(xframe,LineColor(kBlack));
   rhdata.plotOn(xframe);

   // xframe->GetYaxis()->SetRangeUser(0.1,10.*xframe->GetMaximum());
   xframe->Draw();


   // compute the chi2
   RooGoF roogof(xframe->getHist(xframe->nameOf(0)),xframe->getCurve(xframe->nameOf(6)));
   double pvalue=0, chi2val=0;
   int ndf=hdata->GetNbinsX();
   int d_ndf=result->floatParsFinal().getSize();
   roogof.BCChi2Test(pvalue,chi2val,ndf,d_ndf);

   TLegend* leg = new TLegend(0.16,0.59,0.53,0.92);
   leg->SetBorderSize(0);
   leg->SetFillStyle(4000);
   leg->AddEntry(xframe,Form("pt1>%.0f, pt2>%.0f, tkiso<%.0f",pt1cut, pt2cut, isocut),"");
   leg->AddEntry(xframe,Form("#chi^{2}/ndf = %.2f/%d (pval = %.1f%s)",chi2val,ndf,pvalue*100.,"%"),"");
   leg->AddEntry(xframe->nameOf(0),Form("%s (%.0f)","Data",rhdata.sumEntries()),"lp");
   leg->AddEntry(xframe->nameOf(1),Form("%s (%.1f)","DYMuMu",ndy.getVal()),"f");
   leg->AddEntry(xframe->nameOf(2),Form("%s (%.1f)","SS Data",ndataSS.getVal()),"f");
   leg->AddEntry(xframe->nameOf(3),Form("%s (%.1f)","DYTauTau",ndytautau.getVal()),"f");
   leg->AddEntry(xframe->nameOf(4),Form("%s (%.1f)","TT",ntt.getVal()),"f");
   leg->AddEntry(xframe->nameOf(5),Form("%s (%.1f)","VV",nww.getVal()+nwz.getVal()+nzz.getVal()),"f");
   leg->AddEntry(xframe->nameOf(6),Form("%s","Fit"),"l");
   leg->Draw();

   c1.SaveAs(Form("fits_optimise/fit_%.0f_%.0f_%.0f.pdf",pt1cut,pt2cut,isocut));
   // c1.SaveAs(Form("fits_optimise/fit_%.0f_%.0f_%.0f.root",pt1cut,pt2cut,isocut));
   return result;
}

void fixhist(TH1D *hist) {
   for (int i=1; i<=hist->GetNbinsX(); i++) {
      if (hist->GetBinContent(i)<=0) hist->SetBinContent(i,1e-6);
   }
}
