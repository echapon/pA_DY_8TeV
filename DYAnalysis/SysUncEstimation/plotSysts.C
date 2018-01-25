#include "syst.C"
#include "../Include/MyCanvas.C"

#include "TString.h"
#include "TGraphAsymmErrors.h"
#include <sstream>

using namespace std;
using DYana::var;

void plotSysts(var thevar) {
   vector<TString> tags;
   vector<TGraphAsymmErrors*> graphs;
   vector<map<bin, syst> > systs;

   // total
   systs.push_back(readSyst_all(thevar,false,"","../"));
   tags.push_back(systs.back().begin()->second.name.c_str());

   // individual systs
   TString systfilename = "Systematics/csv/rewNtracks_" + TString(varname(thevar)) + ".csv";
   cout << systfilename.Data() << endl;
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = "Systematics/csv/MomCorr_" + TString(varname(thevar)) + ".csv";
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = "Systematics/csv/tnp_up_" + TString(varname(thevar)) + ".csv";
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());
   systfilename = "Systematics/csv/acceffstat_up_" + TString(varname(thevar)) + ".csv";
   systs.push_back(readSyst(systfilename.Data()));
   tags.push_back(systs.back().begin()->second.name.c_str());

   // add stat for comparison
   systfilename = "Systematics/csv/stat_" + TString(varname(thevar)) + ".csv";
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
         dy.push_back(it->second.value);
         valmax = max(valmax,dy.back());
      }

      TGraphAsymmErrors *thegraph = new TGraphAsymmErrors(x.size(),x.data(),y.data(),dx.data(),dx.data(),dy.data(),dy.data());
      thegraph->Sort();
      thegraph->SetFillStyle(0);
      graphs.push_back(thegraph);
   }


   MyCanvas c1(Form("systematics_%s",varname(thevar)),xaxistitle(thevar),"Rel. uncertainty",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar) c1.SetLogx();
   c1.CanvasWithMultipleGraphs(graphs,tags,"5");
   c1.PrintCanvas();
   c1.PrintCanvas_C();
}
