#include "syst.C"
#include "../Include/MyCanvas.C"

#include "TString.h"
#include "TGraphAsymmErrors.h"
#include <sstream>

using namespace std;
using DYana::var;

bool plotboxes = false; // if true, plot boxes instead of lines

void plotSysts_tnp(var thevar, bool plotabs=false) {
   vector<TString> tags;
   vector<TGraphAsymmErrors*> graphs;
   vector<map<bin, syst> > systs;

   TString csvdir = plotabs ? "csv_abs" : "csv";

   // individual systs
   TString systfilename = csvdir+"/tnp_tot_" + TString(varname(thevar)) + ".csv";
   cout << systfilename.Data() << endl;
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = csvdir+"/tnp_trigstat_" + TString(varname(thevar)) + ".csv";
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = csvdir+"/tnp_muidstat_" + TString(varname(thevar)) + ".csv";
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = csvdir+"/tnp_isostat_" + TString(varname(thevar)) + ".csv";
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = csvdir+"/tnp_trigsyst_" + TString(varname(thevar)) + ".csv";
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = csvdir+"/tnp_muidsyst_" + TString(varname(thevar)) + ".csv";
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = csvdir+"/tnp_isosyst_" + TString(varname(thevar)) + ".csv";
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = csvdir+"/tnp_muidbinned_" + TString(varname(thevar)) + ".csv";
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = csvdir+"/tnp_isobinned_" + TString(varname(thevar)) + ".csv";
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = csvdir+"/tnp_PU_" + TString(varname(thevar)) + ".csv";
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = csvdir+"/tnp_STA_" + TString(varname(thevar)) + ".csv";
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
         dy.push_back(plotabs ? it->second.value : it->second.value*100.);
         valmax = max(valmax,dy.back());
      }

      TGraphAsymmErrors *thegraph;
      if (plotboxes) thegraph = new TGraphAsymmErrors(x.size(),x.data(),y.data(),dx.data(),dx.data(),dy.data(),dy.data());
      else {
         thegraph = new TGraphAsymmErrors(x.size(),x.data(),dy.data(),y.data(),y.data(),y.data(),y.data());
         thegraph->SetMinimum(0);
      }
      thegraph->Sort();
      thegraph->SetFillStyle(0);
      graphs.push_back(thegraph);
   }


   MyCanvas c1(Form("systematics_tnp_%s_%s",plotabs ? "abs" : "rel",varname(thevar)),xaxistitle(thevar),plotabs ? "Abs. uncertainty" : "Rel. uncertainty (%)",800,800);
   c1.SetLegendPosition(0.65,0.69,0.95,0.93);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) c1.SetLogx();
   c1.CanvasWithMultipleGraphs(graphs,tags, plotboxes ? "5" : "LP");
   c1.PrintCanvas();
   c1.PrintCanvas_C();
}

void plotSysts_tnp(bool plotabs=false) {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      plotSysts_tnp(thevar_i, plotabs);
   }
}
