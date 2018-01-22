#include "../Include/bin.h"
#include "../BkgEst/interface/defs.h"

#include <map>
#include <fstream>

using namespace std;

using namespace DYana;

void createSyst(const char* fnom="nominal.root", const char* fsyst="syst.root", const char* systname="somesyst", var thevar=mass) {
   // open the files
   TFile *tfnom = TFile::Open(fnom);
   TFile *tfsyst = TFile::Open(fnom);
   if (!tfnom || !tfsyst) return;

   // get the histos
   TH1D* hnom = (TH1D*) tfnom->Get(Form("hy_%s",varname(thevar)));
   TH1D* hsyst = (TH1D*) tfsyst->Get(Form("hy_%s",varname(thevar)));
   if (!hnom || !hsyst) return;

   // get the relative difference
   map<bin,double> thesyst;
   for (int i=1; i<hnom->GetNbinsX(); i++) {
      bin thebin;
      thebin.first = hnom->GetBinLowEdge(i);
      thebin.second = thebin.first + hnom->GetBinWidth(i);
      double theval = hsyst->GetBinContent(i)-hnom->GetBinContent(i);
      thesyst[thebin] = theval;
   }

   // print to a csv
   ofstream systfile(Form("%s_%s.csv",systname,varname(thevar)));
   systfile << systname << endl;
   for (vector<bin,double>::const_iterator it = thesys.begin(), it != thesyst.end(), it++) {
      systfile << it->first.low() << ", " << it->first.high() << ", " << it->second << endl;
   }
   systfile.close();
}
