#ifndef fakerate_h
#define fakerate_h

#include "PhysicsMuon.h"
#include "TH1.h"
#include "TFile.h"

#include <iostream>

namespace fr {
  const char* fakeratefile_templates = "/afs/cern.ch/work/h/hckim/public/ForEmilien_20190506_v2/input/estimateFRinput/fakerate_reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt2_L3Mu12_beforeCor.root";
  const char* fakeratefile_dataMC = "/afs/cern.ch/work/e/echapon/public/DY_pA_2016/FR_Emilien_20190508/FRhistos_2muSS.root";

   TH1D gFR_template_barrel;
   TH1D gFR_template_endcap;
   TH1D gFR_xsec_barrel;
   TH1D gFR_xsec_endcap;
};

void initFRhistos() {
   using namespace fr;
   TFile *f_temp = TFile::Open(fakeratefile_templates);
   TFile *f_dataMC = TFile::Open(fakeratefile_dataMC);

   std::cout << "Nominal (template) FR file: " << fakeratefile_templates << std::endl;
   std::cout << "Alternative (data-MC) FR file: " << fakeratefile_dataMC << std::endl;

   gFR_template_barrel = *(TH1D*) f_temp->Get("FR_template_barrel")->Clone("FR1");
   gFR_template_endcap = *(TH1D*) f_temp->Get("FR_template_endcap")->Clone("FR2");
   gFR_xsec_barrel = *(TH1D*) f_dataMC->Get("FR_xsec_barrel")->Clone("FR3");
   gFR_xsec_endcap = *(TH1D*) f_dataMC->Get("FR_xsec_endcap")->Clone("FR4");
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
   while (fakerate <= 0.) { // this is to avoid 0 fakerate in the high pt bins in some cases: look for the closest lower pt bin with nonzero fakerate
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
   while (fakerate <= 0.) { // this is to avoid 0 fakerate in the high pt bins in some cases: look for the closest lower pt bin with nonzero fakerate
      fakerate = hFR_temp->GetBinContent(ibin);
      ibin--;
   }

   return fakerate;
};

#endif // ifndef fakerate_h
