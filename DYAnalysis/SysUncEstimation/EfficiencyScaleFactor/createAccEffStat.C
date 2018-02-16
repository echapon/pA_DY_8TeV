#include "TFile.h"
#include "TEfficiency.h"
#include "TH1.h"

#include "../Include/bin.h"
#include "../BkgEst/interface/defs.h"

#include <map>
#include <fstream>

using namespace std;

using namespace DYana;

void createAccEffStat(const char* accefffile, var thevar) {
   // open the files
   TFile *tf = TFile::Open(accefffile);
   if (!tf || !tf->IsOpen()) return;

   // get the histos
   TH1D *hden = (TH1D*) tf->Get(Form("h_%s_AccTotal",varname(thevar)));
   TH1D *hnum = (TH1D*) tf->Get(Form("h_%s_EffPass",varname(thevar)));
   if (!hden || !hnum) return;
   TEfficiency *teff = new TEfficiency(*hnum,*hden);

   // get the uncert
   map<bin,double> thesyst_l, thesyst_h;
   for (int i=1; i<=hden->GetNbinsX(); i++) {
      bin thebin;
      double x, y, dx, dy_l, dy_h;
      x = hden->GetBinCenter(i);
      dx = hden->GetBinWidth(i)/2.;
      y = teff->GetEfficiency(i);
      dy_l = teff->GetEfficiencyErrorLow(i);
      dy_h = teff->GetEfficiencyErrorUp(i);
      thebin.first = x-dx;
      thebin.second = x+dx;
      double theval = dy_l/y;
      thesyst_l[thebin] = theval;
      theval = dy_h/y;
      thesyst_h[thebin] = theval;
      // cout << theval << endl;
   }

   // print to a csv
   ofstream systfile_h(Form("csv/acceffstat_up_%s.csv",varname(thevar)));
   systfile_h << "Acc Eff (stat, up)" << endl;
   for (map<bin,double>::const_iterator it = thesyst_h.begin(); it != thesyst_h.end(); it++) {
      systfile_h << it->first.low() << ", " << it->first.high() << ", " << it->second << endl;
   }
   systfile_h.close();
   cout << "closed " << Form("csv/acceffstat_up_%s.csv",varname(thevar)) << endl;

   ofstream systfile_l(Form("csv/acceffstat_down_%s.csv",varname(thevar)));
   systfile_l << "Acc Eff (stat, down)" << endl;
   for (map<bin,double>::const_iterator it = thesyst_l.begin(); it != thesyst_l.end(); it++) {
      systfile_l << it->first.low() << ", " << it->first.high() << ", " << it->second << endl;
   }
   systfile_l.close();
   cout << "closed " << Form("csv/acceffstat_down_%s.csv",varname(thevar)) << endl;

   tf->Close();
}
