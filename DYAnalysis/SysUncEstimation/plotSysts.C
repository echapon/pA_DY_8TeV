#include "syst.C"
#include "../Include/MyCanvas.C"

#include "TString.h"
#include "TGraphAsymmErrors.h"
#include <sstream>

using namespace std;
using DYana::var;

bool plotboxes = false; // if true, plot boxes instead of lines

void plotSysts(bool noacc, var thevar) {
   vector<TString> tags;
   vector<TGraphAsymmErrors*> graphs;
   vector<map<bin, syst> > systs;

   // stat
   TString systfilename = "csv/stat_" + TString(varname(thevar)) + ".csv";
   cout << systfilename.Data() << endl;
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());

   // total
   systs.push_back(readSyst_all(thevar,true,Form("tex/systs%s_%s.tex", (noacc) ? "_noacc" : "", varname(thevar)),"../",noacc));
   tags.push_back(systs.back().begin()->second.name.c_str());

   // individual systs
   systfilename = "csv/rewNtracks_" + TString(varname(thevar)) + ".csv";
   cout << systfilename.Data() << endl;
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = "csv/MomCorr_smooth_" + TString(varname(thevar)) + ".csv";
   cout << systfilename.Data() << endl;
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = "csv/tnp_tot_" + TString(varname(thevar)) + ".csv";
   cout << systfilename.Data() << endl;
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   if (!noacc) systfilename = "csv/acceffstat_up_" + TString(varname(thevar)) + ".csv";
   else systfilename = "csv/effstat_up_" + TString(varname(thevar)) + ".csv";
   cout << systfilename.Data() << endl;
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = "csv/bkg_" + TString(varname(thevar)) + ".csv";
   cout << systfilename.Data() << endl;
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   if (!noacc) systfilename = "csv/AccEff_theory_" + TString(varname(thevar)) + ".csv";
   else systfilename = "csv/Eff_theory_" + TString(varname(thevar)) + ".csv";
   cout << systfilename.Data() << endl;
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = "csv/DetResUnfold_smooth_" + TString(varname(thevar)) + ".csv";
   cout << systfilename.Data() << endl;
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = "csv/FSRUnfold_smooth_" + TString(varname(thevar)) + ".csv";
   cout << systfilename.Data() << endl;
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());

   // lumi
   systfilename = "csv/lumi_" + TString(varname(thevar)) + ".csv";
   cout << systfilename.Data() << endl;
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());

   for (unsigned int i=0; i<systs.size(); i++) {
      map<bin, syst> thesyst = systs[i];
      vector<double> x, y, dx, dy;

      double valmax=-1e99;

      for (map<bin, syst>::const_iterator it=thesyst.begin(); it!=thesyst.end(); it++) {
         bin it2 = it->first;

         double low = it2.low(), high = it2.high();
         x.push_back((low+high)/2.);
         dx.push_back((high-low)/2.);
         y.push_back(0);
         dy.push_back(fabs(it->second.value*100.));
         valmax = max(valmax,dy.back());
      }

      TGraphAsymmErrors *thegraph;
      if (plotboxes) thegraph = new TGraphAsymmErrors(x.size(),x.data(),y.data(),dx.data(),dx.data(),dy.data(),dy.data());
      else {
         thegraph = new TGraphAsymmErrors(x.size(),x.data(),dy.data(),y.data(),y.data(),y.data(),y.data());
         thegraph->SetMinimum(0);
         if (thevar==var::phistar) thegraph->SetMaximum(9.9);
         if (thevar==var::rap1560) thegraph->SetMaximum(19);
      }
      thegraph->Sort();
      thegraph->SetFillStyle(0);
      graphs.push_back(thegraph);
   }


   MyCanvas c1(Form("systematics%s_%s", (noacc) ? "_noacc" : "", varname(thevar)),xaxistitle(thevar),"Rel. uncertainty (%)",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) c1.SetLogx();
   if (thevar==var::phistar) c1.SetYRange(0,9.9);
   c1.SetLegendPosition(0.25,0.60,0.65,0.90);
   c1.CanvasWithMultipleGraphs(graphs,tags, plotboxes ? "5" : "LP");
   c1.PrintCanvas();
   // c1.PrintCanvas_C();
}

void plotSysts(bool noacc=false) {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      plotSysts(noacc, thevar_i);
   }
}
