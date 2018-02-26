#include "../../BkgEst/interface/defs.h"
#include "../../Include/MyCanvas.C"
#include "../../Include/texUtils.h"
#include "../syst.C"

#include "TFile.h"
#include "TH1.h"
#include "TString.h"

#include <vector>

using namespace std;
using DYana::var;

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

void Sys_FSR(var thevar) {
   const char* thevarname = varname(thevar);

   // Powheg vs Pyquen
   TFile *fnom = TFile::Open("../FSRCorrection/xsec_FSRcor_Powheg_MomCorr_0.root");
   TFile *fpyq = TFile::Open("../FSRCorrection/xsec_FSRcor_Pyquen_MomCorr_0.root");
   TH1D *hnom = (TH1D*) fnom->Get(Form("h_Measured_unfoldedMLE_%s",thevarname));
   TH1D *hpyq = (TH1D*) fpyq->Get(Form("h_Measured_unfoldedMLE_%s",thevarname));
   
   TH1D *hmod = (TH1D*) hnom->Clone("hmod");
   hmod->Add(hpyq,-1); // take the difference
   hmod->Divide(hnom); // make it relative
   hmod->Scale(100.);  // make it in %
   TGraphAsymmErrors *gmod = h2gae(hmod);
   map<bin,syst> systmod;
   for (int i=0; i<gmod->GetN(); i++) {
      syst thesyst;
      thesyst.name = "Unfold (det.res., model)";
      thesyst.value = gmod->GetY()[i]/100.;
      double x = gmod->GetX()[i];
      double xmin = x-gmod->GetEXlow()[i];
      double xmax = x+gmod->GetEXhigh()[i];
      systmod[bin(xmin,xmax)] = thesyst;
      gmod->SetPointEYlow(i,0);
      gmod->SetPointEYhigh(i,0);
   }

   // pPb vs PbP
   TFile *fPbP = TFile::Open("../FSRCorrection/xsec_FSRcor_Powheg_MomCorr_1.root");
   TFile *fpPb = TFile::Open("../FSRCorrection/xsec_FSRcor_Powheg_MomCorr_2.root");
   TH1D *hPbP = (TH1D*) fPbP->Get(Form("h_Measured_unfoldedMLE_%s",thevarname));
   TH1D *hpPb = (TH1D*) fpPb->Get(Form("h_Measured_unfoldedMLE_%s",thevarname));
   
   TH1D *hrun = (TH1D*) hpPb->Clone("hrun");
   hrun->Add(hPbP,-1); // take the difference
   hrun->Divide(hpPb); // make it relative
   hrun->Scale(0.5);   // take half of the difference (https://en.wikipedia.org/wiki/Variance#Sample_variance)
   hrun->Scale(100.);  // make it in %
   TGraphAsymmErrors *grun = h2gae(hrun);
   map<bin,syst> systrun;
   for (int i=0; i<grun->GetN(); i++) {
      syst thesyst;
      thesyst.name = "Unfold (det.res., pPb/PbP)";
      thesyst.value = grun->GetY()[i]/100.;
      double x = grun->GetX()[i];
      double xmin = x-grun->GetEXlow()[i];
      double xmax = x+grun->GetEXhigh()[i];
      systrun[bin(xmin,xmax)] = thesyst;
      grun->SetPointEYlow(i,0);
      grun->SetPointEYhigh(i,0);
   }

   // combine the two
   vector< map<bin,syst> > theSysts;
   theSysts.push_back(systmod);
   theSysts.push_back(systrun);
   map<bin,syst> syst_tot = combineSyst(theSysts,"Unfold (FSR)");

   // print the csv
   ofstream of_syst(Form("csv/FSRUnfold_%s.csv",thevarname));
   of_syst << "Unfold (det.res.)" << endl;
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
   vector<TGraphAsymmErrors*> graphs;
   graphs.push_back(new TGraphAsymmErrors(x.size(),x.data(),dy.data(),y.data(),y.data(),y.data(),y.data()));
   graphs[0]->Sort();
   graphs[0]->SetMinimum(0);
   graphs.push_back(gmod);
   graphs.push_back(grun);
   vector<TString> ynames;
   ynames.push_back("Total");
   ynames.push_back("Pyquen vs Powheg");
   ynames.push_back("pPb vs PbP");


   // MyCanvas c1(Form("systematics_UnfoldFSR_%s",thevarname),xaxistitle(thevar),"Rel. uncertainty (%)",800,800);
   TString cname(Form("systematics_UnfoldFSR_%s",thevarname));
   MyCanvas c1(cname,xaxistitle(thevar),"Rel. uncertainty (%)",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar) c1.SetLogx();
   c1.SetYRange(-1.9,1.9);
   c1.CanvasWithMultipleGraphs(graphs,ynames, "LPX");
   c1.PrintCanvas();
   c1.PrintCanvas_C();
}

void Sys_FSR() {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      Sys_FSR(thevar_i);
   }
}
