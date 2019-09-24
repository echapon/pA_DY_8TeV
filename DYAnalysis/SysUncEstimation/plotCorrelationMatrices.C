#include "syst.C"
#include "../BkgEst/interface/defs.h"
#include "../Include/PlotTools.h"
#include "../Include/MyCanvas.C"

using namespace DYana;

void plotCorrelationMatrices(bool noacc, var thevar) {
   vector<TString> tags;
   tags.push_back("rewNtracks");
   tags.push_back("MomCorr_smooth");
   tags.push_back("tnp_tot");
   if (!noacc) tags.push_back("acceffstat_up");
   else tags.push_back("effstat_up");
   tags.push_back("bkg");
   if (!noacc) tags.push_back("AccEff_theory");
   else tags.push_back("Eff_theory");
   tags.push_back("DetResUnfold_smooth");
   tags.push_back("FSRUnfold_smooth");

   TString cormatname = (noacc) ? "cormat_noacc" : "cormat";

   for (vector<TString>::const_iterator it=tags.begin(); it!=tags.end(); it++) {
      TString systfilename = "./csv/" + *it + "_" + TString(varname(thevar)) + ".csv";
      cout << systfilename << endl;
      TH2D *hm = matrix2hist(readSyst_cor(systfilename.Data()),varname(thevar));

      MyCanvas c("correlation_matrices/"+cormatname+"_"+*it+"_"+varname(thevar),xaxistitle(thevar),xaxistitle(thevar));
      if (thevar==var::mass || thevar==var::pt || thevar==var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) {
         c.SetLogx();
         c.SetLogy();
      }
      c.CanvasWith2DHistogram(hm,"COLZ");
      c.PrintCanvas();
   }

   // and finally the total correlation matrix
   TH2D *hm = matrix2hist(readSyst_all_cor(thevar,"../"),varname(thevar));

   MyCanvas c("correlation_matrices/"+cormatname+"_total_"+TString(varname(thevar)),xaxistitle(thevar),xaxistitle(thevar));
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) {
      c.SetLogx();
      c.SetLogy();
   }
   c.CanvasWith2DHistogram(hm,"COLZ");
   c.PrintCanvas();
}

void plotCorrelationMatrices(bool noacc) {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      plotCorrelationMatrices(noacc, thevar_i);
   }
}
