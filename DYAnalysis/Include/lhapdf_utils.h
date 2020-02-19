#ifndef lhapdf_utils_h
#define lhapdf_utils_h


#include "TH1.h"
#include "TH2.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "LHAPDF/PDFSet.h"
#include "LHAPDF/PDF.h"

#include <iostream>

using namespace std;
using namespace LHAPDF;

TGraphAsymmErrors* pdfuncert(vector<TH1D*> h, const char* pdfname, bool errnom=false) {
   // if errnom = true, then we also include the uncertainty on the nominal
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
      double err_low = u.errminus;
      double err_high = u.errplus;
      if (errnom) {
         err_low = sqrt(pow(err_low,2)+pow(h[0]->GetBinError(i+1),2));
         err_high = sqrt(pow(err_high,2)+pow(h[0]->GetBinError(i+1),2));
      }
      g->SetPointError(i,exl,exh,err_low,err_high);
   }

   return g;
}

TH2D* pdfcorr(vector<TH1D*> h, const char* pdfname) {
   PDFSet pdfset(pdfname);
   size_t nm = pdfset.size();
   if (h.size() != nm) {
      cout << "Error, expected " << nm << " histos for " << pdfname << ", but I got << " << h.size() << endl;
      return NULL;
   }

   int n = h[0]->GetNbinsX();
   TH2D *ans = new TH2D(TString(h[0]->GetName())+"_pdfcorr",TString(h[0]->GetTitle())+" (PDF corr)",
         n,h[0]->GetXaxis()->GetXbins()->GetArray(),
         n,h[0]->GetXaxis()->GetXbins()->GetArray());

   for (int i=1; i<=n; i++) { // loop on the bins in X
      for (int j=1; j<=n; j++) { // loop on the bins in Y
         // loop on the members to get the PDF uncertainty
         vector<double> y1,y2;
         for (unsigned int k=0; k<nm; k++) {
            y1.push_back(h[k]->GetBinContent(i));
            y2.push_back(h[k]->GetBinContent(j));
         }

         double c = pdfset.correlation(y1,y2);

         // set point
         int b = ans->GetBin(i,j);
         ans->SetBinContent(b,c);
      }
   }

   return ans;
}

TMatrixT<double> hist2mat(TH2D* thehist) {
   int nbins = thehist->GetNbinsX();
   TMatrixT<double> ans(nbins,nbins);

   for (int i=0; i<nbins; i++) {
      for (int j=0; j<nbins; j++) {
         ans[i][j] = thehist->GetBinContent(i+1,j+1);
      }
   }

   return ans;
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
// NB: if pid>1000, we compute the valence contribution for pid-1000
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
      if (pid<1000) ans->SetBinContent(i,pdf->xfxQ2(pid,ans->GetBinCenter(i),q2));
      else  ans->SetBinContent(i,pdf->xfxQ2(pid-1000,ans->GetBinCenter(i),q2) - pdf->xfxQ2(-(pid-1000),ans->GetBinCenter(i),q2));
   }

   delete[] bins;
   return ans;
}

// plot the pdf with uncertainties
TGraphAsymmErrors* gpdf(const char* pdfname, int pid, double q2, double xmin, double xmax, int npoints, const char* refpdfname="") {
   PDFSet pdfset(pdfname);
   size_t nm = pdfset.size();
   vector<TH1D*> v;
   for (size_t i=0; i<nm; i++) {
      v.push_back(hpdf(pdfname,i,pid,q2,xmin,xmax,npoints));
   }
   TGraphAsymmErrors *ans = pdfuncert(v,pdfname);

   TH1D *hrefpdf;
   bool rel = (TString(refpdfname)!="");
   if (rel) {
      hrefpdf = hpdf(refpdfname,0,pid,q2,xmin,xmax,npoints);
   }

   // if a ref pdf is provided: divide the answer by it
   if (rel) {
      for (int i=0; i<ans->GetN(); i++) {
         double x = ans->GetX()[i];
         double y = ans->GetY()[i];
         double exl = ans->GetEXlow()[i];
         double exh = ans->GetEXhigh()[i];
         double eyl = ans->GetEYlow()[i];
         double eyh = ans->GetEYhigh()[i];
         double yref = hrefpdf->GetBinContent(i+1);
         ans->SetPoint(i,x,y/yref);
         ans->SetPointError(i,exl,exh,eyl/yref,eyh/yref);
      }
   }

   return ans;
}

// test
TH2D *testpdfcorr(const char* pdfname, int pid, double q2, double xmin, double xmax, int npoints) {
   PDFSet pdfset(pdfname);
   size_t nm = pdfset.size();
   vector<TH1D*> v;
   for (size_t i=0; i<nm; i++) {
      v.push_back(hpdf(pdfname,i,pid,q2,xmin,xmax,npoints));
   }
   return pdfcorr(v,pdfname);
}

// compare g, s, uv, dv, u, d, ubar, dbar in a single canvas
// can provide a comma-separated list of pdf names
void canvaspdfs(const char* pdfnames, double q2, double xmin, double xmax, int npoints, const char* refpdfname="") {
   TString tpdfnames(pdfnames);

   vector<int> colors;
   colors.push_back(kBlue+1);
   colors.push_back(kGreen+1);
   colors.push_back(kYellow+1);
   colors.push_back(kRed+1);
   colors.push_back(kCyan+1);
   colors.push_back(kMagenta+1);
   vector<int> styles;
   styles.push_back(3004);
   styles.push_back(3005);
   styles.push_back(3006);
   styles.push_back(3007);
   styles.push_back(3215);
   styles.push_back(3275);

   TCanvas *cpdf = new TCanvas("cpdf","cpdf",600,1200);
   cpdf->Divide(2,4);
   cpdf->cd(1);
   gPad->SetLogx();

   double x1=0.1, y1=0.7, x2=0.4, y2=0.9;
   if (TString(refpdfname)=="") {x1=0.65; x2=0.95;}
   TLegend *tleg = new TLegend(x1,y1,x2,y2);
   tleg->SetBorderSize(0);

   TString tok;
   Ssiz_t from = 0;
   int cnt=0;
   int pid=21;
   while (tpdfnames.Tokenize(tok, from, ",")) {
      TGraphAsymmErrors *g = gpdf(tok, pid, q2, xmin, xmax, npoints, refpdfname);
      g->SetLineColor(colors[cnt]);
      g->SetFillColor(colors[cnt]);
      g->SetFillStyle(styles[cnt]);
      if (cnt==0) g->Draw("ALE3");
      else g->Draw("LE3");
      tleg->AddEntry(g,tok,"LF");
      cnt++;
   }
   tleg->Draw();

   cpdf->cd(2);
   gPad->SetLogx();
   tok="";
   from = 0;
   cnt=0;
   pid=3;
   while (tpdfnames.Tokenize(tok, from, ",")) {
      TGraphAsymmErrors *g = gpdf(tok, pid, q2, xmin, xmax, npoints, refpdfname);
      g->SetLineColor(colors[cnt]);
      g->SetFillColor(colors[cnt]);
      g->SetFillStyle(styles[cnt]);
      if (cnt==0) g->Draw("ALE3");
      else g->Draw("LE3");
      cnt++;
   }

   cpdf->cd(3);
   gPad->SetLogx();
   tok="";
   from = 0;
   cnt=0;
   pid=1002;
   while (tpdfnames.Tokenize(tok, from, ",")) {
      TGraphAsymmErrors *g = gpdf(tok, pid, q2, xmin, xmax, npoints, refpdfname);
      g->SetLineColor(colors[cnt]);
      g->SetFillColor(colors[cnt]);
      g->SetFillStyle(styles[cnt]);
      if (cnt==0) g->Draw("ALE3");
      else g->Draw("LE3");
      cnt++;
   }

   cpdf->cd(4);
   gPad->SetLogx();
   tok="";
   from = 0;
   cnt=0;
   pid=1001;
   while (tpdfnames.Tokenize(tok, from, ",")) {
      TGraphAsymmErrors *g = gpdf(tok, pid, q2, xmin, xmax, npoints, refpdfname);
      g->SetLineColor(colors[cnt]);
      g->SetFillColor(colors[cnt]);
      g->SetFillStyle(styles[cnt]);
      if (cnt==0) g->Draw("ALE3");
      else g->Draw("LE3");
      cnt++;
   }

   cpdf->cd(5);
   gPad->SetLogx();
   tok="";
   from = 0;
   cnt=0;
   pid=2;
   while (tpdfnames.Tokenize(tok, from, ",")) {
      TGraphAsymmErrors *g = gpdf(tok, pid, q2, xmin, xmax, npoints, refpdfname);
      g->SetLineColor(colors[cnt]);
      g->SetFillColor(colors[cnt]);
      g->SetFillStyle(styles[cnt]);
      if (cnt==0) g->Draw("ALE3");
      else g->Draw("LE3");
      cnt++;
   }

   cpdf->cd(6);
   gPad->SetLogx();
   tok="";
   from = 0;
   cnt=0;
   pid=1;
   while (tpdfnames.Tokenize(tok, from, ",")) {
      TGraphAsymmErrors *g = gpdf(tok, pid, q2, xmin, xmax, npoints, refpdfname);
      g->SetLineColor(colors[cnt]);
      g->SetFillColor(colors[cnt]);
      g->SetFillStyle(styles[cnt]);
      if (cnt==0) g->Draw("ALE3");
      else g->Draw("LE3");
      cnt++;
   }

   cpdf->cd(7);
   gPad->SetLogx();
   tok="";
   from = 0;
   cnt=0;
   pid=-2;
   while (tpdfnames.Tokenize(tok, from, ",")) {
      TGraphAsymmErrors *g = gpdf(tok, pid, q2, xmin, xmax, npoints, refpdfname);
      g->SetLineColor(colors[cnt]);
      g->SetFillColor(colors[cnt]);
      g->SetFillStyle(styles[cnt]);
      if (cnt==0) g->Draw("ALE3");
      else g->Draw("LE3");
      cnt++;
   }

   cpdf->cd(8);
   gPad->SetLogx();
   tok="";
   from = 0;
   cnt=0;
   pid=-1;
   while (tpdfnames.Tokenize(tok, from, ",")) {
      TGraphAsymmErrors *g = gpdf(tok, pid, q2, xmin, xmax, npoints, refpdfname);
      g->SetLineColor(colors[cnt]);
      g->SetFillColor(colors[cnt]);
      g->SetFillStyle(styles[cnt]);
      if (cnt==0) g->Draw("ALE3");
      else g->Draw("LE3");
      cnt++;
   }
}

#endif // #ifndef lhapdf_utils_h
