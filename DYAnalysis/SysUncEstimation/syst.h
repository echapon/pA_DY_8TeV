#ifndef syst_h
#define syst_h

#include "../Include/bin.h"
#include "../BkgEst/interface/defs.h"
#include "TString.h"
#include "TSystemFile.h"
#include "TSystemDirectory.h"
#include "TCollection.h"
#include "TList.h"
#include "TMatrixT.h"
#include <map>
#include <string>
#include <fstream>
#include <iostream>

struct syst {
   string name;
   double value;
};

typedef pair<bin,bin> bin2;

using namespace std;
using DYana::var;

map<bin, syst>   readSyst(const char* systfile);
TMatrixT<double> readSyst_cor(const char* systfile);
map<bin2, syst>  readSyst_cov(const char* systfile);
map<bin, syst>   combineSyst(vector< map<bin, syst> > theSysts, string name="Total");
TMatrixT<double> combineSyst_cor(vector< map<bin2, syst> > theSysts);
map<bin2, syst>  combineSyst_cov(vector< map<bin2, syst> > theSysts, string name="Total");
map<bin, syst>   readSyst_all(var thevar, bool doPrintTex=false, const char* texName="SysUncEstimation/tex/systs.tex", TString prefix="./", bool noacc=false);
TMatrixT<double> readSyst_all_cor(var thevar, TString prefix="./", bool noacc=false, bool withlumi=true);
map<bin2, syst>  readSyst_all_cov(var thevar, TString prefix="./", bool noacc=false, bool withlumi=true);
void             printTex(vector< map<bin, syst> > theSysts, const char* texName="SysUncEstimation/tex/systs.tex");
map<bin, vector<syst> >  vm2mv(vector< map<bin,syst> > v);
map<bin, syst>   smooth(map<bin, syst> thesyst, int ntimes=10);
void             smooth(const char* systfile="csv/somesyst.csv", const char* systsuffix="_smooth", int ntimes=10);
void             smoothstudy(const char* systfile, int nmax=10, const char* cname="");
TMatrixT<double> map2mat(map<bin2, syst> themap);
TMatrixT<double> diag(const char* systfile);

#endif // ifndef syst_h
