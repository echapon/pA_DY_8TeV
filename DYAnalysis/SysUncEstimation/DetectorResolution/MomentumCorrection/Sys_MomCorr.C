#include "../../../BkgEst/interface/defs.h"
#include "../../../Include/MyCanvas.C"
#include "../../../Include/texUtils.h"
#include "../../syst.C"

#include "TFile.h"
#include "TH1.h"

#include <vector>

using namespace std;
using DYana::var;

// HOW TO USE THIS FILE
// [0] .include /dir/to/lhapdf/include
// [1] gSystem->Load("/dir/to/lhapdf/lib/libLHAPDF.so");
// .L AccEff/Sys_AccEff.C+

const int   nmax[9]     = {1,100,1,1,5,5,0,7,1};
const char* varnames[9] = {"Default","Stat","Zpt","Ewk","CorDm","FitDm","LHEw","Run","AMCNLO"};

void Sys_MomCorr_compute(TString file, var thevar, TGraphAsymmErrors *&gErr, int itype) {
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

   for (int i=0; i<nmax[itype]; i++) {
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
         } else {
            // for stat: take the RMS
            gErr->SetPointEYlow(j,pow(gErr->GetEYlow()[j],2)+pow(gtmp->GetY()[j]-1,2));
            gErr->SetPointEYhigh(j,pow(gErr->GetEYhigh()[j],2)+pow(gtmp->GetY()[j]-1,2));
         }
      }
      graphs.push_back(gtmp);
      graphNames.push_back(Form("%s %d",varnames[itype],i));
   }

   // for stat: need to take the square root for RMS
   if (itype==1) {
      for (int j=0; j<gErr->GetN(); j++) {
         gErr->SetPointEYlow(j,sqrt(gErr->GetEYlow()[j]));
         gErr->SetPointEYhigh(j,sqrt(gErr->GetEYhigh()[j]));
      }
   }

   c1.SetYRange(0.96,1.04);
   c1.CanvasWithMultipleGraphs(graphs, graphNames, "LPX");
   c1.PrintCanvas();
   c1.PrintCanvas_C();
}

void Sys_MomCorr(const char* file, var thevar) {
   vector<TGraphAsymmErrors*> graphs;
   TGraphAsymmErrors *gErr = NULL;
   graphs.push_back(gErr); // placeholder for the total
   Sys_MomCorr_compute(file, thevar, gErr, 1);
   graphs.push_back(gErr);
   Sys_MomCorr_compute(file, thevar, gErr, 2);
   graphs.push_back(gErr);
   Sys_MomCorr_compute(file, thevar, gErr, 4);
   graphs.push_back(gErr);
   Sys_MomCorr_compute(file, thevar, gErr, 5);
   graphs.push_back(gErr);
   Sys_MomCorr_compute(file, thevar, gErr, 7);
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
         graphs[i]->SetPoint(j,x,max(eyl,eyh));
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
   
   // print the csv
   ofstream of_syst(Form("csv/MomCorr_%s.csv",varname(thevar)));
   of_syst << "MomCorr" << endl;
   for (map<bin,syst>::const_iterator it=syst_tot.begin(); it!=syst_tot.end(); it++) {
      of_syst << it->first.low() << ", " << it->first.high() << ", " << it->second.value << endl;
   }
   of_syst.close();

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


   MyCanvas c1(Form("systematics_MomCorr_%s",varname(thevar)),xaxistitle(thevar),"Rel. uncertainty (%)",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar) c1.SetLogx();
   // if (thevar==var::phistar) c1.SetYRange(0,6);
   c1.CanvasWithMultipleGraphs(graphs,ynames, "LP");
   c1.PrintCanvas();
   c1.PrintCanvas_C();
}

void Sys_MomCorr(const char* file) {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      Sys_MomCorr(file,thevar_i);
   }
}
