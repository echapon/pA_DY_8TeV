#include "syst.C"
#include "../BkgEst/interface/defs.h"
#include "../Include/PlotTools.h"
#include "../Include/MyCanvas.C"

using namespace DYana;

void plotCorrelationMatrices(bool noacc, var thevar) {
   // general multiplicator to enlarge or reduce the size of everything
   const float sizemod = (thevar==var::mass) ? 0.9 : 1.1;
   float lumiTextSize0 = lumiTextSize;
   float lumiTextOffset0 = lumiTextSize;
   float cmsTextSize0 = cmsTextSize;
   float cmsTextOffset0 = cmsTextSize;
   lumiTextSize *= sizemod;
   if (sizemod>1) lumiTextOffset *= 0.5;
   else lumiTextOffset *= 0.8;
   cmsTextSize *= sizemod;
   if (sizemod>1) cmsTextOffset *= 0.5;
   else cmsTextOffset *= 0.8;

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


   // better for the correlation matrix
   const Int_t Number = 3;
   Double_t Red[Number]    = { 0.00, 1.00, 1.00};
   Double_t Green[Number]  = { 0.00, 1.00, 0.00};
   Double_t Blue[Number]   = { 1.00, 1.00, 0.00};
   Double_t Length[Number] = { 0.00, 0.50, 1.00 };
   Int_t nb=50;
   TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);

   for (vector<TString>::const_iterator it=tags.begin(); it!=tags.end(); it++) {
      TString systfilename = "./csv/" + *it + "_" + TString(varname(thevar)) + ".csv";
      cout << systfilename << endl;
      TH2D *hm = matrix2hist(readSyst_cor(systfilename.Data()),varname(thevar));

		if (thevar==var::mass || thevar==var::mass3bins || thevar==var::pt || thevar == var::phistar || thevar == var::pt1560 || thevar == var::phistar1560) {
			hm->GetXaxis()->SetMoreLogLabels();
			hm->GetYaxis()->SetMoreLogLabels();
		}




      MyCanvas c("correlation_matrices/"+cormatname+"_"+*it+"_"+varname(thevar),xaxistitle(thevar),xaxistitle(thevar));
      if (thevar==var::mass || thevar==var::pt || thevar==var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) {
         c.SetLogx();
         c.SetLogy();
      }
      c.SetZRange(-1,1);
      TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
/*
		if (thevar==var::mass) {
			c.LowerEdge_X=bins[0];c.UpperEdge_X=bins[binnum];
			c.LowerEdge_Y=bins[0];c.UpperEdge_Y=bins[binnum];
		}
		else if (thevar==var::mass3bins) {
			c.LowerEdge_X=bins3[0];c.UpperEdge_X=bins3[binnum3];
			c.LowerEdge_Y=bins3[0];c.UpperEdge_Y=bins3[binnum3];
		}
		else if (thevar==var::pt) {
			c.LowerEdge_X=0.5;c.UpperEdge_X=ptbin_meas[ptbinnum_meas-1];
			c.LowerEdge_Y=0.5;c.UpperEdge_Y=ptbin_meas[ptbinnum_meas-1];
		}
		else if (thevar==var::pt1560) {
			c.LowerEdge_X=0.5;c.UpperEdge_X=ptbin_meas_1560[ptbinnum_meas_1560-1];
			c.LowerEdge_Y=0.5;c.UpperEdge_Y=ptbin_meas_1560[ptbinnum_meas_1560-1];
		}
		else if (thevar==var::phistar) {
			c.LowerEdge_X=0.002;c.UpperEdge_X=phistarbin[phistarnum];
			c.LowerEdge_Y=0.002;c.UpperEdge_Y=phistarbin[phistarnum];
		}
*/
		if (thevar==var::phistar || thevar==var::phistar1560) {
			c.isSetNoExpo_MoreLogLabels_X = kFALSE;
			c.isSetNoExpo_MoreLogLabels_Y = kFALSE;
		}
      c.CanvasWith2DHistogram(hm,"COLZ");
      CMS_lumi(c.c, 111, 0);
      c.PrintCanvas();
   }

   // and finally the total correlation matrix. DO NOT INCLUDE LUMI HERE
   TH2D *hm = matrix2hist(readSyst_all_cor(thevar,"../",noacc,false),varname(thevar));

		if (thevar==var::mass || thevar==var::mass3bins || thevar==var::pt || thevar == var::phistar || thevar == var::pt1560 || thevar == var::phistar1560) {
			hm->GetXaxis()->SetMoreLogLabels();
			hm->GetYaxis()->SetMoreLogLabels();
		}
		hm->GetXaxis()->SetNoExponent();
		hm->GetYaxis()->SetNoExponent();


   MyCanvas c("correlation_matrices/"+cormatname+"_total_"+TString(varname(thevar)),xaxistitle(thevar),xaxistitle(thevar));
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) {
      c.SetLogx();
      c.SetLogy();
   }
   c.SetZRange(-1,1);
   TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
		if (thevar==var::phistar || thevar==var::phistar1560) {
			c.isSetNoExpo_MoreLogLabels_X = kFALSE;
			c.isSetNoExpo_MoreLogLabels_Y = kFALSE;
		}

	//std::cout << "******* hm.GetEntries() : " << hm->GetEntries() << std::endl;
   c.CanvasWith2DHistogram(hm,"COLZ");
   CMS_lumi(c.c, 111, 0);
   c.PrintCanvas();

   // restore CMS parameters
   lumiTextSize = lumiTextSize0;
   lumiTextOffset = lumiTextSize0;
   cmsTextSize = cmsTextSize0;
   cmsTextOffset = cmsTextSize0;
}

void plotCorrelationMatrices(bool noacc) {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      plotCorrelationMatrices(noacc, thevar_i);
   }
}
