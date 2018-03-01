#include "../Include/lhapdf_utils.h"

#include "TFile.h"

TGraphAsymmErrors* pdfuncert_plotter(const char* filename, const char* hname, const char* pdfname, bool rel=true) {
   TFile *f = TFile::Open(filename);
   vector<TH1D*> v;
   int i=0;
   v.push_back((TH1D*) f->Get(Form("%s%d",hname,i)));
   if (TString(pdfname).Contains("EPPS16")) {
      for (i=285; i<=324; i++) {
         v.push_back((TH1D*) f->Get(Form("%s%d",hname,i)));
      }
   }
   for (i=112; i<=167; i++) {
      v.push_back((TH1D*) f->Get(Form("%s%d",hname,i)));
   }
   TGraphAsymmErrors *ans = pdfuncert(v,pdfname);

   if (rel) {
      for (int i=0; i<ans->GetN(); i++) {
         double x = ans->GetX()[i];
         double y = ans->GetY()[i];
         double exl = ans->GetEXlow()[i];
         double exh = ans->GetEXhigh()[i];
         double eyl = ans->GetEYlow()[i];
         double eyh = ans->GetEYhigh()[i];
         ans->SetPoint(i,x,1);
         ans->SetPointError(i,exl,exh,eyl/y,eyh/y);
      }
   }

   return ans;
}
