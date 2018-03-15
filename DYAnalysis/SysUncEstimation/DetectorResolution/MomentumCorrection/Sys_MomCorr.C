#include "../../../BkgEst/interface/defs.h"
#include "../../../Include/MyCanvas.C"
#include "../../../Include/texUtils.h"
#include "../../syst.C"

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

const int   nmax[9]     = {1,100,1,1,5,5,0,7,1};
const char* varnames[9] = {"Default","Stat","Zpt","Ewk","CorDm","FitDm","LHEw","Run","AMCNLO"};

TH2D* Sys_MomCorr_compute(TString file, var thevar, TGraphAsymmErrors *&gErr, int itype) {
   // itype = 1 -> stat
   //         2 -> Zpt
   //         4 -> CorDm
   //         5 -> FitDm
   //         7 -> Run

   TString hname;
   if (thevar==var::mass) hname = "h_mass2";
   else if (thevar==var::pt) hname = "h_diPt2_M60to120";
   else if (thevar==var::rap1560) hname = "h_diRap2_M15to60";
   else if (thevar==var::rap60120) hname = "h_diRap2_M60to120";
   else if (thevar==var::phistar) hname = "h_Phistar2_M60to120";

   vector<TGraphAsymmErrors*> graphs;
   vector<TMatrixT<double> > mcor;
   vector<TString> graphNames;
   gErr = NULL;

   MyCanvas c1(Form("DetectorResolution/MomentumCorrection/momcorvariations_%s%d",varname(thevar),itype),xaxistitle(thevar),"Ratio to nominal",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar) c1.SetLogx();

   TFile *f = TFile::Open(file);
   TH1D *hnom = (TH1D*) f->Get(hname + "_Data1");
   hnom->Add((TH1D*) f->Get(hname + "_Data2"));

   gErr = new TGraphAsymmErrors(hnom,hnom,"pois n"); // fill gErr with 1's
   for (int j=0; j<gErr->GetN(); j++) {
      gErr->SetPointEYlow(j,0);
      gErr->SetPointEYhigh(j,0);
   }
   int nbins = gErr->GetN();

   for (int i=0; i<nmax[itype]; i++) {
      // stat needs only 1 matrix
      if (!(itype==1 && i>0)) mcor.push_back(TMatrixT<double>(nbins,nbins));

      if (itype==7 && i!=6) continue; // only do run H for run variations
      TString s = file;
      TFile *fi = TFile::Open(s.ReplaceAll("00",Form("%d%d",itype,i)));
      TH1D* htmp = (TH1D*) fi->Get(hname + "_Data1");
      htmp->Add((TH1D*) fi->Get(hname + "_Data2"));

      // ratio to nominal
      TGraphAsymmErrors *gtmp = new TGraphAsymmErrors(htmp,hnom,"pois n");
      for (int j=0; j<gErr->GetN(); j++) {
         if (itype != 1) {
            // take the envelope, except for stat
            gErr->SetPointEYlow(j,-min(gtmp->GetY()[j]-gErr->GetY()[j],-gErr->GetEYlow()[j]));
            gErr->SetPointEYhigh(j,max(gtmp->GetY()[j]-gErr->GetY()[j],gErr->GetEYhigh()[j]));
            
            double diffj = gtmp->GetY()[j]-gErr->GetY()[j];
            for (int k=0; k<nbins; k++) {
               double diffk = gtmp->GetY()[k]-gErr->GetY()[k];
               mcor[i][j][k] = diffj*diffk;
            }
         } else {
            // for stat: take the RMS
            gErr->SetPointEYlow(j,pow(gErr->GetEYlow()[j],2)+pow(gtmp->GetY()[j]-1,2));
            gErr->SetPointEYhigh(j,pow(gErr->GetEYhigh()[j],2)+pow(gtmp->GetY()[j]-1,2));
            
            double diffj = gtmp->GetY()[j]-gErr->GetY()[j];
            for (int k=0; k<nbins; k++) {
               double diffk = gtmp->GetY()[k]-gErr->GetY()[k];
               if (i==0) mcor[0][j][k] = 0;
               mcor[0][j][k] += diffj*diffk / (nmax[itype]-1);
            }
         }
      }
      graphs.push_back(gtmp);
      graphNames.push_back(Form("%s %d",varnames[itype],i));

      // turn the cov matrix into a correlation matrix
      if (itype != 1) {
         for (int j=0; j<nbins; j++) {
            for (int k=0; k<nbins; k++) {
               if (j==k) continue;
               if (mcor[i][j][j]==0 || mcor[i][k][k]==0) mcor[i][j][k] = 0;
               else mcor[i][j][k] *= 1./sqrt(fabs(mcor[i][j][j]*mcor[i][k][k]));
            }
         }
         for (int j=0; j<nbins; j++) mcor[i][j][j] = 1;
      }
   }

   // for stat: need to take the square root for RMS
   if (itype==1) {
      for (int j=0; j<gErr->GetN(); j++) {
         gErr->SetPointEYlow(j,sqrt(gErr->GetEYlow()[j]));
         gErr->SetPointEYhigh(j,sqrt(gErr->GetEYhigh()[j]));
      }

      // turn the cov matrix into a correlation matrix
      for (int j=0; j<nbins; j++) {
         for (int k=0; k<nbins; k++) {
            if (j==k) continue;
            if (mcor[0][j][j]==0 || mcor[0][k][k]==0) mcor[0][j][k] = 0;
            else mcor[0][j][k] *= 1./sqrt(fabs(mcor[0][j][j]*mcor[0][k][k]));
         }
      }
      for (int j=0; j<nbins; j++) mcor[0][j][j] = 1;
   }

   // produce the correlation matrix as the average of the individual correlation matrices
   TH2D *ans = new TH2D(TString(hnom->GetName())+TString("_corr")+(Long_t)itype,TString(hnom->GetTitle())+TString(" (corr ")+(Long_t)itype+")",
         nbins,hnom->GetXaxis()->GetXbins()->GetArray(),
         nbins,hnom->GetXaxis()->GetXbins()->GetArray());
   int nvars = mcor.size();
   for (int j=0; j<nbins; j++) {
      for (int k=0; k<nbins; k++) {
         if (itype!=1) {
            double val=0;
            for (int i=0; i<nvars; i++) val += mcor[i][j][k];
            ans->SetBinContent(j+1,k+1,itype==7 ? val : val/(nvars)); // for itype==7 (Run), there is only 1 variation
         } else {
            ans->SetBinContent(j+1,k+1,mcor[0][j][k]);
         }
      }
   }

   c1.SetYRange(0.96,1.04);
   c1.CanvasWithMultipleGraphs(graphs, graphNames, "LPX");
   c1.PrintCanvas();
   c1.PrintCanvas_C();

   return ans;
}

void Sys_MomCorr(const char* file, var thevar) {
   vector<TGraphAsymmErrors*> graphs;
   vector<TH2D*> hcor;
   TGraphAsymmErrors *gErr = NULL;
   hcor.push_back(NULL);
   graphs.push_back(gErr); // placeholder for the total
   hcor.push_back(Sys_MomCorr_compute(file, thevar, gErr, 1));
   graphs.push_back(gErr);
   hcor.push_back(Sys_MomCorr_compute(file, thevar, gErr, 2));
   graphs.push_back(gErr);
   hcor.push_back(Sys_MomCorr_compute(file, thevar, gErr, 4));
   graphs.push_back(gErr);
   hcor.push_back(Sys_MomCorr_compute(file, thevar, gErr, 5));
   graphs.push_back(gErr);
   hcor.push_back(Sys_MomCorr_compute(file, thevar, gErr, 7));
   graphs.push_back(gErr);

   // print to a TeX
   vector<TString> ynames;
   ynames.push_back("Total");
   ynames.push_back("stat");
   ynames.push_back("Zpt");
   ynames.push_back("CorDm");
   ynames.push_back("FitDm");
   ynames.push_back("Run");

   // produce the combined syst, plot it and print it
   vector< map<bin,syst> > theSysts;
   for (unsigned int i=1; i<graphs.size(); i++) {
      map<bin,syst> thesyst;
      for (int j=0; j<graphs[i]->GetN(); j++) {
         double eyl, eyh, x, xmin, xmax;
         x = graphs[i]->GetX()[j];
         xmin = x - graphs[i]->GetEXlow()[j];
         xmax = x + graphs[i]->GetEXhigh()[j];
         eyl = graphs[i]->GetEYlow()[j];
         eyh = graphs[i]->GetEYhigh()[j];

         // the graph was centered around 1 and with errors corresponding to the syst: put y at the syst and with 0 error
         graphs[i]->SetPoint(j,x,max(eyl,eyh)*100.);
         graphs[i]->SetPointEYlow(j,0);
         graphs[i]->SetPointEYhigh(j,0);

         syst tmpsyst;
         tmpsyst.name = ynames[i];
         tmpsyst.value = max(eyl,eyh);
         thesyst[bin(xmin,xmax)] = tmpsyst;
      }
      theSysts.push_back(thesyst);
   }

   map<bin,syst> syst_tot = combineSyst(theSysts,"MomCorr");

   // compute the correlation matrix
   // first, build covariance matrices
   int nbins = graphs[1]->GetN();
   vector<TMatrixT<double> > covmat;
   TMatrixT<double> cov_all(nbins,nbins), cor_all(nbins,nbins);
   for (int j=0; j<nbins; j++) {
      double xj, xminj, xmaxj;
      xj = graphs[1]->GetX()[j];
      xminj = xj-graphs[1]->GetEXlow()[j];
      xmaxj = xj+graphs[1]->GetEXhigh()[j];
      bin binj(xminj,xmaxj);
      for (int k=0; k<nbins; k++) {
         double xk, xmink, xmaxk;
         xk = graphs[1]->GetX()[k];
         xmink = xk-graphs[1]->GetEXlow()[k];
         xmaxk = xk+graphs[1]->GetEXhigh()[k];
         bin bink(xmink,xmaxk);

         if (j==0 && k==0) covmat.push_back(TMatrixT<double>(nbins,nbins));
         for (unsigned int i=1; i<graphs.size(); i++) {
            if (j==0 && k==0) covmat.push_back(TMatrixT<double>(nbins,nbins));
            covmat[i][j][k] = theSysts[i-1][binj].value * theSysts[i-1][bink].value * hcor[i]->GetBinContent(j+1,k+1); 
            cov_all[j][k] += covmat[i][j][k];
         }

         // compute the correlation
         if (j==k) cor_all[j][k] = 1;
         else if (syst_tot[binj].value==0 || syst_tot[bink].value==0) cor_all[j][k] = 0;
         else cor_all[j][k] = cov_all[j][k] / (syst_tot[binj].value*syst_tot[bink].value);
      }
   }
   
   // print the csv
   ofstream of_syst(Form("csv/MomCorr_%s.csv",varname(thevar)));
   ofstream of_cor(Form("cor/MomCorr_%s.csv",varname(thevar)));
   of_syst << "MomCorr" << endl;
   of_cor << "MomCorr" << endl;
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

   // make the graph for the total syst
   vector<double> x, y, dx, dy;
   for (map<bin, syst>::const_iterator it=syst_tot.begin(); it!=syst_tot.end(); it++) {
      bin it2 = it->first;

      double low = it2.low(), high = it2.high();
      x.push_back((low+high)/2.);
      dx.push_back((high-low)/2.);
      y.push_back(0);
      dy.push_back(it->second.value*100.);
   }
   graphs[0] = new TGraphAsymmErrors(x.size(),x.data(),dy.data(),y.data(),y.data(),y.data(),y.data());
   graphs[0]->Sort();
   graphs[0]->SetMinimum(0);


   MyCanvas c1(Form("systematics_MomCorr_%s",varname(thevar)),xaxistitle(thevar),"Rel. uncertainty (%)",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar) c1.SetLogx();
   if (thevar==var::pt) c1.SetYRange(0,3.9);
   c1.CanvasWithMultipleGraphs(graphs,ynames, "LPX");
   c1.PrintCanvas();
   c1.PrintCanvas_C();
}

void Sys_MomCorr(const char* file) {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      Sys_MomCorr(file,thevar_i);
   }
}
