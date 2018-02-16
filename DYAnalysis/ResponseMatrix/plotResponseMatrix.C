#include "TFile.h"
#include "Include/MyCanvas.C"
#include "BkgEst/interface/defs.h"
#include "Include/CMS_lumi.C"

using namespace DYana;

void plotResponseMatrix(TFile *f, var thevar);

void plotResponseMatrix(const char* matrixfile="ResponseMatrix/ROOTFile_ResponseMatrix_Powheg_MomUnCorr_0.root") {
   TFile *f = TFile::Open(matrixfile);
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      plotResponseMatrix(f, thevar_i);
   }
}
void plotResponseMatrix(TFile *f, var thevar) {
   const char* thevarname = varname(thevar);
   TH1D *h_gen = (TH1D*) f->Get(Form("h_%s_gen",thevarname));
   TH1D *h_reco = (TH1D*) f->Get(Form("h_%s_reco",thevarname));
   TH1D *h_resol = (TH1D*) f->Get(Form("h_%s_resol",thevarname));
   TH2D *h_response = (TH2D*) f->Get(Form("h_%s_response",thevarname));

   MyCanvas c1D(Form("ResponseMatrix/c_%s_1D",thevarname),xaxistitle(thevar),"Entries",800,800);
   MyCanvas cres(Form("ResponseMatrix/c_%s_resol",thevarname),xaxistitle(thevar),"Entries",800,800);
   MyCanvas c2D(Form("ResponseMatrix/c_%s_response",thevarname),xaxistitle(thevar),"Entries",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar) {
      c1D.SetLogx();
      c2D.SetLogx();
      c2D.SetLogy();
      c2D.SetLogz();
   }
   if (thevar==var::mass) c1D.SetRatioRange(0.8,1.2);
   else if (thevar==var::pt) c1D.SetRatioRange(0.9,1.1);
   else c1D.SetRatioRange(0.95,1.05);

   // add stat boxes
   gStyle->SetOptStat("emrou");

   // for CMS lumi
   extraText = "Simulation";
   writeExtraText = true;

   c1D.CanvasWithHistogramsRatioPlot(h_gen,h_reco,"Gen","Reco","Gen/Reco",kBlack,kRed,false,false,"EP","EPSAME");
   CMS_lumi(c1D.c, 111, 0);
   c1D.PrintCanvas();

   cres.CanvasWithOneHistogram(h_resol,"EP",kBlack);
   CMS_lumi(cres.c, 111, 0);
   cres.PrintCanvas();

   c2D.CanvasWith2DHistogram(h_response,"COLZ");
   CMS_lumi(c2D.c, 111, 0);
   c2D.PrintCanvas();

   // normalise the response matrix
   int n = h_response->GetNbinsX();
   for (int i=1; i<=n; i++)
      for (int j=1; j<=n; j++)
         if (i!=j) h_response->SetBinContent(i,j,h_response->GetBinContent(i,j)/sqrt(h_response->GetBinContent(i,i)*h_response->GetBinContent(j,j)));
   for (int i=1; i<=n; i++) h_response->SetBinContent(i,i,1);
}
