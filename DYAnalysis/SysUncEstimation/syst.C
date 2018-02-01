#ifndef syst_C
#define syst_C

#include "syst.h"


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
   tags.push_back("MomCorr");
   tags.push_back("tnp_tot");
   tags.push_back("acceffstat_up");
   tags.push_back("bkg");

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

   ofstream file(texName);
   file << "\\begin{tabular}{|c|"; 
   for (unsigned int i=0; i<nsyst; i++) {
      if (i==nsyst-1) file << "|";
      file << "c";
      if (i==nsyst-1) file << "|";
   }
   file << "}" << endl;
   file << "\\hline" << endl;
   file << "bin";
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
      file << thebin.low() << ", " << thebin.high();
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

#endif // ifndef syst_C
