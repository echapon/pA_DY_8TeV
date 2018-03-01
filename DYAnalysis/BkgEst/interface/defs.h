#ifndef defs_h
#define defs_h

#include <vector>
#include "analysis.h"
#include "samples_v20170519.h"
#include "samples_v20170830.h"
#include "samples_v20170830_Pyquen.h"
#include "samples_v20180111.h"
#include "samples_v20180111_Pyquen.h"
#include "samples_v20180111_CT14.h"

// UPDATED IN 2017
// 1st part, PbP
// brilcalc lumi --normtag /afs/cern.ch/user/l/lumipro/public/Normtags/normtag_HI2016.json -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/HI/Cert_285479-285832_HI8TeV_PromptReco_pPb_Collisions16_JSON_NoL1T.txt --hltpath HLT_PAL3Mu12_v1
const double lumi_part1 = 62587.059e-6; // mub-1 -> pb-1
// 2nd part, pPb
// brilcalc lumi --normtag /afs/cern.ch/user/l/lumipro/public/Normtags/normtag_HI2016.json -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/HI/Cert_285952-286496_HI8TeV_PromptReco_Pbp_Collisions16_JSON_NoL1T.txt --hltpath HLT_PAL3Mu12_v1
const double lumi_part2 = 111912.220e-6; // mub-1 -> pb-1
const double lumi_all = lumi_part1 + lumi_part2;

const int runcut = 285900;

// eta_lab <-> eta_CM
// for Pbp (1st part): eta_CM = eta_lab + 0.465
// for pPb (2nd part): eta_CM = eta_lab - 0.465
const double rapshift = 0.465;

namespace DYana {
   // put the default samples here
#ifndef CT14_GENONLY
   using namespace DYana_v20180111;
#else
   using namespace DYana_CT14;
#endif


   // kinematic bins
   // mass
   const int binnum = 13;
   double bins[14] = {15,20,30,40,50,60,76,86,96,106,120,150,200,600};
   // pt
   const int ptbinnum_endcap = 10;
   double ptbin_endcap[ptbinnum_endcap+1] = {15,20,25,30,35,40,50,70,100,200,500};
   const int ptbinnum = 13;
   double ptbin[ptbinnum+1] = {15,17,20,22,25,30,35,40,45,50,70,100,200,500};
   const int etabinnum = 2;
   double etabin[etabinnum+1] = {0,1.2,2.4};
   // pt for measurement 60-120
   const int ptbinnum_meas = 21;
   double ptbin_meas[ptbinnum_meas+1] = {0,2,4,6,8,10,12,14,16,18,20,25,30,40,45,50,60,70,85,100,125,200};
   // rapidity binning
   const int rapbinnum_1560 = 12;
   double rapbin_1560[rapbinnum_1560+1] = {-2.87,-2.4,-1.93,-1.6,-1.2,-0.8,-0.4,0,0.4,0.8,1.2,1.6,1.93};
   const int rapbinnum_60120 = 24;
   double rapbin_60120[rapbinnum_60120+1] = {-2.87,-2.6,-2.4,-2.2,-1.93,-1.8,-1.6,-1.4,-1.2,-1.,-0.8,-0.6,-0.4,-0.2,0,0.2,0.4,0.6,0.8,1.,1.2,1.4,1.6,1.8,1.93};
   // phi* binning
   const int phistarnum = 10;
   double phistarbin[phistarnum+1] = {0, 0.01, 0.03, 0.05, 0.07, 0.1, 0.15, 0.2, 0.3, 0.5, 3.};

   // object selection
   bool MuSel(PhysicsMuon *mu) {
      // cout << mu->acceptance(cuts::ptmin2,cuts::etamax) << " " << mu->tightMuonID() << " " << mu->isolation(cuts::isomax) << endl;
      return mu->acceptance(cuts::ptmin2,cuts::etamax) && mu->tightMuonID() && mu->isolation(cuts::isomax);
   };
   bool EleSel(PhysicsElectron *el) {
      return el->acceptance(20,2.4) && el->WPMedium();
   };

   // sgn switch pA vs Ap
   int runsgn(int run) { 
      // 285480-285832: PbP
      // 285956-286496: pPb
      return (run>285470 && run<runcut) ? -1 : 1;
   };

   // enum for variable type
   enum var : int {
      mass=0, 
      pt, 
      phistar, 
      rap60120, 
      rap1560,
      ALLvar
   };

   var str2var(TString variable) {
      if (variable.Contains("mass")) return mass;
      else if (variable.Contains("pt")) return pt;
      else if (variable.Contains("phistar")) return phistar;
      else if (variable.Contains("rap60120")) return rap60120;
      else if (variable.Contains("rap1560")) return rap1560;
      return mass;
   };

   const char* varname(var thevar) {
      if (thevar==var::mass) return "mass";
      else if (thevar==var::pt) return "pt";
      else if (thevar==var::phistar) return "phistar";
      else if (thevar==var::rap60120) return "rap60120";
      else if (thevar==var::rap1560) return "rap1560";
      else return "unknown";
   };

   const char* Varname(var thevar) {
      if (thevar==var::mass) return "Mass";
      else if (thevar==var::pt) return "Pt";
      else if (thevar==var::phistar) return "Phistar";
      else if (thevar==var::rap60120) return "Rap60120";
      else if (thevar==var::rap1560) return "Rap1560";
      else return "unknown";
   };

   const char* xaxistitle(var thevar) {
      if (thevar==var::mass) return "M [GeV/c^{2}]";
      else if (thevar==var::pt) return "p_{T} [GeV/c]";
      else if (thevar==var::phistar) return "#phi^{*}";
      else return "y_{CM}";
   };

   const char* xaxistitletex(var thevar) {
      if (thevar==var::mass) return "\\mmumu [\\GeVcc]";
      else if (thevar==var::pt) return "\\pt [\\GeVc]";
      else if (thevar==var::phistar) return "\\phistar";
      else return "$y_\\text{CM}$";
   };

   const char* xaxistitle(TString variable) {return xaxistitle(str2var(variable));};

   int nbinsvar(var thevar) {
      if (thevar==var::mass) return binnum;
      else if (thevar==var::pt) return ptbinnum_meas;
      else if (thevar==var::phistar) return phistarnum;
      else if (thevar==var::rap1560) return rapbinnum_1560;
      else return rapbinnum_60120;
   };

   double* binsvar(var thevar) {
      if (thevar==var::mass) return bins;
      else if (thevar==var::pt) return ptbin_meas;
      else if (thevar==var::phistar) return phistarbin;
      else if (thevar==var::rap1560) return rapbin_1560;
      else return rapbin_60120;
   };

   // Z pt rweighting
   double zptWeight(double pt) {
      double p0 = -0.37;
      double p1 = -0.3703;
      double p2 = 1.187;
      double weight = (pt>0) ? p0*pow(pt,p1)+p2 : 0.5;
      if (weight<0.5) weight = 0.5; // do not allow too large weights
      return (1./weight); // we just computed the POWHEG/data ratio: return data/Powheg
   }
};

#endif // ifndef def_h
