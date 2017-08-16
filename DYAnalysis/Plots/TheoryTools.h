#ifndef TheoryTools_h
#define TheoryTools_h

#include "../BkgEst/interface/defs.h"

using namespace DYana;

// rebin theory using as inputs the output of MCFM as in https://github.com/echapon/MCFM/blob/master/MCFM-8.0/src/User/nplotter_Z_only.f
TH1F *rebin_theory(TH1F *id1, TH1F *id19, TH1F *id20, const char *name="theory_rebinned") {
   TH1F *hnew = new TH1F(name,name,binnum,bins);

   for (int ibin=1; ibin<=binnum; ibin++) {
      double massc = hnew->GetBinCenter(ibin);
      double massw = (massc-hnew->GetBinLowEdge(ibin));

      TH1F *ho;
      if (massc<60) ho = id20;
      else if (massc<120) ho = id19;
      else ho = id1;

      double s=0,e=0;
      for (int jbin=1; jbin<=ho->GetNbinsX(); jbin++) {
         double c = ho->GetBinCenter(jbin);
         double w = (c-ho->GetBinLowEdge(jbin));
         if (c<massc-massw) continue;
         if (c>massc+massw) break;
         s+=ho->GetBinContent(jbin)*2.*w;
         e=sqrt(e*e+pow(ho->GetBinError(jbin)*2.*w,2));
      }

      hnew->SetBinContent(ibin,s/(2.*massw));
      hnew->SetBinError(ibin,e/(2.*massw));
   }

   return hnew;
}

#endif // ifndef TheoryTools_h
