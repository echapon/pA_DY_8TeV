#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "TFile.h"
#include "TGraphAsymmErrors.h"

#include "../Include/bin.h"
#include "../../BkgEst/interface/defs.h"

using namespace std;
using namespace DYana;

void parse_dat(const char* fname_in, const char* accefffile) {
   // open the files
   TFile *tf = TFile::Open(accefffile);
   if (!tf || !tf->IsOpen()) return;

   // open input .dat
   ifstream fin(fname_in);

   // the first line is a header
   string theline;
   fin.ignore(1e6,'\n');

   vector< map<bin,double> > thesysts; 
   vector<string> systtags;
   vector<string> systlabels;
   // tnp_tot,tnp_trigstat,tnp_muidstat,tnp_isostat,tnp_trigsyst,tnp_muidsyst,tnp_isosyst,tnp_muidbinned,tnp_isobinned,tnp_PU,tnp_STA;
   // create the syst maps
   systtags.push_back("tot");
   systlabels.push_back("TnP (total)");
   thesysts.push_back(map<bin,double>());
   systtags.push_back("trigstat");
   systlabels.push_back("TnP (trig stat)");
   thesysts.push_back(map<bin,double>());
   systtags.push_back("muidstat");
   systlabels.push_back("TnP (muid stat)");
   thesysts.push_back(map<bin,double>());
   systtags.push_back("isostat");
   systlabels.push_back("TnP (iso stat)");
   thesysts.push_back(map<bin,double>());
   systtags.push_back("trigsyst");
   systlabels.push_back("TnP (trig syst)");
   thesysts.push_back(map<bin,double>());
   systtags.push_back("muidsyst");
   systlabels.push_back("TnP (muid syst)");
   thesysts.push_back(map<bin,double>());
   systtags.push_back("isosyst");
   systlabels.push_back("TnP (iso syst)");
   thesysts.push_back(map<bin,double>());
   systtags.push_back("muidbinned");
   systlabels.push_back("TnP (muid binned)");
   thesysts.push_back(map<bin,double>());
   systtags.push_back("isobinned");
   systlabels.push_back("TnP (iso binned)");
   thesysts.push_back(map<bin,double>());
   systtags.push_back("PU");
   systlabels.push_back("TnP (PU)");
   thesysts.push_back(map<bin,double>());
   systtags.push_back("STA");
   systlabels.push_back("TnP (STA)");
   thesysts.push_back(map<bin,double>());


   ///////////////////////////////////////////////////////////////////////////////////

   // mass
   var thevar = var::mass;
   // get the graph
   TGraphAsymmErrors *g = (TGraphAsymmErrors*) tf->Get(Form("g_Eff_%s_Corr_tnp_syst",Varname(thevar)));
   if (!g) return;

   for (int i=0; i<binnum; i++) {
      bin thebin(bins[i],bins[i+1]);
      for (int j=0; j<thesysts.size(); j++) {
         fin >> thesysts[j][thebin];
         thesysts[j][thebin] = thesysts[j][thebin]/g->GetY()[i];
      }
   }

   // now print all that to csv
   for (int j=0; j<thesysts.size(); j++) {
      string fname = Form("csv/tnp_%s_%s.csv",systtags[j].c_str(),varname(thevar));
      ofstream systfile(fname);
      systfile << systlabels[j] << endl;
      for (map<bin,double>::const_iterator it = thesysts[j].begin(); it != thesysts[j].end(); it++) {
         systfile << it->first.low() << ", " << it->first.high() << ", " << it->second << endl;
      }
      systfile.close();
      cout << "closed " << fname << endl;
   }

   // skip a line
   fin.ignore(1e6,'\n');

   // reset maps
   for (int j=0; j<thesysts.size(); j++) thesysts[j].clear();

   ///////////////////////////////////////////////////////////////////////////////////

   // pt
   thevar = var::pt;
   // get the graph
   g = (TGraphAsymmErrors*) tf->Get(Form("g_Eff_%s_Corr_tnp_syst",Varname(thevar)));
   if (!g) return;

   for (int i=0; i<ptbinnum_meas; i++) {
      bin thebin(ptbin_meas[i],ptbin_meas[i+1]);
      for (int j=0; j<thesysts.size(); j++) {
         fin >> thesysts[j][thebin];
         thesysts[j][thebin] = thesysts[j][thebin]/g->GetY()[i];
      }
   }

   // now print all that to csv
   for (int j=0; j<thesysts.size(); j++) {
      string fname = Form("csv/tnp_%s_%s.csv",systtags[j].c_str(),varname(thevar));
      ofstream systfile(fname);
      systfile << systlabels[j] << endl;
      for (map<bin,double>::const_iterator it = thesysts[j].begin(); it != thesysts[j].end(); it++) {
         systfile << it->first.low() << ", " << it->first.high() << ", " << it->second << endl;
      }
      systfile.close();
      cout << "closed " << fname << endl;
   }

   // skip a line
   fin.ignore(1e6,'\n');

   // reset maps
   for (int j=0; j<thesysts.size(); j++) thesysts[j].clear();

   ///////////////////////////////////////////////////////////////////////////////////

   // phistar
   thevar = var::phistar;
   // get the graph
   g = (TGraphAsymmErrors*) tf->Get(Form("g_Eff_%s_Corr_tnp_syst",Varname(thevar)));
   if (!g) return;

   for (int i=0; i<phistarnum; i++) {
      bin thebin(phistarbin[i],phistarbin[i+1]);
      for (int j=0; j<thesysts.size(); j++) {
         fin >> thesysts[j][thebin];
         thesysts[j][thebin] = thesysts[j][thebin]/g->GetY()[i];
      }
   }

   // now print all that to csv
   for (int j=0; j<thesysts.size(); j++) {
      string fname = Form("csv/tnp_%s_%s.csv",systtags[j].c_str(),varname(thevar));
      ofstream systfile(fname);
      systfile << systlabels[j] << endl;
      for (map<bin,double>::const_iterator it = thesysts[j].begin(); it != thesysts[j].end(); it++) {
         systfile << it->first.low() << ", " << it->first.high() << ", " << it->second << endl;
      }
      systfile.close();
      cout << "closed " << fname << endl;
   }

   // skip a line
   fin.ignore(1e6,'\n');

   // reset maps
   for (int j=0; j<thesysts.size(); j++) thesysts[j].clear();

   ///////////////////////////////////////////////////////////////////////////////////

   // rap 15-60
   thevar = var::rap1560;
   // get the graph
   g = (TGraphAsymmErrors*) tf->Get(Form("g_Eff_%s_Corr_tnp_syst",Varname(thevar)));
   if (!g) return;

   for (int i=0; i<rapbinnum_1560; i++) {
      bin thebin(rapbin_1560[i],rapbin_1560[i+1]);
      for (int j=0; j<thesysts.size(); j++) {
         fin >> thesysts[j][thebin];
         thesysts[j][thebin] = thesysts[j][thebin]/g->GetY()[i];
      }
   }

   // now print all that to csv
   for (int j=0; j<thesysts.size(); j++) {
      string fname = Form("csv/tnp_%s_%s.csv",systtags[j].c_str(),varname(thevar));
      ofstream systfile(fname);
      systfile << systlabels[j] << endl;
      for (map<bin,double>::const_iterator it = thesysts[j].begin(); it != thesysts[j].end(); it++) {
         systfile << it->first.low() << ", " << it->first.high() << ", " << it->second << endl;
      }
      systfile.close();
      cout << "closed " << fname << endl;
   }

   // skip a line
   fin.ignore(1e6,'\n');

   // reset maps
   for (int j=0; j<thesysts.size(); j++) thesysts[j].clear();

   ///////////////////////////////////////////////////////////////////////////////////

   // rap 60120
   thevar = var::rap60120;
   // get the graph
   g = (TGraphAsymmErrors*) tf->Get(Form("g_Eff_%s_Corr_tnp_syst",Varname(thevar)));
   if (!g) return;

   for (int i=0; i<rapbinnum_60120; i++) {
      bin thebin(rapbin_60120[i],rapbin_60120[i+1]);
      for (int j=0; j<thesysts.size(); j++) {
         fin >> thesysts[j][thebin];
         thesysts[j][thebin] = thesysts[j][thebin]/g->GetY()[i];
      }
   }

   // now print all that to csv
   for (int j=0; j<thesysts.size(); j++) {
      string fname = Form("csv/tnp_%s_%s.csv",systtags[j].c_str(),varname(thevar));
      ofstream systfile(fname);
      systfile << systlabels[j] << endl;
      for (map<bin,double>::const_iterator it = thesysts[j].begin(); it != thesysts[j].end(); it++) {
         systfile << it->first.low() << ", " << it->first.high() << ", " << it->second << endl;
      }
      systfile.close();
      cout << "closed " << fname << endl;
   }

}
