#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TList.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooRealVar.h"
#include "RooPlot.h"

#include "../Include/bin.h"

using namespace RooFit;

RooKeysPdf *kernelpdf(TTree* tr, const char* name, RooRealVar *var,
      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double isocut, double chi2cut);
void fixhist(TH1D *hist);
void fitdxy(const char* datafile, const char* mcfile, 
      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double isocut=1e99, double chi2cut=20) {
   TFile *fdata = TFile::Open(datafile);
   TFile *fmc = TFile::Open(mcfile);

   TH1D *hdata = new TH1D("hdata","Data",80,-4,4);
   TH1D *hdy = new TH1D("hdy","DY",80,-4,4);
   TH1D *hdytautau = new TH1D("hdytautau","DY #tautau",80,-4,4);
   TH1D *htt = new TH1D("htt","ttbar",80,-4,4);
   TH1D *hvv = new TH1D("hvv","Diboson",80,-4,4);
   TH1D *hdataSS1 = new TH1D("hdataSS1","DataSS1",80,-4,4);
   TH1D *hdataSS2 = new TH1D("hdataSS2","DataSS2",80,-4,4);

   // data
   cout << "Filling data histo" << endl;
   TTree *tr = (TTree*) fdata->Get("tr_Data1");
   tr->Draw("log(vtxnormchi2)/log(10)>>+hdata",
         Form("sign==0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&maxrelPFiso<%f&&vtxnormchi2<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,isocut,chi2cut),
         "goff");
   tr = (TTree*) fdata->Get("tr_Data2");
   tr->Draw("log(vtxnormchi2)/log(10)>>+hdata",
         Form("sign==0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&maxrelPFiso<%f&&vtxnormchi2<%f",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,isocut,chi2cut),
         "goff");

   // DY
   cout << "Filling DY histo" << endl;
   tr = (TTree*) fmc->Get("tr_DYMuMu1030");
   tr->Draw("log(vtxnormchi2)/log(10)>>+hdy",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&maxrelPFiso<%f&&vtxnormchi2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,isocut,chi2cut),
         "goff");
   tr = (TTree*) fmc->Get("tr_DYMuMu1030_PbP");
   tr->Draw("log(vtxnormchi2)/log(10)>>+hdy",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&maxrelPFiso<%f&&vtxnormchi2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,isocut,chi2cut),
         "goff");
   tr = (TTree*) fmc->Get("tr_DYMuMu30");
   tr->Draw("log(vtxnormchi2)/log(10)>>+hdy",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&maxrelPFiso<%f&&vtxnormchi2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,isocut,chi2cut),
         "goff");
   tr = (TTree*) fmc->Get("tr_DYMuMu30_PbP");
   tr->Draw("log(vtxnormchi2)/log(10)>>+hdy",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&maxrelPFiso<%f&&vtxnormchi2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,isocut,chi2cut),
         "goff");

   // DYtautau
   cout << "Filling DYtautau histo" << endl;
   tr = (TTree*) fmc->Get("tr_DYTauTau1030");
   tr->Draw("log(vtxnormchi2)/log(10)>>+hdytautau",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&maxrelPFiso<%f&&vtxnormchi2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,isocut,chi2cut),
         "goff");
   tr = (TTree*) fmc->Get("tr_DYTauTau1030");
   tr->Draw("log(vtxnormchi2)/log(10)>>+hdytautau",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&maxrelPFiso<%f&&vtxnormchi2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,isocut,chi2cut),
         "goff");

   // ttbar
   cout << "Filling ttbar histo" << endl;
   tr = (TTree*) fmc->Get("tr_TT");
   tr->Draw("log(vtxnormchi2)/log(10)>>+htt",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&maxrelPFiso<%f&&vtxnormchi2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,isocut,chi2cut),
         "goff");

   // diboson
   cout << "Filling diboson histo" << endl;
   tr = (TTree*) fmc->Get("tr_WW");
   tr->Draw("log(vtxnormchi2)/log(10)>>+hvv",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&maxrelPFiso<%f&&vtxnormchi2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,isocut,chi2cut),
         "goff");
   tr = (TTree*) fmc->Get("tr_WZ");
   tr->Draw("log(vtxnormchi2)/log(10)>>+hvv",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&maxrelPFiso<%f&&vtxnormchi2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,isocut,chi2cut),
         "goff");
   tr = (TTree*) fmc->Get("tr_ZZ");
   tr->Draw("log(vtxnormchi2)/log(10)>>+hvv",
         Form("weight*(sign==0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&maxrelPFiso<%f&&vtxnormchi2<%f)",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,isocut,chi2cut),
         "goff");

   // data SS, same sign dxy
   cout << "Filling data SS histo" << endl;
   tr = (TTree*) fdata->Get("tr_Data1");
   tr->Draw("0.95*log(vtxnormchi2)/log(10)>>+hdataSS1",
         Form("sign!=0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&vtxnormchi2<%f&&dxyVTX1*dxyVTX2/abs(dxyVTX1*dxyVTX2)>0",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,chi2cut),
         "goff");
   tr = (TTree*) fdata->Get("tr_Data2");
   tr->Draw("0.95*log(vtxnormchi2)/log(10)>>+hdataSS1",
         Form("sign!=0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&vtxnormchi2<%f&&dxyVTX1*dxyVTX2/abs(dxyVTX1*dxyVTX2)>0",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,chi2cut),
         "goff");

   // data SS, same sign dxy
   cout << "Filling data SS histo" << endl;
   tr = (TTree*) fdata->Get("tr_Data1");
   tr->Draw("0.95*log(vtxnormchi2)/log(10)>>+hdataSS2",
         Form("sign!=0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&vtxnormchi2<%f&&dxyVTX1*dxyVTX2/abs(dxyVTX1*dxyVTX2)<=0",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,chi2cut),
         "goff");
   tr = (TTree*) fdata->Get("tr_Data2");
   tr->Draw("0.95*log(vtxnormchi2)/log(10)>>+hdataSS2",
         Form("sign!=0&&diMass>%f&&diMass<%f&&diRapidity>%f&&diRapidity<%f&&diPt>%f&&diPt<%f&&vtxnormchi2<%f&&dxyVTX1*dxyVTX2/abs(dxyVTX1*dxyVTX2)<=0",
            massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,chi2cut),
         "goff");


   // -----------------------------------------------
   // Now histos are filled. Move to the fitting part
   // -----------------------------------------------

   // create variables
   RooRealVar logdxy("logdxy","log_{10}(vtx #chi^{2}/ndf)",-4,4,"");
   RooRealVar ndy("ndy","N(DYMuMu)",hdy->Integral(),0,2000*hdy->Integral());
   RooRealVar ndytautau("ndytautau","N(DYTauTau)",hdytautau->Integral(),0.9*hdytautau->Integral(),1.1*hdytautau->Integral()); ndytautau.setConstant(true);
   RooRealVar ntt("ntt","N(TT)",htt->Integral(),0.9*htt->Integral(),1.1*htt->Integral()); ntt.setConstant(true);
   RooRealVar nvv("nvv","N(VV)",hvv->Integral(),0.9*hvv->Integral(),1.1*hvv->Integral()); nvv.setConstant(true);
   RooRealVar ndataSS1("ndataSS1","N(HF)",hdataSS1->Integral(),0,1000*hdataSS1->Integral()); 
   RooRealVar ndataSS2("ndataSS2","N(HF)",hdataSS2->Integral(),0,1000*hdataSS2->Integral()); 

   // convert histos to RooFit objects
   RooDataHist rhdata("rhdata","Data",RooArgList(logdxy),hdata);
   fixhist(hdy);
   RooDataHist rhdy("rhdy","DYMuMu",RooArgList(logdxy),hdy);
   RooHistPdf pdy("pdy","DYMuMu",RooArgList(logdxy),rhdy);
   fixhist(hdytautau);
   RooDataHist rhdytautau("rhdytautau","DYTauTau",RooArgList(logdxy),hdytautau);
   RooHistPdf pdytautau("pdytautau","DYTauTau",RooArgList(logdxy),rhdytautau);
   fixhist(htt);
   RooDataHist rhtt("rhtt","TT",RooArgList(logdxy),htt);
   RooHistPdf ptt("ptt","TT",RooArgList(logdxy),rhtt);
   fixhist(hvv);
   RooDataHist rhvv("rhvv","VV",RooArgList(logdxy),hvv);
   RooHistPdf pvv("pvv","VV",RooArgList(logdxy),rhvv);
   fixhist(hdataSS1);
   RooDataHist rhdataSS1("rhdataSS1","HF",RooArgList(logdxy),hdataSS1);
   RooHistPdf pdataSS1("pdataSS1","HF",RooArgList(logdxy),rhdataSS1);
   fixhist(hdataSS2);
   RooDataHist rhdataSS2("rhdataSS2","HF",RooArgList(logdxy),hdataSS2);
   RooHistPdf pdataSS2("pdataSS2","HF",RooArgList(logdxy),rhdataSS2);

   // // tmp test: fit dataSS
   // RooRealVar ndf1("ndf1","ndf1",0.001,0,1e5);
   // RooRealVar ndf2("ndf2","ndf2",2,0,1e5);
   // RooRealVar ndfshift("ndfshift","ndfshift",0,-1e5,1e5);
   // RooFormulaVar chi2("chi2","exp(log(10)*@0)*@1+@2",RooArgSet(logdxy,ndf1,ndfshift));
   // RooChiSquarePdf chi2pdf("chi2pdf","chi2pdf",chi2,ndf2);
   // chi2pdf.fitTo(rhdataSS);
   // RooPlot* chi2frame = logdxy.frame(Title("Fit to logdxy")) ;
   // rhdataSS.plotOn(chi2frame);
   // chi2pdf.plotOn(chi2frame);
   // chi2frame->Draw();
   // return;

   // make a rookeys pdf
   TChain *dytrees = new TChain("tr_DYMuMu");
   dytrees->AddFile(mcfile,TTree::kMaxEntries,"tr_DYMuMu1030");
   dytrees->AddFile(mcfile,TTree::kMaxEntries,"tr_DYMuMu30");
   dytrees->AddFile(mcfile,TTree::kMaxEntries,"tr_DYMuMu1030_PbP");
   dytrees->AddFile(mcfile,TTree::kMaxEntries,"tr_DYMuMu30_PbP");
   // RooKeysPdf *rkpdy = kernelpdf(dytrees,"trdy_all",&logdxy,massbin1,massbin2,rapbin1,rapbin2,ptbin1,ptbin2,isocut,chi2cut);


   // // smear
   // RooRealVar mg("mg","mg",0,-1,1) ;
   // RooRealVar sg("sg","sg",0.1,1e-6,1) ;
   // RooGaussian gauss("gauss","gauss",logdxy,mg,sg) ;
   // logdxy.setBins(10000,"cache") ;
   // RooFFTConvPdf pdy_smear("pdy_smear","Smeared DYMuMu",logdxy,pdy,gauss) ;

   // make the model
   RooAddPdf model("model","Fit model",RooArgList(pdy,pdytautau,ptt,pvv,pdataSS1,pdataSS2),RooArgList(ndy,ndytautau,ntt,nvv,ndataSS1,ndataSS2));
   // RooAddPdf model("model","Fit model",RooArgList(pdy,pdytautau,ptt,pvv,pdataSS1),RooArgList(ndy,ndytautau,ntt,nvv,ndataSS1));

   // do the fit
   model.fitTo(rhdata,Minos(),Extended(true));

   // draw the results
   TCanvas *c1 = new TCanvas();
   RooPlot* xframe = logdxy.frame(Title("Fit to logdxy")) ;
   rhdata.plotOn(xframe);
   // model.plotOn(xframe,Components(RooArgSet(pdataSS,pvv,ptt,pdytautau)),LineColor(kGreen+1),FillColor(kGreen+1),DrawOption("F"));
   // model.plotOn(xframe,Components(RooArgSet(pdataSS,pvv,ptt)),LineColor(kRed+3),FillColor(kRed+3),DrawOption("F"));
   // model.plotOn(xframe,Components(RooArgSet(pdataSS,pvv)),LineColor(kRed+1),FillColor(kRed+1),DrawOption("F"));
   model.plotOn(xframe,Components(RooArgSet(pdataSS1,pdataSS2)),LineColor(kViolet),FillColor(kViolet),DrawOption("F"));
   model.plotOn(xframe,LineColor(kBlack));
   model.plotOn(xframe,Components(pdy),LineColor(kRed));
   rhdata.plotOn(xframe);
   xframe->Draw();

}

RooKeysPdf *kernelpdf(TTree* tr, const char* name, RooRealVar *var, double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double isocut, double chi2cut) {
   int sign; tr->SetBranchAddress("sign",&sign);
   float vtxnormchi2; tr->SetBranchAddress("vtxnormchi2",&vtxnormchi2);
   float diMass; tr->SetBranchAddress("diMass",&diMass);
   float diRapidity; tr->SetBranchAddress("diRapidity",&diRapidity);
   float diPt; tr->SetBranchAddress("diPt",&diPt);
   float maxrelPFiso; tr->SetBranchAddress("maxrelPFiso",&maxrelPFiso);
   // float vtxnormchi2; tr->SetBranchAddress("vtxnormchi2",&vtxnormchi2);
   float weight; tr->SetBranchAddress("weight",&weight);
   tr->SetBranchStatus("*",0);
   tr->SetBranchStatus("sign",1);
   tr->SetBranchStatus("vtxnormchi2",1);
   tr->SetBranchStatus("di*",1);
   tr->SetBranchStatus("maxrelPFiso",1);
   tr->SetBranchStatus("vtxnormchi2",1);
   tr->SetBranchStatus("weight",1);


   RooRealVar rweight("weight","weight",0,1e6);
   RooArgSet cols(*var,rweight);
   RooDataSet ds("ds","ds",cols);

   for (int i=0; i<tr->GetEntries(); i++) {
      tr->GetEntry(i);
      if (diMass<massbin1 || diMass>massbin2) continue;
      if (diRapidity<rapbin1 || diRapidity>rapbin2) continue;
      if (diPt<ptbin1 || diPt>ptbin2) continue;
      if (log(vtxnormchi2)/log(10)>isocut) continue;
      if (vtxnormchi2>chi2cut) continue;
      rweight.setVal(weight);
      var->setVal(log(vtxnormchi2)/log(10));
      ds.add(cols,weight);
   }

   return new RooKeysPdf(name,name,*var,ds,RooKeysPdf::NoMirror,1) ;
}

void fixhist(TH1D *hist) {
   for (int i=1; i<=hist->GetNbinsX(); i++) {
      if (hist->GetBinContent(i)<=0) hist->SetBinContent(i,1e-6);
   }
}
