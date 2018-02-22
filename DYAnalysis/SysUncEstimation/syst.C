#ifndef syst_C
#define syst_C

#include "syst.h"
#include "TString.h"


map<bin, syst> readSyst(const char* systfile) {
   map<bin, syst> ans;

   ifstream file(systfile);
   if (!(file.good())) return ans;

   string systname; getline(file,systname);

   string line;
   double binmin=0, binmax=0, value=0;

   while (file.good()) {
      getline(file,line);
      if (line.size()==0) break;
      TString tline(line.c_str());
      TString t; Int_t from = 0, cnt=0;
      while (tline.Tokenize(t, from , ",")) {
         t.Strip(TString::kBoth,' ');
         value = atof(t.Data());
         if (cnt==0) binmin = atof(t.Data());
         else if (cnt==1) binmax = value;
         else if (cnt>3) {
            cout << "Warning, too many fields, I'll take the last one." << endl;
            continue;
         }
         cnt++;
      }
      bin thebin(binmin,binmax);
      syst thesyst; thesyst.value = value; thesyst.name = systname;

      ans[thebin] = thesyst;
   }

   file.close();
  
   return ans;
};

map<bin, syst> combineSyst(vector< map<bin, syst> > theSysts, string name) {
   map<bin, syst> ans;

   vector< map<bin, syst> >::const_iterator it;
   for (it=theSysts.begin(); it!=theSysts.end(); it++) {
      map<bin, syst>::const_iterator it2;
      for (it2=it->begin(); it2!=it->end(); it2++) {
         bin thebin = it2->first;
         syst thesyst = it2->second;
         thesyst.name = name;

         // if we already have a syst for this bin, sum quadractically the existing syst and the new syst
         if (ans.find(thebin) != ans.end())
         {
           thesyst.value = sqrt(pow(thesyst.value,2) + pow(ans[thebin].value,2));
         }
         thesyst.value = fabs(thesyst.value);
         ans[thebin] = thesyst;
      }
   }

   return ans;
};

map<bin, syst> readSyst_all(var thevar, bool doPrintTex, const char* texName, TString prefix) {
   vector< map<bin, syst> > systmap_all;

   vector<TString> tags;
   tags.push_back("rewNtracks");
   tags.push_back("MomCorr_smooth");
   tags.push_back("tnp_tot");
   tags.push_back("acceffstat_up");
   tags.push_back("bkg_smooth");
   tags.push_back("AccEff_theory");
   tags.push_back("DetResUnfold_smooth");

   for (vector<TString>::const_iterator it=tags.begin(); it!=tags.end(); it++) {
      map<bin,syst> systmap;
      TString systfilename = prefix + "SysUncEstimation/csv/" + TString(*it) + "_" + TString(varname(thevar)) + ".csv";
      cout << systfilename << endl;
      systmap = readSyst(systfilename.Data());
      systmap_all.push_back(systmap);
   }

   map<bin,syst> ans = combineSyst(systmap_all, "Total");
   systmap_all.push_back(ans);

   if (doPrintTex) printTex(systmap_all, texName);

   return ans;
};

void printTex(vector< map<bin, syst> > theSysts, const char* texName) {
   unsigned int nsyst = theSysts.size();

   // find out the variable from the texName
   TString ts(texName);
   TString tvarname("bin");
   if (ts.Contains("mass")) tvarname = "\\mmumu [\\GeVcc]";
   else if (ts.Contains("pt")) tvarname = "\\pt [\\GeVc]";
   else if (ts.Contains("phistar")) tvarname = "\\phistar";
   else if (ts.Contains("rap")) tvarname = "$y_\\text{CM}$";

   ofstream file(texName);
   file << "\\begin{tabular}{|c|"; 
   for (unsigned int i=0; i<nsyst; i++) {
      if (i==nsyst-1) file << "|";
      file << "c";
      if (i==nsyst-1) file << "|";
   }
   file << "}" << endl;
   file << "\\hline" << endl;
   file << tvarname.Data();
   for (unsigned int i=0; i<nsyst; i++) file << " & " << theSysts[i].begin()->second.name;
   file<< "\\\\" << endl;
   file << "\\hline" << endl;

   map<bin, vector<syst> > themap = vm2mv(theSysts);
   map<bin, vector<syst> >::const_iterator itm;
   for (itm=themap.begin(); itm!=themap.end(); itm++) {
      vector<syst> v = itm->second;
      if (v.size() != nsyst) {
         cout << "Error, not all systematics have the same size. Aborting." << endl;
         file.close();
         return;
      }
      bin thebin = itm->first;
      if (itm != themap.begin()) file << "\\hline" << endl;
      file.unsetf(ios::fixed);
      file.precision(5);
      file << "$[" << thebin.low() << "-" << thebin.high() << "]$";
      file.precision(1);
      file.setf(ios::fixed);

      for (unsigned int i=0; i<nsyst; i++) {
         file << " & " << 100.*v[i].value;
      }
      file << " \\\\" << endl;
   }

   file << "\\hline" << endl;
   file << "\\end{tabular}" << endl;
   file.close();
   cout << "Closed " << texName << endl;
}

map<bin, vector<syst> > vm2mv(vector< map<bin,syst> > v) {
   map<bin, vector<syst> > ans;

   vector<map<bin,syst> >::const_iterator itv;
   map<bin,syst>::const_iterator itm;
   for (itv=v.begin(); itv!=v.end(); itv++) {
      for (itm=itv->begin(); itm!=itv->end(); itm++) {
         bin thebin = itm->first;
         if (ans.find(thebin)==ans.end()) ans[thebin] = vector<syst>();
         ans[thebin].push_back(itm->second);
      }
   }

   return ans;
}

map<bin, syst> smooth(map<bin, syst> thesyst, int ntimes) {
   vector<syst> values;
   vector<bin> bins;
   map<bin, syst>::const_iterator it;
   for (it=thesyst.begin(); it!=thesyst.end(); it++) {
      values.push_back(it->second);
      bins.push_back(it->first);
   }
   
   int nbins = values.size();
   TH1F h1("htempsyst","",nbins,0,nbins);
   
   for (int i=0; i<nbins; i++) {
      h1.SetBinContent(i+1,values[i].value);
   }

   // smooth here
   h1.Smooth(ntimes);

   // export to a new syst
   map<bin, syst> ans;
   for (int i=0; i<nbins; i++) {
      syst thesyst; 
      thesyst.name = values[0].name; 
      thesyst.value = h1.GetBinContent(i+1);
      ans[bins[i]] = thesyst;
   }

   return ans;
}

void smooth(const char* systfile, const char* systsuffix, int ntimes) {
   map<bin,syst> smoothedsyst = smooth(readSyst(systfile),ntimes);

   TString newsystname(systfile);
   newsystname.ReplaceAll("_mass.csv",Form("%s_mass.csv",systsuffix));
   newsystname.ReplaceAll("_pt.csv",Form("%s_pt.csv",systsuffix));
   newsystname.ReplaceAll("_phistar.csv",Form("%s_phistar.csv",systsuffix));
   newsystname.ReplaceAll("_rap1560.csv",Form("%s_rap1560.csv",systsuffix));
   newsystname.ReplaceAll("_rap60120.csv",Form("%s_rap60120.csv",systsuffix));
   
   ofstream of(newsystname.Data());
   of << smoothedsyst.begin()->second.name << " (smoothed)" << endl;
   map<bin, syst>::const_iterator it;
   for (it=smoothedsyst.begin(); it!=smoothedsyst.end(); it++) {
      of << it->first.low() << ", " << it->first.high() << ", " << it->second.value << endl;
   }
   of.close();
}

#endif // ifndef syst_C
