#include "TFile.h"
#include "TGraphAsymmErrors.h"

#include "../Include/bin.h"
#include "../BkgEst/interface/defs.h"

#include <map>
#include <fstream>

using namespace std;

using namespace DYana;

void createTnpSyst(const char* accefffile, var thevar) {
   // open the files
   TFile *tf = TFile::Open(accefffile);
   if (!tf || !tf->IsOpen()) return;

   // get the graph
   TGraphAsymmErrors *g = (TGraphAsymmErrors*) tf->Get(Form("g_Eff_%s_Corr_tnp_syst",Varname(thevar)));
   if (!g) return;

   // get the uncert
   map<bin,double> thesyst_l, thesyst_h;
   for (int i=0; i<g->GetN(); i++) {
      bin thebin;
      double x, y, dx, dy_l, dy_h;
      x = g->GetX()[i];
      dx = g->GetEXlow()[i];
      y = g->GetY()[i];
      dy_l = g->GetEYlow()[i];
      dy_h = g->GetEYhigh()[i];
      thebin.first = x-dx;
      thebin.second = x+dx;
      double theval = dy_l/y;
      thesyst_l[thebin] = theval;
      theval = dy_h/y;
      thesyst_h[thebin] = theval;
      // cout << theval << endl;
   }

   // print to a csv
   ofstream systfile_h(Form("csv/tnp_up_%s.csv",varname(thevar)));
   systfile_h << "TnP (up)" << endl;
   for (map<bin,double>::const_iterator it = thesyst_h.begin(); it != thesyst_h.end(); it++) {
      systfile_h << it->first.low() << ", " << it->first.high() << ", " << it->second << endl;
   }
   systfile_h.close();
   cout << "closed " << Form("csv/tnp_up_%s.csv",varname(thevar)) << endl;

   ofstream systfile_l(Form("csv/tnp_down_%s.csv",varname(thevar)));
   systfile_l << "TnP (down)" << endl;
   for (map<bin,double>::const_iterator it = thesyst_l.begin(); it != thesyst_l.end(); it++) {
      systfile_l << it->first.low() << ", " << it->first.high() << ", " << it->second << endl;
   }
   systfile_l.close();
   cout << "closed " << Form("csv/tnp_down_%s.csv",varname(thevar)) << endl;

   tf->Close();
}
