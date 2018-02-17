#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TEfficiency.h"

#include <iostream>

#include "../BkgEst/interface/defs.h"
#include "TheoryTools.h"
#include "../Include/bin.h"
#include "../SysUncEstimation/syst.C"
#include "../Include/MyCanvas.C"
#include "../Include/texUtils.h"
#include "../Include/lhapdf_utils.h"

#define DIRTYPREFSR

using namespace DYana;
using namespace std;

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

TGraphAsymmErrors *h2gae(TH1 *h) {
   const int n = h->GetNbinsX();
   double *x = new double[n];
   double *y = new double[n];
   double *exl = new double[n];
   double *exh = new double[n];
   double *eyl = new double[n];
   double *eyh = new double[n];

   for (int i=0; i<n; i++) {
      x[i] = h->GetBinCenter(i+1);
      y[i] = h->GetBinContent(i+1);
      exl[i] = h->GetBinWidth(i+1)/2.;
      exh[i] = exl[i];
      eyl[i] = h->GetBinError(i+1);
      eyh[i] = eyl[i];
   }

   TGraphAsymmErrors *ans = new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);

   delete[] x; delete[] y; delete[] exl; delete[] exh; delete[] eyl; delete[] eyh;
   return ans;
}

void myXsec(const char* datafile="ROOTFile_YieldHistogram.root", // data and bkg histos
      const char* accefffile="ROOTFile_AccEff.root",             // acceptance and efficiency
      const char* outputfile="Plots/results/xsec.root",          // where to write the output xsec
      bool forsyst=false,                                        // if true, don't print canvases and tables
      bool doxsec=true) {                                        // if false, don't do dxsec/dxxx, just correct for acc eff
   TFile *fy = TFile::Open(datafile);
   TFile *fae = TFile::Open(accefffile);

   TFile *fout = TFile::Open(outputfile,"RECREATE");


   // loop on the variables
   for (int ivar=0; ivar<ALLvar; ivar++) {
   // for (int ivar=0; ivar<pt; ivar++) { // only mass for now
      var thevar = static_cast<var>(ivar);
      // the systs
      map<bin,syst> thesyst = readSyst_all(thevar,false,"","./");

      TH1D *hy = (TH1D*) fy->Get(Form("h_%s_bkgsub_DataDrivenBkg_All1",varname(thevar)));
      TH1D *hy_statonly;
      TGraphAsymmErrors* gres = NULL;
      TGraphAsymmErrors* gres_statonly = NULL;
      if (hy) {
         if (doxsec) {
            hy->Scale(1./lumi_all);
            Obtain_dSigma_dX(hy);
         }
         hy->SetName(Form("hy_%s",varname(thevar)));

         hy_statonly = (TH1D*) hy->Clone(Form("hy_statonly_%s",varname(thevar)));

         // TGraph *gae = (TGraph*) fae->Get("g_AccEff_Corr_tnp");
         // get acc and eff and multiply them in the loop
         TEfficiency *TEff_Acc = (TEfficiency*)fae->Get(Form("TEff_Acc_%s",Varname(thevar)));
         TGraphAsymmErrors *ga = (TGraphAsymmErrors*)TEff_Acc->CreateGraph()->Clone();

         TEfficiency *TEff_Eff = (TEfficiency*)fae->Get(Form("TEff_Eff_%s",Varname(thevar)));
         TGraphAsymmErrors *ge = (TGraphAsymmErrors*)TEff_Eff->CreateGraph()->Clone();

         gres = (TGraphAsymmErrors*) ga->Clone(Form("gres_%s",varname(thevar)));
         gres_statonly = (TGraphAsymmErrors*) ga->Clone(Form("gres_statonly_%s",varname(thevar)));

         for (int i=0; i<ga->GetN(); i++) {
            bin thebin;
            thebin.first = hy->GetBinLowEdge(i+1);
            thebin.second = thebin.first + hy->GetBinWidth(i+1);
            double val = hy->GetBinContent(i+1)/(ga->GetY()[i]*ge->GetY()[i]);
            double errl,errh;
            double errl_stat,errh_stat;
            // cout << hy->GetBinError(i+1)/hy->GetBinContent(i+1) << " ";
            // cout << ga->GetEYlow()[i]/ga->GetY()[i] << " ";
            // cout << ge->GetEYlow()[i]/ge->GetY()[i] << " ";
            // cout << thesyst[thebin].value << endl;
            errl = sqrt(
                  pow(hy->GetBinError(i+1)/hy->GetBinContent(i+1),2) // stat
                  // + pow(ga->GetEYlow()[i]/ga->GetY()[i],2) // acceptance
                  // + pow(ge->GetEYlow()[i]/ge->GetY()[i],2) // efficiency
                  + pow(thesyst[thebin].value,2) // other systs
                  );
            errl = errl * val;
            errh = sqrt(
                  pow(hy->GetBinError(i+1)/hy->GetBinContent(i+1),2) // stat
                  // + pow(ga->GetEYhigh()[i]/ga->GetY()[i],2) // acceptance
                  // + pow(ge->GetEYhigh()[i]/ge->GetY()[i],2) // efficiency
                  + pow(thesyst[thebin].value,2) // other systs
                  );
            errh = errh * val;
            errl_stat = hy->GetBinError(i+1)/(ga->GetY()[i]*ge->GetY()[i]);
            errh_stat = errl_stat;

            gres->SetPoint(i,hy->GetBinCenter(i+1),val);
            gres->SetPointError(i,hy->GetBinWidth(i+1)/2.,hy->GetBinWidth(i+1)/2.,errl,errh);
            gres_statonly->SetPoint(i,hy->GetBinCenter(i+1),val);
            gres_statonly->SetPointError(i,hy->GetBinWidth(i+1)/2.,hy->GetBinWidth(i+1)/2.,errl_stat,errh_stat);
            hy->SetBinContent(i+1,val);
            hy->SetBinError(i+1,(errl+errh)/2.);
            hy_statonly->SetBinContent(i+1,val);
            hy_statonly->SetBinError(i+1,(errl_stat+errh_stat)/2.);
         }
      } else {
         // if we haven't found the histo... maybe we're looking at the output of the FSR unfolding, and then everything is ready! just get the result.
         hy = (TH1D*) fy->Get(Form("h_Measured_unfoldedMLE_%s",varname(thevar)));
         hy_statonly = (TH1D*) fy->Get(Form("h_Measured_unfoldedMLE_statonly_%s",varname(thevar)));
         cout << hy->GetBinContent(1) << endl;

         if (doxsec) {
            hy->Scale(1./lumi_all);
            Obtain_dSigma_dX(hy);
            hy_statonly->Scale(1./lumi_all);
            Obtain_dSigma_dX(hy_statonly);
         }
         cout << hy->GetBinContent(1) << endl;

         if (!hy) {
            cout << "Error, can't find yield histo in input file." << endl;
            return;
         }

         hy->SetName(Form("hy_%s",varname(thevar)));
         hy_statonly->SetName(Form("hy_statonly_%s",varname(thevar)));

         gres = h2gae(hy);
         gres->SetName(Form("gres_%s",varname(thevar)));
         gres_statonly = h2gae(hy_statonly);
         gres_statonly->SetName(Form("gres_statonly_%s",varname(thevar)));
      }

      bool logx=false, logy=false;
      TString xtitle = xaxistitle(thevar);
      TString ytitle;
      TString xtitle_tex, ytitle_tex;
      int lx=800, ly=800;

      if (thevar==mass) {
         ytitle = "d#sigma/dM [nb/GeV/c^{2}]";
         xtitle_tex = "\\mmumu (\\GeVcc)";
         ytitle_tex = "$\\dd\\sigma/\\dd\\mmumu$ (nb/\\GeVcc)";
         logx= true; logy=true;
      } else if (thevar==pt) {
         ytitle = "d#sigma/dp_{T} [nb/GeV/c]";
         xtitle_tex = "\\pt (\\GeVc)";
         ytitle_tex = "$\\dd\\sigma/\\dd\\pt$ (nb/\\GeVc)";
         logx = true; logy=true;
      } else if (thevar==phistar) {
         ytitle = "d#sigma/d#phi^{*} [nb]";
         xtitle_tex = "\\phistar";
         ytitle_tex = "$\\dd\\sigma/\\dd\\phistar$ (nb)";
         logx = true; logy=true;
      } else if (thevar==rap60120) {
         ytitle = "d#sigma/dy [nb]";
         xtitle_tex = "\\ylab";
         ytitle_tex = "$\\dd\\sigma/\\dd\\ylab$ (nb)";
      } else if (thevar==rap1560) {
         ytitle = "d#sigma/dy [nb]";
         xtitle_tex = "\\ylab";
         ytitle_tex = "$\\dd\\sigma/\\dd\\ylab$ (nb)";
      }

      MyCanvas c1(Form("Plots/results/plots/result_%s",varname(thevar)),xtitle,ytitle,lx,ly);
      if (logx) c1.SetLogx();
      if (logy) c1.SetLogy(false);

      // // temporary dummy theory
      // TFile *fth = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/MCFM/MCFM-8.0/Bin/DYpA8TeV/CT14_eigenvectors/Z_only_nlo_CT14nlo_90___90___DY15600_CT14nlo_CT14nlo_7_7.root");
      // TH1F *hth1 = (TH1F*) fth->Get("id1");
      // TH1F *hth19 = (TH1F*) fth->Get("id19");
      // TH1F *hth20 = (TH1F*) fth->Get("id20");
      // TH1F *hth = rebin_theory(hth1,hth19,hth20);
      // hth->Scale(208*1e-3);

      TFile *fth = TFile::Open("/afs/cern.ch/work/e/echapon/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_Eff_weights_MomUnCorr_Powheg_PAL3Mu12_0_rewboth.root");
      vector<TH1D*> hth;
      int i=0;
      hth.push_back((TH1D*) fth->Get(Form("h_%s_AccTotal%d",varname(thevar),i)));
      hth.back()->Scale(1./lumi_all);
      Obtain_dSigma_dX(hth.back());
      for (i=285; i<=324; i++) {
         hth.push_back((TH1D*) fth->Get(Form("h_%s_AccTotal%d",varname(thevar),i)));
         hth.back()->Scale(1./lumi_all);
         Obtain_dSigma_dX(hth.back());
      }
      for (i=112; i<=167; i++) {
         hth.push_back((TH1D*) fth->Get(Form("h_%s_AccTotal%d",varname(thevar),i)));
         hth.back()->Scale(1./lumi_all);
         Obtain_dSigma_dX(hth.back());
      }

      TGraphAsymmErrors *gth = pdfuncert(hth, "EPPS16nlo_CT14nlo_Pb208");
      gth->SetMarkerSize(0);
      gth->SetName(Form("gth_%s",varname(thevar)));

      // dirty hack to plot the pre-FSR theory... fix for the final results!
#ifdef DIRTYPREFSR
      TFile *fsr = TFile::Open("FSRCorrection/ROOTFile_FSRCorrections_DressedLepton_Powheg_0.root");
      TH1D *hpre = (TH1D*) fsr->Get(Form("h_%s_preFSR",varname(thevar)));
      TH1D *hpost = (TH1D*) fsr->Get(Form("h_%s_postFSR",varname(thevar)));
      for (int i=0; i<gth->GetN(); i++) {
         double ratio = hpre->GetBinContent(i+1)/hpost->GetBinContent(i+1);
         double x = gth->GetX()[i];
         double y = gth->GetY()[i]*ratio;
         double exl = gth->GetEXlow()[i];
         double exh = gth->GetEXhigh()[i];
         double eyl = gth->GetEYlow()[i]*ratio;
         double eyh = gth->GetEYhigh()[i]*ratio;
         gth->SetPoint(i,x,y);
         gth->SetPointError(i,exl,exh,eyl,eyh);
      }
#endif

      if (!forsyst) {
         c1.CanvasWithGraphRatioPlot(gres,gth,
               "Data","Powheg","Data/Powheg",
               kBlack,kRed,
               "EP","5");
         c1.PrintCanvas();
         c1.PrintCanvas_C();

         // print graph
         inittex(Form("Plots/results/tex/%s.tex",varname(thevar)),xtitle.Data(),ytitle.Data());
         printGraph(gres_statonly,gres,Form("Plots/results/tex/%s.tex",varname(thevar)));
         closetex(Form("Plots/results/tex/%s.tex",varname(thevar)));
      }


      // write to file
      fout->cd();
      gres->Write();
      gth->Write();
      hy->Write();
      hy_statonly->Write();

      fth->Close();
   }

   // close file
   fout->Write();
   fout->Close();
   fy->Close();
   fae->Close();
}
