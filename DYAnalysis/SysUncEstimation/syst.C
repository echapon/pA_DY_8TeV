#ifndef syst_C
#define syst_C

#include "syst.h"
#include "../Include/MyCanvas.C"
#include "../BkgEst/interface/defs.h"
#include "TString.h"
#include "TGraphAsymmErrors.h"


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

TMatrixT<double> readSyst_cor(const char* systfile) {
   TString tsystfile(systfile);
   tsystfile.ReplaceAll("csv/","cor/");

   int nbins=0;
   if (tsystfile.Contains("mass3bins")) nbins = DYana::nbinsvar("mass3bins");
   else if (tsystfile.Contains("mass")) nbins = DYana::nbinsvar("mass");
   else if (tsystfile.Contains("pt")) nbins = DYana::nbinsvar("pt");
   else if (tsystfile.Contains("phistar1560")) nbins = DYana::nbinsvar("phistar1560");
   else if (tsystfile.Contains("pt1560")) nbins = DYana::nbinsvar("pt1560");
   else if (tsystfile.Contains("phistar")) nbins = DYana::nbinsvar("phistar");
   else if (tsystfile.Contains("rap1560")) nbins = DYana::nbinsvar("rap1560");
   else if (tsystfile.Contains("rap60120")) nbins = DYana::nbinsvar("rap60120");
   else return TMatrixT<double>(1,1);
   TMatrixT<double> ans = TMatrixT<double>(nbins,nbins);

   ifstream file(tsystfile.Data());
   if (!(file.good())) {
      // maybe we can guess what the correlation matrix should be... let's see.
      // by default: make it a diagonal matrix (no correlations)
      for (int i=0; i<nbins; i++) ans[i][i] = 1;

      // in some cases the syst is 100% correlated
      if (tsystfile.Contains("rewNtracks") || tsystfile.Contains("Unfold") || tsystfile.Contains("lumi")) {
         for (int i=0; i<nbins; i++) {
            for (int j=0; j<nbins; j++) {
               ans[i][j] = 1;
            }
         }
      }

      return ans;
   }

   string systname; getline(file,systname);

   string line;
   double binmin=0, binmax=0, value=0;

   int i=0;
   while (file.good()) {
      getline(file,line);
      if (line.size()==0) break;
      TString tline(line.c_str());
      TString t; Int_t from = 0, j=0;
      while (tline.Tokenize(t, from , ",")) {
         t.Strip(TString::kBoth,' ');
         value = atof(t.Data());
         ans[i][j] = value;
         j++;
      }
      i++;
   }

   file.close();
  
   return ans;
}

map<bin2, syst> readSyst_cov(const char* systfile) {
   map<bin, syst> thesyst_val = readSyst(systfile);
   TMatrixT<double> thesyst_cor = readSyst_cor(systfile);
   map<bin2, syst> ans;

   map<bin, syst>::const_iterator it1,it2;
   int i1=0;
   for (it1=thesyst_val.begin(); it1!=thesyst_val.end(); it1++) {
      int i2=0;
      for (it2=thesyst_val.begin(); it2!=thesyst_val.end(); it2++) {
         syst tmpsyst; tmpsyst.name = it1->second.name;
         tmpsyst.value = it1->second.value * it2->second.value * thesyst_cor[i1][i2];
         ans[bin2(it1->first,it2->first)] = tmpsyst;
         i2++;
      }
      i1++;
   }

   return ans;
}

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

map<bin2, syst> combineSyst_cov(vector< map<bin2, syst> > theSysts, string name) {
   if (theSysts.size()==0) return map<bin2, syst>();
   map<bin2, syst> ans;

   vector<map<bin2, syst> >::const_iterator it;
   map<bin2, syst>::const_iterator it2;
   for (it=theSysts.begin(); it!=theSysts.end(); it++) {
      for (it2=it->begin(); it2!=it->end(); it2++) {
         if (ans.find(it2->first)!=ans.end()) ans[it2->first].value += it2->second.value;
         else {
            ans[it2->first] = it2->second;
            ans[it2->first].name = name;
         }
      }
   }

   return ans;
}

TMatrixT<double> combineSyst_cor(vector< map<bin2, syst> > theSysts) {
   map<bin2, syst> cov_tot = combineSyst_cov(theSysts);
   
   int nbins = sqrt(cov_tot.size());
   TMatrixT<double> ans(nbins,nbins);

   // convert the covariance matrix to a plain TMatrix
   map<bin2, syst>::const_iterator it;
   int i=0, j=0;
   for (it=cov_tot.begin(); it!=cov_tot.end(); it++) {
      ans[i][j] = it->second.value;
      j++;
      if (j==nbins) {
         j=0;
         i++;
      }
   }

   // convert this covariance matrix into a correlation matrix
   for (i=0; i<nbins; i++) {
      for (j=0; j<nbins; j++) {
         if (i==j) continue;
         ans[i][j] *= 1./sqrt(fabs(ans[i][i]*ans[j][j]));
      }
   }
   for (i=0; i<nbins; i++) ans[i][i] = 1;

   return ans;
}

map<bin, syst> readSyst_all(var thevar, bool doPrintTex, const char* texName, TString prefix, bool noacc) {
   vector< map<bin, syst> > systmap_all;

   vector<TString> tags;
   tags.push_back("rewNtracks");
   tags.push_back("MomCorr_smooth");
   tags.push_back("tnp_tot");
   if (!noacc) tags.push_back("acceffstat_up");
   else tags.push_back("effstat_up");
   tags.push_back("bkg");
   if (!noacc) tags.push_back("AccEff_theory");
   else tags.push_back("Eff_theory");
   tags.push_back("DetResUnfold_smooth");
   tags.push_back("FSRUnfold_smooth");

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

TMatrixT<double> readSyst_all_cor(var thevar, TString prefix, bool noacc) {
   vector< map<bin2, syst> > systmap_all;

   vector<TString> tags;
   tags.push_back("rewNtracks");
   tags.push_back("MomCorr_smooth");
   tags.push_back("tnp_tot");
   if (!noacc) tags.push_back("acceffstat_up");
   else tags.push_back("effstat_up");
   tags.push_back("bkg");
   if (!noacc) tags.push_back("AccEff_theory");
   else tags.push_back("Eff_theory");
   tags.push_back("DetResUnfold_smooth");
   tags.push_back("FSRUnfold_smooth");
   tags.push_back("lumi");

   for (vector<TString>::const_iterator it=tags.begin(); it!=tags.end(); it++) {
      map<bin2,syst> systmap;
      TString systfilename = prefix + "SysUncEstimation/csv/" + TString(*it) + "_" + TString(varname(thevar)) + ".csv";
      cout << systfilename << endl;
      systmap = readSyst_cov(systfilename.Data());
      systmap_all.push_back(systmap);
   }

   TMatrixT<double> ans = combineSyst_cor(systmap_all);
   return ans;
}

map<bin2, syst>  readSyst_all_cov(var thevar, TString prefix, bool noacc) {
   vector< map<bin2, syst> > systmap_all;

   vector<TString> tags;
   tags.push_back("rewNtracks");
   tags.push_back("MomCorr_smooth");
   tags.push_back("tnp_tot");
   if (!noacc) tags.push_back("acceffstat_up");
   else tags.push_back("effstat_up");
   tags.push_back("bkg");
   if (!noacc) tags.push_back("AccEff_theory");
   else tags.push_back("Eff_theory");
   tags.push_back("DetResUnfold_smooth");
   tags.push_back("FSRUnfold_smooth");
   tags.push_back("lumi");

   for (vector<TString>::const_iterator it=tags.begin(); it!=tags.end(); it++) {
      map<bin2,syst> systmap;
      TString systfilename = prefix + "SysUncEstimation/csv/" + TString(*it) + "_" + TString(varname(thevar)) + ".csv";
      cout << systfilename << endl;
      systmap = readSyst_cov(systfilename.Data());
      systmap_all.push_back(systmap);
   }

   map<bin2, syst> ans = combineSyst_cov(systmap_all, "Total");
   return ans;
}

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
   for (unsigned int i=0; i<nsyst; i++) {
      file << " & " << theSysts[i].begin()->second.name;
   }
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
   newsystname.ReplaceAll("_mass3bins.csv",Form("%s_mass3bins.csv",systsuffix));
   newsystname.ReplaceAll("_pt.csv",Form("%s_pt.csv",systsuffix));
   newsystname.ReplaceAll("_phistar.csv",Form("%s_phistar.csv",systsuffix));
   newsystname.ReplaceAll("_pt1560.csv",Form("%s_pt1560.csv",systsuffix));
   newsystname.ReplaceAll("_phistar1560.csv",Form("%s_phistar1560.csv",systsuffix));
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

void smoothstudy(const char* systfile, int nmax, const char* cname) {
   vector< map<bin,syst> > smoothedsysts;
   for (int i=0; i<nmax; i++) smoothedsysts.push_back(smooth(readSyst(systfile),i));

   vector<TGraphAsymmErrors*> graphs;
   vector<TString> tags;
   for (unsigned int i=0; i<smoothedsysts.size(); i++) {
      map<bin, syst> thesyst = smoothedsysts[i];
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

      TGraphAsymmErrors *thegraph = new TGraphAsymmErrors(x.size(),x.data(),dy.data(),y.data(),y.data(),y.data(),y.data());
      thegraph->Sort();
      thegraph->SetFillStyle(0);
      graphs.push_back(thegraph);
      tags.push_back(Form("%d",i));
   }

   TString tcname(cname);
   if (tcname=="") tcname="tmp";
   MyCanvas c1(tcname,DYana::xaxistitle(systfile),"Rel. uncertainty (%)",800,800);
   
   TString tsystfile;
   if (tsystfile.Contains("mass") || tsystfile.Contains("pt") || tsystfile.Contains("phistar")) c1.SetLogx();
   c1.SetLegendPosition(0.25,0.60,0.65,0.90);
   c1.CanvasWithMultipleGraphs(graphs,tags, "LP");
   
   if (tcname != "tmp") {
      c1.PrintCanvas();
      c1.PrintCanvas_C();
   }
}

TMatrixT<double> map2mat(map<bin2, syst> themap) {
   int nbins = sqrt(themap.size());
   TMatrixT<double> ans(nbins,nbins);

   // convert the covariance matrix to a plain TMatrix
   map<bin2, syst>::const_iterator it;
   int i=0, j=0;
   for (it=themap.begin(); it!=themap.end(); it++) {
      ans[i][j] = it->second.value;
      j++;
      if (j==nbins) {
         j=0;
         i++;
      }
   }

   return ans;
}

TMatrixT<double> diag(const char* systfile) {
   map<bin,syst> themap = readSyst(systfile);

   int nbins = themap.size();
   TMatrixT<double> ans(nbins,nbins);

   for (int i=0; i<nbins; i++) {
      for (int j=0; j<nbins; j++) {
         ans[i][j] = 0;
      }
   }

   map<bin, syst>::const_iterator it;
   int i=0, j=0;
   for (it=themap.begin(); it!=themap.end(); it++) {
      ans[i][i] = pow(it->second.value,2);
      i++;
   }

   return ans;
}

#endif // ifndef syst_C
