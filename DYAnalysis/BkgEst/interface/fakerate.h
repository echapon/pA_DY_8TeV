#ifndef fakerate_h
#define fakerate_h

#include "PhysicsMuon.h"
#include "TH1.h"
#include "TFile.h"

namespace fr {
//   const char* fakeratefile = "/afs/cern.ch/work/e/echapon/public/DY_pA_2016/fakerate.root";
	const char* fakeratefile = "../estimateFR/result/fakerate_opt16_QCDopt2_histFR_QCDin0p875Data.root";

   TH1D gFR_template_barrel;
   TH1D gFR_template_endcap;
   TH1D gFR_xsec_barrel;
   TH1D gFR_xsec_endcap;
};

void initFRhistos() {
   using namespace fr;
   TFile *f = TFile::Open(fakeratefile);
   gFR_template_barrel = *(TH1D*) f->Get("FR_template_barrel")->Clone("FR1");
   gFR_template_endcap = *(TH1D*) f->Get("FR_template_endcap")->Clone("FR2");
   gFR_xsec_barrel = *(TH1D*) f->Get("FR_xsec_barrel")->Clone("FR3");
   gFR_xsec_endcap = *(TH1D*) f->Get("FR_xsec_endcap")->Clone("FR4");
   f->Close();
};

double FR_template(PhysicsMuon muon){
   using namespace fr;

   double pT = muon.pt;
   double eta = muon.eta;
   double fakerate = -999;

   if (fabs(eta)<1.2) return gFR_template_barrel.GetBinContent(gFR_template_barrel.FindBin(pT));
   else return gFR_template_endcap.GetBinContent(gFR_template_endcap.FindBin(pT));

   return fakerate;
};

double FR_ratio(PhysicsMuon muon){
   using namespace fr;

   double pT = muon.pt;
   double eta = muon.eta;
   double fakerate = -999;

   if (fabs(eta)<1.2) return gFR_xsec_barrel.GetBinContent(gFR_xsec_barrel.FindBin(pT));
   else return gFR_xsec_endcap.GetBinContent(gFR_xsec_endcap.FindBin(pT));

   return fakerate;
};

#endif // ifndef fakerate_h
