#ifndef lhapdf_utils_h
#define lhapdf_utils_h


#include "TH1.h"
#include "TGraphAsymmErrors.h"

#include "LHAPDF/PDFSet.h"
#include "LHAPDF/PDF.h"

#include <iostream>

using namespace std;
using namespace LHAPDF;

TGraphAsymmErrors* pdfuncert(vector<TH1D*> h, const char* pdfname) {
   PDFSet pdfset(pdfname);
   size_t nm = pdfset.size();
   if (h.size() != nm) {
      cout << "Error, expected " << nm << " histos for " << pdfname << ", but I got << " << h.size() << endl;
      return NULL;
   }

   int n = h[0]->GetNbinsX();
   TGraphAsymmErrors *g = new TGraphAsymmErrors(n);
   g->SetName(Form("%s_pdf",h[0]->GetName()));

   for (int i=0; i<n; i++) { // loop on the bins
      double x = h[0]->GetBinCenter(i+1);
      double exl = h[0]->GetBinWidth(i+1)/2.;
      double exh = exl;

      // loop on the members to get the PDF uncertainty
      vector<double> y;
      for (unsigned int j=0; j<nm; j++) y.push_back(h[j]->GetBinContent(i+1));

      double cl = 100. *erf(1/sqrt(2)); // we want 68% CL (1 sigma)
      PDFUncertainty u = pdfset.uncertainty(y,cl);

      // set point
      g->SetPoint(i,x,y[0]);
      // set error
      g->SetPointError(i,exl,exh,u.errminus,u.errplus);
   }

   return g;
}

TGraphAsymmErrors* hist2graph(TH1 *hist, double syst=0) {
   int n = hist->GetNbinsX();
   TGraphAsymmErrors *ans = new TGraphAsymmErrors(n);
   ans->SetName(TString(hist->GetName()) + "_graph");
   for (int i=0; i<n; i++) {
      double x = hist->GetBinCenter(i+1);
      double ex = hist->GetBinWidth(i+1)/2.;
      double y = hist->GetBinContent(i+1);
      double ey = sqrt(pow(hist->GetBinError(i+1),2)+syst*syst);
      ans->SetPoint(i,x,y);
      ans->SetPointError(i,ex,ex,ey,ey);
   }

   return ans;
}

// plot the pdf
TH1D* hpdf(const char* pdfname, int imem, int pid, double q2, double xmin, double xmax, int npoints) {
   // make a constant binning in log(x)
   double *bins = new double[npoints+1];
   xmin = max(xmin,1e-10);
   xmax = min(xmax,1.);
   double logx = log(xmin);
   double dlogx = (log(xmax)-log(xmin))/npoints;
   for (int i=0; i<=npoints; i++) {
      bins[i] = exp(logx);
      logx += dlogx;
   }

   TH1D *ans = new TH1D(Form("h_%s_%d_%d_%f",pdfname,imem,pid,q2),"",npoints,bins);
   const PDF* pdf = mkPDF(pdfname, imem);

   // fill the histo
   for (int i=1; i<=npoints; i++) {
      if (i==1) cout << pdf->xfxQ2(pid,ans->GetBinCenter(i),q2) << endl;
      ans->SetBinContent(i,pdf->xfxQ2(pid,ans->GetBinCenter(i),q2));
   }

   delete[] bins;
   return ans;
}

// plot the pdf with uncertainties
TGraphAsymmErrors* gpdf(const char* pdfname, int pid, double q2, double xmin, double xmax, int npoints) {
   PDFSet pdfset(pdfname);
   size_t nm = pdfset.size();
   vector<TH1D*> v;
   for (size_t i=0; i<nm; i++) {
      v.push_back(hpdf(pdfname,i,pid,q2,xmin,xmax,npoints));
   }
   return pdfuncert(v,pdfname);
}

#endif // #ifndef lhapdf_utils_h
