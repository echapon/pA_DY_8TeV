#include "TFile.h"
#include "Include/MyCanvas.C"
#include "BkgEst/interface/defs.h"
#include "Include/CMS_lumi.C"

using namespace DYana;

void plotResponseMatrix(TFile *f, var thevar);

void plotResponseMatrix(const char* matrixfile="FSRCorrection/ROOTFile_FSRCorrections_DressedLepton_Powheg_0.root") {
   TFile *f = TFile::Open(matrixfile);
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      plotResponseMatrix(f, thevar_i);
   }
}
void plotResponseMatrix(TFile *f, var thevar) {
   const char* thevarname = varname(thevar);
   TH1D *h_preFSR = (TH1D*) f->Get(Form("h_%s_preFSR",thevarname));
   TH1D *h_postFSR = (TH1D*) f->Get(Form("h_%s_postFSR",thevarname));
   TH1D *h_ratio = (TH1D*) f->Get(Form("h_%s_ratio",thevarname));
   TH2D *h_postpreFSR_tot = (TH2D*) f->Get(Form("h_%s_postpreFSR_tot",thevarname));

   MyCanvas c1D(Form("FSRCorrection/c_%s_1D",thevarname),xaxistitle(thevar),"Entries",800,800);
   MyCanvas cres(Form("FSRCorrection/c_%s_ratio",thevarname),xaxistitle(thevar),"Entries",800,800);
   MyCanvas c2D(Form("FSRCorrection/c_%s_postpreFSR_tot",thevarname),xaxistitle(thevar),"Entries",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar) {
      c1D.SetLogx();
      c2D.SetLogx();
      c2D.SetLogy();
   }
   if (thevar==var::mass) c1D.SetRatioRange(0.8,1.2);
   else if (thevar==var::pt) c1D.SetRatioRange(0.9,1.1);
   else c1D.SetRatioRange(0.95,1.05);

   // add stat boxes
   gStyle->SetOptStat("emrou");

   // for CMS lumi
   extraText = "Simulation";
   writeExtraText = true;

   c1D.CanvasWithHistogramsRatioPlot(h_preFSR,h_postFSR,"pre-FSR","post-FSR","pre-FSR/post-FSR",kBlack,kRed,false,false,"EP","EPSAME");
   CMS_lumi(c1D.c, 111, 0);
   c1D.PrintCanvas();

   cres.CanvasWithOneHistogram(h_ratio,"EP",kBlack);
   CMS_lumi(cres.c, 111, 0);
   cres.PrintCanvas();

   c2D.CanvasWith2DHistogram(h_postpreFSR_tot,"COLZ");
   CMS_lumi(c2D.c, 111, 0);
   c2D.PrintCanvas();

   // normalise the response matrix
   int n = h_postpreFSR_tot->GetNbinsX();
   for (int i=1; i<=n; i++)
      for (int j=1; j<=n; j++)
         if (i!=j) h_postpreFSR_tot->SetBinContent(i,j,h_postpreFSR_tot->GetBinContent(i,j)/sqrt(h_postpreFSR_tot->GetBinContent(i,i)*h_postpreFSR_tot->GetBinContent(j,j)));
   for (int i=1; i<=n; i++) h_postpreFSR_tot->SetBinContent(i,i,1);
}
