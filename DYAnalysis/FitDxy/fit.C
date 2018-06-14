#include "../BkgEst/interface/defs.h"

using namespace RooFit;
using namespace DYana;

const int nrebin = 1;

void fixhist(TH1D *hist);
RooFitResult* fit(const char* histfile, const char* varname, double varmin, double varmax);

void fit(const char* histfile, const char* outputfile) {
   TFile *f = TFile::Open(outputfile,"RECREATE");

   // mass
   TDirectory *dir = f->mkdir("mass");
   dir->cd();
   TH1D *hndy = new TH1D("hndy","raw fitted mass yields",binnum,bins);
   TH1D *hfracSS1 = new TH1D("hfracSS1","frac(HF type 1)",binnum,bins);
   TH1D *hdataSS = new TH1D("hdataSS","N(HF)",binnum,bins);
   for (int i=0; i<binnum; i++) {
      RooFitResult *result = fit(histfile,"mass",bins[i],bins[i+1]);
      if (!result) continue;
      hndy->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getVal());
      hndy->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getError());
      if (result->floatParsFinal().find("fracSS1")) {
         hfracSS1->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getVal());
         hfracSS1->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getError());
      }
      hdataSS->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getVal());
      hdataSS->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getError());
      dir->cd();
      result->Write();
   }
   dir->cd();
   hndy->Write();
   hfracSS1->Write();
   hdataSS->Write();

   // rap1560
   dir = f->mkdir("rap1560");
   dir->cd();
   hndy = new TH1D("hndy","raw fitted rap1560 yields",rapbinnum_1560,rapbin_1560);
   hfracSS1 = new TH1D("hfracSS1","frac(HF type 1)",rapbinnum_1560,rapbin_1560);
   hdataSS = new TH1D("hdataSS","N(HF)",rapbinnum_1560,rapbin_1560);
   for (int i=0; i<rapbinnum_1560; i++) {
      RooFitResult *result = fit(histfile,"rap1560",rapbin_1560[i],rapbin_1560[i+1]);
      if (!result) continue;
      hndy->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getVal());
      hndy->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getError());
      if (result->floatParsFinal().find("fracSS1")) {
         hfracSS1->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getVal());
         hfracSS1->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getError());
      }
      hdataSS->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getVal());
      hdataSS->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getError());
      dir->cd();
      result->Write();
   }
   dir->cd();
   hndy->Write();
   hfracSS1->Write();
   hdataSS->Write();

   // rap60120
   dir = f->mkdir("rap60120");
   dir->cd();
   hndy = new TH1D("hndy","raw fitted rap60120 yields",rapbinnum_60120,rapbin_60120);
   hfracSS1 = new TH1D("hfracSS1","frac(HF type 1)",rapbinnum_60120,rapbin_60120);
   hdataSS = new TH1D("hdataSS","N(HF)",rapbinnum_60120,rapbin_60120);
   for (int i=0; i<rapbinnum_60120; i++) {
      RooFitResult *result = fit(histfile,"rap60120",rapbin_60120[i],rapbin_60120[i+1]);
      if (!result) continue;
      hndy->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getVal());
      hndy->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getError());
      if (result->floatParsFinal().find("fracSS1")) {
         hfracSS1->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getVal());
         hfracSS1->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getError());
      }
      hdataSS->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getVal());
      hdataSS->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getError());
      dir->cd();
      result->Write();
   }
   dir->cd();
   hndy->Write();
   hfracSS1->Write();
   hdataSS->Write();

   // pt
   dir = f->mkdir("pt");
   dir->cd();
   hndy = new TH1D("hndy","raw fitted pt yields",ptbinnum_meas,ptbin_meas);
   hfracSS1 = new TH1D("hfracSS1","frac(HF type 1)",ptbinnum_meas,ptbin_meas);
   hdataSS = new TH1D("hdataSS","N(HF)",ptbinnum_meas,ptbin_meas);
   for (int i=0; i<ptbinnum_meas; i++) {
      RooFitResult *result = fit(histfile,"pt",ptbin_meas[i],ptbin_meas[i+1]);
      if (!result) continue;
      hndy->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getVal());
      hndy->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getError());
      if (result->floatParsFinal().find("fracSS1")) {
         hfracSS1->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getVal());
         hfracSS1->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getError());
      }
      hdataSS->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getVal());
      hdataSS->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getError());
      dir->cd();
      result->Write();
   }
   dir->cd();
   hndy->Write();
   hfracSS1->Write();
   hdataSS->Write();

   // phistar
   dir = f->mkdir("phistar");
   dir->cd();
   hndy = new TH1D("hndy","raw fitted phistar yields",phistarnum,phistarbin);
   hfracSS1 = new TH1D("hfracSS1","frac(HF type 1)",phistarnum,phistarbin);
   hdataSS = new TH1D("hdataSS","N(HF)",phistarnum,phistarbin);
   for (int i=0; i<phistarnum; i++) {
      RooFitResult *result = fit(histfile,"phistar",phistarbin[i],phistarbin[i+1]);
      if (!result) continue;
      hndy->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getVal());
      hndy->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getError());
      if (result->floatParsFinal().find("fracSS1")) {
         hfracSS1->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getVal());
         hfracSS1->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getError());
      }
      hdataSS->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getVal());
      hdataSS->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getError());
      dir->cd();
      result->Write();
   }
   dir->cd();
   hndy->Write();
   hfracSS1->Write();
   hdataSS->Write();

   f->Close();
}

RooFitResult* fit(const char* histfile, const char* varname, double varmin, double varmax) {
   TFile *f = TFile::Open(histfile);
   f->cd(Form("%s/%.2f_%.2f",varname,varmin,varmax));

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
   RooRealVar var("var","log_{10}(vtx #chi^{2}/ndf)",-4,4,"");
   RooRealVar ndy("ndy","N(DYMuMu)",hdy->Integral(),0,2*hdy->Integral());
   RooRealVar ndytautau("ndytautau","N(DYTauTau)",hdytautau->Integral(),0.9*hdytautau->Integral(),1.1*hdytautau->Integral()); ndytautau.setConstant(true);
   RooRealVar ntt("ntt","N(TT)",htt->Integral(),0.9*htt->Integral(),1.1*htt->Integral()); ntt.setConstant(true);
   RooRealVar nww("nww","N(WW)",hww->Integral(),0.9*hww->Integral(),1.1*hww->Integral()); nww.setConstant(true);
   RooRealVar nwz("nwz","N(WZ)",hwz->Integral(),0.9*hwz->Integral(),1.1*hwz->Integral()); nwz.setConstant(true);
   RooRealVar nzz("nzz","N(ZZ)",hzz->Integral(),0.9*hzz->Integral(),1.1*hzz->Integral()); nzz.setConstant(true);
   RooRealVar ndataSS("ndataSS","N(HF)",hdataSS1->Integral()+hdataSS2->Integral(),0,10*hdataSS1->Integral()+10*hdataSS2->Integral()); 
   RooRealVar fracSS1("fracSS1","frac(HF type1)",hdataSS1->Integral()/(hdataSS1->Integral()+hdataSS2->Integral()),0.01,0.99); 
   // avoid problem if not enough events
   if (hdataSS1->Integral()+hdataSS2->Integral()<50) {
      fracSS1.setVal(0.5);
      fracSS1.setConstant(true);
   }

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

   // do the fit
   RooFitResult *result = model.fitTo(rhdata,Minos(),Extended(true),Save());

   // draw the results
   TCanvas c1;
   RooPlot* xframe = var.frame(Title("Fit to var")) ;
   rhdata.plotOn(xframe);
   model.plotOn(xframe,Components(RooArgSet(pdataSS1,pdataSS2)),LineColor(kViolet),FillColor(kViolet),DrawOption("F"));
   model.plotOn(xframe,LineColor(kBlack));
   model.plotOn(xframe,Components(pdy),LineColor(kRed));
   rhdata.plotOn(xframe);
   xframe->Draw();

   c1.SaveAs(Form("fit_%s_%.2f_%.2f.pdf",varname,varmin,varmax));
   f->Close();
   return result;
}

void fixhist(TH1D *hist) {
   for (int i=1; i<=hist->GetNbinsX(); i++) {
      if (hist->GetBinContent(i)<=0) hist->SetBinContent(i,1e-6);
   }
}
