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
#include "../Include/PlotTools.h"

using namespace DYana;
using namespace std;

void replaceCentralValues(TGraphAsymmErrors *tg, TH1D *hist);

void getTheory(TString sample, TGraphAsymmErrors* &gth, vector<TH1D*> &hth, TH2D* &hcor, bool preFSR, bool correctforacc, var thevar);

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

void myXsec(const char* datafile="FSRCorrection/xsec_FSRcor_Powheg_MomCorr00_0.root",                           // data and bkg histos
      const char* accefffile="ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root",// acceptance and efficiency
      const char* outputfile="Plots/results/xsec_nom_detcor_FSRcor.root",                        // where to write the output xsec
      bool forsyst=false,                                        // if true, don't print canvases and tables
      bool doxsec=true,                                          // if false, don't do dxsec/dxxx, just correct for acc eff
      bool correctforacc=true,                                   // if false, do not correct for acceptance, only efficiency
      bool preFSR=true,                                          // true for preFSR, false for postFSR (ie before FSR unfolding)
      TString sample1="CT14",
      TString sample2="EPPS16") {
   TFile *fy = TFile::Open(datafile);
   TFile *fae = TFile::Open(accefffile);

   TFile *fout = TFile::Open(outputfile,"RECREATE");

   // for legend
   TString sample1_leg = sample1;
   TString sample2_leg = sample2;
   if (sample1_leg == "EPPS16") sample1_leg = "CT14+EPPS16";
   if (sample2_leg == "EPPS16") sample2_leg = "CT14+EPPS16";

   ofstream chi2file(Form("Plots/results/tex/chi2%s.tex",correctforacc ? "" : "_noacc"));
   ofstream chi2file_nocor(Form("Plots/results/tex/chi2_nocor%s.tex",correctforacc ? "" : "_noacc"));
   chi2file << "\\begin{tabular}{lcccccc}" << endl;
   chi2file << "\\multirow{2}{4em}{Observable} & \\multicolumn{3}{c}{"<<sample1_leg<<"} & \\multicolumn{3}{c}{"<<sample2_leg<<"} \\\\" << endl;
   chi2file << "& $\\chi^{2}$ & dof & Prob. [\\%] & $\\chi^{2}$ & dof & Prob. [\\%] \\\\" << endl;
   chi2file << "\\hline" << endl;
   chi2file_nocor << "\\begin{tabular}{lcccccc}" << endl;
   chi2file_nocor << "\\multirow{2}{4em}{Observable} & \\multicolumn{3}{c}{"<<sample1_leg<<"} & \\multicolumn{3}{c}{"<<sample2_leg<<"} \\\\" << endl;
   chi2file_nocor << "& $\\chi^{2}$ & dof & Prob. [\\%] & $\\chi^{2}$ & dof & Prob. [\\%] \\\\" << endl;
   chi2file_nocor << "\\hline" << endl;

   // loop on the variables
   for (int ivar=0; ivar<ALLvar; ivar++) {
   // for (int ivar=0; ivar<pt; ivar++) { // only mass for now
      var thevar = static_cast<var>(ivar);

      // general multiplicator to enlarge or reduce the size of everything
      const float sizemod = (thevar==var::mass) ? 0.8 : 1.2;
      float lumiTextSize0 = lumiTextSize;
      float lumiTextOffset0 = lumiTextSize;
      float cmsTextSize0 = cmsTextSize;
      float cmsTextOffset0 = cmsTextSize;
      lumiTextSize *= 1.1*sizemod;
      if (sizemod>1) lumiTextOffset *= 0.5;
      else lumiTextOffset *= 0.8;
      cmsTextSize *= sizemod;
      if (sizemod>1) cmsTextOffset *= 0.5;
      else cmsTextOffset *= 0.8;

      // the systs
      map<bin,syst> thesyst = readSyst_all(thevar,false,"","./",!correctforacc);

      TH1D *hy = (TH1D*) fy->Get(Form("h_%s_bkgsub_DataDrivenBkg_All1",varname(thevar)));
      TH1D *hy_statonly;
      TGraphAsymmErrors* gres = NULL;
      TGraphAsymmErrors* gres_statonly = NULL;


      if (hy) {
         cout << "bkgsub histo FOUND!" << endl;
         if (doxsec) {
            hy->Scale(1.e-3/lumi_all); // pb -> nb
            Obtain_dSigma_dX(hy);
         }
         hy->SetName(Form("hy_%s",varname(thevar)));

         hy_statonly = (TH1D*) hy->Clone(Form("hy_statonly_%s",varname(thevar)));

         // TGraph *gae = (TGraph*) fae->Get("g_AccEff_Corr_tnp");
         // get acc and eff and multiply them in the loop
         TEfficiency *TEff_Acc = (TEfficiency*)fae->Get(Form("TEff_Acc_%s",Varname(thevar)));
         TGraphAsymmErrors *ga = (TGraphAsymmErrors*)TEff_Acc->CreateGraph()->Clone();

         TEfficiency *TEff_Eff = (TEfficiency*)fae->Get(Form("TEff_Eff_%s_Corr_tnp0",Varname(thevar)));
         TGraphAsymmErrors *ge = (TGraphAsymmErrors*)TEff_Eff->CreateGraph()->Clone();

         gres = (TGraphAsymmErrors*) ga->Clone(Form("gres_%s",varname(thevar)));
         gres_statonly = (TGraphAsymmErrors*) ga->Clone(Form("gres_statonly_%s",varname(thevar)));

         for (int i=0; i<ga->GetN(); i++) {
            bin thebin;
            thebin.first = hy->GetBinLowEdge(i+1);
            thebin.second = thebin.first + hy->GetBinWidth(i+1);
            double ae = (correctforacc) ? ga->GetY()[i]*ge->GetY()[i] : ge->GetY()[i];
            double val = hy->GetBinContent(i+1)/ae;
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
            errl_stat = hy->GetBinError(i+1)/ae;
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
         cout << "bkg sub histo NOT FOUND! Will use unfolded histo instead." << endl;
         // if we haven't found the histo... maybe we're looking at the output of the FSR unfolding, and then everything is ready! just get the result.
         hy = (TH1D*) fy->Get(Form("h_Measured_unfoldedMLE_%s",varname(thevar)));
         hy_statonly = (TH1D*) fy->Get(Form("h_Measured_unfoldedMLE_statonly_%s",varname(thevar)));
         // cout << hy->GetBinContent(1) << endl;

         if (doxsec) {
            hy->Scale(1.e-3/lumi_all); // pb -> nb
            Obtain_dSigma_dX(hy);
            hy_statonly->Scale(1.e-3/lumi_all); // pb -> nb
            Obtain_dSigma_dX(hy_statonly);
         }
         // cout << hy->GetBinContent(1) << endl;

         if (!hy) {
            cout << "Error, can't find yield histo " << Form("h_Measured_unfoldedMLE_%s",varname(thevar)) << " in input file " << fy->GetName() << "." << endl;
            return;
         }

         hy->SetName(Form("hy_%s",varname(thevar)));
         hy_statonly->SetName(Form("hy_statonly_%s",varname(thevar)));

         gres = Convert_HistToGraph(hy);
         gres->SetName(Form("gres_%s",varname(thevar)));
         gres_statonly = Convert_HistToGraph(hy_statonly);
         gres_statonly->SetName(Form("gres_statonly_%s",varname(thevar)));
      }

      bool logx=false, logy=false;
      TString xtitle = xaxistitle(thevar);
      TString ytitle;
      TString xtitle_tex, ytitle_tex;
      int lx=800, ly=800;

      if (thevar==mass) {
         ytitle = "d#sigma/dm_{#mu#mu} [nb/GeV]";
         xtitle_tex = "\\mmumu (\\GeV)";
         ytitle_tex = "$\\dd\\sigma/\\dd\\mmumu$ (nb/\\GeV)";
         logx= true; logy=true;
      } else if (thevar==pt || thevar==pt1560) {
         ytitle = "d#sigma/dp_{T} [nb/GeV]";
         xtitle_tex = "\\pt (\\GeV)";
         ytitle_tex = "$\\dd\\sigma/\\dd\\pt$ (nb/\\GeV)";
         logx = true; logy=true;
      } else if (thevar==phistar || thevar==phistar1560) {
         ytitle = "d#sigma/d#phi^{*} [nb]";
         xtitle_tex = "\\phistar";
         ytitle_tex = "$\\dd\\sigma/\\dd\\phistar$ (nb)";
         logx = true; logy=true;
      } else if (thevar==rap60120) {
         ytitle = "d#sigma/dy [nb]";
         xtitle_tex = "\\yCM";
         ytitle_tex = "$\\dd\\sigma/\\dd\\yCM$ (nb)";
      } else if (thevar==rap1560) {
         ytitle = "d#sigma/dy [nb]";
         xtitle_tex = "\\yCM";
         ytitle_tex = "$\\dd\\sigma/\\dd\\yCM$ (nb)";
      }

      MyCanvas c1(Form("Plots/results/plots/result%s_%s_%s_%s",correctforacc ? "" : "_noacc",varname(thevar),sample1.Data(),sample2.Data()),xtitle,ytitle,lx,ly);
      if (logx) c1.SetLogx();
      if (logy) c1.SetLogy(false);
		if (thevar==var::phistar || thevar==var::phistar1560) {
			c1.isSetNoExpo_MoreLogLabels_X = kFALSE;
			c1.isSetNoExpo_MoreLogLabels_Y = kFALSE;
		}

      // // temporary dummy theory
      // TFile *fth = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/MCFM/MCFM-8.0/Bin/DYpA8TeV/CT14_eigenvectors/Z_only_nlo_CT14nlo_90___90___DY15600_CT14nlo_CT14nlo_7_7.root");
      // TH1F *hth1 = (TH1F*) fth->Get("id1");
      // TH1F *hth19 = (TH1F*) fth->Get("id19");
      // TH1F *hth20 = (TH1F*) fth->Get("id20");
      // TH1F *hth = rebin_theory(hth1,hth19,hth20);
      // hth->Scale(208*1e-3);

      // EPPS16
      // TFile *fth_EPPS16 = TFile::Open("/afs/cern.ch/work/e/echapon/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_Eff_weights_MomUnCorr_Powheg_PAL3Mu12_0_rewboth_EPPS16.root");
      // TFile *fth_EPPS16 = TFile::Open("/afs/cern.ch/work/e/echapon/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16.root");

      vector<TH1D*> hth_EPPS16;
      TGraphAsymmErrors *gth_EPPS16 = NULL;
      TH2D *hth_EPPS16_cor = NULL;
      getTheory(sample2, gth_EPPS16, hth_EPPS16, hth_EPPS16_cor, preFSR, correctforacc, thevar);

      // CT14
      vector<TH1D*> hth_CT14;
      TGraphAsymmErrors *gth_CT14 = NULL;
      TH2D *hth_CT14_cor = NULL;
      getTheory(sample1, gth_CT14, hth_CT14, hth_CT14_cor, preFSR, correctforacc, thevar);

      if (doxsec && !forsyst) {
         if (thevar==var::rap60120 || thevar==var::rap1560) c1.SetYRange(19,72);
         if (!correctforacc && thevar==var::rap1560) c1.SetYRange(0,10);
         if (!correctforacc && thevar==var::rap60120) c1.SetYRange(0,52);
         if (thevar==var::mass && correctforacc) c1.SetYRange(2e-4,90);
         if (thevar==var::mass && !correctforacc) c1.SetYRange(2e-4,20);
         if (thevar==var::pt1560 && correctforacc) c1.SetYRange(5e-4,7e1);
         if (thevar==var::pt1560 && !correctforacc) c1.SetYRange(2e-4,4);
         if (thevar==var::phistar1560 && correctforacc) c1.SetYRange(2,3e3);
         if (thevar==var::phistar1560 && !correctforacc) c1.SetYRange(4e-1,3e2);
         if (thevar==var::pt && correctforacc) c1.SetYRange(8e-4,4e1);
         if (thevar==var::pt && !correctforacc) c1.SetYRange(8e-4,30);
         if (thevar==var::phistar && correctforacc) c1.SetYRange(3e-1,5e3);
         if (thevar==var::phistar && !correctforacc) c1.SetYRange(2e-1,3e3);

         if (thevar==var::pt || thevar==var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) {
            if (gth_CT14) fixXaxis(gth_CT14);
            if (gth_EPPS16) fixXaxis(gth_EPPS16);
            fixXaxis(gres);

            int nbins = nbinsvar(thevar);
            double xmin = gres->GetX()[0] - gres->GetEXlow()[0];
            double xmax = gres->GetX()[nbins-1] + gres->GetEXhigh()[nbins-1];
            c1.SetXRange(xmin,xmax);
         }

         // // let's take central values from pre-FSR, BUT the (n)PDF uncertainties are the ones we just obtained
         // TFile *fFSR_CT14=NULL;
         // TFile *fFSR_EPPS16=NULL;
         // if (correctforacc) {
         //    fFSR_CT14 = TFile::Open("FSRCorrection/nozptrew/ROOTFile_FSRCorrections_DressedLepton_CT14_0.root");
         //    fFSR_EPPS16 = TFile::Open("FSRCorrection/nozptrew/ROOTFile_FSRCorrections_DressedLepton_Powheg_0.root");
         // } else {
         //    fFSR_CT14 = TFile::Open("FSRCorrection/nozptrew/ROOTFile_FSRCorrections_DressedLepton_CT14_0_noacc.root");
         //    fFSR_EPPS16 = TFile::Open("FSRCorrection/nozptrew/ROOTFile_FSRCorrections_DressedLepton_Powheg_0_noacc.root");
         // }
         // TH1D *hpreFSR_CT14 = (TH1D*) fFSR_CT14->Get(Form("h_%s_preFSR",varname(thevar)));
         // hpreFSR_CT14->Scale(1e-3/lumi_all); // pb -> nb
         // Obtain_dSigma_dX(hpreFSR_CT14);
         // TH1D *hpreFSR_EPPS16 = (TH1D*) fFSR_EPPS16->Get(Form("h_%s_preFSR",varname(thevar)));
         // hpreFSR_EPPS16->Scale(1e-3/lumi_all); // pb -> nb
         // Obtain_dSigma_dX(hpreFSR_EPPS16);
         // replaceCentralValues(gth_CT14,hpreFSR_CT14);
         // replaceCentralValues(gth_EPPS16,hpreFSR_EPPS16);

         // style stuff
         // legend
         double xl1 = 0.55, yl1 = 0.70, xl2 = 0.98, yl2 = 0.90;
         double dxl = sizemod * (xl2-xl1), dyl = sizemod * (yl2-yl1);
         if (thevar==var::pt || thevar==var::pt1560 || thevar==var::phistar || thevar==var::phistar1560) {
            xl2 = 0.68;
            yl2 = 0.78;
         }
         if (thevar==var::rap60120 || thevar==var::rap1560) {
            xl2 = 1.03;
            yl2 = 0.92;
         }
         if (!correctforacc && (thevar==var::rap60120 || thevar==var::rap1560)) {
            xl2 = 0.88;
            yl2 = 0.58;
         }
         c1.SetLegendPosition(xl2-dxl, yl2-dyl, xl2, yl2);
         c1.SizeMod = sizemod;


         // c1.PrintVariables();
         c1.CanvasWithThreeGraphsRatioPlot(gth_CT14,gth_EPPS16,gres,
               sample1_leg,sample2_leg,"Data","Pred./Data",
               kBlue,kRed,kBlack,
               "5","5","EP",true);

         // add label
         c1.TopPad->cd();
         TLatex latex;
         latex.SetNDC();
         latex.SetTextSize(0.035*sizemod);
         double xlatex=.18, ylatex=0.5, dylatex=0.05*sizemod;//0.04
         if (thevar==var::rap1560 || thevar==rap60120) ylatex=0.87;//0.9 (Hyunchul)
         latex.SetTextAlign(12);  //centered
         if (thevar!=rap1560 && thevar!=rap60120) {
            latex.DrawLatex(xlatex,ylatex,"-2.87 < |y_{CM}| < 1.93");
            ylatex -= dylatex;
         }
         if (thevar==pt1560 || thevar==phistar1560 || thevar==rap1560) {
            latex.DrawLatex(xlatex,ylatex,"15 < m_{#mu#mu} < 60 GeV");
            ylatex -= dylatex;
         }
         if (thevar==pt || thevar==phistar || thevar==rap60120) {
            latex.DrawLatex(xlatex,ylatex,"60 < m_{#mu#mu} < 120 GeV");
            ylatex -= dylatex;
         }
         if (!correctforacc) {
            latex.DrawLatex(xlatex,ylatex,"|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV");
            ylatex -= dylatex;
         }

         c1.PrintCanvas();
         c1.PrintCanvas_C();

         // write the ratio to file
         TFile *fratio = TFile::Open("Plots/results/expthratios.root","UPDATE");
         c1.g_ratio1->Write(Form("%s_%s",varname(thevar),sample1.Data()));
         c1.g_ratio2->Write(Form("%s_%s",varname(thevar),sample2.Data()));
         fratio->Close();

         // print graph
         inittex(Form("Plots/results/tex/%s%s.tex",varname(thevar),correctforacc ? "" : "_noacc"),xtitle.Data(),ytitle.Data());
         printGraph(gres_statonly,gres,Form("Plots/results/tex/%s%s.tex",varname(thevar),correctforacc ? "" : "_noacc"));
         closetex(Form("Plots/results/tex/%s%s.tex",varname(thevar),correctforacc ? "" : "_noacc"));

         //////////////////////////
         //   chi2 computation   //
         //////////////////////////

         // chi2, no correlations
         int nbins = nbinsvar(thevar);
         double chi2_CT14_nocor = chi2(gres,gth_CT14);
         double chi2_EPPS16_nocor = chi2(gres,gth_EPPS16);
         cout << varname(thevar) << ", " << sample1 << ", w/o cor: chi2/ndf = " << chi2_CT14_nocor << "/" << nbins << " (p = " << TMath::Prob(chi2_CT14_nocor,nbins) << ")" << endl;
         cout << varname(thevar) << ", " << sample2 << ", w/o cor: chi2/ndf = " << chi2_EPPS16_nocor << "/" << nbins << " (p = " << TMath::Prob(chi2_EPPS16_nocor,nbins) << ")" << endl;

         // chi2, with correlations
         // data stat
         TMatrixT<double> cov_stat = diag(Form("SysUncEstimation/csv/stat_%s.csv",varname(thevar)));
         
         // need to account for correlations arising from unfoldings
         // detector resolution
         TFile *fdetres = TFile::Open("ResponseMatrix/yields_detcor_Powheg_MomCorr00_0.root");
         TH2D *hcor_detres = (TH2D*) fdetres->Get("h_Measured_unfoldedMLE_cov_"+TString(varname(thevar)));
         // normalise the covariance matrix
         for (int i=1; i<=nbins; i++)
            for (int j=1; j<=nbins; j++)
               if (i!=j) hcor_detres->SetBinContent(i,j,hcor_detres->GetBinContent(i,j)/sqrt(hcor_detres->GetBinContent(i,i)*hcor_detres->GetBinContent(j,j)));
         for (int i=1; i<=nbins; i++) hcor_detres->SetBinContent(i,i,1);
         TMatrixT<double> cor_stat_detres = hist2mat(hcor_detres);

         // FSR
         TFile *fFSR = TFile::Open(Form("FSRCorrection/xsec%s_FSRcor_Powheg_MomCorr00_0.root", correctforacc ? "" : "_noacc"));
         TH2D *hcor_FSR = (TH2D*) fFSR->Get("h_Measured_unfoldedMLE_cov_statonly_"+TString(varname(thevar)));
         // normalise the covariance matrix
         for (int i=1; i<=nbins; i++)
            for (int j=1; j<=nbins; j++)
               if (i!=j) hcor_FSR->SetBinContent(i,j,hcor_FSR->GetBinContent(i,j)/sqrt(hcor_FSR->GetBinContent(i,i)*hcor_FSR->GetBinContent(j,j)));
         for (int i=1; i<=nbins; i++) hcor_FSR->SetBinContent(i,i,1);
         TMatrixT<double> cor_stat_FSR = hist2mat(hcor_FSR);

         cov_stat = cov_stat * cor_stat_detres * cor_stat_FSR;

         // data syst
         TMatrixT<double> cov_syst = map2mat(readSyst_all_cov(thevar, "./", !correctforacc));
         // what we have is in "%^2". Need to multiply by central values.
         for (int i=0; i<nbins; i++) {
            for (int j=0; j<nbins; j++) {
               cov_stat[i][j] *= gres->GetY()[i] * gres->GetY()[j];
               cov_syst[i][j] *= gres->GetY()[i] * gres->GetY()[j];
            }
         }
         // data total
         TMatrixT<double> cov_statsyst = cov_stat + cov_syst;
         // CT14
         TMatrixT<double> cov_CT14(nbins,nbins);
         for (int i=0; i<nbins; i++) {
            for (int j=0; j<nbins; j++) {
               double eyli = gth_CT14->GetEYlow()[i];
               double eylj = gth_CT14->GetEYlow()[j];
               double eyhi = gth_CT14->GetEYhigh()[i];
               double eyhj = gth_CT14->GetEYhigh()[j];
               double eyi = (eyhi+eyli)/2.;
               double eyj = (eyhj+eylj)/2.;
               cov_CT14[i][j] = hth_CT14_cor->GetBinContent(i+1,j+1) * eyi * eyj;
            }
         }
         // EPPS16
         TMatrixT<double> cov_EPPS16(nbins,nbins);
         for (int i=0; i<nbins; i++) {
            for (int j=0; j<nbins; j++) {
               double eyli = gth_EPPS16->GetEYlow()[i];
               double eylj = gth_EPPS16->GetEYlow()[j];
               double eyhi = gth_EPPS16->GetEYhigh()[i];
               double eyhj = gth_EPPS16->GetEYhigh()[j];
               double eyi = (eyhi+eyli)/2.;
               double eyj = (eyhj+eylj)/2.;
               cov_EPPS16[i][j] = hth_EPPS16_cor->GetBinContent(i+1,j+1) * eyi * eyj;
            }
         }

         // compute chi2
         double chi2_CT14_withcor = chi2(gres,gth_CT14,cov_statsyst,cov_CT14);
         double chi2_EPPS16_withcor = chi2(gres,gth_EPPS16,cov_statsyst,cov_EPPS16);
         cout << varname(thevar) << ", " << sample1 << ", w/ cor: chi2/ndf = " << chi2_CT14_withcor << "/" << nbins << " (p = " << TMath::Prob(chi2_CT14_withcor,nbins) << ")" << endl;
         cout << varname(thevar) << ", " << sample2 << ", w/ cor: chi2/ndf = " << chi2_EPPS16_withcor << "/" << nbins << " (p = " << TMath::Prob(chi2_EPPS16_withcor,nbins) << ")" << endl;

         // // let's debug the details
         // ofstream tmpfile(Form("debug_cov_%s.txt",varname(thevar)));
         // for (int i=0; i<nbins; i++) {
         //    tmpfile << i << ": " <<
         //       "data: " << (gres->GetEYlow()[i] + gres->GetEYhigh()[i])/2. << " " << sqrt(cov_stat[i][i] + cov_syst[i][i]) << " (" << sqrt(cov_stat[i][i]) << ", " << sqrt(cov_syst[i][i]) << "); " <<
         //       "CT14: " << (gth_CT14->GetEYlow()[i] + gth_CT14->GetEYhigh()[i])/2. << " " << sqrt(cov_CT14[i][i]) << "; " <<
         //       "EPPS16: " << (gth_EPPS16->GetEYlow()[i] + gth_EPPS16->GetEYhigh()[i])/2. << " " << sqrt(cov_EPPS16[i][i]) << endl;
         // }
         // tmpfile << endl << "---------------------------------------" << endl;
         // for (int i=0; i<nbins; i++) {
         //    for (int j=0; j<nbins; j++) {
         //       tmpfile << cov_stat[i][j] << " ";
         //    }
         //    tmpfile << endl;
         // }
         // tmpfile << endl << "---------------------------------------" << endl;
         // for (int i=0; i<nbins; i++) {
         //    for (int j=0; j<nbins; j++) {
         //       tmpfile << cov_syst[i][j] << " ";
         //    }
         //    tmpfile << endl;
         // }
         // tmpfile << endl << "---------------------------------------" << endl;
         // for (int i=0; i<nbins; i++) {
         //    for (int j=0; j<nbins; j++) {
         //       tmpfile << cov_CT14[i][j] << " ";
         //    }
         //    tmpfile << endl;
         // }
         // tmpfile << endl << "---------------------------------------" << endl;
         // for (int i=0; i<nbins; i++) {
         //    for (int j=0; j<nbins; j++) {
         //       tmpfile << cov_EPPS16[i][j] << " ";
         //    }
         //    tmpfile << endl;
         // }
         // tmpfile.close();

         // print those chi2's to tex
         chi2file << xtitle_tex;
         chi2file_nocor << xtitle_tex;
         if (thevar==var::rap1560 || thevar==pt1560 || thevar==phistar1560) {
            chi2file << " ($15 < \\mmumu < 60\\GeV$)";
            chi2file_nocor << " ($15 < \\mmumu < 60\\GeV$)";
         } else if (thevar==var::rap60120 || thevar==pt || thevar==phistar) {
            chi2file << " ($60 < \\mmumu < 120\\GeV$)";
            chi2file_nocor << " ($60 < \\mmumu < 120\\GeV$)";
         }
         chi2file << " & " << chi2_CT14_withcor << " & " << nbins << " & " << TMath::Prob(chi2_CT14_withcor,nbins)*100.
            << " & " << chi2_EPPS16_withcor << " & " << nbins << " & " << TMath::Prob(chi2_EPPS16_withcor,nbins)*100. << " \\\\" << endl;
         chi2file_nocor << " & " << chi2_CT14_nocor << " & " << nbins << " & " << TMath::Prob(chi2_CT14_nocor,nbins)*100.
            << " & " << chi2_EPPS16_nocor << " & " << nbins << " & " << TMath::Prob(chi2_EPPS16_nocor,nbins)*100. << " \\\\" << endl;

         fdetres->Close();
         fFSR->Close();
      }


      // write to file
      fout->cd();
      gres->Write();
      if (gth_CT14) gth_CT14->Write();
      if (gth_EPPS16) gth_EPPS16->Write();
      hy->Write();
      hy_statonly->Write();

      // restore CMS parameters
      lumiTextSize = lumiTextSize0;
      lumiTextOffset = lumiTextSize0;
      cmsTextSize = cmsTextSize0;
      cmsTextOffset = cmsTextSize0;
   }

   // close file
   fout->Write();
   fout->Close();
   fy->Close();
   fae->Close();

   // close chi2 tex files
   chi2file << "\\end{tabular}" << endl;
   chi2file.close();
   chi2file_nocor << "\\end{tabular}" << endl;
   chi2file_nocor.close();
}

void replaceCentralValues(TGraphAsymmErrors *tg, TH1D *hist) {
   if (!tg || !hist) return;
   if (tg->GetN() != hist->GetNbinsX()) return;

   for (int i=0; i<tg->GetN(); i++) {
      double x = tg->GetX()[i];
      double y = tg->GetY()[i];
      double exl = tg->GetEXlow()[i];
      double exh = tg->GetEXhigh()[i];
      double eyl = tg->GetEYlow()[i];
      double eyh = tg->GetEYhigh()[i];

      double y2 = hist->GetBinContent(i+1);

      tg->SetPoint(i,x,y2);
      tg->SetPointError(i,exl,exh,y2*eyl/y,y2*eyh/y);
   }
}

void getTheory(TString sample, TGraphAsymmErrors* &gth, vector<TH1D*> &hth, TH2D* &hth_cor, bool preFSR, bool correctforacc, var thevar) {
   TFile *fth = NULL;
   TString pdfname;
   int imin1, imax1; // for PDF error sets
   int imin2=-1, imax2=-1; // case when we need 2 ranges (eg for p + Pb variations)
   bool refCT14 = false;

   if (sample=="EPPS16") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_EPPS16.root");
      pdfname = "EPPS16nlo_CT14nlo_Pb208";
      imin1 = 285;
      imax1 = 324;
      imin2 = 112;
      imax2 = 167;
   } else if (sample=="CT14") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_CT14.root");
      pdfname = "CT14nlo";
      imin1 = 112;
      imax1 = 167;
   } else if (sample=="MG5 (CT14)") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_MG5_rewt_CT14nlo_CT14nlo_rewisospin.root");
      pdfname = "CT14nlo";
      imin1 = 1;
      imax1 = 56;
   } else if (sample=="MG5 (EPPS16)") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_MG5_rewt_CT14nlo_EPPS16nlo_CT14nlo_Pb208.root");
      pdfname = "EPPS16nlo_CT14nlo_Pb208";
      imin1 = 1;
      imax1 = 96;
   } else if (sample=="NNPDF3.1") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_NNPDF31_nnlo_as_0118_mc_hessian_pdfas_NNPDF31_nnlo_as_0118_mc_hessian_pdfas_rewisospin.root");
      pdfname = "NNPDF31_nnlo_as_0118_mc_hessian_pdfas";
      imin1 = 1;
      imax1 = 102;
   } else if (sample=="nCTEQ15") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_nCTEQ15FullNuc_208_82.root");
      pdfname = "nCTEQ15FullNuc_208_82";
      imin1 = 1;
      imax1 = 56;
      imin2 = 57;
      imax2 = 88;
      refCT14 = true;
   } else if (sample=="nCTEQ W/Z") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_nCTEQ15WZ_208_82.root");
      pdfname = "nCTEQ15WZ_208_82";
      imin1 = 1;
      imax1 = 56;
      imin2 = 57;
      imax2 = 94;
      refCT14 = true;
   } else if (sample=="nCTEQ W/Z (from CT14)") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_CT14_rewt_CT14nlo_nCTEQ15WZ_208_82.root");
      pdfname = "nCTEQ15WZ_208_82";
      imin1 = 1;
      imax1 = 56;
      imin2 = 57;
      imax2 = 94;
      refCT14 = true;
   } else if (sample=="TUJU19nlopp") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_TUJU19_nlo_1_1_TUJU19_nlo_1_1_rewisospin.root");
      pdfname = "TUJU19_nlo_1_1";
      imin1 = 1;
      imax1 = 26;
   } else if (sample=="TUJU19nlo") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_TUJU19_nlo_1_1_TUJU19_nlo_208_82.root");
      pdfname = "TUJU19_nlo_208_82";
      imin1 = 1;
      imax1 = 58;
   } else if (sample=="TUJU19nnlopp") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_TUJU19_nnlo_1_1_TUJU19_nnlo_1_1_rewisospin.root");
      pdfname = "TUJU19_nnlo_1_1";
      imin1 = 1;
      imax1 = 26;
   } else if (sample=="TUJU19nnlo") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_TUJU19_nnlo_1_1_TUJU19_nnlo_208_82.root");
      pdfname = "TUJU19_nnlo_208_82";
      imin1 = 1;
      imax1 = 58;
   } else if (sample=="nNNPDF1.0") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_nNNPDF10_nlo_as_0118_Pb208.root");
      pdfname = "nNNPDF10_nlo_as_0118_Pb208";
      imin1 = 1;
      imax1 = 56;
      imin2 = 57;
      imax2 = 306;
      refCT14 = true;
   } else if (sample=="nNNPDF2.0") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_nNNPDF20_nlo_as_0118_Pb208.root");
      pdfname = "nNNPDF20_nlo_as_0118_Pb208";
      imin1 = 1;
      imax1 = 56;
      imin2 = 57;
      imax2 = 305;
      refCT14 = true;
   } else {
      cout << "ERROR: unknown sample " << sample << endl;
      return;
   }
   if (!fth->IsOpen()) return;

   int i=0;
   const char* acceffstr = (correctforacc) ? (!preFSR ? "AccTotal" : "AccTotal_pre") : (!preFSR ? "AccPass" : "AccPass_pre");


   hth.push_back((TH1D*) fth->Get(Form("h_%s_%s%d",varname(thevar),acceffstr,i)));
   hth.back()->Scale(1.e-3/lumi_all); // pb -> nb
   Obtain_dSigma_dX(hth.back());
   for (i=imin1; i<=imax1; i++) {
      hth.push_back((TH1D*) fth->Get(Form("h_%s_%s%d",varname(thevar),acceffstr,i)));
      hth.back()->Scale(1.e-3/lumi_all); // pb -> nb
      Obtain_dSigma_dX(hth.back());
   }
   if (imin2>0 && imax2>0) {
      for (i=imin2; i<=imax2; i++) {
         hth.push_back((TH1D*) fth->Get(Form("h_%s_%s%d",varname(thevar),acceffstr,i)));
         hth.back()->Scale(1e-3/lumi_all); // pb -> nb
         Obtain_dSigma_dX(hth.back());
      }
   }

   if (!refCT14) { // normal case: p and Pb PDFs are "consistent" (p error sets are included inside the Pb error sets, eg EPPS or TUJU or proton PDFs)
      gth = pdfuncert(hth, pdfname);
      hth_cor = pdfcorr(hth, pdfname);
   } else { // special case of nCTEQ, nNNPDF
      vector<TH1D*> vCT14, vnPDF;
      TGraphAsymmErrors *gCT14, *gnPDF;
      TH2D *hcorCT14, *hcornPDF;

      vCT14.push_back(hth[0]);
      vnPDF.push_back(hth[0]);
      for (int i=1; i<=(imax1-imin1)+1; i++) vCT14.push_back(hth[i]);
      for (int i=(imax1-imin1)+1;i<=(imax1+imax2-imin1-imin2)+1; i++) vnPDF.push_back(hth[i]);

      gCT14 = pdfuncert(vCT14, "CT14nlo");
      hcorCT14 = pdfcorr(vCT14, "CT14nlo");
      hcorCT14->SetName("cortmp1");
      gnPDF = pdfuncert(vnPDF, pdfname);
      hcornPDF = pdfcorr(vnPDF, pdfname);
      hcornPDF->SetName("cortmp2");

      // now let's combine what we have for CT14 and for the nPDF
      int nbins = gCT14->GetN();
      gth = new TGraphAsymmErrors(nbins);
      for (int i=0; i<nbins; i++) {
         double x = gCT14->GetX()[i];
         double exl = gCT14->GetEXlow()[i];
         double exh = gCT14->GetEXhigh()[i];
         double y1 = gCT14->GetY()[i];
         double eyl1 = gCT14->GetEYlow()[i];
         double eyh1 = gCT14->GetEYhigh()[i];
         double y2 = gnPDF->GetY()[i];
         double eyl2 = gnPDF->GetEYlow()[i];
         double eyh2 = gnPDF->GetEYhigh()[i];

         if (y1 != y2) cout << "ERROR: for " << i << " (" << x << ") we have y1 = " << y1 << " != y2 = " << y2 << endl;
         gth->SetPoint(i,x,y1);
         gth->SetPointError(i, exl, exh, sqrt(pow(eyl1,2)+pow(eyl2,2)), sqrt(pow(eyh1,2)+pow(eyh2,2)));

      }

      hth_cor = (TH2D*) hcorCT14->Clone("htmp");
      for (int i=0; i<nbins; i++) {
         double eyl1i = gCT14->GetEYlow()[i];
         double eyh1i = gCT14->GetEYhigh()[i];
         double eyl2i = gnPDF->GetEYlow()[i];
         double eyh2i = gnPDF->GetEYhigh()[i];
         double ey1i = (eyl1i + eyh1i) / 2.;
         double ey2i = (eyl2i + eyh2i) / 2.;
         double eyi = sqrt(pow(ey1i,2)+pow(ey2i,2));

         for (int j=0; j<nbins; j++) {
            double eyl1j = gCT14->GetEYlow()[j];
            double eyh1j = gCT14->GetEYhigh()[j];
            double eyl2j = gnPDF->GetEYlow()[j];
            double eyh2j = gnPDF->GetEYhigh()[j];
            double ey1j = (eyl1j + eyh1j) / 2.;
            double ey2j = (eyl2j + eyh2j) / 2.;
            double eyj = sqrt(pow(ey1j,2)+pow(ey2j,2));

            double cor1 = hcorCT14->GetBinContent(i+1,j+1);
            double cor2 = hcorCT14->GetBinContent(i+1,j+1);
            double var1 = cor1*ey1i*ey1j;
            double var2 = cor2*ey2i*ey2j;
            double var = var1 + var2;
            double cor = var / (eyi*eyj);
            hth_cor->SetBinContent(i+1,j+1,cor);
            hth_cor->SetBinError(i+1,j+1,0);
         }
      }
   }

   gth->SetMarkerSize(0);
   gth->SetName(Form("gth_%s_%s",sample.Data(),varname(thevar)));
   // clone hth_cor, otherwise it disappears
   hth_cor = (TH2D*) hth_cor->Clone(Form("hth_cor_%s_%s",sample.Data(),varname(thevar)));

   // fth->Close();
}
