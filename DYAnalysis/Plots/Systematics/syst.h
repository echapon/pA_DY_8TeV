#ifndef syst_h
#define syst_h

#include "../Include/bin.h"
#include "TString.h"
#include "TSystemFile.h"
#include "TSystemDirectory.h"
#include "TCollection.h"
#include "TList.h"
#include <map>
#include <string>
#include <fstream>
#include <iostream>

#include "RooWorkspace.h"
#include "RooRealVar.h"

struct syst {
   string name;
   double value;
};

using namespace std;

map<bin, syst> readSyst(const char* systfile, const char* );
map<bin, syst> combineSyst(vector< map<bin, syst> > theSysts, string name="Total");
map<bin, syst> readSyst_all(var thevar, bool doPrintTex=false, const char* texName="Systematics/systs.tex");
map<bin, syst> readSyst_all_pass(var thevar, const char* token);
void printTex(vector< map<bin, syst> > theSysts, const char* texName="Systematics/systs.tex");
map<bin, vector<syst> > vm2mv(vector< map<bin,syst> > v);

#endif // ifndef syst_h
