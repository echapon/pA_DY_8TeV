#include "../Include/bin.h"
#include "../BkgEst/interface/defs.h"

#include <map>
#include <fstream>

using namespace std;

using namespace DYana;

void createSyst(const char* fnom="nominal.root", const char* fsyst="syst.root", const char* systname="somesyst", var thevar=mass) {
   // open the files
   TFile *tfnom = TFile::Open(fnom);
   TFile *tfsyst = TFile::Open(fsyst);
   if (!tfnom || !tfsyst || !tfnom->IsOpen() || !tfsyst->IsOpen()) return;

   // get the histos
   TH1D* hnom = (TH1D*) tfnom->Get(Form("hy_%s",varname(thevar)));
   TH1D* hsyst = (TH1D*) tfsyst->Get(Form("hy_%s",varname(thevar)));
   if (!hnom || !hsyst) return;

   // get the relative difference
   map<bin,double> thesyst;
   for (int i=1; i<=hnom->GetNbinsX(); i++) {
      bin thebin;
      thebin.first = hnom->GetBinLowEdge(i);
      thebin.second = thebin.first + hnom->GetBinWidth(i);
      double theval = (hsyst->GetBinContent(i)-hnom->GetBinContent(i))/hnom->GetBinContent(i);
      thesyst[thebin] = theval;
      // cout << theval << endl;
   }

   // print to a csv
   ofstream systfile(Form("csv/%s_%s.csv",systname,varname(thevar)));
   systfile << systname << endl;
   for (map<bin,double>::const_iterator it = thesyst.begin(); it != thesyst.end(); it++) {
      systfile << it->first.low() << ", " << it->first.high() << ", " << it->second << endl;
   }
   systfile.close();
   cout << "closed " << Form("csv/%s_%s.csv",systname,varname(thevar)) << endl;

   tfnom->Close();
   tfsyst->Close();
}

void createStat(const char* fnom, var thevar=mass) {
   // open the files
   TFile *tfnom = TFile::Open(fnom);
   if (!tfnom) return;

   // get the histos
   TH1D* hnom = (TH1D*) tfnom->Get(Form("hy_statonly_%s",varname(thevar)));
   if (!hnom) return;

   // get the uncert
   map<bin,double> thesyst;
   for (int i=1; i<=hnom->GetNbinsX(); i++) {
      bin thebin;
      thebin.first = hnom->GetBinLowEdge(i);
      thebin.second = thebin.first + hnom->GetBinWidth(i);
      double theval = hnom->GetBinError(i)/hnom->GetBinContent(i);
      thesyst[thebin] = theval;
      // cout << theval << endl;
   }

   // print to a csv
   ofstream systfile(Form("csv/stat_%s.csv",varname(thevar)));
   systfile << "stat." << endl;
   for (map<bin,double>::const_iterator it = thesyst.begin(); it != thesyst.end(); it++) {
      systfile << it->first.low() << ", " << it->first.high() << ", " << it->second << endl;
   }
   systfile.close();
   cout << "closed " << Form("csv/stat_%s.csv",varname(thevar)) << endl;

   tfnom->Close();
}

// create all systs
void createSystAll(var thevar=mass) {
   createSyst("../Plots/results/xsec_nom.root","../Plots/results/xsec_rewNtracks.root","rewNtracks",thevar);
   createSyst("../Plots/results/xsec_nom.root","../Plots/results/xsec_MomCorr.root","MomCorr",thevar);
   createSyst("../Plots/results/xsec_nom.root","../Plots/results/xsec_notnprew.root","TnpRew",thevar);

   createStat("../Plots/results/xsec_nom.root",thevar);
}
