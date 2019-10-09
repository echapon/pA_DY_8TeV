#ifndef fakerate_h
#define fakerate_h

#include "PhysicsMuon.h"
#include "TH1.h"
#include "TFile.h"

#include <iostream>

namespace fr {
  // const char* fakeratefile_templates = "/afs/cern.ch/work/h/hckim/public/ForEmilien_20190506_v2/input/estimateFRinput/fakerate_reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt2_L3Mu12_beforeCor.root";
  // const char* fakeratefile_templates = "/afs/cern.ch/work/h/hckim/public/ForEmilien_20190514/FRfinal/fakerate_reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt2_L3Mu12.root";
  const char* fakeratefile_templates = "/afs/cern.ch/work/e/echapon/public/DY_pA_2016/FR_HCK_v20190918/fakerate_QCDopt3_ptincl_correctedFR.root";
  const char* fakeratefile_dataMC = "/afs/cern.ch/work/e/echapon/public/DY_pA_2016/FR_Emilien_20190508/FRhistos_2muSS.root";
  const char* fakeratefile_dataMC_2muOS_hichi2 = "/afs/cern.ch/work/e/echapon/public/DY_pA_2016/FR_Emilien_20191009/FRhistos_2muOS_hichi2.root";
  const char* fakeratefile_dataMC_2muSS = "/afs/cern.ch/work/e/echapon/public/DY_pA_2016/FR_Emilien_20191009/FRhistos_2muSS.root";
  // const char* fakeratefile_dataMC = "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate_Emilien/FRhistos_2muSS.root"; // ARC CHECK

   TH1D gFR_template_barrel;
   TH1D gFR_template_endcap;
   TH1D gFR_xsec_barrel;
   TH1D gFR_xsec_endcap;
};

void initFRhistos() {
   using namespace fr;
   TFile *f_temp = TFile::Open(fakeratefile_dataMC_2muOS_hichi2);
   TFile *f_dataMC = TFile::Open(fakeratefile_dataMC_2muSS);

   std::cout << "Nominal (data-MC, 2muOS_hichi2) FR file: " << fakeratefile_dataMC_2muOS_hichi2 << std::endl;
   std::cout << "Alternative (data-MC, 2muSS) FR file: " << fakeratefile_dataMC_2muSS << std::endl;

   gFR_template_barrel = *(TH1D*) f_temp->Get("hFR2_barrel")->Clone("FR1");
   gFR_template_endcap = *(TH1D*) f_temp->Get("hFR2_endcap")->Clone("FR2");
   gFR_xsec_barrel = *(TH1D*) f_dataMC->Get("hFR2_barrel")->Clone("FR3");
   gFR_xsec_endcap = *(TH1D*) f_dataMC->Get("hFR2_endcap")->Clone("FR4");
   f_temp->Close();
   f_dataMC->Close();
};

double FR_template(PhysicsMuon muon){
   using namespace fr;

   double pT = muon.pt;
   double eta = muon.eta;
   double fakerate = -999;
   TH1D *hFR_temp = NULL;

   if (fabs(eta)<1.2) hFR_temp = &gFR_template_barrel;
   else hFR_temp = &gFR_template_endcap;

   int ibin = hFR_temp->FindBin(pT);
   while (fakerate <= 0. || fakerate >= 0.95) { // this is to avoid buggy fakerate in the high pt bins in some cases: look for the closest lower pt bin with nonzero fakerate
      fakerate = hFR_temp->GetBinContent(ibin);
      ibin--;
   }

   return fakerate;
};

double FR_ratio(PhysicsMuon muon){
   using namespace fr;

   double pT = muon.pt;
   double eta = muon.eta;
   double fakerate = -999;
   TH1D *hFR_temp = NULL;

   if (fabs(eta)<1.2) hFR_temp = &gFR_xsec_barrel;
   else hFR_temp = &gFR_xsec_endcap;

   int ibin = hFR_temp->FindBin(pT);
   while (fakerate <= 0. || fakerate >= 0.95) { // this is to avoid buggy fakerate in the high pt bins in some cases: look for the closest lower pt bin with nonzero fakerate
      fakerate = hFR_temp->GetBinContent(ibin);
      ibin--;
   }

   return fakerate;
};

#endif // ifndef fakerate_h
