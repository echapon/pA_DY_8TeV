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
#include "samples_v20180111_noEWK.h"
#include "samples_v20180111_negweights.h"
#include "samples_v20180814.h"
#include "samples_v20180814_Pyquen.h"
#include "samples_v20180814_CT14.h"

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
   using namespace DYana_v20180814;
   // using namespace DYana_v20170830;
#else
   using namespace DYana_v20180814_CT14;
#endif


   // kinematic bins
   // mass (compatible with SMP-14-003)
   const int binnum = 13;
   double bins[14] = {15,20,30,40,50,60,76,86,96,106,120,150,200,600};
   const int binnum3 = 3;
   double bins3[14] = {15,60,120,600};
   // pt, for FR
   const int ptbinnum_endcap = 9;
   double ptbin_endcap[ptbinnum_endcap+1] = {10,15,20,25,30,35,40,50,70,200};
   const int ptbinnum = 12;
   double ptbin[ptbinnum+1] = {10,15,17,20,22,25,30,35,40,45,50,70,200};
   const int etabinnum = 2;
   double etabin[etabinnum+1] = {0,1.2,2.4};
   
   // pt for measurement 60-120 (almost same as SMP-14-012)
   const int ptbinnum_meas = 17;
   double ptbin_meas[ptbinnum_meas+1] = {0,2.5,5,7.5,10,12.5,15,17.5,20,30,40,50,70,90,110,150,200,300};//190,250,600};
   const int ptbinnum_meas_1560 = 8;
   double ptbin_meas_1560[ptbinnum_meas_1560+1] = {0,5,10,15,20,40,70,110,200};//190,600};
   
   // rapidity binning (compatible with, but finer than, HIN-15-002)
   const int rapbinnum_1560 = 12;
   double rapbin_1560[rapbinnum_1560+1] = {-2.87,-2.4,-1.93,-1.6,-1.2,-0.8,-0.4,0,0.4,0.8,1.2,1.6,1.93};
   const int rapbinnum_60120 = 24;
   double rapbin_60120[rapbinnum_60120+1] = {-2.87,-2.6,-2.4,-2.2,-1.93,-1.8,-1.6,-1.4,-1.2,-1.,-0.8,-0.6,-0.4,-0.2,0,0.2,0.4,0.6,0.8,1.,1.2,1.4,1.6,1.8,1.93};

   // phi* binning (rebinned from with SMP-17-002)
   const int phistarnum = 17;
   double phistarbin[phistarnum+1] = {0, 0.008, 0.016, 0.024, 0.034, 0.045, 0.057, 0.072, 0.091, 0.114, 0.145, 0.189, 0.258, 0.391, 0.695, 1.153, 1.947, 3.277};
   const int phistarnum_1560 = 9;
   double phistarbin_1560[phistarnum_1560+1] = {0, 0.008, 0.024, 0.045, 0.072, 0.114, 0.189, 0.391, 1.153, 3.277};

   // object selection
   bool MuSel(PhysicsMuon *mu) {
      // cout << mu->pt << " " << mu->eta << " " << mu->acceptance(cuts::ptmin2,cuts::etamax) << " " << mu->tightMuonID() << " " << mu->trkisolation(cuts::isomax) << endl;
      // return mu->acceptance(cuts::ptmin2,cuts::etamax) && mu->tightMuonID() && mu->isolation(cuts::isomax);
      return mu->acceptance(cuts::ptmin2,cuts::etamax) && mu->tightMuonID() && mu->trkisolation(cuts::isomax);
   };
   bool MuSel_noiso(PhysicsMuon *mu) {
      return mu->acceptance(cuts::ptmin2,cuts::etamax) && mu->tightMuonID();
      // return mu->acceptance(cuts::ptmin2,cuts::etamax) && mu->looseMuonID();
   };
   bool EleSel(PhysicsElectron *el) {
      return el->acceptance(cuts::ptmin2,cuts::etamax) && el->WPMedium() && el->trkisolation(cuts::isomax);
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
      mass3bins, 
      pt, 
      phistar, 
      rap60120, 
      rap1560,
      pt1560,
      phistar1560,
      ALLvar
   };

   var str2var(TString variable) {
      if (variable.Contains("mass3bins")) return mass3bins;
      else if (variable.Contains("mass")) return mass;
      else if (variable.Contains("pt1560")) return pt1560;
      else if (variable.Contains("phistar1560")) return phistar1560;
      else if (variable.Contains("pt")) return pt;
      else if (variable.Contains("phistar")) return phistar;
      else if (variable.Contains("rap60120")) return rap60120;
      else if (variable.Contains("rap1560")) return rap1560;
      else if (variable.Contains("Mass3bins")) return mass3bins;
      else if (variable.Contains("Mass")) return mass;
      else if (variable.Contains("Rap1560")) return rap1560;
      else if (variable.Contains("Phistar1560")) return phistar1560;
      else if (variable.Contains("Pt1560")) return pt1560;
      else if (variable.Contains("Pt")) return pt;
      else if (variable.Contains("Phistar")) return phistar;
      else if (variable.Contains("Rap60120")) return rap60120;
      return mass;
   };

   const char* varname(var thevar) {
      if (thevar==var::mass) return "mass";
      else if (thevar==var::mass3bins) return "mass3bins";
      else if (thevar==var::pt) return "pt";
      else if (thevar==var::phistar) return "phistar";
      else if (thevar==var::rap60120) return "rap60120";
      else if (thevar==var::rap1560) return "rap1560";
      else if (thevar==var::pt1560) return "pt1560";
      else if (thevar==var::phistar1560) return "phistar1560";
      else return "unknown";
   };

   const char* Varname(var thevar) {
      if (thevar==var::mass) return "Mass";
      else if (thevar==var::mass3bins) return "Mass3bins";
      else if (thevar==var::pt) return "Pt";
      else if (thevar==var::phistar) return "Phistar";
      else if (thevar==var::rap60120) return "Rap60120";
      else if (thevar==var::rap1560) return "Rap1560";
      else if (thevar==var::pt1560) return "Pt1560";
      else if (thevar==var::phistar1560) return "Phistar1560";
      else return "unknown";
   };

   const char* xaxistitle(var thevar) {
      if (thevar==var::mass || thevar==var::mass3bins) return "M [GeV/c^{2}]";
      else if (thevar==var::pt || thevar==var::pt1560) return "p_{T} [GeV/c]";
      else if (thevar==var::phistar || thevar==var::phistar1560) return "#phi^{*}";
      else return "y_{CM}";
   };

   const char* xaxistitletex(var thevar) {
      if (thevar==var::mass || thevar==var::mass3bins) return "\\mmumu [\\GeVcc]";
      else if (thevar==var::pt || thevar==var::pt1560) return "\\pt [\\GeVc]";
      else if (thevar==var::phistar || thevar==var::phistar1560) return "\\phistar";
      else return "$y_\\text{CM}$";
   };

   const char* xaxistitle(TString variable) {return xaxistitle(str2var(variable));};

   int nbinsvar(var thevar) {
      if (thevar==var::mass) return binnum;
      else if (thevar==var::mass3bins) return binnum3;
      else if (thevar==var::pt) return ptbinnum_meas;
      else if (thevar==var::phistar) return phistarnum;
      else if (thevar==var::rap1560) return rapbinnum_1560;
      else if (thevar==var::pt1560) return ptbinnum_meas_1560;
      else if (thevar==var::phistar1560) return phistarnum_1560;
      else return rapbinnum_60120;
   };

   double* binsvar(var thevar) {
      if (thevar==var::mass) return bins;
      else if (thevar==var::mass3bins) return bins3;
      else if (thevar==var::pt) return ptbin_meas;
      else if (thevar==var::phistar) return phistarbin;
      else if (thevar==var::rap1560) return rapbin_1560;
      else if (thevar==var::pt1560) return ptbin_meas_1560;
      else if (thevar==var::phistar1560) return phistarbin_1560;
      else return rapbin_60120;
   };

   int nbinsvar(TString variable) {return nbinsvar(str2var(variable));}
   double* binsvar(TString variable) {return binsvar(str2var(variable));}

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
