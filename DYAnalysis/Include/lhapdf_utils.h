#ifndef lhapdf_utils_h
#define lhapdf_utils_h


#include "TH1.h"
#include "TGraphAsymmErrors.h"

#include "LHAPDF/PDFSet.h"

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

      double cl = 100. *erf(1/sqrt(2));
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

#endif // #ifndef lhapdf_utils_h
