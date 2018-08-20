#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TList.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooRealVar.h"
#include "RooKeysPdf.h"
#include "RooDataSet.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "TH2.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TChain.h"

#include "../BkgEst/interface/defs.h"
#include "../Include/bin.h"

using namespace RooFit;
using namespace DYana;

RooKeysPdf *kernelpdf(TTree* tr, const char* name, RooRealVar *var,
      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double isocut, double chi2cut);
void fixhist(TH1D *hist);
void fitdxy(const char* datafile, const char* mcfile, 
//void fitdxy_hckim(const char* datafile="smalltrees/ROOTFile_Histogram_InvMass_PAL1DoubleMu0_Data_MomCorr00_noHFrew_notnprew.root", const char    * mcfile="smalltrees/ROOTFile_Histogram_InvMass_PAL1DoubleMu0_Powheg_MomCorr00_rewboth_tnprew.root",

      double massbin1, double massbin2,
      double rapbin1, double rapbin2,
      double ptbin1, double ptbin2,
      double isocut=1e99, double chi2cut=20) {
/*
double massbin1 = 60.0,//bins[0],
double massbin2 = 76.0,//bins[0+1],
double rapbin1 = rapbin_60120[0],
double rapbin2 =  rapbin_60120[rapbinnum_60120],
double ptbin1 = -1,
double ptbin2 =  1e99,
double isocut =  1e99,
double chi2cut=4000) {
*/
	for (int i=0;i<binnum;i++) {
	massbin1 = bins[i];
	massbin2 = bins[i+1];
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
   tr = (TTree*) fmc->Get("tr_DYTauTau30");
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
   TCanvas *c1 = new TCanvas("c1","",800,800);
	c1->SetLogy(1);
   RooPlot* xframe = logdxy.frame(Title("Fit to logdxy")) ;
   //xframe->SetAxisRange(0.01,1000,"Y");	
   rhdata.plotOn(xframe);
   // model.plotOn(xframe,Components(RooArgSet(pdataSS,pvv,ptt,pdytautau)),LineColor(kGreen+1),FillColor(kGreen+1),DrawOption("F"));
   // model.plotOn(xframe,Components(RooArgSet(pdataSS,pvv,ptt)),LineColor(kRed+3),FillColor(kRed+3),DrawOption("F"));
   // model.plotOn(xframe,Components(RooArgSet(pdataSS,pvv)),LineColor(kRed+1),FillColor(kRed+1),DrawOption("F"));
   model.plotOn(xframe,Components(RooArgSet(pdataSS1,pdataSS2)),LineColor(kViolet),FillColor(kViolet),DrawOption("F"));
   model.plotOn(xframe,LineColor(kBlack));
   model.plotOn(xframe,Components(pdy),LineColor(kRed));
   model.plotOn(xframe,Components(pdytautau),LineColor(kOrange));
   model.plotOn(xframe,Components(ptt),LineColor(kGreen+2));
   model.plotOn(xframe,Components(pvv),LineColor(kBlue));
   rhdata.plotOn(xframe);
   xframe->Draw();

	TLegend* leg = new TLegend(0.7,0.65,0.95,0.90);
	leg->SetFillColor(kWhite);
	leg->SetLineColor(kWhite);
	leg->AddEntry(xframe->nameOf(0),Form("%s",xframe->nameOf(0)),"lp");
	leg->AddEntry(xframe->nameOf(1),Form("%s",xframe->nameOf(1)),"lf");
	leg->AddEntry(xframe->nameOf(2),Form("%s",xframe->nameOf(2)),"l");//what?
	leg->AddEntry(xframe->nameOf(3),Form("%s",xframe->nameOf(3)),"l");
	leg->Draw();

	//c1->SaveAs(".pdf");

	gStyle->SetOptStat(0);
   TH2D *frame2 = new TH2D("frame2","Fit to logdxy;log_{10}(vtx #chi^{2}/ndf);Entries",100,-4,4,100,0.002,50000);
	frame2->Draw("");
   RooPlot* xframe2 = logdxy.frame(Title("Fit to logdxy")) ;
//	xframe2->SetMinimum(0.005);
//	xframe2->SetMaximum(1000);
//	xframe2->SetAxisRange(0.005,1000,"y");
   rhdata.plotOn(xframe2);
   model.plotOn(xframe2,Components(RooArgSet(pvv,ptt,pdytautau,pdy,pdataSS1,pdataSS2)),FillColor(kViolet),LineColor(kViolet),DrawOption("F"));
   model.plotOn(xframe2,Components(RooArgSet(pvv,ptt,pdytautau,pdy)),FillColor(kRed),LineColor(kRed),DrawOption("F"));
   model.plotOn(xframe2,Components(RooArgSet(pvv,ptt,pdytautau)),FillColor(kOrange),LineColor(kOrange),DrawOption("F"));
   model.plotOn(xframe2,Components(RooArgSet(pvv,ptt)),FillColor(kGreen+2),LineColor(kGreen+2),DrawOption("F"));
   model.plotOn(xframe2,Components(RooArgSet(pvv)),FillColor(kBlue),LineColor(kBlue),DrawOption("F"));
   model.plotOn(xframe2,LineColor(kBlack));
   rhdata.plotOn(xframe2);
   xframe2->Draw("axissame");

	TLegend* leg2 = new TLegend(0.13,0.60,0.35,0.88);
	leg2->SetFillColor(kWhite);
	leg2->SetFillStyle(4000);//window is transparent
	leg2->SetLineColor(kWhite);
	leg2->SetLineWidth(0);
	leg2->AddEntry(frame2,Form("mass : %.1f - %.1f",bins[i],bins[i+1]),"");
	leg2->AddEntry(xframe2->nameOf(0),Form("%s","Data"),"lp");
	leg2->AddEntry(xframe2->nameOf(1),Form("%s","SS Data"),"f");
	leg2->AddEntry(xframe2->nameOf(2),Form("%s","DYMuMu"),"f");
	leg2->AddEntry(xframe2->nameOf(3),Form("%s","DYTauTau"),"f");
	leg2->AddEntry(xframe2->nameOf(4),Form("%s","TT"),"f");
	leg2->AddEntry(xframe2->nameOf(5),Form("%s","VV"),"f");
	leg2->AddEntry(xframe2->nameOf(6),Form("%s","Fit"),"l");

	leg2->Draw();

	c1->SaveAs(Form("dxyfit_mass_%.1f_%.1f.pdf",bins[i],bins[i+1]));

}
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
