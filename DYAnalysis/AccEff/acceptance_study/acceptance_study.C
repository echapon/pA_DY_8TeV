#include "TRandom.h"
#include "TMath.h"
#include "TH2.h"
#include "TLorentzVector.h"

// pt cuts SMP-16-009: 22, 10
// my pt cuts: 15, 10

void acceptance_study() {
   gRandom = new TRandom3();

   TH2F *hnum = new TH2F("hnum","hnum",100,0,50,100,0,50);
   hnum->GetXaxis()->SetTitle("p_{T} [GeV/c]");
   hnum->GetYaxis()->SetTitle("M [GeV/c^{2}]");
   TH2F *hden = (TH2F*) hnum->Clone("hden");

   double mumass = 105e-3;
   for (int i=0; i<10000000; i++) {
      double pt1 = gRandom->Uniform(0,50);
      double eta1 = gRandom->Uniform(-2.4,2.4);
      double phi1 = gRandom->Uniform(-TMath::Pi(),TMath::Pi());
      double pt2 = gRandom->Uniform(0,50);
      double eta2 = gRandom->Uniform(-2.4,2.4);
      double phi2 = gRandom->Uniform(-TMath::Pi(),TMath::Pi());
      TLorentzVector t1; t1.SetPtEtaPhiM(pt1,eta1,phi1,mumass);
      TLorentzVector t2; t2.SetPtEtaPhiM(pt2,eta2,phi2,mumass);
      TLorentzVector t = t1+t2;
      hden->Fill(t.Pt(),t.M());
      // if (max(pt1,pt2)>15 && min(pt1,pt2)>10) hnum->Fill(t.Pt(),t.M());
      if (max(pt1,pt2)>15 && min(pt1,pt2)>15) hnum->Fill(t.Pt(),t.M());
   }

   hnum->Divide(hden);
   hnum->Draw("COLZ");
}
