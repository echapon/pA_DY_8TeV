#include "../BkgEst/interface/defs.h"
#include "Plots/TheoryTools.h"

void Obtain_dSigma_dM(TH1D *h){
	Int_t nBins = h->GetNbinsX();
	for(Int_t i=0; i<nBins; i++)
	{
		Int_t i_bin = i+1;
		Double_t BinWidth = h->GetBinWidth(i_bin);

		Double_t low = h->GetBinLowEdge(i_bin);
		Double_t high = h->GetBinLowEdge(i_bin + 1);

		Double_t xSec = h->GetBinContent(i_bin);
		Double_t xSec_dM = xSec / BinWidth;

		Double_t error_before = h->GetBinError(i_bin);
		Double_t error_after = error_before / BinWidth;

		h->SetBinContent(i_bin, xSec_dM);
		h->SetBinError(i_bin, error_after);

		// printf("%2dth bin [%5.lf, %5.lf] (xSec, BinWidth, dSigma/dM) = (%15.9lf, %6.1lf, %15.9lf), (error_before, error_after) = (%8.5lf, %8.5lf)\n", 
			// i_bin, low, high, xSec, BinWidth, xSec_dM, error_before, error_after );
	}
}

void myXsec() {
   TFile *fy = TFile::Open("ROOTFile_YieldHistogram.root");
   TFile *fae = TFile::Open("ROOTFile_AccEff.root");

   TH1D *hy = (TH1D*) fy->Get("h_yield_OS_MCBasedBkg1");
   TGraph *gae = (TGraph*) fae->Get("g_AccEff_Corr_tnp");

   for (int i=0; i<gae->GetN(); i++) {
      hy->SetBinContent(i+1,hy->GetBinContent(i+1)/gae->GetY()[i]);
      hy->SetBinError(i+1,hy->GetBinError(i+1)/gae->GetY()[i]);
   }
   hy->Scale(1./lumi_all);
   Obtain_dSigma_dM(hy);
   hy->GetXaxis()->SetTitle("M [GeV/c^{2}]");
   hy->GetYaxis()->SetTitle("d#sigma/dm [nb/GeV/c^{2}]");

   TCanvas* c1 = new TCanvas();
   c1->SetLogx();
   c1->SetLogy();
   hy->Draw();

   // temporary dummy theory
   TFile *fth = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/MCFM/MCFM-8.0/Bin/DYpA8TeV/CT14_eigenvectors/Z_only_nlo_CT14nlo_90___90___DY15600_CT14nlo_CT14nlo_7_7.root");
   TH1F *hth1 = (TH1F*) fth->Get("id1");
   TH1F *hth19 = (TH1F*) fth->Get("id19");
   TH1F *hth20 = (TH1F*) fth->Get("id20");
   TH1F *hth = rebin_theory(hth1,hth19,hth20);
   hth->Scale(208*1e-3);
   hth->SetLineColor(kRed);
   hth->Draw("same HIST");
}
