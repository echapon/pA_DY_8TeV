#include <iostream>
#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFrame.h>

#include "Include/tdrstyle.C"
#include "Include/CMS_lumi.C"
#include "Include/MyCanvas.C"
#include "Include/lhapdf_utils.h"

const int rfb_rapbinnum_1560 = 5;
const double rfb_rapbin_1560[rfb_rapbinnum_1560+1] = {0.0,0.4,0.8,1.2,1.6,1.93};
const int rfb_rapbinnum_60120 = 10;
const double rfb_rapbin_60120[rfb_rapbinnum_60120+1] = {0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,1.93};

TGraphAsymmErrors *RFB_1560(TGraphAsymmErrors *g);
TGraphAsymmErrors *RFB_60120(TGraphAsymmErrors *g);
TH1D *RFB_1560(TH1D *h);
TH1D *RFB_60120(TH1D *h);
void Obtain_dSigma_dX(TH1D *h);
TGraphAsymmErrors *h2g(TH1D* h);
TH1D *g2h(TGraphAsymmErrors *g);

using namespace std;

void CompRFB (const char* infile="Plots/results/xsec_nom.root") {
   TFile* fin = TFile::Open(infile);

   TGraphAsymmErrors* gaeres_rap1560 = (TGraphAsymmErrors*)fin->Get("gres_rap1560");
   TGraphAsymmErrors* gaeres_rap60120 = (TGraphAsymmErrors*)fin->Get("gres_rap60120");
   TGraphAsymmErrors* gaeth_rap1560 = (TGraphAsymmErrors*)fin->Get("gth_rap1560");
   TGraphAsymmErrors* gaeth_rap60120 = (TGraphAsymmErrors*)fin->Get("gth_rap60120");

   // data: dirty F/B ratio... will account for F/B correlations later
   TGraphAsymmErrors *grfbp_rap1560 = RFB_1560(gaeres_rap1560);
   TGraphAsymmErrors *grfbp_rap60120 = RFB_60120(gaeres_rap60120);

   // for theory, need to account for correlations in the nPDF uncertainties, since we know how to do it
   // EPPS16
   TFile *fth_EPPS16 = TFile::Open("/afs/cern.ch/work/e/echapon/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16.root");
   vector<TH1D*> hth1560_EPPS16,hth60120_EPPS16;
   int i=0;
   hth1560_EPPS16.push_back(RFB_1560((TH1D*) fth_EPPS16->Get(Form("h_rap1560_AccTotal_pre%d",i))));
   hth60120_EPPS16.push_back(RFB_60120((TH1D*) fth_EPPS16->Get(Form("h_rap60120_AccTotal_pre%d",i))));
   for (i=285; i<=324; i++) {
      hth1560_EPPS16.push_back(RFB_1560((TH1D*) fth_EPPS16->Get(Form("h_rap1560_AccTotal_pre%d",i))));
      hth60120_EPPS16.push_back(RFB_60120((TH1D*) fth_EPPS16->Get(Form("h_rap60120_AccTotal_pre%d",i))));
   }
   for (i=112; i<=167; i++) {
      hth1560_EPPS16.push_back(RFB_1560((TH1D*) fth_EPPS16->Get(Form("h_rap1560_AccTotal_pre%d",i))));
      hth60120_EPPS16.push_back(RFB_60120((TH1D*) fth_EPPS16->Get(Form("h_rap60120_AccTotal_pre%d",i))));
   }

   TGraphAsymmErrors *gth1560_EPPS16 = pdfuncert(hth1560_EPPS16, "EPPS16nlo_CT14nlo_Pb208");
   TGraphAsymmErrors *gth60120_EPPS16 = pdfuncert(hth60120_EPPS16, "EPPS16nlo_CT14nlo_Pb208");
   gth1560_EPPS16->SetMarkerSize(0);
   gth60120_EPPS16->SetMarkerSize(0);
   gth1560_EPPS16->SetName("gth1560_EPPS16");
   gth60120_EPPS16->SetName("gth60120_EPPS16");

   // CT14
   TFile *fth_CT14 = TFile::Open("/afs/cern.ch/work/e/echapon/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_CT14.root");
   vector<TH1D*> hth1560_CT14,hth60120_CT14;
   i=0;
   hth1560_CT14.push_back(RFB_1560((TH1D*) fth_CT14->Get(Form("h_rap1560_AccTotal_pre%d",i))));
   hth60120_CT14.push_back(RFB_60120((TH1D*) fth_CT14->Get(Form("h_rap60120_AccTotal_pre%d",i))));
   for (i=112; i<=167; i++) {
      hth1560_CT14.push_back(RFB_1560((TH1D*) fth_CT14->Get(Form("h_rap1560_AccTotal_pre%d",i))));
      hth60120_CT14.push_back(RFB_60120((TH1D*) fth_CT14->Get(Form("h_rap60120_AccTotal_pre%d",i))));
   }

   TGraphAsymmErrors *gth1560_CT14 = pdfuncert(hth1560_CT14, "CT14nlo");
   TGraphAsymmErrors *gth60120_CT14 = pdfuncert(hth60120_CT14, "CT14nlo");
   gth1560_CT14->SetMarkerSize(0);
   gth60120_CT14->SetMarkerSize(0);
   gth1560_CT14->SetName("gth1560_CT14");
   gth60120_CT14->SetName("gth60120_CT14");


   // do the plotting here

   MyCanvas c_1560("Plots/grfbp_rap1560","|y|","R_{FB}",800,800);
   c_1560.SetYRange(0.45,1.85);
   c_1560.CanvasWithThreeGraphsRatioPlot(gth1560_CT14,gth1560_EPPS16,grfbp_rap1560,
         "Powheg (CT14)","Powheg (EPPS16)","Data","Powheg/Data",
         kBlue,kRed,kBlack,
         "5","5","EP");
   c_1560.PrintCanvas();

   MyCanvas c_60120("Plots/grfbp_rap60120","|y|","R_{FB}",800,800);
   c_60120.SetYRange(0.65,1.35);
   c_60120.CanvasWithThreeGraphsRatioPlot(gth60120_CT14,gth60120_EPPS16,grfbp_rap60120,
         "Powheg (CT14)","Powheg (EPPS16)","Data","Powheg/Data",
         kBlue,kRed,kBlack,
         "5","5","EP");
   c_60120.PrintCanvas();
}

TGraphAsymmErrors *RFB_1560(TGraphAsymmErrors *g) {
   TH1D* f_rap1560_temp = new TH1D("f_rap1560_temp",";|y_{CM}| (15<M<60 (GeV/c^{2}));Forward/Backward (Xsec)",rfb_rapbinnum_1560,rfb_rapbin_1560);
   TH1D* b_rap1560_temp = new TH1D("b_rap1560_temp",";|y_{CM}| (15<M<60 (GeV/c^{2}));Forward/Backward (Xsec)",rfb_rapbinnum_1560,rfb_rapbin_1560);

   f_rap1560_temp->Sumw2();
   b_rap1560_temp->Sumw2();

   for (int i=2;i<2+rfb_rapbinnum_1560;i++) {
      // std::cout << "|y|: " << g->GetX()[13-i] << " / " << g->GetX()[i] << std::endl;
      if (g->GetY()[13-i]<=0) {f_rap1560_temp->SetBinContent(7-i,0.0);f_rap1560_temp->SetBinError(7-i,0.0);}
      else {f_rap1560_temp->SetBinContent(7-i,g->GetY()[13-i]);f_rap1560_temp->SetBinError(7-i,g->GetErrorY(13-i));}
      if (g->GetY()[i]<=0) {b_rap1560_temp->SetBinContent(7-i,0.0);b_rap1560_temp->SetBinError(7-i,0.0);}
      else {b_rap1560_temp->SetBinContent(7-i,g->GetY()[i]);b_rap1560_temp->SetBinError(7-i,g->GetErrorY(i));}
   }
   return new TGraphAsymmErrors(f_rap1560_temp,b_rap1560_temp,"pois n");
}

TGraphAsymmErrors *RFB_60120(TGraphAsymmErrors *g) {
   TH1D* f_rap60120_temp = new TH1D("f_rap60120_temp",";|y_{CM}| (60<M<120 (GeV/c^{2}));Forward/Backward (Xsec)",rfb_rapbinnum_60120,rfb_rapbin_60120);
   TH1D* b_rap60120_temp = new TH1D("b_rap60120_temp",";|y_{CM}| (60<M<120 (GeV/c^{2}));Forward/Backward (Xsec)",rfb_rapbinnum_60120,rfb_rapbin_60120);

   f_rap60120_temp->Sumw2();
   b_rap60120_temp->Sumw2();

   for (int i=4;i<4+rfb_rapbinnum_60120;i++) {
      // std::cout << "|y|: " << g->GetX()[27-i] << " / " << g->GetX()[i] << std::endl;
      if (g->GetY()[27-i]<=0) {f_rap60120_temp->SetBinContent(14-i,0.0);f_rap60120_temp->SetBinError(14-i,0.0);}
      else {f_rap60120_temp->SetBinContent(14-i,g->GetY()[27-i]);f_rap60120_temp->SetBinError(14-i,g->GetErrorY(27-i));}
      if (g->GetY()[i]<=0) {b_rap60120_temp->SetBinContent(14-i,0.0);b_rap60120_temp->SetBinError(14-i,0.0);}
      else {b_rap60120_temp->SetBinContent(14-i,g->GetY()[i]);b_rap60120_temp->SetBinError(14-i,g->GetErrorY(i));}
   }
   return new TGraphAsymmErrors(f_rap60120_temp,b_rap60120_temp,"pois n");
}

TH1D *RFB_1560(TH1D *h) {
   TGraphAsymmErrors *g = h2g(h);
   return g2h(RFB_1560(g));
}

TH1D *RFB_60120(TH1D *h) {
   TGraphAsymmErrors *g = h2g(h);
   return g2h(RFB_60120(g));
}

void Obtain_dSigma_dX(TH1D *h){
	Int_t nBins = h->GetNbinsX();
	for(Int_t i=0; i<nBins; i++)
	{
		Int_t i_bin = i+1;
		Double_t BinWidth = h->GetBinWidth(i_bin);

		Double_t low = h->GetBinLowEdge(i_bin);
		Double_t high = h->GetBinLowEdge(i_bin + 1);

		Double_t xSec = h->GetBinContent(i_bin);
		Double_t xSec_dX = xSec / BinWidth;

		Double_t error_before = h->GetBinError(i_bin);
		Double_t error_after = error_before / BinWidth;

		h->SetBinContent(i_bin, xSec_dX);
		h->SetBinError(i_bin, error_after);

		// printf("%2dth bin [%5.lf, %5.lf] (xSec, BinWidth, dSigma/dM) = (%15.9lf, %6.1lf, %15.9lf), (error_before, error_after) = (%8.5lf, %8.5lf)\n", 
			// i_bin, low, high, xSec, BinWidth, xSec_dM, error_before, error_after );
	}
}

TGraphAsymmErrors *h2g(TH1D* h) {
   const int nbins = h->GetNbinsX();
   TGraphAsymmErrors *g = new TGraphAsymmErrors(nbins);
   for (int i=0; i<nbins; i++) {
      double x = h->GetBinCenter(i+1);
      double y = h->GetBinContent(i+1);
      double ex = h->GetBinWidth(i+1)/2.;
      double ey = h->GetBinError(i+1);
      g->SetPoint(i,x,y);
      g->SetPointError(i,ex,ex,ey,ey);
   }
   return g;
}

TH1D *g2h(TGraphAsymmErrors *g) {
   const int nbins = g->GetN();
   double bins[nbins+1];

   for (int i=0; i<nbins; i++) bins[i] = g->GetX()[i]-g->GetEXlow()[i];
   bins[nbins] = g->GetX()[nbins-1]+g->GetEXhigh()[nbins-1];

   TH1D *hans = new TH1D("hans","",nbins,bins);
   for (int i=0; i<nbins; i++) {
      hans->SetBinContent(i+1,g->GetY()[i]);
      hans->SetBinError(i+1,g->GetErrorY(i));
   }

   return hans;
}
