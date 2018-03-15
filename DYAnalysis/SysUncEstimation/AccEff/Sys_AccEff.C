#include "../../BkgEst/interface/defs.h"
#include "../../Include/MyCanvas.C"
#include "../../Include/texUtils.h"
#include "../../Include/lhapdf_utils.h"
#include "../syst.C"

#include "TFile.h"
#include "TH1.h"
#include "TMatrixT.h"

#include <vector>

using namespace std;
using DYana::var;

// HOW TO USE THIS FILE
// [0] .include /dir/to/lhapdf/include
// [1] gSystem->Load("/dir/to/lhapdf/lib/libLHAPDF.so");
// .L AccEff/Sys_AccEff.C+

TH2D* Sys_AccEff_scales(const char* file, var thevar, TGraphAsymmErrors *&gAcc, TGraphAsymmErrors *&gEff, TGraphAsymmErrors *&gAccEff) {
   TFile *fin = TFile::Open(file);

   vector<TH1D*> hAccTotal, hAccPass, hEffTotal, hEffPass_Corr_tnp;
   vector<TMatrixT<double> > mcor;
   vector<TGraphAsymmErrors*> gAccEffs;
   vector<TString> graphNames;

   int idx[7] = {0, 8, 4, 6, 3, 2, 1}; // muR, muF = (1, 1), (0.5, 0.5), (2, 2), (0.5, 1), (2, 1), (1, 0.5), (1, 2)
   const char* gn[7] = {"(1,1)", "(0.5,0.5)", "(2,2)", "(0.5,1)", "(2,1)", "(1,0.5)", "(1,2)"};
   MyCanvas c1(Form("AccEff/AccEff_scales_%s",varname(thevar)),xaxistitle(thevar),"Acc #times Eff",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar) c1.SetLogx();

   for (int i=0; i<7; i++) {
      hAccTotal.push_back((TH1D*) fin->Get(Form("h_%s_AccTotal%d",varname(thevar),idx[i])));
      hAccPass.push_back((TH1D*) fin->Get(Form("h_%s_AccPass%d",varname(thevar),idx[i])));
      hEffTotal.push_back((TH1D*) fin->Get(Form("h_%s_EffTotal%d",varname(thevar),idx[i])));
      hEffPass_Corr_tnp.push_back((TH1D*) fin->Get(Form("h_%s_EffPass_Corr_tnp%d",varname(thevar),idx[i])));
   }

   gAcc = new TGraphAsymmErrors(hAccPass[0],hAccTotal[0]);
   gAcc->SetName("gAcc_scales");
   gEff = new TGraphAsymmErrors(hEffPass_Corr_tnp[0],hEffTotal[0]);
   gEff->SetName("gEff_scales");
   gAccEff = new TGraphAsymmErrors(hEffPass_Corr_tnp[0],hAccTotal[0]);
   gAccEff->SetName("gAccEff_scales");
   gAccEffs.push_back(gAccEff);
   graphNames.push_back(gn[0]);

   int nbins = gAcc->GetN();
   // put a dummy empty matrix at i=0
   mcor.push_back(TMatrixT<double>(nbins,nbins));

   for (int i=1; i<7; i++) {
      TGraphAsymmErrors *gAccTmp = new TGraphAsymmErrors(hAccPass[i],hAccTotal[i]);
      TGraphAsymmErrors *gEffTmp = new TGraphAsymmErrors(hEffPass_Corr_tnp[i],hEffTotal[i]);
      TGraphAsymmErrors *gAccEffTmp = new TGraphAsymmErrors(hEffPass_Corr_tnp[i],hAccTotal[i]);
      gAccEffs.push_back(gAccEffTmp);
      graphNames.push_back(gn[i]);

      mcor.push_back(TMatrixT<double>(nbins,nbins));

      for (int j=0; j<nbins; j++) {
         if (i==1) {
            gAcc->SetPointEYlow(j,0);
            gAcc->SetPointEYhigh(j,0);
            gEff->SetPointEYlow(j,0);
            gEff->SetPointEYhigh(j,0);
            gAccEff->SetPointEYlow(j,0);
            gAccEff->SetPointEYhigh(j,0);
         }
         gAcc->SetPointEYlow(j,-min(gAccTmp->GetY()[j]-gAcc->GetY()[j],-gAcc->GetEYlow()[j]));
         gAcc->SetPointEYhigh(j,max(gAccTmp->GetY()[j]-gAcc->GetY()[j],gAcc->GetEYhigh()[j]));
         gEff->SetPointEYlow(j,-min(gEffTmp->GetY()[j]-gEff->GetY()[j],-gEff->GetEYlow()[j]));
         gEff->SetPointEYhigh(j,max(gEffTmp->GetY()[j]-gEff->GetY()[j],gEff->GetEYhigh()[j]));
         gAccEff->SetPointEYlow(j,-min(gAccEffTmp->GetY()[j]-gAccEff->GetY()[j],-gAccEff->GetEYlow()[j]));
         gAccEff->SetPointEYhigh(j,max(gAccEffTmp->GetY()[j]-gAccEff->GetY()[j],gAccEff->GetEYhigh()[j]));

         double diffj = gAccTmp->GetY()[j]-gAcc->GetY()[j];
         for (int k=0; k<nbins; k++) {
            double diffk = gAccTmp->GetY()[k]-gAcc->GetY()[k];
            mcor[i][j][k] = diffj*diffk;
         }
      } // loop on bins

      // turn the cov matrix into a correlation matrix
      for (int j=0; j<nbins; j++) {
         for (int k=0; k<nbins; k++) {
            if (j==k) continue;
            mcor[i][j][k] *= 1./sqrt(fabs(mcor[i][j][j]*mcor[i][k][k]));
         }
      }
      for (int j=0; j<nbins; j++) mcor[i][j][j] = 1;
   } // loop on scales

   // compute ratio to first graph
   for (vector<TGraphAsymmErrors*>::iterator it=gAccEffs.begin()+1; it!=gAccEffs.end(); it++) 
      for (int i=0; i<(*it)->GetN(); i++)
         (*it)->SetPoint(i,(*it)->GetX()[i],(*it)->GetY()[i]/gAccEffs[0]->GetY()[i]);

   for (int i=0; i<gAccEffs[0]->GetN(); i++) {
      // rescale output
      gAcc->SetPointEYlow(i,gAcc->GetEYlow()[i]/gAcc->GetY()[i]);
      gAcc->SetPointEYhigh(i,gAcc->GetEYhigh()[i]/gAcc->GetY()[i]);
      gEff->SetPointEYlow(i,gEff->GetEYlow()[i]/gEff->GetY()[i]);
      gEff->SetPointEYhigh(i,gEff->GetEYhigh()[i]/gEff->GetY()[i]);
      gAccEff->SetPointEYlow(i,gAccEff->GetEYlow()[i]/gAccEff->GetY()[i]);
      gAccEff->SetPointEYhigh(i,gAccEff->GetEYhigh()[i]/gAccEff->GetY()[i]);

      gAccEffs[0]->SetPoint(i,gAccEffs[0]->GetX()[i],1);
   }

   // produce the correlation matrix as the average of the individual correlation matrices
   TH2D *ans = new TH2D(TString(hEffPass_Corr_tnp[0]->GetName())+"_scalecorr",TString(hEffPass_Corr_tnp[0]->GetTitle())+" (scales corr)",
         nbins,hEffPass_Corr_tnp[0]->GetXaxis()->GetXbins()->GetArray(),
         nbins,hEffPass_Corr_tnp[0]->GetXaxis()->GetXbins()->GetArray());
   int nvars = mcor.size();
   for (int j=0; j<nbins; j++) {
      for (int k=0; k<nbins; k++) {
         double val=0;
         for (int i=1; i<nvars; i++) val += mcor[i][j][k];
         ans->SetBinContent(j+1,k+1,val/(nvars-1));
      }
   }

   c1.SetYRange(0.96,1.04);
   c1.CanvasWithMultipleGraphs(gAccEffs, graphNames, "LPX");
   c1.PrintCanvas();
   c1.PrintCanvas_C();

   return ans;
}

TH2D* Sys_AccEff_alphas(const char* file, var thevar, TGraphAsymmErrors *&gAcc, TGraphAsymmErrors *&gEff, TGraphAsymmErrors *&gAccEff) {
   TFile *fin = TFile::Open(file);

   vector<TH1D*> hAccTotal, hAccPass, hEffTotal, hEffPass_Corr_tnp;
   vector<TMatrixT<double> > mcor;
   vector<TGraphAsymmErrors*> gAccEffs;
   vector<TString> graphNames;

   int idx[3] = {0, 168, 169}; // nominal, 0.117, 0.119 
   const char* gn[3] = {"#alpha_{S}=0.118","#alpha_{S}=0.117","#alpha_{S}=0.119"};
   MyCanvas c1(Form("AccEff/AccEff_alphas_%s",varname(thevar)),xaxistitle(thevar),"Acc #times Eff",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar) c1.SetLogx();

   for (int i=0; i<3; i++) {
      hAccTotal.push_back((TH1D*) fin->Get(Form("h_%s_AccTotal%d",varname(thevar),idx[i])));
      hAccPass.push_back((TH1D*) fin->Get(Form("h_%s_AccPass%d",varname(thevar),idx[i])));
      hEffTotal.push_back((TH1D*) fin->Get(Form("h_%s_EffTotal%d",varname(thevar),idx[i])));
      hEffPass_Corr_tnp.push_back((TH1D*) fin->Get(Form("h_%s_EffPass_Corr_tnp%d",varname(thevar),idx[i])));
   }

   gAcc = new TGraphAsymmErrors(hAccPass[0],hAccTotal[0]);
   gAcc->SetName("gAcc_alphas");
   gEff = new TGraphAsymmErrors(hEffPass_Corr_tnp[0],hEffTotal[0]);
   gEff->SetName("gEff_alphas");
   gAccEff = new TGraphAsymmErrors(hEffPass_Corr_tnp[0],hAccTotal[0]);
   gAccEff->SetName("gAccEff_alphas");
   gAccEffs.push_back(gAccEff);
   graphNames.push_back(gn[0]);

   int nbins = gAcc->GetN();
   // put a dummy empty matrix at i=0
   mcor.push_back(TMatrixT<double>(nbins,nbins));

   for (int i=1; i<3; i++) {
      TGraphAsymmErrors *gAccTmp = new TGraphAsymmErrors(hAccPass[i],hAccTotal[i]);
      TGraphAsymmErrors *gEffTmp = new TGraphAsymmErrors(hEffPass_Corr_tnp[i],hEffTotal[i]);
      TGraphAsymmErrors *gAccEffTmp = new TGraphAsymmErrors(hEffPass_Corr_tnp[i],hAccTotal[i]);
      gAccEffs.push_back(gAccEffTmp);
      graphNames.push_back(gn[i]);

      mcor.push_back(TMatrixT<double>(nbins,nbins));

      for (int j=0; j<gAccTmp->GetN(); j++) {
         if (i==1) {
            gAcc->SetPointEYlow(j,0);
            gAcc->SetPointEYhigh(j,0);
            gEff->SetPointEYlow(j,0);
            gEff->SetPointEYhigh(j,0);
            gAccEff->SetPointEYlow(j,0);
            gAccEff->SetPointEYhigh(j,0);
         }
         gAcc->SetPointEYlow(j,-min(gAccTmp->GetY()[j]-gAcc->GetY()[j],-gAcc->GetEYlow()[j]));
         gAcc->SetPointEYhigh(j,max(gAccTmp->GetY()[j]-gAcc->GetY()[j],gAcc->GetEYhigh()[j]));
         gEff->SetPointEYlow(j,-min(gEffTmp->GetY()[j]-gEff->GetY()[j],-gEff->GetEYlow()[j]));
         gEff->SetPointEYhigh(j,max(gEffTmp->GetY()[j]-gEff->GetY()[j],gEff->GetEYhigh()[j]));
         gAccEff->SetPointEYlow(j,-min(gAccEffTmp->GetY()[j]-gAccEff->GetY()[j],-gAccEff->GetEYlow()[j]));
         gAccEff->SetPointEYhigh(j,max(gAccEffTmp->GetY()[j]-gAccEff->GetY()[j],gAccEff->GetEYhigh()[j]));

         double diffj = gAccTmp->GetY()[j]-gAcc->GetY()[j];
         for (int k=0; k<nbins; k++) {
            double diffk = gAccTmp->GetY()[k]-gAcc->GetY()[k];
            mcor[i][j][k] = diffj*diffk;
         }
      } // loop on bins

      // turn the cov matrix into a correlation matrix
      for (int j=0; j<nbins; j++) {
         for (int k=0; k<nbins; k++) {
            if (j==k) continue;
            mcor[i][j][k] *= 1./sqrt(fabs(mcor[i][j][j]*mcor[i][k][k]));
         }
      }
      for (int j=0; j<nbins; j++) mcor[i][j][j] = 1;
   } // loop on scales

   // compute ratio to first graph
   for (vector<TGraphAsymmErrors*>::iterator it=gAccEffs.begin()+1; it!=gAccEffs.end(); it++) 
      for (int i=0; i<(*it)->GetN(); i++)
         (*it)->SetPoint(i,(*it)->GetX()[i],(*it)->GetY()[i]/gAccEffs[0]->GetY()[i]);

   for (int i=0; i<gAccEffs[0]->GetN(); i++) {
      // rescale output
      gAcc->SetPointEYlow(i,gAcc->GetEYlow()[i]/gAcc->GetY()[i]);
      gAcc->SetPointEYhigh(i,gAcc->GetEYhigh()[i]/gAcc->GetY()[i]);
      gEff->SetPointEYlow(i,gEff->GetEYlow()[i]/gEff->GetY()[i]);
      gEff->SetPointEYhigh(i,gEff->GetEYhigh()[i]/gEff->GetY()[i]);
      gAccEff->SetPointEYlow(i,gAccEff->GetEYlow()[i]/gAccEff->GetY()[i]);
      gAccEff->SetPointEYhigh(i,gAccEff->GetEYhigh()[i]/gAccEff->GetY()[i]);

      gAccEffs[0]->SetPoint(i,gAccEffs[0]->GetX()[i],1);
   }

   // produce the correlation matrix as the average of the individual correlation matrices
   TH2D *ans = new TH2D(TString(hEffPass_Corr_tnp[0]->GetName())+"_alphascorr",TString(hEffPass_Corr_tnp[0]->GetTitle())+" (alphaS corr)",
         nbins,hEffPass_Corr_tnp[0]->GetXaxis()->GetXbins()->GetArray(),
         nbins,hEffPass_Corr_tnp[0]->GetXaxis()->GetXbins()->GetArray());
   int nvars = mcor.size();
   for (int j=0; j<nbins; j++) {
      for (int k=0; k<nbins; k++) {
         double val=0;
         for (int i=1; i<nvars; i++) val += mcor[i][j][k];
         ans->SetBinContent(j+1,k+1,val/(nvars-1));
      }
   }

   c1.SetYRange(0.99,1.01);
   c1.CanvasWithMultipleGraphs(gAccEffs, graphNames, "LPX");
   c1.PrintCanvas();
   c1.PrintCanvas_C();
   
   // scale by a factor 1.5 the errors, because PDF4LHC recommends delta alpha_S = 0.0015 instead of 0.001
   for (int j=0; j<gAcc->GetN(); j++) {
      gAcc->SetPointEYlow(j,1.5*gAcc->GetEYlow()[j]);
      gAcc->SetPointEYhigh(j,1.5*gAcc->GetEYhigh()[j]);
      gEff->SetPointEYlow(j,1.5*gEff->GetEYlow()[j]);
      gEff->SetPointEYhigh(j,1.5*gEff->GetEYhigh()[j]);
      gAccEff->SetPointEYlow(j,1.5*gAccEff->GetEYlow()[j]);
      gAccEff->SetPointEYhigh(j,1.5*gAccEff->GetEYhigh()[j]);
   }

   return ans;
}

TH2D* Sys_AccEff_EPPS16(const char* file, var thevar, TGraphAsymmErrors *&gAcc, TGraphAsymmErrors *&gEff, TGraphAsymmErrors *&gAccEff) {
   TFile *fin = TFile::Open(file);

   vector<TH1D*> hAcc, hEff, hAccEff;
   vector<TString> histNames;
   MyCanvas c1(Form("AccEff/AccEff_EPPS16_%s",varname(thevar)),xaxistitle(thevar),"Acc #times Eff",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar) c1.SetLogx();

   int i=0;
   hAcc.push_back((TH1D*) fin->Get(Form("h_%s_AccPass%d",varname(thevar),i)));
   hAcc.back()->Divide((TH1D*) fin->Get(Form("h_%s_AccTotal%d",varname(thevar),i)));
   hEff.push_back((TH1D*) fin->Get(Form("h_%s_EffPass_Corr_tnp%d",varname(thevar),i)));
   hEff.back()->Divide((TH1D*) fin->Get(Form("h_%s_EffTotal%d",varname(thevar),i)));
   hAccEff.push_back((TH1D*) fin->Get(Form("h_%s_EffPass_Corr_tnp%d",varname(thevar),i)));
   hAccEff.back()->Multiply((TH1D*) fin->Get(Form("h_%s_EffTotal%d",varname(thevar),i)));
   hAccEff.back()->Divide((TH1D*) fin->Get(Form("h_%s_AccTotal%d",varname(thevar),i)));
   histNames.push_back(Form("%d",i));

   for (i=285; i<=324; i++) {
      hAcc.push_back((TH1D*) fin->Get(Form("h_%s_AccPass%d",varname(thevar),i)));
      hAcc.back()->Divide((TH1D*) fin->Get(Form("h_%s_AccTotal%d",varname(thevar),i)));
      hEff.push_back((TH1D*) fin->Get(Form("h_%s_EffPass_Corr_tnp%d",varname(thevar),i)));
      hEff.back()->Divide((TH1D*) fin->Get(Form("h_%s_EffTotal%d",varname(thevar),i)));
      hAccEff.push_back((TH1D*) fin->Get(Form("h_%s_EffPass_Corr_tnp%d",varname(thevar),i)));
      hAccEff.back()->Multiply((TH1D*) fin->Get(Form("h_%s_EffTotal%d",varname(thevar),i)));
      hAccEff.back()->Divide((TH1D*) fin->Get(Form("h_%s_AccTotal%d",varname(thevar),i)));
      histNames.push_back(Form("%d",i-284));
   }

   for (i=112; i<=167; i++) {
      hAcc.push_back((TH1D*) fin->Get(Form("h_%s_AccPass%d",varname(thevar),i)));
      hAcc.back()->Divide((TH1D*) fin->Get(Form("h_%s_AccTotal%d",varname(thevar),i)));
      hEff.push_back((TH1D*) fin->Get(Form("h_%s_EffPass_Corr_tnp%d",varname(thevar),i)));
      hEff.back()->Divide((TH1D*) fin->Get(Form("h_%s_EffTotal%d",varname(thevar),i)));
      hAccEff.push_back((TH1D*) fin->Get(Form("h_%s_EffPass_Corr_tnp%d",varname(thevar),i)));
      hAccEff.back()->Multiply((TH1D*) fin->Get(Form("h_%s_EffTotal%d",varname(thevar),i)));
      hAccEff.back()->Divide((TH1D*) fin->Get(Form("h_%s_AccTotal%d",varname(thevar),i)));
      histNames.push_back(Form("%d",i-111+40));
   }

   gAcc = pdfuncert(hAcc, "EPPS16nlo_CT14nlo_Pb208");
   gEff = pdfuncert(hEff, "EPPS16nlo_CT14nlo_Pb208");
   gAccEff = pdfuncert(hAccEff, "EPPS16nlo_CT14nlo_Pb208");

   // compute ratio to first graph
   for (vector<TH1D*>::iterator it=hAccEff.begin()+1; it!=hAccEff.end(); it++) (*it)->Divide(hAccEff[0]);
   hAccEff[0]->Divide(hAccEff[0]);

   c1.SetYRange(0.90,1.10);
   c1.CanvasWithMultipleHistograms(hAccEff, histNames, "HIST LP");
   c1.PrintCanvas();
   c1.PrintCanvas_C();

   return  pdfcorr(hAccEff, "EPPS16nlo_CT14nlo_Pb208");
}

TH2D* Sys_AccEff_Zpt(const char* file, var thevar, TGraphAsymmErrors *&gAcc, TGraphAsymmErrors *&gEff, TGraphAsymmErrors *&gAccEff) {
   TFile *fin[2] = {0};
   fin[0] = TFile::Open(file);
   TString file_nozpt("../ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth_noZptrew.root");
   fin[1] = TFile::Open(file_nozpt);

   vector<TH1D*> hAccTotal, hAccPass, hEffTotal, hEffPass_Corr_tnp;
   vector<TGraphAsymmErrors*> gAccEffs;
   vector<TString> graphNames;

   const char* gn[2] = {"Nominal", "No p_{T}^{Z} reweighting"};
   MyCanvas c1(Form("AccEff/AccEff_Zpt_%s",varname(thevar)),xaxistitle(thevar),"Acc #times Eff",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar) c1.SetLogx();

   hAccTotal.push_back((TH1D*) fin[0]->Get(Form("h_%s_AccTotal0",varname(thevar))));
   hAccPass.push_back((TH1D*) fin[0]->Get(Form("h_%s_AccPass0",varname(thevar))));
   hEffTotal.push_back((TH1D*) fin[0]->Get(Form("h_%s_EffTotal0",varname(thevar))));
   hEffPass_Corr_tnp.push_back((TH1D*) fin[0]->Get(Form("h_%s_EffPass_Corr_tnp0",varname(thevar))));
   hAccTotal.push_back((TH1D*) fin[1]->Get(Form("h_%s_AccTotal",varname(thevar))));
   hAccPass.push_back((TH1D*) fin[1]->Get(Form("h_%s_AccPass",varname(thevar))));
   hEffTotal.push_back((TH1D*) fin[1]->Get(Form("h_%s_EffTotal",varname(thevar))));
   hEffPass_Corr_tnp.push_back((TH1D*) fin[1]->Get(Form("h_%s_EffPass_Corr_tnp0",varname(thevar))));

   gAcc = new TGraphAsymmErrors(hAccPass[0],hAccTotal[0]);
   gAcc->SetName("gAcc_Zpt");
   gEff = new TGraphAsymmErrors(hEffPass_Corr_tnp[0],hEffTotal[0]);
   gEff->SetName("gEff_Zpt");
   gAccEff = new TGraphAsymmErrors(hEffPass_Corr_tnp[0],hAccTotal[0]);
   gAccEff->SetName("gAccEff_Zpt");
   gAccEffs.push_back(gAccEff);
   graphNames.push_back(gn[0]);

   int nbins = gAcc->GetN();
   TH2D *ans = new TH2D(TString(hEffPass_Corr_tnp[0]->GetName())+"_Zptcorr",TString(hEffPass_Corr_tnp[0]->GetTitle())+" (Zpt corr)",
         nbins,hEffPass_Corr_tnp[0]->GetXaxis()->GetXbins()->GetArray(),
         nbins,hEffPass_Corr_tnp[0]->GetXaxis()->GetXbins()->GetArray());

   int i=1;
   TGraphAsymmErrors *gAccTmp = new TGraphAsymmErrors(hAccPass[i],hAccTotal[i]);
   TGraphAsymmErrors *gEffTmp = new TGraphAsymmErrors(hEffPass_Corr_tnp[i],hEffTotal[i]);
   TGraphAsymmErrors *gAccEffTmp = new TGraphAsymmErrors(hEffPass_Corr_tnp[i],hAccTotal[i]);
   gAccEffs.push_back(gAccEffTmp);
   graphNames.push_back(gn[i]);

   for (int j=0; j<gAccTmp->GetN(); j++) {
      if (i==1) {
         gAcc->SetPointEYlow(j,0);
         gAcc->SetPointEYhigh(j,0);
         gEff->SetPointEYlow(j,0);
         gEff->SetPointEYhigh(j,0);
         gAccEff->SetPointEYlow(j,0);
         gAccEff->SetPointEYhigh(j,0);
      }
      gAcc->SetPointEYlow(j,-min(gAccTmp->GetY()[j]-gAcc->GetY()[j],-gAcc->GetEYlow()[j]));
      gAcc->SetPointEYhigh(j,max(gAccTmp->GetY()[j]-gAcc->GetY()[j],gAcc->GetEYhigh()[j]));
      gEff->SetPointEYlow(j,-min(gEffTmp->GetY()[j]-gEff->GetY()[j],-gEff->GetEYlow()[j]));
      gEff->SetPointEYhigh(j,max(gEffTmp->GetY()[j]-gEff->GetY()[j],gEff->GetEYhigh()[j]));
      gAccEff->SetPointEYlow(j,-min(gAccEffTmp->GetY()[j]-gAccEff->GetY()[j],-gAccEff->GetEYlow()[j]));
      gAccEff->SetPointEYhigh(j,max(gAccEffTmp->GetY()[j]-gAccEff->GetY()[j],gAccEff->GetEYhigh()[j]));

      double diffj = gAccTmp->GetY()[j]-gAcc->GetY()[j];
      for (int k=0; k<nbins; k++) {
         double diffk = gAccTmp->GetY()[k]-gAcc->GetY()[k];
         ans->SetBinContent(j+1,k+1,diffj*diffk);
      }
   } // loop on bins

   // turn the cov matrix into a correlation matrix
   for (int j=0; j<nbins; j++) {
      for (int k=0; k<nbins; k++) {
         if (j==k) continue;
         ans->SetBinContent(j+1,k+1,ans->GetBinContent(j+1,k+1) / sqrt(fabs(ans->GetBinContent(j+1,j+1)*ans->GetBinContent(k+1,k+1))));
      }
   }
   for (int j=0; j<nbins; j++) ans->SetBinContent(j+1,j+1,1);

   // compute ratio to first graph
   for (vector<TGraphAsymmErrors*>::iterator it=gAccEffs.begin()+1; it!=gAccEffs.end(); it++) 
      for (int i=0; i<(*it)->GetN(); i++)
         (*it)->SetPoint(i,(*it)->GetX()[i],(*it)->GetY()[i]/gAccEffs[0]->GetY()[i]);

   for (int i=0; i<gAccEffs[0]->GetN(); i++) {
      // rescale output
      gAcc->SetPointEYlow(i,gAcc->GetEYlow()[i]/gAcc->GetY()[i]);
      gAcc->SetPointEYhigh(i,gAcc->GetEYhigh()[i]/gAcc->GetY()[i]);
      gEff->SetPointEYlow(i,gEff->GetEYlow()[i]/gEff->GetY()[i]);
      gEff->SetPointEYhigh(i,gEff->GetEYhigh()[i]/gEff->GetY()[i]);
      gAccEff->SetPointEYlow(i,gAccEff->GetEYlow()[i]/gAccEff->GetY()[i]);
      gAccEff->SetPointEYhigh(i,gAccEff->GetEYhigh()[i]/gAccEff->GetY()[i]);

      gAccEffs[0]->SetPoint(i,gAccEffs[0]->GetX()[i],1);
   }

   c1.SetYRange(0.98,1.07);
   if (thevar==var::mass) c1.SetYRange(0.98,1.24);
   c1.CanvasWithMultipleGraphs(gAccEffs, graphNames, "LPX");
   c1.PrintCanvas();
   c1.PrintCanvas_C();

   return ans;
}

void Sys_AccEff(const char* file, var thevar) {
   TGraphAsymmErrors *gAcc_scales=0, *gEff_scales=0, *gAccEff_scales=0;
   TH2D *hcormatrix_scales = Sys_AccEff_scales(file, thevar, gAcc_scales, gEff_scales, gAccEff_scales);
   TGraphAsymmErrors *gAcc_alphas=0, *gEff_alphas=0, *gAccEff_alphas=0;
   TH2D *hcormatrix_alphas = Sys_AccEff_alphas(file, thevar, gAcc_alphas, gEff_alphas, gAccEff_alphas);
   TGraphAsymmErrors *gAcc_EPPS16=0, *gEff_EPPS16=0, *gAccEff_EPPS16=0;
   TH2D *hcormatrix_EPPS16 = Sys_AccEff_EPPS16(file, thevar, gAcc_EPPS16, gEff_EPPS16, gAccEff_EPPS16);
   TGraphAsymmErrors *gAcc_Zpt=0, *gEff_Zpt=0, *gAccEff_Zpt=0;
   TH2D *hcormatrix_Zpt = Sys_AccEff_Zpt(file, thevar, gAcc_Zpt, gEff_Zpt, gAccEff_Zpt);

   // print to a TeX
   vector<string> ynames;
   ynames.push_back("scales");
   ynames.push_back("$\\alpha_S$");
   ynames.push_back("CT14+EPPS16");
   ynames.push_back("Zpt");

   // create special graphs for making the table, with the relative uncertainty, and print the csv at the same time
   map<bin, syst> syst_scales;
   map<bin, syst> syst_alphas;
   map<bin, syst> syst_EPPS16;
   map<bin, syst> syst_Zpt;
   TGraphAsymmErrors *gAccEff4tex_scales = (TGraphAsymmErrors*) gAccEff_scales->Clone("gAccEff4text_scales");
   TGraphAsymmErrors *gAccEff4tex_alphas = (TGraphAsymmErrors*) gAccEff_alphas->Clone("gAccEff4text_alphas");
   TGraphAsymmErrors *gAccEff4tex_EPPS16 = (TGraphAsymmErrors*) gAccEff_EPPS16->Clone("gAccEff4text_EPPS16");
   TGraphAsymmErrors *gAccEff4tex_Zpt = (TGraphAsymmErrors*) gAccEff_Zpt->Clone("gAccEff4text_Zpt");

   for (int i=0; i<gAccEff4tex_scales->GetN(); i++) {
      double eyl, eyh, x, xmin, xmax;
      x = gAccEff4tex_scales->GetX()[i];
      xmin = x-gAccEff4tex_scales->GetEXlow()[i];
      xmax = x+gAccEff4tex_scales->GetEXhigh()[i];

      eyl = gAccEff4tex_scales->GetEYlow()[i];
      eyh = gAccEff4tex_scales->GetEYhigh()[i];
      syst tmpsyst; tmpsyst.name = "AccEff (scales)";
      tmpsyst.value = max(eyl,eyh);
      syst_scales[bin(xmin,xmax)] = tmpsyst;
      gAccEff4tex_scales->SetPoint(i,x,0.999);
      gAccEff4tex_scales->SetPointEYlow(i,eyl);
      gAccEff4tex_scales->SetPointEYhigh(i,eyh);

      eyl = gAccEff4tex_alphas->GetEYlow()[i];
      eyh = gAccEff4tex_alphas->GetEYhigh()[i];
      tmpsyst.name = "AccEff (alphaS)";
      tmpsyst.value = max(eyl,eyh);
      syst_alphas[bin(xmin,xmax)] = tmpsyst;
      gAccEff4tex_alphas->SetPoint(i,x,0.999);
      gAccEff4tex_alphas->SetPointEYlow(i,eyl);
      gAccEff4tex_alphas->SetPointEYhigh(i,eyh);

      eyl = gAccEff4tex_EPPS16->GetEYlow()[i]/gAccEff4tex_EPPS16->GetY()[i];
      eyh = gAccEff4tex_EPPS16->GetEYhigh()[i]/gAccEff4tex_EPPS16->GetY()[i];
      tmpsyst.name = "AccEff (CT14+EPPS16)";
      tmpsyst.value = max(eyl,eyh);
      syst_EPPS16[bin(xmin,xmax)] = tmpsyst;
      gAccEff4tex_EPPS16->SetPoint(i,x,0.999);
      gAccEff4tex_EPPS16->SetPointEYlow(i,eyl);
      gAccEff4tex_EPPS16->SetPointEYhigh(i,eyh);

      eyl = gAccEff4tex_Zpt->GetEYlow()[i];
      eyh = gAccEff4tex_Zpt->GetEYhigh()[i];
      tmpsyst.name = "AccEff (Zpt)";
      tmpsyst.value = max(eyl,eyh);
      syst_Zpt[bin(xmin,xmax)] = tmpsyst;
      gAccEff4tex_Zpt->SetPoint(i,x,0.999);
      gAccEff4tex_Zpt->SetPointEYlow(i,eyl);
      gAccEff4tex_Zpt->SetPointEYhigh(i,eyh);
   }

   // print the syst to a table
   TString filename = Form("tex/AccEff_scales_%s.tex",varname(thevar));
   inittex(filename.Data(),xaxistitletex(thevar),ynames);
   vector<TGraphAsymmErrors*> tg;
   tg.push_back(gAccEff4tex_scales);
   tg.push_back(gAccEff4tex_alphas);
   tg.push_back(gAccEff4tex_EPPS16);
   tg.push_back(gAccEff4tex_Zpt);
   printGraph(tg,filename.Data());
   closetex(filename.Data());
   // remove all the unwanted 0.999
   TString command = "sed -i 's/0.999//g' " + filename;
   system(command.Data());


   // produce the combined syst, plot it and print it
   vector< map<bin,syst> > theSysts;
   theSysts.push_back(syst_scales);
   theSysts.push_back(syst_alphas);
   theSysts.push_back(syst_EPPS16);
   theSysts.push_back(syst_Zpt);
   map<bin,syst> syst_tot = combineSyst(theSysts,"AccEff (theory)");

   // compute the correlation matrix
   // first, build covariance matrices
   int nbins = gAccEff4tex_scales->GetN();
   TMatrixT<double> cov_scales(nbins,nbins), cov_alphas(nbins,nbins), cov_EPPS16(nbins,nbins), cov_Zpt(nbins,nbins), cov_all(nbins,nbins), cor_all(nbins,nbins);
   // TH2D *hcor = new TH2D("hcor","",nbins,0,nbins,nbins,0,nbins);
   for (int i=0; i<nbins; i++) {
      double xi, xmini, xmaxi;
      xi = gAccEff4tex_scales->GetX()[i];
      xmini = xi-gAccEff4tex_scales->GetEXlow()[i];
      xmaxi = xi+gAccEff4tex_scales->GetEXhigh()[i];
      bin bini(xmini,xmaxi);
      for (int j=0; j<nbins; j++) {
         double xj, xminj, xmaxj;
         xj = gAccEff4tex_scales->GetX()[j];
         xminj = xj-gAccEff4tex_scales->GetEXlow()[j];
         xmaxj = xj+gAccEff4tex_scales->GetEXhigh()[j];
         bin binj(xminj,xmaxj);

         cov_scales[i][j] = syst_scales[bini].value * syst_scales[binj].value * hcormatrix_scales->GetBinContent(i+1,j+1); 
         cov_alphas[i][j] = syst_alphas[bini].value * syst_alphas[binj].value * hcormatrix_alphas->GetBinContent(i+1,j+1); 
         cov_EPPS16[i][j] = syst_EPPS16[bini].value * syst_EPPS16[binj].value * hcormatrix_EPPS16->GetBinContent(i+1,j+1);
         cov_Zpt[i][j] = syst_Zpt[bini].value * syst_Zpt[binj].value * hcormatrix_Zpt->GetBinContent(i+1,j+1);

         // sum all covariance matrices
         cov_all[i][j] = cov_scales[i][j]+cov_alphas[i][j]+cov_EPPS16[i][j]+cov_Zpt[i][j];
         // compute the correlation
         cor_all[i][j] = cov_all[i][j] / (syst_tot[bini].value*syst_tot[binj].value);
         // hcor->SetBinContent(i+1,j+1,cor_all[i][j]);
      }
   }
   
   // print the csv... and the correlation matrix
   ofstream of_syst(Form("csv/AccEff_theory_%s.csv",varname(thevar)));
   ofstream of_cor(Form("cor/AccEff_theory_%s.csv",varname(thevar)));
   of_syst << "AccEff (theory)" << endl;
   of_cor << "AccEff (theory)" << endl;
   int i=0;
   for (map<bin,syst>::const_iterator it=syst_tot.begin(); it!=syst_tot.end(); it++) {
      of_syst << it->first.low() << ", " << it->first.high() << ", " << it->second.value << endl;

      of_cor << it->first.low() << ", " << it->first.high();
      for (int j=0; j<nbins; j++) of_cor << ", " << cor_all[i][j];
      of_cor << endl;
      i++;
   }
   of_syst.close();
   of_cor.close();

   // make the plot
   vector<TString> tags;
   vector<TGraphAsymmErrors*> graphs;
   vector<map<bin, syst> > systs;

   systs.push_back(syst_tot);
   tags.push_back("Total");
   systs.push_back(syst_scales);
   tags.push_back("scales");
   systs.push_back(syst_alphas);
   tags.push_back("#alpha_{S}");
   systs.push_back(syst_EPPS16);
   tags.push_back("CT14+EPPS16");
   systs.push_back(syst_Zpt);
   tags.push_back("Zpt");

   for (unsigned int i=0; i<systs.size(); i++) {
      map<bin, syst> thesyst = systs[i];
      vector<double> x, y, dx, dy;

      double valmax=-1e99;

      for (map<bin, syst>::const_iterator it=thesyst.begin(); it!=thesyst.end(); it++) {
         bin it2 = it->first;

         double low = it2.low(), high = it2.high();
         x.push_back((low+high)/2.);
         dx.push_back((high-low)/2.);
         y.push_back(0);
         dy.push_back(it->second.value*100.);
         valmax = max(valmax,dy.back());
      }

      TGraphAsymmErrors *thegraph;
      thegraph = new TGraphAsymmErrors(x.size(),x.data(),dy.data(),y.data(),y.data(),y.data(),y.data());
      thegraph->SetMinimum(0);
      if (thevar==var::phistar) thegraph->SetMaximum(6);
      thegraph->Sort();
      thegraph->SetFillStyle(0);
      graphs.push_back(thegraph);
   }


   MyCanvas c1(Form("systematics_AccEff_%s",varname(thevar)),xaxistitle(thevar),"Rel. uncertainty (%)",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar) c1.SetLogx();
   if (thevar==var::phistar) c1.SetYRange(0,6);
   c1.CanvasWithMultipleGraphs(graphs,tags, "LP");
   c1.PrintCanvas();
   c1.PrintCanvas_C();
}

void Sys_AccEff(const char* file) {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      Sys_AccEff(file,thevar_i);
   }
}
