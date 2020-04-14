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
      bool preFSR=true) {                                        // true for preFSR, false for postFSR (ie before FSR unfolding)
   TFile *fy = TFile::Open(datafile);
   TFile *fae = TFile::Open(accefffile);

   TFile *fout = TFile::Open(outputfile,"RECREATE");

   ofstream chi2file(Form("Plots/results/tex/chi2%s.tex",correctforacc ? "" : "_noacc"));
   ofstream chi2file_nocor(Form("Plots/results/tex/chi2_nocor%s.tex",correctforacc ? "" : "_noacc"));
   chi2file << "\\begin{tabular}{lcccccc}" << endl;
   chi2file << "\\multirow{2}{4em}{Observable} & \\multicolumn{3}{c}{CT14} & \\multicolumn{3}{c}{CT14+EPPS16} \\\\" << endl;
   chi2file << "& $\\chi^{2}$ & dof & Prob. [\\%] & $\\chi^{2}$ & dof & Prob. [\\%] \\\\" << endl;
   chi2file << "\\hline" << endl;
   chi2file_nocor << "\\begin{tabular}{lcccccc}" << endl;
   chi2file_nocor << "\\multirow{2}{4em}{Observable} & \\multicolumn{3}{c}{CT14} & \\multicolumn{3}{c}{CT14+EPPS16} \\\\" << endl;
   chi2file_nocor << "& $\\chi^{2}$ & dof & Prob. [\\%] & $\\chi^{2}$ & dof & Prob. [\\%] \\\\" << endl;
   chi2file_nocor << "\\hline" << endl;

   // loop on the variables
   for (int ivar=0; ivar<ALLvar; ivar++) {
   // for (int ivar=0; ivar<pt; ivar++) { // only mass for now
      var thevar = static_cast<var>(ivar);
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
            cout << "Error, can't find yield histo in input file." << endl;
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
         ytitle = "d#sigma/dM [nb/GeV]";
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

      MyCanvas c1(Form("Plots/results/plots/result%s_%s",correctforacc ? "" : "_noacc",varname(thevar)),xtitle,ytitle,lx,ly);
      if (logx) c1.SetLogx();
      if (logy) c1.SetLogy(false);

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
      TFile *fth_EPPS16 = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_EPPS16.root");
      vector<TH1D*> hth_EPPS16;
      int i=0;
      const char* acceffstr = (correctforacc) ? (!preFSR ? "AccTotal" : "AccTotal_pre") : (!preFSR ? "AccPass" : "AccPass_pre");

      TGraphAsymmErrors *gth_EPPS16 = NULL;
      TH2D *hth_EPPS16_cor = NULL;
      if (fth_EPPS16->IsOpen()) { // skip the theory part if we don't want dsigma/dX
         hth_EPPS16.push_back((TH1D*) fth_EPPS16->Get(Form("h_%s_%s%d",varname(thevar),acceffstr,i)));
         hth_EPPS16.back()->Scale(1.e-3/lumi_all); // pb -> nb
         Obtain_dSigma_dX(hth_EPPS16.back());
         for (i=285; i<=324; i++) {
            hth_EPPS16.push_back((TH1D*) fth_EPPS16->Get(Form("h_%s_%s%d",varname(thevar),acceffstr,i)));
            hth_EPPS16.back()->Scale(1.e-3/lumi_all); // pb -> nb
            Obtain_dSigma_dX(hth_EPPS16.back());
         }
         for (i=112; i<=167; i++) {
            hth_EPPS16.push_back((TH1D*) fth_EPPS16->Get(Form("h_%s_%s%d",varname(thevar),acceffstr,i)));
            hth_EPPS16.back()->Scale(1e-3/lumi_all); // pb -> nb
            Obtain_dSigma_dX(hth_EPPS16.back());
         }


         gth_EPPS16 = pdfuncert(hth_EPPS16, "EPPS16nlo_CT14nlo_Pb208");
         hth_EPPS16_cor = pdfcorr(hth_EPPS16, "EPPS16nlo_CT14nlo_Pb208");
         gth_EPPS16->SetMarkerSize(0);
         gth_EPPS16->SetName(Form("gth_EPPS16_%s",varname(thevar)));
         hth_EPPS16_cor->SetName(Form("hth_EPPS16_cor_%s",varname(thevar)));
      }

      // CT14
      TFile *fth_CT14 = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_CT14.root");
      vector<TH1D*> hth_CT14;
      i=0;
      TGraphAsymmErrors *gth_CT14 = NULL;
      TH2D *hth_CT14_cor = NULL;
      if (fth_CT14->IsOpen()) { // skip the theory part if we don't want dsigma/dX
         hth_CT14.push_back((TH1D*) fth_CT14->Get(Form("h_%s_%s%d",varname(thevar),acceffstr,i)));
         hth_CT14.back()->Scale(1.e-3/lumi_all); // pb -> nb
         Obtain_dSigma_dX(hth_CT14.back());
         for (i=112; i<=167; i++) {
            hth_CT14.push_back((TH1D*) fth_CT14->Get(Form("h_%s_%s%d",varname(thevar),acceffstr,i)));
            hth_CT14.back()->Scale(1.e-3/lumi_all); // pb -> nb
            Obtain_dSigma_dX(hth_CT14.back());
         }

         gth_CT14 = pdfuncert(hth_CT14, "CT14nlo");
         hth_CT14_cor = pdfcorr(hth_CT14, "CT14nlo");
         gth_CT14->SetMarkerSize(0);
         gth_CT14->SetName(Form("gth_CT14_%s",varname(thevar)));
         hth_CT14_cor->SetName(Form("hth_CT14_cor_%s",varname(thevar)));
      }


      if (doxsec && !forsyst) {
         if (thevar==var::rap60120 || thevar==var::rap1560) c1.SetYRange(14,69);
         if (!correctforacc && thevar==var::rap1560) c1.SetYRange(0,10);
         if (!correctforacc && thevar==var::rap60120) c1.SetYRange(0,49);
         if (thevar==var::mass && correctforacc) c1.SetYRange(2e-4,90);
         if (thevar==var::mass && !correctforacc) c1.SetYRange(2e-4,20);
         if (thevar==var::pt1560 && correctforacc) c1.SetYRange(5e-4,7e1);
         if (thevar==var::pt1560 && !correctforacc) c1.SetYRange(2e-4,2);
         if (thevar==var::phistar1560 && correctforacc) c1.SetYRange(2,2e3);
         if (thevar==var::phistar1560 && !correctforacc) c1.SetYRange(4e-1,2e2);
         if (thevar==var::pt && correctforacc) c1.SetYRange(8e-4,2e1);
         if (thevar==var::pt && !correctforacc) c1.SetYRange(8e-4,9);
         if (thevar==var::phistar && correctforacc) c1.SetYRange(3e-1,4e3);
         if (thevar==var::phistar && !correctforacc) c1.SetYRange(2e-1,2e3);

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

         // c1.PrintVariables();
         c1.CanvasWithThreeGraphsRatioPlot(gth_CT14,gth_EPPS16,gres,
               "Powheg (CT14)","Powheg (EPPS16)","Data","Powheg/Data",
               kBlue,kRed,kBlack,
               "5","5","EP",true);

         // add label
         c1.TopPad->cd();
         TLatex latex;
         latex.SetNDC();
         latex.SetTextSize(0.03);
         double xlatex=.2, ylatex=0.5, dylatex=0.04;
         if (thevar==var::rap1560 || thevar==rap60120) ylatex=0.85;//0.9 (Hyunchul)
         latex.SetTextAlign(12);  //centered
         if (thevar!=rap1560 && thevar!=rap60120) {
            latex.DrawLatex(xlatex,ylatex,"-2.87 < |y_{CM}| < 1.93");
            ylatex -= dylatex;
         }
         if (thevar==pt1560 || thevar==phistar1560 || thevar==rap1560) {
            latex.DrawLatex(xlatex,ylatex,"15 < M < 60 GeV");
            ylatex -= dylatex;
         }
         if (thevar==pt || thevar==phistar || thevar==rap60120) {
            latex.DrawLatex(xlatex,ylatex,"60 < M < 120 GeV");
            ylatex -= dylatex;
         }
         if (!correctforacc) {
            latex.DrawLatex(xlatex,ylatex,"|#eta_{lab}^{#mu}|<2.4, p_{T}^{#mu} > 15 (10) GeV");
            ylatex -= dylatex;
         }

         c1.PrintCanvas();
         c1.PrintCanvas_C();

         // write the ratio to file
         TFile *fratio = TFile::Open("Plots/results/expthratios.root","UPDATE");
         c1.g_ratio1->Write(Form("%s_CT14",varname(thevar)));
         c1.g_ratio2->Write(Form("%s_EPPS16",varname(thevar)));
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
         cout << varname(thevar) << ", CT14, w/o cor: chi2/ndf = " << chi2_CT14_nocor << "/" << nbins << " (p = " << TMath::Prob(chi2_CT14_nocor,nbins) << ")" << endl;
         cout << varname(thevar) << ", EPPS16, w/o cor: chi2/ndf = " << chi2_EPPS16_nocor << "/" << nbins << " (p = " << TMath::Prob(chi2_EPPS16_nocor,nbins) << ")" << endl;

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
         cout << varname(thevar) << ", CT14, w/ cor: chi2/ndf = " << chi2_CT14_withcor << "/" << nbins << " (p = " << TMath::Prob(chi2_CT14_withcor,nbins) << ")" << endl;
         cout << varname(thevar) << ", EPPS16, w/ cor: chi2/ndf = " << chi2_EPPS16_withcor << "/" << nbins << " (p = " << TMath::Prob(chi2_EPPS16_withcor,nbins) << ")" << endl;

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
