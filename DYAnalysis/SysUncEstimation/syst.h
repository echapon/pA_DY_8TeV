#ifndef syst_h
#define syst_h

#include "../Include/bin.h"
#include "../BkgEst/interface/defs.h"
#include "TString.h"
#include "TSystemFile.h"
#include "TSystemDirectory.h"
#include "TCollection.h"
#include "TList.h"
#include <map>
#include <string>
#include <fstream>
#include <iostream>

struct syst {
   string name;
   double value;
};

using namespace std;
using DYana::var;

map<bin, syst> readSyst(const char* systfile, const char* );
map<bin, syst> combineSyst(vector< map<bin, syst> > theSysts, string name="Total");
map<bin, syst> readSyst_all(var thevar, bool doPrintTex=false, const char* texName="SysUncEstimation/systs.tex", TString prefix="SysUncEstimation/");
void printTex(vector< map<bin, syst> > theSysts, const char* texName="SysUncEstimation/systs.tex");
map<bin, vector<syst> > vm2mv(vector< map<bin,syst> > v);

#endif // ifndef syst_h
