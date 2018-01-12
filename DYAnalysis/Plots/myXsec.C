#include "../BkgEst/interface/defs.h"
#include "Plots/TheoryTools.h"
#include "Include/bin.h"
#include "Plots/Systematics/syst.h"

using namespace DYana;

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

void myXsec(const char* datafile="ROOTFile_YieldHistogram.root", const char* accefffile="ROOTFile_AccEff.root", const char* outputfile="result.root", bool dosyst=true) {
   TFile *fy = TFile::Open(datafile);
   TFile *fae = TFile::Open(accefffile);

   TFile *fout = TFile::Open(outputfile,"RECREATE");


   // loop on the variables
   for (int thevar=0; thevar<ALLvar; thevar++) {
      // the systs
      map<bin,syst> thesyst = readSyst_all(thevar);

      TH1D *hy = (TH1D*) fy->Get("h_yield_OS_MCBasedBkg1");
      // TGraph *gae = (TGraph*) fae->Get("g_AccEff_Corr_tnp");
      // get acc and eff and multiply them in the loop
      TEfficiency *TEff_Acc = (TEfficiency*)f_input->Get(Form("TEff_Acc_%s",Variablename(thevar)));
      TGraphAsymmErrors *ga = (TGraphAsymmErrors*)TEff_Acc->CreateGraph()->Clone();

      TEfficiency *TEff_Eff = (TEfficiency*)f_input->Get(Form("TEff_Eff_%s",Variablename(thevar)));
      TGraphAsymmErrors *ge = (TGraphAsymmErrors*)TEff_Eff->CreateGraph()->Clone();

      for (int i=0; i<ga->GetN(); i++) {
         bin thebin;
         thebin.first = hy->GetBinLowEdge(i);
         thebin.second = thebin.first + hy->GetBinWidth(i);
         hy->SetBinContent(i+1,hy->GetBinContent(i+1)/(ga>GetY()[i]*ge>GetY()[i]));
         double err;
         if (dosyst) {
            err = sqrt(
                  pow(hy->GetBinError(i+1)/hy->GetBinContent(i+1),2) // stat
                  + pow(ga->GetEY()[i]/ga->GetY()[i],2) // acceptance
                  + pow(ge->GetEY()[i]/ge->GetY()[i],2) // efficiency
                  + pow(thesyst[thebin],2) // other systs
                  );
            err = err * hy->GetBinContent(i+1)/(ga>GetY()[i]*ge>GetY()[i]);
         } else {
            err = hy->GetBinError(i+1)/(ga>GetY()[i]*ge>GetY()[i])
         }
         hy->SetBinError(i+1,err);
      }
      hy->Scale(1./lumi_all);
      Obtain_dSigma_dX(hy);
      if (thevar==mass) {
         hy->GetXaxis()->SetTitle("M [GeV/c^{2}]");
         hy->GetYaxis()->SetTitle("d#sigma/dM [nb/GeV/c^{2}]");
      } else if (thevar==pt) {
         hy->GetXaxis()->SetTitle("p_{T} [GeV/c]");
         hy->GetYaxis()->SetTitle("d#sigma/dp_{T} [nb/GeV/c]");
      } else if (thevar==phistar) {
         hy->GetXaxis()->SetTitle("#phi^{*}");
         hy->GetYaxis()->SetTitle("d#sigma/d#phi^{*} [nb]");
      } else if (thevar==rap60120) {
         hy->GetXaxis()->SetTitle("y_{CM}");
         hy->GetYaxis()->SetTitle("d#sigma/dy [nb]");
      } else if (thevar==rap1560) {
         hy->GetXaxis()->SetTitle("y_{CM}");
         hy->GetYaxis()->SetTitle("d#sigma/dy [nb]");
      }

      TCanvas* c1 = new TCanvas();
      c1->SetLogx();
      c1->SetLogy();
      hy->Draw();

      // // temporary dummy theory
      // TFile *fth = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/MCFM/MCFM-8.0/Bin/DYpA8TeV/CT14_eigenvectors/Z_only_nlo_CT14nlo_90___90___DY15600_CT14nlo_CT14nlo_7_7.root");
      // TH1F *hth1 = (TH1F*) fth->Get("id1");
      // TH1F *hth19 = (TH1F*) fth->Get("id19");
      // TH1F *hth20 = (TH1F*) fth->Get("id20");
      // TH1F *hth = rebin_theory(hth1,hth19,hth20);
      // hth->Scale(208*1e-3);

      TFile *fth = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_0_norew.root");
      TH1F *hth = (TH1F*) hth->Get(Form("h_%s_AccTotal",varname(thevar)));
      hth->Scale((Xsec(DYMuMu1030)+Xsec(DYMuMu30))/(Nevts(DYMuMu1030)+Nevts(DYMuMu30)));
      Obtain_dSigma_dX(hth);
      hth->SetLineColor(kRed);
      hth->Draw("same HIST");
   }
}
