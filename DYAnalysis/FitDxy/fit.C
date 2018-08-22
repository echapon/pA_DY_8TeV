#include "../BkgEst/interface/defs.h"
#include "RooGoF.C"

#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"
#include "RooFitResult.h"
#include "RooDataHist.h"
#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooPlot.h"
#include "RooArgSet.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"
#include "RooGaussian.h"
#include "RooProdPdf.h"

#include <map>

using namespace RooFit;
using namespace DYana;
using namespace std;

const int nrebin = 1;
const double xmin = -7;
const double xmax = 4;

const bool dofullnonisosub = false;

void fixhist(TH1D* &hist, bool dozero=true);
RooFitResult* fit(const char* histfile, const char* varname, double varmin, double varmax, int idxbkg1, int idxbkg2, float &chi2, int &ndf);
void fillvals(RooFitResult *r, map<TString,float> &vals);

void fit(const char* histfile, const char* outputfile, int idxbkg1=1, int idxbkg2=2) {
   TFile *f = TFile::Open(outputfile,"RECREATE");
   f->cd();

   // a tree to hold the results
   TTree *tr = new TTree("fit_results","fit results");
   vector<TString> vars;
   vars.push_back("ndy");
   vars.push_back("ntt");
   vars.push_back("nww");
   vars.push_back("nwz");
   vars.push_back("nzz");
   vars.push_back("ndataSS");
   vars.push_back("fracSS1");
   map<TString,float> vals;
   for (unsigned int i=0; i<vars.size(); i++) {
      vals[vars[i]+"_ini"] = 0; 
      tr->Branch(vars[i]+"_ini",&(vals[vars[i]+"_ini"]),vars[i]+"_ini/F");
      vals[vars[i]+"_val"] = 0;
      tr->Branch(vars[i]+"_val",&(vals[vars[i]+"_val"]),vars[i]+"_val/F");
      vals[vars[i]+"_err"] = 0;
      tr->Branch(vars[i]+"_err",&(vals[vars[i]+"_err"]),vars[i]+"_err/F");
   }
   float massbin1; float massbin2;
   float rapbin1; float rapbin2;
   float ptbin1; float ptbin2;
   float phistarbin1; float phistarbin2;
   float chi2; int ndf;
   tr->Branch("massbin1",&massbin1,"massbin1/F");
   tr->Branch("massbin2",&massbin2,"massbin2/F");
   tr->Branch("rapbin1",&rapbin1,"rapbin1/F");
   tr->Branch("rapbin2",&rapbin2,"rapbin2/F");
   tr->Branch("ptbin1",&ptbin1,"ptbin1/F");
   tr->Branch("ptbin2",&ptbin2,"ptbin2/F");
   tr->Branch("phistarbin1",&phistarbin1,"phistarbin1/F");
   tr->Branch("phistarbin2",&phistarbin2,"phistarbin2/F");
   tr->Branch("chi2",&chi2,"chi2/F");
   tr->Branch("ndf",&ndf,"ndf/I");

   // mass
   TDirectory *dir = f->mkdir("mass");
   dir->cd();
   TH1D *hndy_ini = new TH1D("hndy_ini","initial mass yields",binnum,bins);
   TH1D *hndy = new TH1D("hndy","raw fitted mass yields",binnum,bins);
   TH1D *hfracSS1 = new TH1D("hfracSS1","frac(HF type 1)",binnum,bins);
   TH1D *hdataSS = new TH1D("hdataSS","N(HF)",binnum,bins);
   rapbin1 = rapbin_60120[0];
   rapbin2 = rapbin_60120[rapbinnum_60120];
   ptbin1 = ptbin_meas[0];
   ptbin2 = ptbin_meas[ptbinnum_meas];
   phistarbin1 = phistarbin[0];
   phistarbin2 = phistarbin[phistarnum];

   for (int i=0; i<binnum; i++) {
      massbin1 = bins[i];
      massbin2 = bins[i+1];
      RooFitResult *result = fit(histfile,"mass",massbin1,massbin2,idxbkg1,idxbkg2,chi2,ndf);
      if (!result) continue;
      hndy_ini->SetBinContent(i+1,((RooRealVar*) (result->floatParsInit().find("ndy")))->getVal());
      hndy_ini->SetBinError(i+1,((RooRealVar*) (result->floatParsInit().find("ndy")))->getError());
      hndy->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getVal());
      hndy->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getError());
      if (result->floatParsFinal().find("fracSS1")) {
         hfracSS1->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getVal());
         hfracSS1->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getError());
      }
      hdataSS->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getVal());
      hdataSS->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getError());
      dir->cd();
      result->Write(Form("mass_%.0f_%.0f",massbin1,massbin2));

      fillvals(result,vals);
      tr->Fill();
   }
   dir->cd();
   hndy_ini->Write();
   hndy->Write();
   hfracSS1->Write();
   hdataSS->Write();

   // rap1560
   dir = f->mkdir("rap1560");
   dir->cd();
   hndy_ini = new TH1D("hndy_ini","raw fitted rap1560 yields",rapbinnum_1560,rapbin_1560);
   hndy = new TH1D("hndy","raw fitted rap1560 yields",rapbinnum_1560,rapbin_1560);
   hfracSS1 = new TH1D("hfracSS1","frac(HF type 1)",rapbinnum_1560,rapbin_1560);
   hdataSS = new TH1D("hdataSS","N(HF)",rapbinnum_1560,rapbin_1560);
   massbin1 = 11;
   massbin2 = 60;
   for (int i=0; i<rapbinnum_1560; i++) {
      rapbin1 = rapbin_1560[i];
      rapbin2 = rapbin_1560[i+1];
      RooFitResult *result = fit(histfile,"rap1560",rapbin1,rapbin2,idxbkg1,idxbkg2,chi2,ndf);
      if (!result) continue;
      hndy_ini->SetBinContent(i+1,((RooRealVar*) (result->floatParsInit().find("ndy")))->getVal());
      hndy_ini->SetBinError(i+1,((RooRealVar*) (result->floatParsInit().find("ndy")))->getError());
      hndy->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getVal());
      hndy->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getError());
      if (result->floatParsFinal().find("fracSS1")) {
         hfracSS1->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getVal());
         hfracSS1->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getError());
      }
      hdataSS->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getVal());
      hdataSS->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getError());
      dir->cd();
      result->Write(Form("rap1560_%.0f_%.0f",rapbin1,rapbin2));

      fillvals(result,vals);
      tr->Fill();
   }
   dir->cd();
   hndy_ini->Write();
   hndy->Write();
   hfracSS1->Write();
   hdataSS->Write();

   // rap60120
   dir = f->mkdir("rap60120");
   dir->cd();
   hndy_ini = new TH1D("hndy_ini","raw fitted rap60120 yields",rapbinnum_60120,rapbin_60120);
   hndy = new TH1D("hndy","raw fitted rap60120 yields",rapbinnum_60120,rapbin_60120);
   hfracSS1 = new TH1D("hfracSS1","frac(HF type 1)",rapbinnum_60120,rapbin_60120);
   hdataSS = new TH1D("hdataSS","N(HF)",rapbinnum_60120,rapbin_60120);
   massbin1 = 60;
   massbin2 = 120;
   for (int i=0; i<rapbinnum_60120; i++) {
      rapbin1 = rapbin_60120[i];
      rapbin2 = rapbin_60120[i+1];
      RooFitResult *result = fit(histfile,"rap60120",rapbin1,rapbin2,idxbkg1,idxbkg2,chi2,ndf);
      if (!result) continue;
      hndy_ini->SetBinContent(i+1,((RooRealVar*) (result->floatParsInit().find("ndy")))->getVal());
      hndy_ini->SetBinError(i+1,((RooRealVar*) (result->floatParsInit().find("ndy")))->getError());
      hndy->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getVal());
      hndy->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getError());
      if (result->floatParsFinal().find("fracSS1")) {
         hfracSS1->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getVal());
         hfracSS1->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getError());
      }
      hdataSS->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getVal());
      hdataSS->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getError());
      dir->cd();
      result->Write(Form("rap60120_%.0f_%.0f",rapbin1,rapbin2));

      fillvals(result,vals);
      tr->Fill();
   }
   dir->cd();
   hndy_ini->Write();
   hndy->Write();
   hfracSS1->Write();
   hdataSS->Write();

   // pt
   dir = f->mkdir("pt");
   dir->cd();
   hndy_ini = new TH1D("hndy_ini","raw fitted pt yields",ptbinnum_meas,ptbin_meas);
   hndy = new TH1D("hndy","raw fitted pt yields",ptbinnum_meas,ptbin_meas);
   hfracSS1 = new TH1D("hfracSS1","frac(HF type 1)",ptbinnum_meas,ptbin_meas);
   hdataSS = new TH1D("hdataSS","N(HF)",ptbinnum_meas,ptbin_meas);
   rapbin1 = rapbin_60120[0];
   rapbin2 = rapbin_60120[rapbinnum_60120];
   for (int i=0; i<ptbinnum_meas; i++) {
      ptbin1 = ptbin_meas[i];
      ptbin2 = ptbin_meas[i+1];
      RooFitResult *result = fit(histfile,"pt",ptbin1,ptbin2,idxbkg1,idxbkg2,chi2,ndf);
      if (!result) continue;
      hndy_ini->SetBinContent(i+1,((RooRealVar*) (result->floatParsInit().find("ndy")))->getVal());
      hndy_ini->SetBinError(i+1,((RooRealVar*) (result->floatParsInit().find("ndy")))->getError());
      hndy->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getVal());
      hndy->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getError());
      if (result->floatParsFinal().find("fracSS1")) {
         hfracSS1->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getVal());
         hfracSS1->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getError());
      }
      hdataSS->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getVal());
      hdataSS->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getError());
      dir->cd();
      result->Write(Form("pt_%.0f_%.0f",ptbin1,ptbin2));

      fillvals(result,vals);
      tr->Fill();
   }
   dir->cd();
   hndy_ini->Write();
   hndy->Write();
   hfracSS1->Write();
   hdataSS->Write();

   // phistar
   dir = f->mkdir("phistar");
   dir->cd();
   hndy_ini = new TH1D("hndy_ini","raw fitted phistar yields",phistarnum,phistarbin);
   hndy = new TH1D("hndy","raw fitted phistar yields",phistarnum,phistarbin);
   hfracSS1 = new TH1D("hfracSS1","frac(HF type 1)",phistarnum,phistarbin);
   hdataSS = new TH1D("hdataSS","N(HF)",phistarnum,phistarbin);
   ptbin1 = ptbin_meas[0];
   ptbin2 = ptbin_meas[ptbinnum_meas];
   for (int i=0; i<phistarnum; i++) {
      phistarbin1 = phistarbin[i];
      phistarbin2 = phistarbin[i+1];
      RooFitResult *result = fit(histfile,"phistar",phistarbin1,phistarbin2,idxbkg1,idxbkg2,chi2,ndf);
      if (!result) continue;
      hndy_ini->SetBinContent(i+1,((RooRealVar*) (result->floatParsInit().find("ndy")))->getVal());
      hndy_ini->SetBinError(i+1,((RooRealVar*) (result->floatParsInit().find("ndy")))->getError());
      hndy->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getVal());
      hndy->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndy")))->getError());
      if (result->floatParsFinal().find("fracSS1")) {
         hfracSS1->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getVal());
         hfracSS1->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("fracSS1")))->getError());
      }
      hdataSS->SetBinContent(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getVal());
      hdataSS->SetBinError(i+1,((RooRealVar*) (result->floatParsFinal().find("ndataSS")))->getError());
      dir->cd();
      result->Write(Form("phistar_%.0f_%.0f",phistarbin1,phistarbin2));

      fillvals(result,vals);
      tr->Fill();
   }
   dir->cd();
   hndy_ini->Write();
   hndy->Write();
   hfracSS1->Write();
   hdataSS->Write();

   f->cd();
   tr->Write();
   f->Close();
}

RooFitResult* fit(const char* histfile, const char* varname, double varmin, double varmax, int idxbkg1, int idxbkg2, float &chi2, int &ndf) {
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
   TH1D *hdyNI0 = (TH1D*) gDirectory->Get("DYMuMuNI0"); hdyNI0->Rebin(nrebin);
   TH1D *hdyNI1 = (TH1D*) gDirectory->Get("DYMuMuNI1"); hdyNI1->Rebin(nrebin);
   TH1D *hdyNI2 = (TH1D*) gDirectory->Get("DYMuMuNI2"); hdyNI2->Rebin(nrebin);
   TH1D *hotherNI0 = (TH1D*) gDirectory->Get("hotherNI0"); hotherNI0->Rebin(nrebin);
   TH1D *hotherNI1 = (TH1D*) gDirectory->Get("hotherNI1"); hotherNI1->Rebin(nrebin);
   TH1D *hotherNI2 = (TH1D*) gDirectory->Get("hotherNI2"); hotherNI2->Rebin(nrebin);
   TH1D *hdataSS1 = (TH1D*) gDirectory->Get(Form("DataSS%d",idxbkg1)); hdataSS1->Rebin(nrebin);
   TH1D *hdataSS2 = (TH1D*) gDirectory->Get(Form("DataSS%d",idxbkg2)); hdataSS2->Rebin(nrebin);

   // // estimate of the bkg yield
   TH1D *hdataSSiso = (TH1D*) f->Get(TString(varname)+"/hdataSSiso");
   double NbkgA = hdataSSiso->GetBinContent(hdataSSiso->FindBin((varmin+varmax)/2.));
   TH1D *hdataSSnoniso = (TH1D*) f->Get(TString(varname)+"/hdataSSnoniso");
   double NbkgB = hdataSSnoniso->GetBinContent(hdataSSnoniso->FindBin((varmin+varmax)/2.));
   TH1D *hdataOSnoniso = (TH1D*) f->Get(TString(varname)+"/hdataOSnoniso");
   double NbkgD = hdataOSnoniso->GetBinContent(hdataOSnoniso->FindBin((varmin+varmax)/2.))-hdyNI0->Integral()-hotherNI0->Integral();
   double NbkgC = NbkgB>0 ? NbkgA*NbkgD/NbkgB : 0.1;

   // create variables
   int nxbins = hdata->GetNbinsX();
   double xmin = hdata->GetBinCenter(1)-hdata->GetBinWidth(1)/2.;
   double xmax = hdata->GetBinCenter(nxbins)+hdata->GetBinWidth(nxbins)/2.;
   RooRealVar var("var","log_{10}(vtx #chi^{2}/ndf)",xmin,xmax,"");
   RooRealVar ndy("ndy","N(DYMuMu)",hdy->Integral(),0,2*hdy->Integral());
   RooRealVar ndytautau("ndytautau","N(DYTauTau)",hdytautau->Integral(),0.9*hdytautau->Integral(),1.1*hdytautau->Integral()); ndytautau.setConstant(true);
   RooRealVar ntt("ntt","N(TT)",htt->Integral(),0.9*htt->Integral(),1.1*htt->Integral()); ntt.setConstant(true);
   RooRealVar nww("nww","N(WW)",hww->Integral(),0.9*hww->Integral(),1.1*hww->Integral()); nww.setConstant(true);
   RooRealVar nwz("nwz","N(WZ)",hwz->Integral(),0.9*hwz->Integral(),1.1*hwz->Integral()); nwz.setConstant(true);
   RooRealVar nzz("nzz","N(ZZ)",hzz->Integral(),0.9*hzz->Integral(),1.1*hzz->Integral()); nzz.setConstant(true);
   // RooRealVar ndataSS("ndataSS","N(bkg)",NbkgC,0,1.5*hdata->Integral()); 
   RooRealVar ndataSS("ndataSS","N(bkg)",0.1*hdata->Integral(),0,1.5*hdata->Integral()); 
   RooRealVar fracSS1("fracSS1","frac(SS bkg)",0.5,0.001,0.999); 
   // RooRealVar fracSS1("fracSS1","frac(HF type1)",1,0.001,1); 
   // fracSS1.setConstant(true);
   // avoid problem if not enough events
   if (hdataSS1->Integral()+hdataSS2->Integral()<50) {
      fracSS1.setVal(0.5);
      fracSS1.setConstant(true);
   }

   // convert histos to RooFit objects
   fixhist(hdata,false);
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
   fixhist(hdataSS2);
   RooDataHist rhdataSS2("rhdataSS2","HF2",RooArgList(var),hdataSS2);
   fixhist(hdyNI0);
   RooDataHist rhdyNI0("rhdyNI0","DY#mu#mu non iso 0",RooArgList(var),hdyNI0);
   RooHistPdf pdyNI0("pdyNI0","pdyNI0",RooArgList(var),rhdyNI0);
   fixhist(hdyNI1);
   RooDataHist rhdyNI1("rhdyNI1","DY#mu#mu non iso 1",RooArgList(var),hdyNI1);
   RooHistPdf pdyNI1("pdyNI1","pdyNI1",RooArgList(var),rhdyNI1);
   fixhist(hdyNI2);
   RooDataHist rhdyNI2("rhdyNI2","DY#mu#mu non iso 2",RooArgList(var),hdyNI2);
   RooHistPdf pdyNI2("pdyNI2","pdyNI2",RooArgList(var),rhdyNI2);
   fixhist(hotherNI0);
   RooDataHist rhotherNI0("rhotherNI0","other non iso 0",RooArgList(var),hotherNI0);
   RooHistPdf potherNI0("potherNI0","potherNI0",RooArgList(var),rhotherNI0);
   fixhist(hotherNI1);
   RooDataHist rhotherNI1("rhotherNI1","other non iso 1",RooArgList(var),hotherNI1);
   RooHistPdf potherNI1("potherNI1","potherNI1",RooArgList(var),rhotherNI1);
   fixhist(hotherNI2);
   RooDataHist rhotherNI2("rhotherNI2","other non iso 2",RooArgList(var),hotherNI2);
   RooHistPdf potherNI2("potherNI2","potherNI2",RooArgList(var),rhotherNI2);

   // for SS/noniso background: need to subtract the OS non iso part
   RooAbsPdf *pdataSS1=NULL, *pdataSS2=NULL;
   RooDataHist *rhdataSS1_sub=NULL, *rhdataSS2_sub=NULL;
   RooHistPdf pdataSS1_0 ("pdataSS1_0","HF1",RooArgList(var),rhdataSS1);
   RooHistPdf pdataSS2_0 ("pdataSS2_0","HF2",RooArgList(var),rhdataSS2);
   RooFormulaVar ndyNI0("ndyNI0","ndyNI0",Form("-@0*%f",hdyNI0->Integral()/hdy->Integral()),ndy);
   RooFormulaVar ndyNI1("ndyNI1","ndyNI1",Form("-@0*%f",hdyNI1->Integral()/hdy->Integral()),ndy);
   RooFormulaVar ndyNI2("ndyNI2","ndyNI2",Form("-@0*%f",hdyNI2->Integral()/hdy->Integral()),ndy);
   RooFormulaVar nother0("nother0","nother0",Form("-(ndytautau+ntt+nww+nwz+nzz)*%f",hotherNI0->Integral()/(hdytautau->Integral()+htt->Integral()+hww->Integral()+hwz->Integral()+hzz->Integral())),RooArgList(ndytautau,ntt,nww,nwz,nzz));
   RooFormulaVar nother1("nother1","nother1",Form("-(ndytautau+ntt+nww+nwz+nzz)*%f",hotherNI1->Integral()/(hdytautau->Integral()+htt->Integral()+hww->Integral()+hwz->Integral()+hzz->Integral())),RooArgList(ndytautau,ntt,nww,nwz,nzz));
   RooFormulaVar nother2("nother2","nother2",Form("-(ndytautau+ntt+nww+nwz+nzz)*%f",hotherNI2->Integral()/(hdytautau->Integral()+htt->Integral()+hww->Integral()+hwz->Integral()+hzz->Integral())),RooArgList(ndytautau,ntt,nww,nwz,nzz));

   if (dofullnonisosub) {
      // method 1: implement correctly the floating Ndy in the bkg PDF, at the risk that the PDF can become negative
      // dataSS1
      if (idxbkg1==8 || idxbkg1==11 || idxbkg1==14) { // dxy1*dxy2>0
         pdataSS1 = new RooAddPdf("pdataSS1","HF1",
               RooArgList(pdataSS1_0,pdyNI1,potherNI1),
               RooArgList(RooConst(hdataSS1->Integral()),ndyNI1,nother1));
      } else if (idxbkg1==9 || idxbkg1==12 || idxbkg1==15) { // dxy1*dxy2<=0
         pdataSS1 = new RooAddPdf("pdataSS1","HF1",
               RooArgList(pdataSS1_0,pdyNI2,potherNI2),
               RooArgList(RooConst(hdataSS1->Integral()),ndyNI2,nother2));
      } else if (idxbkg1==10 || idxbkg1==13) { // dxy1*dxy2>0 and <=0
         pdataSS1 = new RooAddPdf("pdataSS1","HF1",
               RooArgList(pdataSS1_0,pdyNI0,potherNI0),
               RooArgList(RooConst(hdataSS1->Integral()),ndyNI0,nother0));
      } else { // no OS data
         pdataSS1 = new RooHistPdf(pdataSS1_0,"pdataSS1");
      }

      // dataSS2
      if (idxbkg1==8 || idxbkg1==11 || idxbkg1==14) { // dxy1*dxy2>0
         pdataSS2 = new RooAddPdf("pdataSS2","HF2",
               RooArgList(pdataSS2_0,pdyNI1,potherNI1),
               RooArgList(RooConst(hdataSS2->Integral()),ndyNI1,nother1));
      } else if (idxbkg1==9 || idxbkg1==12 || idxbkg1==15) { // dxy1*dxy2<=0
         pdataSS2 = new RooAddPdf("pdataSS2","HF2",
               RooArgList(pdataSS2_0,pdyNI2,potherNI2),
               RooArgList(RooConst(hdataSS2->Integral()),ndyNI2,nother2));
      } else if (idxbkg1==10 || idxbkg1==13) { // dxy1*dxy2>0 and <=0
         pdataSS2 = new RooAddPdf("pdataSS2","HF2",
               RooArgList(pdataSS2_0,pdyNI0,potherNI0),
               RooArgList(RooConst(hdataSS2->Integral()),ndyNI0,nother0));
      } else { // no OS data
         pdataSS2 = new RooHistPdf(pdataSS2_0,"pdataSS2");
      }
   } else {
      // method 2: subtract noniso at histogram level
      for (int i=0; i<=hdataSS1->GetNbinsX()+1; i++) {
         double val1 = hdataSS1->GetBinContent(i);
         if (idxbkg1==8 || idxbkg1==11 || idxbkg1==14) { // dxy1*dxy2>0
            val1 += -hdyNI1->GetBinContent(i)-hotherNI1->GetBinContent(i);
         } else if (idxbkg1==9 || idxbkg1==12 || idxbkg1==15) { // dxy1*dxy2<=0
            val1 += -hdyNI2->GetBinContent(i)-hotherNI2->GetBinContent(i);
         } else if (idxbkg1==10 || idxbkg1==13) { // dxy1*dxy2>0 and <=0
            val1 += -hdyNI0->GetBinContent(i)-hotherNI0->GetBinContent(i);
         }
         hdataSS1->SetBinContent(i,max(0.,val1));
      }
      for (int i=0; i<=hdataSS2->GetNbinsX()+1; i++) {
         double val2 = hdataSS2->GetBinContent(i);
         if (idxbkg1==8 || idxbkg1==11 || idxbkg1==14) { // dxy1*dxy2>0
            val2 += -hdyNI1->GetBinContent(i)-hotherNI1->GetBinContent(i);
         } else if (idxbkg1==9 || idxbkg1==12 || idxbkg1==15) { // dxy1*dxy2<=0
            val2 += -hdyNI2->GetBinContent(i)-hotherNI2->GetBinContent(i);
         } else if (idxbkg1==10 || idxbkg1==13) { // dxy1*dxy2>0 and <=0
            val2 += -hdyNI0->GetBinContent(i)-hotherNI0->GetBinContent(i);
         }
         hdataSS2->SetBinContent(i,max(0.,val2));
      }
      rhdataSS1_sub = new RooDataHist("rhdataSS1_sub","HF1",RooArgList(var),hdataSS1);
      pdataSS1 = new RooHistPdf("pdataSS1","HF1",RooArgList(var),*rhdataSS1_sub);
      rhdataSS2_sub = new RooDataHist("rhdataSS2_sub","HF2",RooArgList(var),hdataSS2);
      pdataSS2 = new RooHistPdf("pdataSS2","HF2",RooArgList(var),*rhdataSS2_sub);
   }

   RooAddPdf pdataSS("pdataSS","HF",RooArgSet(*pdataSS1,*pdataSS2),fracSS1);
   
   // make the model
   RooAddPdf model("model","Fit model",RooArgList(pdy,pdytautau,ptt,pww,pwz,pzz,pdataSS),RooArgList(ndy,ndytautau,ntt,nww,nwz,nzz,ndataSS));
   RooAddPdf pvv("pvv","diboson",RooArgList(pww,pwz,pzz),RooArgList(nww,nwz,nzz)); // for plotting

   // // constrain the number of background events according to what we have found
   // RooGaussian fconstraint("fconstraint","fconstraint",ndataSS,RooConst(NbkgC),RooConst(max(40.,sqrt(NbkgC)))) ;
   // RooProdPdf modelc("modelc","model with constraint",RooArgSet(model,fconstraint)) ;

   // do the fit
   // RooFitResult *result = modelc.fitTo(rhdata,Minos(),Extended(true),Save(),Constrain(ndataSS));
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
   double chi2val=0, pvalue=0;
   ndf=hdata->GetNbinsX();
   int d_ndf=result->floatParsFinal().getSize();
   roogof.BCChi2Test(pvalue,chi2val,ndf,d_ndf);
   chi2 = chi2val;

   TLegend* leg = new TLegend(0.16,0.59,0.53,0.92);
   leg->SetBorderSize(0);
   leg->SetFillStyle(4000);
   leg->AddEntry(xframe,Form("%s: %.2f - %.2f",varname,varmin,varmax),"");
   leg->AddEntry(xframe,Form("#chi^{2}/ndf = %.2f/%d (pval = %.1f%s)",chi2val,ndf,pvalue*100.,"%"),"");
   leg->AddEntry(xframe->nameOf(0),Form("%s (%.0f)","Data",rhdata.sumEntries()),"lp");
   leg->AddEntry(xframe->nameOf(1),Form("%s (%.1f)","DYMuMu",ndy.getVal()),"f");
   leg->AddEntry(xframe->nameOf(2),Form("%s (%.1f)","SS Data",ndataSS.getVal()),"f");
   leg->AddEntry(xframe->nameOf(3),Form("%s (%.1f)","DYTauTau",ndytautau.getVal()),"f");
   leg->AddEntry(xframe->nameOf(4),Form("%s (%.1f)","TT",ntt.getVal()),"f");
   leg->AddEntry(xframe->nameOf(5),Form("%s (%.1f)","VV",nww.getVal()+nwz.getVal()+nzz.getVal()),"f");
   leg->AddEntry(xframe->nameOf(6),Form("%s","Fit"),"l");
   leg->Draw();

   c1.SaveAs(Form("fit_%s_%.2f_%.2f.pdf",varname,varmin,varmax));
   c1.SaveAs(Form("fit_%s_%.2f_%.2f.root",varname,varmin,varmax));
   f->Close();

   delete pdataSS1;
   delete pdataSS2;
   delete rhdataSS1_sub;
   delete rhdataSS2_sub;

   return result;
}

void fixhist(TH1D* &hist, bool dozero) {
   // set empty bins to a tiny event content
   if (dozero) {
      for (int i=1; i<=hist->GetNbinsX(); i++) {
         if (hist->GetBinContent(i)<=0) hist->SetBinContent(i,1e-7);
      }
   }

   // now, rebin between -6 and 2
   const int nbins1 = hist->GetNbinsX();
   vector<double> thebins2;
   for (int i=1; i<=nbins1; i++) {
      double xval = hist->GetBinLowEdge(i);
      if (xval<xmin-1e-10) continue;
      if (xval>xmax+1e-10) continue;
      thebins2.push_back(xval);
   }
   // last bin
   double xval = hist->GetBinLowEdge(nbins1) + hist->GetBinWidth(nbins1);
   if (xval>=xmin-1e-10 && xval<=xmax+1e-10) thebins2.push_back(xval);

   // rebinned histo
   const char* histname = hist->GetName();
   int nbins2 = thebins2.size()-1;
   TH1D *hnew = (TH1D*) hist->Rebin(nbins2,"tmp",thebins2.data());
   delete hist;
   hist = hnew;
   hist->SetName(histname);

   // move under- and overflow to the first and last bins
   hist->SetBinContent(1,hist->GetBinContent(0)+hist->GetBinContent(1));
   hist->SetBinError(1,sqrt(pow(hist->GetBinError(0),2)+pow(hist->GetBinError(1),2)));
   hist->SetBinContent(0,0);
   hist->SetBinError(0,0);
   hist->SetBinContent(nbins2,hist->GetBinContent(nbins2)+hist->GetBinContent(nbins2+1));
   hist->SetBinError(nbins2,sqrt(pow(hist->GetBinError(nbins2),2)+pow(hist->GetBinError(nbins2+1),2)));
   hist->SetBinContent(nbins2+1,0);
   hist->SetBinError(nbins2+1,0);
}

void fillvals(RooFitResult *r, map<TString,float> &vals) {
   map<TString,float>::iterator it;

   for (it = vals.begin(); it != vals.end(); it++) {
      // parse the tag to extract the variable name as well as the suffix (_ini, _val, _err)
      TString tag = it->first;
      TString varname, suffix;
      TString tok; Ssiz_t from = 0;
      tag.Tokenize(tok, from, "_");
      varname = tok;
      tag.Tokenize(tok, from, "_");
      suffix = tok;

      // get the value
      if (suffix=="ini") {
         RooRealVar *rvar = (RooRealVar*) r->floatParsInit().find(varname);
         if (rvar) it->second = rvar->getVal();
         else {
            rvar = (RooRealVar*) r->constPars().find(varname);
            if (rvar) it->second = rvar->getVal();
            else it->second = 0;
         }
      } else if (suffix=="val") {
         RooRealVar *rvar = (RooRealVar*) r->floatParsFinal().find(varname);
         if (rvar) it->second = rvar->getVal();
         else {
            rvar = (RooRealVar*) r->constPars().find(varname);
            if (rvar) it->second = rvar->getVal();
            else it->second = 0;
         }
      } else if (suffix=="err") {
         RooRealVar *rvar = (RooRealVar*) r->floatParsFinal().find(varname);
         if (rvar) it->second = rvar->getError();
         else  it->second = 0;
      }
   }
}
