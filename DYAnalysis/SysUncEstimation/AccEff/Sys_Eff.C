#include "../../BkgEst/interface/defs.h"
#include "../../Include/MyCanvas.C"
#include "../../Include/texUtils.h"
#include "../../Include/lhapdf_utils.h"
#include "../../Include/PlotTools.h"
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
// .L Eff/Sys_Eff.C+

void checkNumDen(TH1 *hnum, TH1 *hden);

TH2D* Sys_Eff_scales(const char* file, var thevar, TGraphAsymmErrors *&gEff) {
   TFile *fin = TFile::Open(file);

   vector<TH1D*> hEffTotal, hEffPass_Corr_tnp;
   vector<TMatrixT<double> > mcor;
   vector<TGraphAsymmErrors*> gEffs;
   vector<TString> graphNames;

   int idx[7] = {0, 8, 4, 6, 3, 2, 1}; // muR, muF = (1, 1), (0.5, 0.5), (2, 2), (0.5, 1), (2, 1), (1, 0.5), (1, 2)
   const char* gn[7] = {"(1,1)", "(0.5,0.5)", "(2,2)", "(0.5,1)", "(2,1)", "(1,0.5)", "(1,2)"};
   MyCanvas c1(Form("AccEff/Eff_scales_%s",varname(thevar)),xaxistitle(thevar),"Eff",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) c1.SetLogx();

   for (int i=0; i<7; i++) {
      hEffTotal.push_back(getHist(fin, Form("h_%s_EffTotal%d",varname(thevar),idx[i])));
      hEffPass_Corr_tnp.push_back(getHist(fin, Form("h_%s_EffPass_Corr_tnp%d",varname(thevar),idx[i])));
   }

   checkNumDen(hEffPass_Corr_tnp[0],hEffTotal[0]);
   gEff = new TGraphAsymmErrors(hEffPass_Corr_tnp[0],hEffTotal[0]);
   gEff->SetName("gEff_scales");
   gEffs.push_back(gEff);
   graphNames.push_back(gn[0]);

   int nbins = gEff->GetN();
   // put a dummy empty matrix at i=0
   mcor.push_back(TMatrixT<double>(nbins,nbins));

   for (int i=1; i<7; i++) {
      checkNumDen(hEffPass_Corr_tnp[i],hEffTotal[i]);
      TGraphAsymmErrors *gEffTmp = new TGraphAsymmErrors(hEffPass_Corr_tnp[i],hEffTotal[i]);
      gEffs.push_back(gEffTmp);
      graphNames.push_back(gn[i]);

      mcor.push_back(TMatrixT<double>(nbins,nbins));

      for (int j=0; j<nbins; j++) {
         if (i==1) {
            gEff->SetPointEYlow(j,0);
            gEff->SetPointEYhigh(j,0);
         }
         gEff->SetPointEYlow(j,-min(gEffTmp->GetY()[j]-gEff->GetY()[j],-gEff->GetEYlow()[j]));
         gEff->SetPointEYhigh(j,max(gEffTmp->GetY()[j]-gEff->GetY()[j],gEff->GetEYhigh()[j]));

         double diffj = gEffTmp->GetY()[j]-gEff->GetY()[j];
         for (int k=0; k<nbins; k++) {
            double diffk = gEffTmp->GetY()[k]-gEff->GetY()[k];
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
   for (vector<TGraphAsymmErrors*>::iterator it=gEffs.begin()+1; it!=gEffs.end(); it++) 
      for (int i=0; i<(*it)->GetN(); i++)
         (*it)->SetPoint(i,(*it)->GetX()[i],(*it)->GetY()[i]/gEffs[0]->GetY()[i]);

   for (int i=0; i<gEffs[0]->GetN(); i++) {
      // rescale output
      gEff->SetPointEYlow(i,gEff->GetEYlow()[i]/gEff->GetY()[i]);
      gEff->SetPointEYhigh(i,gEff->GetEYhigh()[i]/gEff->GetY()[i]);

      gEffs[0]->SetPoint(i,gEffs[0]->GetX()[i],1);
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
   c1.CanvasWithMultipleGraphs(gEffs, graphNames, "LPX");
   c1.PrintCanvas();
   c1.PrintCanvas_C();

   return ans;
}

TH2D* Sys_Eff_alphas(const char* file, var thevar, TGraphAsymmErrors *&gEff) {
   TFile *fin = TFile::Open(file);

   vector<TH1D*> hEffTotal, hEffPass_Corr_tnp;
   vector<TMatrixT<double> > mcor;
   vector<TGraphAsymmErrors*> gEffs;
   vector<TString> graphNames;

   int idx[3] = {0, 168, 169}; // nominal, 0.117, 0.119 
   const char* gn[3] = {"#alpha_{S}=0.118","#alpha_{S}=0.117","#alpha_{S}=0.119"};
   MyCanvas c1(Form("AccEff/Eff_alphas_%s",varname(thevar)),xaxistitle(thevar),"Eff",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) c1.SetLogx();

   for (int i=0; i<3; i++) {
      hEffTotal.push_back(getHist(fin, Form("h_%s_EffTotal%d",varname(thevar),idx[i])));
      hEffPass_Corr_tnp.push_back(getHist(fin, Form("h_%s_EffPass_Corr_tnp%d",varname(thevar),idx[i])));
   }

   checkNumDen(hEffPass_Corr_tnp[0],hEffTotal[0]);
   gEff = new TGraphAsymmErrors(hEffPass_Corr_tnp[0],hEffTotal[0]);
   gEff->SetName("gEff_alphas");
   gEffs.push_back(gEff);
   graphNames.push_back(gn[0]);

   int nbins = gEff->GetN();
   // put a dummy empty matrix at i=0
   mcor.push_back(TMatrixT<double>(nbins,nbins));

   for (int i=1; i<3; i++) {
      checkNumDen(hEffPass_Corr_tnp[i],hEffTotal[i]);
      TGraphAsymmErrors *gEffTmp = new TGraphAsymmErrors(hEffPass_Corr_tnp[i],hEffTotal[i]);
      gEffs.push_back(gEffTmp);
      graphNames.push_back(gn[i]);

      mcor.push_back(TMatrixT<double>(nbins,nbins));

      for (int j=0; j<gEffTmp->GetN(); j++) {
         if (i==1) {
            gEff->SetPointEYlow(j,0);
            gEff->SetPointEYhigh(j,0);
         }
         gEff->SetPointEYlow(j,-min(gEffTmp->GetY()[j]-gEff->GetY()[j],-gEff->GetEYlow()[j]));
         gEff->SetPointEYhigh(j,max(gEffTmp->GetY()[j]-gEff->GetY()[j],gEff->GetEYhigh()[j]));

         double diffj = gEffTmp->GetY()[j]-gEff->GetY()[j];
         for (int k=0; k<nbins; k++) {
            double diffk = gEffTmp->GetY()[k]-gEff->GetY()[k];
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
   for (vector<TGraphAsymmErrors*>::iterator it=gEffs.begin()+1; it!=gEffs.end(); it++) 
      for (int i=0; i<(*it)->GetN(); i++)
         (*it)->SetPoint(i,(*it)->GetX()[i],(*it)->GetY()[i]/gEffs[0]->GetY()[i]);

   for (int i=0; i<gEffs[0]->GetN(); i++) {
      // rescale output
      gEff->SetPointEYlow(i,gEff->GetEYlow()[i]/gEff->GetY()[i]);
      gEff->SetPointEYhigh(i,gEff->GetEYhigh()[i]/gEff->GetY()[i]);

      gEffs[0]->SetPoint(i,gEffs[0]->GetX()[i],1);
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
   c1.CanvasWithMultipleGraphs(gEffs, graphNames, "LPX");
   c1.PrintCanvas();
   c1.PrintCanvas_C();
   
   // scale by a factor 1.5 the errors, because PDF4LHC recommends delta alpha_S = 0.0015 instead of 0.001
   for (int j=0; j<gEff->GetN(); j++) {
      gEff->SetPointEYlow(j,1.5*gEff->GetEYlow()[j]);
      gEff->SetPointEYhigh(j,1.5*gEff->GetEYhigh()[j]);
   }

   return ans;
}

TH2D* Sys_Eff_EPPS16(const char* file, var thevar, TGraphAsymmErrors *&gEff) {
   TFile *fin = TFile::Open(file);

   vector<TH1D*> hEff;
   vector<TString> histNames;
   MyCanvas c1(Form("AccEff/Eff_EPPS16_%s",varname(thevar)),xaxistitle(thevar),"Eff",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) c1.SetLogx();

   int i=0;
   hEff.push_back(getHist(fin, Form("h_%s_EffPass_Corr_tnp%d",varname(thevar),i)));
   hEff.back()->Divide(getHist(fin, Form("h_%s_EffTotal%d",varname(thevar),i)));
   histNames.push_back(Form("%d",i));

   for (i=285; i<=324; i++) {
      hEff.push_back(getHist(fin, Form("h_%s_EffPass_Corr_tnp%d",varname(thevar),i)));
      hEff.back()->Divide(getHist(fin, Form("h_%s_EffTotal%d",varname(thevar),i)));
      histNames.push_back(Form("%d",i-284));
   }

   for (i=112; i<=167; i++) {
      hEff.push_back(getHist(fin, Form("h_%s_EffPass_Corr_tnp%d",varname(thevar),i)));
      hEff.back()->Divide(getHist(fin, Form("h_%s_EffTotal%d",varname(thevar),i)));
      histNames.push_back(Form("%d",i-111+40));
   }

   gEff = pdfuncert(hEff, "EPPS16nlo_CT14nlo_Pb208");

   // compute ratio to first graph
   for (vector<TH1D*>::iterator it=hEff.begin()+1; it!=hEff.end(); it++) (*it)->Divide(hEff[0]);
   hEff[0]->Divide(hEff[0]);

   c1.SetYRange(0.90,1.10);
   c1.CanvasWithMultipleHistograms(hEff, histNames, "HIST LP");
   c1.PrintCanvas();
   c1.PrintCanvas_C();

   return  pdfcorr(hEff, "EPPS16nlo_CT14nlo_Pb208");
}

TH2D* Sys_Eff_Zpt(const char* file, var thevar, TGraphAsymmErrors *&gEff) {
   TFile *fin[2] = {0};
   fin[0] = TFile::Open(file);
   TString file_nozpt("../ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth_noZptrew.root");
   fin[1] = TFile::Open(file_nozpt);

   vector<TH1D*> hEffTotal, hEffPass_Corr_tnp;
   vector<TGraphAsymmErrors*> gEffs;
   vector<TString> graphNames;

   const char* gn[2] = {"Nominal", "No p_{T}^{Z} reweighting"};
   MyCanvas c1(Form("AccEff/Eff_Zpt_%s",varname(thevar)),xaxistitle(thevar),"Eff",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) c1.SetLogx();

   hEffTotal.push_back(getHist(fin[0], Form("h_%s_EffTotal0",varname(thevar))));
   hEffPass_Corr_tnp.push_back(getHist(fin[0], Form("h_%s_EffPass_Corr_tnp0",varname(thevar))));
   hEffTotal.push_back(getHist(fin[1], Form("h_%s_EffTotal",varname(thevar))));
   hEffPass_Corr_tnp.push_back(getHist(fin[1], Form("h_%s_EffPass_Corr_tnp0",varname(thevar))));

   checkNumDen(hEffPass_Corr_tnp[0],hEffTotal[0]);
   gEff = new TGraphAsymmErrors(hEffPass_Corr_tnp[0],hEffTotal[0]);
   gEff->SetName("gEff_Zpt");
   gEffs.push_back(gEff);
   graphNames.push_back(gn[0]);

   int nbins = gEff->GetN();
   TH2D *ans = new TH2D(TString(hEffPass_Corr_tnp[0]->GetName())+"_Zptcorr",TString(hEffPass_Corr_tnp[0]->GetTitle())+" (Zpt corr)",
         nbins,hEffPass_Corr_tnp[0]->GetXaxis()->GetXbins()->GetArray(),
         nbins,hEffPass_Corr_tnp[0]->GetXaxis()->GetXbins()->GetArray());

   int i=1;
   checkNumDen(hEffPass_Corr_tnp[i],hEffTotal[i]);
   TGraphAsymmErrors *gEffTmp = new TGraphAsymmErrors(hEffPass_Corr_tnp[i],hEffTotal[i]);
   gEffs.push_back(gEffTmp);
   graphNames.push_back(gn[i]);

   for (int j=0; j<nbins; j++) {
      if (i==1) {
         gEff->SetPointEYlow(j,0);
         gEff->SetPointEYhigh(j,0);
      }
      gEff->SetPointEYlow(j,-min(gEffTmp->GetY()[j]-gEff->GetY()[j],-gEff->GetEYlow()[j]));
      gEff->SetPointEYhigh(j,max(gEffTmp->GetY()[j]-gEff->GetY()[j],gEff->GetEYhigh()[j]));
   } // loop on bins

   // remove bins with tiny variation, ie "dips" in the variations ("smooth")
   for (int j=1; j<nbins-1; j++) {
      double effNm1 = fabs(gEffTmp->GetY()[j-1] - gEff->GetY()[j-1]);
      double effN = fabs(gEffTmp->GetY()[j] - gEff->GetY()[j]);
      double effNp1 = fabs(gEffTmp->GetY()[j+1] - gEff->GetY()[j+1]);

      if (effN < effNm1 && effN < effNp1) {
         double avgdiff = 0.5*(effNm1+effNp1);
         gEffTmp->SetPoint(j, gEff->GetX()[j], gEff->GetY()[j]+avgdiff);
         gEff->SetPointEYlow(j,avgdiff);
         gEff->SetPointEYhigh(j,avgdiff);
      }
   }

   // make the 2D histo
   for (int j=0; j<nbins; j++) {
      double diffj = gEffTmp->GetY()[j]-gEff->GetY()[j];
      for (int k=0; k<nbins; k++) {
         double diffk = gEffTmp->GetY()[k]-gEff->GetY()[k];
         ans->SetBinContent(j+1,k+1,diffj*diffk);
      }
   }

   // turn the cov matrix into a correlation matrix
   for (int j=0; j<nbins; j++) {
      for (int k=0; k<nbins; k++) {
         if (j==k) continue;
         ans->SetBinContent(j+1,k+1,ans->GetBinContent(j+1,k+1) / sqrt(fabs(ans->GetBinContent(j+1,j+1)*ans->GetBinContent(k+1,k+1))));
      }
   }
   for (int j=0; j<nbins; j++) ans->SetBinContent(j+1,j+1,1);

   // compute ratio to first graph
   for (vector<TGraphAsymmErrors*>::iterator it=gEffs.begin()+1; it!=gEffs.end(); it++) 
      for (int i=0; i<(*it)->GetN(); i++)
         (*it)->SetPoint(i,(*it)->GetX()[i],(*it)->GetY()[i]/gEffs[0]->GetY()[i]);

   for (int i=0; i<gEffs[0]->GetN(); i++) {
      // rescale output
      gEff->SetPointEYlow(i,gEff->GetEYlow()[i]/gEff->GetY()[i]);
      gEff->SetPointEYhigh(i,gEff->GetEYhigh()[i]/gEff->GetY()[i]);

      gEffs[0]->SetPoint(i,gEffs[0]->GetX()[i],1);
   }

   c1.SetYRange(0.98,1.07);
   if (thevar==var::mass) c1.SetYRange(0.98,1.24);
   c1.CanvasWithMultipleGraphs(gEffs, graphNames, "LPX");
   c1.PrintCanvas();
   c1.PrintCanvas_C();

   return ans;
}

void Sys_Eff(const char* file, var thevar) {
   TGraphAsymmErrors *gEff_scales=0;
   TH2D *hcormatrix_scales = Sys_Eff_scales(file, thevar, gEff_scales);
   TGraphAsymmErrors *gEff_alphas=0;
   TH2D *hcormatrix_alphas = Sys_Eff_alphas(file, thevar, gEff_alphas);
   TGraphAsymmErrors *gEff_EPPS16=0;
   TH2D *hcormatrix_EPPS16 = Sys_Eff_EPPS16(file, thevar, gEff_EPPS16);
   TGraphAsymmErrors *gEff_Zpt=0;
   TH2D *hcormatrix_Zpt = Sys_Eff_Zpt(file, thevar, gEff_Zpt);

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
   TGraphAsymmErrors *gEff4tex_scales = (TGraphAsymmErrors*) gEff_scales->Clone("gEff4text_scales");
   TGraphAsymmErrors *gEff4tex_alphas = (TGraphAsymmErrors*) gEff_alphas->Clone("gEff4text_alphas");
   TGraphAsymmErrors *gEff4tex_EPPS16 = (TGraphAsymmErrors*) gEff_EPPS16->Clone("gEff4text_EPPS16");
   TGraphAsymmErrors *gEff4tex_Zpt = (TGraphAsymmErrors*) gEff_Zpt->Clone("gEff4text_Zpt");

   for (int i=0; i<gEff4tex_scales->GetN(); i++) {
      double eyl, eyh, x, xmin, xmax;
      x = gEff4tex_scales->GetX()[i];
      xmin = x-gEff4tex_scales->GetEXlow()[i];
      xmax = x+gEff4tex_scales->GetEXhigh()[i];

      eyl = gEff4tex_scales->GetEYlow()[i];
      eyh = gEff4tex_scales->GetEYhigh()[i];
      syst tmpsyst; tmpsyst.name = "Eff (scales)";
      tmpsyst.value = max(eyl,eyh);
      syst_scales[bin(xmin,xmax)] = tmpsyst;
      gEff4tex_scales->SetPoint(i,x,0.999);
      gEff4tex_scales->SetPointEYlow(i,eyl);
      gEff4tex_scales->SetPointEYhigh(i,eyh);

      eyl = gEff4tex_alphas->GetEYlow()[i];
      eyh = gEff4tex_alphas->GetEYhigh()[i];
      tmpsyst.name = "Eff (alphaS)";
      tmpsyst.value = max(eyl,eyh);
      syst_alphas[bin(xmin,xmax)] = tmpsyst;
      gEff4tex_alphas->SetPoint(i,x,0.999);
      gEff4tex_alphas->SetPointEYlow(i,eyl);
      gEff4tex_alphas->SetPointEYhigh(i,eyh);

      eyl = gEff4tex_EPPS16->GetEYlow()[i]/gEff4tex_EPPS16->GetY()[i];
      eyh = gEff4tex_EPPS16->GetEYhigh()[i]/gEff4tex_EPPS16->GetY()[i];
      tmpsyst.name = "Eff (CT14+EPPS16)";
      tmpsyst.value = max(eyl,eyh);
      syst_EPPS16[bin(xmin,xmax)] = tmpsyst;
      gEff4tex_EPPS16->SetPoint(i,x,0.999);
      gEff4tex_EPPS16->SetPointEYlow(i,eyl);
      gEff4tex_EPPS16->SetPointEYhigh(i,eyh);

      eyl = gEff4tex_Zpt->GetEYlow()[i];
      eyh = gEff4tex_Zpt->GetEYhigh()[i];
      tmpsyst.name = "Eff (Zpt)";
      tmpsyst.value = max(eyl,eyh);
      syst_Zpt[bin(xmin,xmax)] = tmpsyst;
      gEff4tex_Zpt->SetPoint(i,x,0.999);
      gEff4tex_Zpt->SetPointEYlow(i,eyl);
      gEff4tex_Zpt->SetPointEYhigh(i,eyh);
   }

   // print the syst to a table
   TString filename = Form("tex/Eff_scales_%s.tex",varname(thevar));
   inittex(filename.Data(),xaxistitletex(thevar),ynames);
   vector<TGraphAsymmErrors*> tg;
   tg.push_back(gEff4tex_scales);
   tg.push_back(gEff4tex_alphas);
   tg.push_back(gEff4tex_EPPS16);
   tg.push_back(gEff4tex_Zpt);
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
   map<bin,syst> syst_tot = combineSyst(theSysts,"Eff (theory)");

   // compute the correlation matrix
   // first, build covariance matrices
   int nbins = gEff4tex_scales->GetN();
   TMatrixT<double> cov_scales(nbins,nbins), cov_alphas(nbins,nbins), cov_EPPS16(nbins,nbins), cov_Zpt(nbins,nbins), cov_all(nbins,nbins), cor_all(nbins,nbins);
   // TH2D *hcor = new TH2D("hcor","",nbins,0,nbins,nbins,0,nbins);
   for (int i=0; i<nbins; i++) {
      double xi, xmini, xmaxi;
      xi = gEff4tex_scales->GetX()[i];
      xmini = xi-gEff4tex_scales->GetEXlow()[i];
      xmaxi = xi+gEff4tex_scales->GetEXhigh()[i];
      bin bini(xmini,xmaxi);
      for (int j=0; j<nbins; j++) {
         double xj, xminj, xmaxj;
         xj = gEff4tex_scales->GetX()[j];
         xminj = xj-gEff4tex_scales->GetEXlow()[j];
         xmaxj = xj+gEff4tex_scales->GetEXhigh()[j];
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
   ofstream of_syst(Form("csv/Eff_theory_%s.csv",varname(thevar)));
   ofstream of_cor(Form("cor/Eff_theory_%s.csv",varname(thevar)));
   of_syst << "Eff (theory)" << endl;
   of_cor << "Eff (theory)" << endl;
   int i=0;
   for (map<bin,syst>::const_iterator it=syst_tot.begin(); it!=syst_tot.end(); it++) {
      of_syst << it->first.low() << ", " << it->first.high() << ", " << it->second.value << endl;

      for (int j=0; j<nbins; j++) {
         if (j>0) of_cor << ", ";
         of_cor << cor_all[i][j];
      }
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


   MyCanvas c1(Form("systematics_Eff_%s",varname(thevar)),xaxistitle(thevar),"Rel. uncertainty (%)",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) c1.SetLogx();
   if (thevar==var::phistar) c1.SetYRange(0,6);
   c1.CanvasWithMultipleGraphs(graphs,tags, "LP");
   c1.PrintCanvas();
   c1.PrintCanvas_C();
}

void Sys_Eff(const char* file) {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      Sys_Eff(file,thevar_i);
   }
   Sys_Eff(file,var::rapall);
   Sys_Eff(file,var::ptall);
   Sys_Eff(file,var::phistarall);
}

void Sys_Eff_MCstat(const char* file, var thevar) {
   TFile *fin = TFile::Open(file);
   TH1D *hAccTotal = getHist(fin, Form("h_%s_EffTotal",varname(thevar)));
   TH1D *hEffPass_Corr_tnp = getHist(fin, Form("h_%s_EffPass_Corr_tnp0",varname(thevar)));
   cout << hEffPass_Corr_tnp << " " << hAccTotal << endl;
   checkNumDen(hEffPass_Corr_tnp,hAccTotal);
   TGraphAsymmErrors *gae = new TGraphAsymmErrors(hEffPass_Corr_tnp,hAccTotal);

   double* thebins = binsvar(thevar);

   ofstream of_up(Form("csv/effstat_up_%s.csv",varname(thevar)));
   of_up << "Acc Eff (stat, up)" << endl;

   for (int i=0; i<gae->GetN(); i++) {
      of_up << thebins[i] << ", " << thebins[i+1] << ", " << gae->GetEYhigh()[i] / gae->GetY()[i] << endl;
   }

   of_up.close();
   cout << "closed " << Form("csv/effstat_up_%s.csv",varname(thevar)) << endl;

   ofstream of_down(Form("csv/effstat_down_%s.csv",varname(thevar)));
   of_down << "Acc Eff (stat, down)" << endl;

   for (int i=0; i<gae->GetN(); i++) {
      of_down << thebins[i] << ", " << thebins[i+1] << ", " << gae->GetEYhigh()[i] / gae->GetY()[i] << endl;
   }

   of_down.close();
   cout << "closed " << Form("csv/effstat_down_%s.csv",varname(thevar)) << endl;
}

void Sys_Eff_MCstat(const char* file) {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      Sys_Eff_MCstat(file,thevar_i);
   }
   Sys_Eff_MCstat(file,var::rapall);
   Sys_Eff_MCstat(file,var::ptall);
   Sys_Eff_MCstat(file,var::phistarall);
}

void checkNumDen(TH1 *hnum, TH1 *hden) {
   // helper function in case some bin contents are higher in the numerator than denominator
   if (hnum->GetNbinsX() != hden->GetNbinsX()) {
      cout << "ERROR: " << hnum->GetName() << " and " << hden->GetName() << " have different numbers of bins." << endl;
      return;
   }

   for (int i=0; i<= hnum->GetNbinsX()+1; i++) {
      if (hnum->GetBinContent(i) > hden->GetBinContent(i)) {
         cout << "WARNING: " << hnum->GetName() << "(" << i << ") > " << hden->GetName() << "(" << i << ")" << endl;
         hnum->SetBinContent(i,hden->GetBinContent(i));
         hnum->SetBinError(i,hden->GetBinError(i));
      }
   }
}
