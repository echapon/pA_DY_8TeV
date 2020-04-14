#include <iostream>
#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFrame.h>

#include "../Include/tdrstyle.C"
#include "../Include/CMS_lumi.C"
#include "../Include/MyCanvas.C"
#include "../Include/lhapdf_utils.h"
#include "../Include/PlotTools.h"
#include "../SysUncEstimation/syst.C"

const int rfb_rapbinnum_1560 = 5;
const double rfb_rapbin_1560[rfb_rapbinnum_1560+1] = {0.0,0.4,0.8,1.2,1.6,1.93};
const int rfb_rapbinnum_60120 = 10;
const double rfb_rapbin_60120[rfb_rapbinnum_60120+1] = {0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,1.93};

TGraphAsymmErrors *RFB_1560(TGraphAsymmErrors *g, TH1D* hstatonly, TMatrixT<double> mcov);
TGraphAsymmErrors *RFB_60120(TGraphAsymmErrors *g, TH1D* hstatonly, TMatrixT<double> mcov);
TH1D *RFB_1560(TH1D *h, TH1D* hstatonly, TMatrixT<double> mcov);
TH1D *RFB_60120(TH1D *h, TH1D* hstatonly, TMatrixT<double> mcov);
void Obtain_dSigma_dX(TH1D *h);

using namespace std;

void CompRFB (const char* infile="Plots/results/xsec_nom_detcor_FSR.root") {
   TFile* fin = TFile::Open(infile);

   TGraphAsymmErrors* gaeres_rap1560 = (TGraphAsymmErrors*)fin->Get("gres_rap1560");
   TGraphAsymmErrors* gaeres_rap60120 = (TGraphAsymmErrors*)fin->Get("gres_rap60120");
   TH1D* hres_statonly_rap1560 = (TH1D*)fin->Get("hy_statonly_rap1560");
   TH1D* hres_statonly_rap60120 = (TH1D*)fin->Get("hy_statonly_rap60120");
   TGraphAsymmErrors* gaeth_rap1560 = (TGraphAsymmErrors*)fin->Get("gth_rap1560");
   TGraphAsymmErrors* gaeth_rap60120 = (TGraphAsymmErrors*)fin->Get("gth_rap60120");

   // the RFB is computed here. account for correlations!
   TMatrixT<double> mcov_rap1560 = readSyst_all_cor(DYana::rap1560);
   TMatrixT<double> mcov_rap60120 = readSyst_all_cor(DYana::rap60120);
   TGraphAsymmErrors *grfbp_rap1560 = RFB_1560(gaeres_rap1560, hres_statonly_rap1560, mcov_rap1560);
   TGraphAsymmErrors *grfbp_rap60120 = RFB_60120(gaeres_rap60120, hres_statonly_rap60120, mcov_rap60120);

   // dummy matrices for theory (we take care of correlations later)
   TMatrixT<double> m0_rap1560(DYana::rapbinnum_1560,DYana::rapbinnum_1560);
   TMatrixT<double> m0_rap60120(DYana::rapbinnum_60120,DYana::rapbinnum_60120);

   // for theory, need to account for correlations in the nPDF uncertainties, since we know how to do it
   // EPPS16
   TFile *fth_EPPS16 = TFile::Open("/afs/cern.ch/work/e/echapon/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16.root");
   vector<TH1D*> hth1560_EPPS16,hth60120_EPPS16;
   int i=0;
   hth1560_EPPS16.push_back(RFB_1560((TH1D*) fth_EPPS16->Get(Form("h_rap1560_AccTotal_pre%d",i)),NULL,m0_rap1560));
   hth60120_EPPS16.push_back(RFB_60120((TH1D*) fth_EPPS16->Get(Form("h_rap60120_AccTotal_pre%d",i)),NULL,m0_rap60120));
   for (i=285; i<=324; i++) {
      hth1560_EPPS16.push_back(RFB_1560((TH1D*) fth_EPPS16->Get(Form("h_rap1560_AccTotal_pre%d",i)),NULL,m0_rap1560));
      hth60120_EPPS16.push_back(RFB_60120((TH1D*) fth_EPPS16->Get(Form("h_rap60120_AccTotal_pre%d",i)),NULL,m0_rap60120));
   }
   for (i=112; i<=167; i++) {
      hth1560_EPPS16.push_back(RFB_1560((TH1D*) fth_EPPS16->Get(Form("h_rap1560_AccTotal_pre%d",i)),NULL,m0_rap1560));
      hth60120_EPPS16.push_back(RFB_60120((TH1D*) fth_EPPS16->Get(Form("h_rap60120_AccTotal_pre%d",i)),NULL,m0_rap60120));
   }

   TGraphAsymmErrors *gth1560_EPPS16 = pdfuncert(hth1560_EPPS16, "EPPS16nlo_CT14nlo_Pb208",true);
   TGraphAsymmErrors *gth60120_EPPS16 = pdfuncert(hth60120_EPPS16, "EPPS16nlo_CT14nlo_Pb208",true);
   gth1560_EPPS16->SetMarkerSize(0);
   gth60120_EPPS16->SetMarkerSize(0);
   gth1560_EPPS16->SetName("gth1560_EPPS16");
   gth60120_EPPS16->SetName("gth60120_EPPS16");

   // CT14
   TFile *fth_CT14 = TFile::Open("/afs/cern.ch/work/e/echapon/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_CT14.root");
   vector<TH1D*> hth1560_CT14,hth60120_CT14;
   i=0;
   hth1560_CT14.push_back(RFB_1560((TH1D*) fth_CT14->Get(Form("h_rap1560_AccTotal_pre%d",i)),NULL,m0_rap1560));
   hth60120_CT14.push_back(RFB_60120((TH1D*) fth_CT14->Get(Form("h_rap60120_AccTotal_pre%d",i)),NULL,m0_rap60120));
   for (i=112; i<=167; i++) {
      hth1560_CT14.push_back(RFB_1560((TH1D*) fth_CT14->Get(Form("h_rap1560_AccTotal_pre%d",i)),NULL,m0_rap1560));
      hth60120_CT14.push_back(RFB_60120((TH1D*) fth_CT14->Get(Form("h_rap60120_AccTotal_pre%d",i)),NULL,m0_rap60120));
   }

   TGraphAsymmErrors *gth1560_CT14 = pdfuncert(hth1560_CT14, "CT14nlo",true);
   TGraphAsymmErrors *gth60120_CT14 = pdfuncert(hth60120_CT14, "CT14nlo",true);
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
         "5","5","EP",true);
   c_1560.TopPad->cd();
   TLatex latex;
   latex.SetNDC();
   latex.SetTextSize(0.03);
   double xlatex=.2, ylatex=0.9, dylatex=0.04;
   latex.DrawLatex(xlatex,ylatex,"15 < M < 60 GeV");
   c_1560.PrintCanvas();

   MyCanvas c_60120("Plots/grfbp_rap60120","|y|","R_{FB}",800,800);
   c_60120.SetYRange(0.65,1.35);
   c_60120.CanvasWithThreeGraphsRatioPlot(gth60120_CT14,gth60120_EPPS16,grfbp_rap60120,
         "Powheg (CT14)","Powheg (EPPS16)","Data","Powheg/Data",
         kBlue,kRed,kBlack,
         "5","5","EP",true);
   c_60120.TopPad->cd();
   ylatex=0.9;
   latex.DrawLatex(xlatex,ylatex,"60 < M < 120 GeV");
   c_60120.PrintCanvas();
}

TGraphAsymmErrors *RFB_1560(TGraphAsymmErrors *g, TH1D* hstatonly, TMatrixT<double> mcov) {
   TH1D* f_rap1560_temp = new TH1D("f_rap1560_temp",";|y_{CM}| (15<M<60 (GeV));Forward/Backward (Xsec)",rfb_rapbinnum_1560,rfb_rapbin_1560);
   TH1D* b_rap1560_temp = new TH1D("b_rap1560_temp",";|y_{CM}| (15<M<60 (GeV));Forward/Backward (Xsec)",rfb_rapbinnum_1560,rfb_rapbin_1560);

   // arrays to hold stat uncertainties and correlation
   double *f_stat = new double[rfb_rapbinnum_1560];
   double *b_stat = new double[rfb_rapbinnum_1560];
   double *cor    = new double[rfb_rapbinnum_1560];

   f_rap1560_temp->Sumw2();
   b_rap1560_temp->Sumw2();

   for (int i=2;i<2+rfb_rapbinnum_1560;i++) {
      // std::cout << "|y|: " << g->GetX()[13-i] << " / " << g->GetX()[i] << std::endl;
      if (g->GetY()[13-i]<=0) {f_rap1560_temp->SetBinContent(7-i,0.0);f_rap1560_temp->SetBinError(7-i,0.0);}
      else {f_rap1560_temp->SetBinContent(7-i,g->GetY()[13-i]);f_rap1560_temp->SetBinError(7-i,g->GetErrorY(13-i));}
      if (g->GetY()[i]<=0) {b_rap1560_temp->SetBinContent(7-i,0.0);b_rap1560_temp->SetBinError(7-i,0.0);}
      else {b_rap1560_temp->SetBinContent(7-i,g->GetY()[i]);b_rap1560_temp->SetBinError(7-i,g->GetErrorY(i));}

      f_stat[7-i-1] = hstatonly ? hstatonly->GetBinError(13-i+1) : 0;
      b_stat[7-i-1] = hstatonly ? hstatonly->GetBinError(i+1) : 0;
      cor[7-i-1] = mcov[13-i][i];
   }
   TGraphAsymmErrors *ans = new TGraphAsymmErrors(f_rap1560_temp,b_rap1560_temp,"pois n");

   // now, take care of the uncertainties... with correlations
   for (int i=0; i<rfb_rapbinnum_1560; i++) {
      // get the syst
      double f_syst = sqrt(pow(f_rap1560_temp->GetBinError(i+1),2)-pow(f_stat[i],2));
      double b_syst = sqrt(pow(b_rap1560_temp->GetBinError(i+1),2)-pow(b_stat[i],2));

      double f_val = f_rap1560_temp->GetBinContent(i+1);
      double b_val = b_rap1560_temp->GetBinContent(i+1);

      // the stat is uncorrelated between forward and backward (for now... will need to include correlations due to unfolding)
      double stat_err = ans->GetY()[i] * sqrt(pow(f_stat[i]/f_val,2) + pow(b_stat[i]/b_val,2));

      // for the syst... need to account for correlations as provided in the input (see https://en.wikipedia.org/wiki/Propagation_of_uncertainty#Example_formulas)
      double syst_err = ans->GetY()[i] * sqrt(pow(f_syst/f_val,2) + pow(b_syst/b_val,2)
            -2.*cor[i]*f_syst*b_syst/(f_val*b_val));

      double tot_err = sqrt(pow(stat_err,2)+pow(syst_err,2));

      // now set the error in the output
      ans->SetPointEYlow(i,tot_err);
      ans->SetPointEYhigh(i,tot_err);
      // cout << i << ": " << f_val << "/" << b_val << "=" << ans->GetY()[i] << "; (" << f_stat[i] << "," << f_syst << ")/(" << b_stat[i] << "," << b_syst << ") [" << cor[i] << "] = " << stat_err << "," << syst_err << " = " << tot_err << endl;
   }

   // clean up
   delete f_rap1560_temp;
   delete b_rap1560_temp;
   delete[] f_stat;
   delete[] b_stat;
   delete[] cor;

   return ans;
}

TGraphAsymmErrors *RFB_60120(TGraphAsymmErrors *g, TH1D* hstatonly, TMatrixT<double> mcov) {
   TH1D* f_rap60120_temp = new TH1D("f_rap60120_temp",";|y_{CM}| (60<M<120 (GeV));Forward/Backward (Xsec)",rfb_rapbinnum_60120,rfb_rapbin_60120);
   TH1D* b_rap60120_temp = new TH1D("b_rap60120_temp",";|y_{CM}| (60<M<120 (GeV));Forward/Backward (Xsec)",rfb_rapbinnum_60120,rfb_rapbin_60120);

   // arrays to hold stat uncertainties and correlation
   double *f_stat = new double[rfb_rapbinnum_60120];
   double *b_stat = new double[rfb_rapbinnum_60120];
   double *cor    = new double[rfb_rapbinnum_60120];

   f_rap60120_temp->Sumw2();
   b_rap60120_temp->Sumw2();

   for (int i=4;i<4+rfb_rapbinnum_60120;i++) {
      // std::cout << "|y|: " << g->GetX()[27-i] << " / " << g->GetX()[i] << std::endl;
      if (g->GetY()[27-i]<=0) {f_rap60120_temp->SetBinContent(14-i,0.0);f_rap60120_temp->SetBinError(14-i,0.0);}
      else {f_rap60120_temp->SetBinContent(14-i,g->GetY()[27-i]);f_rap60120_temp->SetBinError(14-i,g->GetErrorY(27-i));}
      if (g->GetY()[i]<=0) {b_rap60120_temp->SetBinContent(14-i,0.0);b_rap60120_temp->SetBinError(14-i,0.0);}
      else {b_rap60120_temp->SetBinContent(14-i,g->GetY()[i]);b_rap60120_temp->SetBinError(14-i,g->GetErrorY(i));}

      f_stat[14-i-1] = hstatonly ? hstatonly->GetBinError(27-i+1) : 0;
      b_stat[14-i-1] = hstatonly ? hstatonly->GetBinError(i+1) : 0;
      cor[14-i-1] = mcov[27-i][i];
   }
   TGraphAsymmErrors *ans = new TGraphAsymmErrors(f_rap60120_temp,b_rap60120_temp,"pois n");

   // now, take care of the uncertainties... with correlations
   for (int i=0; i<rfb_rapbinnum_60120; i++) {
      // get the syst
      double f_syst = sqrt(pow(f_rap60120_temp->GetBinError(i+1),2)-pow(f_stat[i],2));
      double b_syst = sqrt(pow(b_rap60120_temp->GetBinError(i+1),2)-pow(b_stat[i],2));

      double f_val = f_rap60120_temp->GetBinContent(i+1);
      double b_val = b_rap60120_temp->GetBinContent(i+1);

      // the stat is uncorrelated between forward and backward (for now... will need to include correlations due to unfolding)
      double stat_err = ans->GetY()[i] * sqrt(pow(f_stat[i]/f_val,2) + pow(b_stat[i]/b_val,2));

      // for the syst... need to account for correlations as provided in the input (see https://en.wikipedia.org/wiki/Propagation_of_uncertainty#Example_formulas)
      double syst_err = ans->GetY()[i] * sqrt(pow(f_syst/f_val,2) + pow(b_syst/b_val,2)
            -2.*cor[i]*f_syst*b_syst/(f_val*b_val));

      double tot_err = sqrt(pow(stat_err,2)+pow(syst_err,2));

      // now set the error in the output
      ans->SetPointEYlow(i,tot_err);
      ans->SetPointEYhigh(i,tot_err);
      // cout << i << ": " << f_val << "/" << b_val << "=" << ans->GetY()[i] << "; (" << f_stat[i] << "," << f_syst << ")/(" << b_stat[i] << "," << b_syst << ") [" << cor[i] << "] = " << stat_err << "," << syst_err << " = " << tot_err << endl;
   }

   // clean up
   delete f_rap60120_temp;
   delete b_rap60120_temp;
   delete[] f_stat;
   delete[] b_stat;
   delete[] cor;

   return ans;
}

TH1D *RFB_1560(TH1D *h, TH1D* hstatonly, TMatrixT<double> mcov) {
   TGraphAsymmErrors *g = Convert_HistToGraph(h);
   return Convert_GraphToHist(RFB_1560(g, hstatonly, mcov));
}

TH1D *RFB_60120(TH1D *h, TH1D* hstatonly, TMatrixT<double> mcov) {
   TGraphAsymmErrors *g = Convert_HistToGraph(h);
   return Convert_GraphToHist(RFB_60120(g, hstatonly, mcov));
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

