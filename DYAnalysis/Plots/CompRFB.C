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
void getTheory(TString sample, TGraphAsymmErrors* &gth, var thevar);

using namespace std;

void CompRFB (const char* infile="Plots/results/xsec_nom_detcor_FSR.root", TString sample1="CT14", TString sample2="EPPS16") {
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
   //###TFile *fth_EPPS16 = TFile::Open("/afs/cern.ch/work/e/echapon/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16.root");

   TGraphAsymmErrors *gth1560_EPPS16 = NULL, *gth60120_EPPS16 = NULL;
   getTheory(sample2, gth1560_EPPS16, DYana::var::rap1560);
   getTheory(sample2, gth60120_EPPS16, DYana::var::rap60120);

   // CT14
   //###TFile *fth_CT14 = TFile::Open("/afs/cern.ch/work/e/echapon/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_CT14.root");
   TGraphAsymmErrors *gth1560_CT14 = NULL, *gth60120_CT14 = NULL;
   getTheory(sample1, gth1560_CT14, DYana::var::rap1560);
   getTheory(sample1, gth60120_CT14, DYana::var::rap60120);

   // do the plotting here

   MyCanvas c_1560("Plots/grfbp_rap1560_" + sample1 + "_" + sample2, "|y_{CM}|", "R_{FB}", 800, 800);
   c_1560.SetYRange(0.45,1.85);
   cout << gth1560_CT14 << " " << gth1560_EPPS16 << " " << grfbp_rap1560 << endl;
   c_1560.CanvasWithThreeGraphsRatioPlot(gth1560_CT14,gth1560_EPPS16,grfbp_rap1560,
         sample1,sample2,"Data","Pred./Data",
         kBlue,kRed,kBlack,
         "5","5","EP",true);
   c_1560.TopPad->cd();
   TLatex latex;
   latex.SetNDC();
   latex.SetTextSize(0.03);
   double xlatex=.2, ylatex=0.85, dylatex=0.045;//0.9,0.040
   latex.DrawLatex(xlatex,ylatex,"15 < m_{#mu#mu} < 60 GeV");
   c_1560.PrintCanvas();

   MyCanvas c_60120("Plots/grfbp_rap60120_" + sample1 + "_" + sample2, "|y_{CM}|", "R_{FB}", 800, 800);
   c_60120.SetYRange(0.65,1.45);//1.35
   c_60120.CanvasWithThreeGraphsRatioPlot(gth60120_CT14,gth60120_EPPS16,grfbp_rap60120,
         sample1,sample2,"Data","Pred./Data",
         kBlue,kRed,kBlack,
         "5","5","EP",true);
   c_60120.TopPad->cd();
   ylatex=0.85;//0.9
   latex.DrawLatex(xlatex,ylatex,"60 < m_{#mu#mu} < 120 GeV");
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

void getTheory(TString sample, TGraphAsymmErrors* &gth, var thevar) {
   TFile *fth = NULL;
   vector<TH1D*> hth;
   bool preFSR = true;
   bool correctforacc = true;
   TString pdfname;
   int imin1, imax1; // for PDF error sets
   int imin2=-1, imax2=-1; // case when we need 2 ranges (eg for p + Pb variations)
   bool refCT14 = false;
   TMatrixT<double> m0(DYana::nbinsvar(thevar),DYana::nbinsvar(thevar));

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
      imax2 = 90;
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
      imax2 = 307;
      refCT14 = true;
   } else if (sample=="nNNPDF2.0") {
      fth = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_nNNPDF20_nlo_as_0118_Pb208.root");
      pdfname = "nNNPDF20_nlo_as_0118_Pb208";
      imin1 = 1;
      imax1 = 56;
      imin2 = 57;
      imax2 = 307;
      refCT14 = true;
   } else {
      cout << "ERROR: unknown sample " << sample << endl;
      return;
   }
   if (!fth->IsOpen()) return;

   int i=0;
   const char* acceffstr = (correctforacc) ? (!preFSR ? "AccTotal" : "AccTotal_pre") : (!preFSR ? "AccPass" : "AccPass_pre");


   if (thevar==DYana::var::rap1560) {
      hth.push_back(RFB_1560((TH1D*) fth->Get(Form("h_rap1560_AccTotal_pre%d",i)),NULL,m0));
      for (i=imin1; i<=imax1; i++) {
         hth.push_back(RFB_1560((TH1D*) fth->Get(Form("h_rap1560_AccTotal_pre%d",i)),NULL,m0));
      }
      if (imin2>0 && imax2>0) {
         for (i=imin2; i<=imax2; i++) {
            hth.push_back(RFB_1560((TH1D*) fth->Get(Form("h_rap1560_AccTotal_pre%d",i)),NULL,m0));
         }
      }
   } else {
      hth.push_back(RFB_60120((TH1D*) fth->Get(Form("h_rap60120_AccTotal_pre%d",i)),NULL,m0));
      for (i=imin1; i<=imax1; i++) {
         hth.push_back(RFB_60120((TH1D*) fth->Get(Form("h_rap60120_AccTotal_pre%d",i)),NULL,m0));
      }
      if (imin2>0 && imax2>0) {
         for (i=imin2; i<=imax2; i++) {
            hth.push_back(RFB_60120((TH1D*) fth->Get(Form("h_rap60120_AccTotal_pre%d",i)),NULL,m0));
         }
      }
   }

   if (!refCT14) { // normal case: p and Pb PDFs are "consistent" (p error sets are included inside the Pb error sets, eg EPPS or TUJU or proton PDFs)
      gth = pdfuncert(hth, pdfname,true);
   } else { // special case of nCTEQ, nNNPDF
      vector<TH1D*> vCT14, vnPDF;
      TGraphAsymmErrors *gCT14, *gnPDF;

      vCT14.push_back(hth[0]);
      vnPDF.push_back(hth[0]);
      for (int i=1; i<(imax1-imin1)+1; i++) vCT14.push_back(hth[i]);
      for (int i=(imax1-imin1)+1;i<(imax1+imax2-imin1-imin2); i++) vnPDF.push_back(hth[i]);

      gCT14 = pdfuncert(vCT14, "CT14nlo", true);
      gnPDF = pdfuncert(vnPDF, pdfname, true);

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
   }

   gth->SetMarkerSize(0);
   gth->SetName(Form("gth_%s_%s",sample.Data(),varname(thevar)));

   // fth->Close();
}
