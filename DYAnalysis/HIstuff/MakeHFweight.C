#define MakeHFweight_cxx
#include "MakeHFweight.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <iostream>

#include "../BkgEst/interface/analysis.h"
#include "../BkgEst/interface/defs.h"

using namespace std;

void MakeHFweight::Loop(HistPack& thepack)
{
//   In a ROOT session, you can do:
//      root> .L MakeHFweight.C
//      root> MakeHFweight t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
   fChain->SetBranchStatus("*",0);  // disable all branches
   fChain->SetBranchStatus("isTightMuon",1);  // activate isTightMuon
   fChain->SetBranchStatus("Muon_pT",1);  // activate Muon_pT
   fChain->SetBranchStatus("Muon_eta",1);  // activate Muon_eta
   fChain->SetBranchStatus("Muon_phi",1);  // activate Muon_phi
   fChain->SetBranchStatus("Muon_charge",1);  // activate Muon_phi
   fChain->SetBranchStatus("nMuon",1);  // activate nMuon
   fChain->SetBranchStatus("runNum",1);  // activate evtNum
   fChain->SetBranchStatus("hiHF",1);  // activate hiHF
   fChain->SetBranchStatus("hiHFplus",1);  // activate hiHFplus
   fChain->SetBranchStatus("hiHFminus",1);  // activate hiHFminus
   fChain->SetBranchStatus("nVertices",1);  // activate nVertices
   fChain->SetBranchStatus("HLT_trigName",1);  // activate nVertices
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   double bins[47] = {0, 21, 27, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60, 63, 66, 69, 72, 75, 78, 81, 84, 87, 90, 93, 96, 99, 102, 105, 108, 111, 114, 117, 120, 123, 126, 129, 132, 135, 141, 147, 153, 159, 168, 180, 192, 216, 300};
   double binsplus[48] = {0, 8, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 84, 88, 92, 96, 100, 104, 110, 118, 126, 142, 200};
   double binsminus[33] = {0, 8, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 66, 70, 74, 82, 200};

   thepack.h_nVertices = new TH1F("h_nVertices_"+thepack.label, "Number of vertices;nPV;Events", 10, 0, 10);
   thepack.h_mass = new TH1F("h_mass_"+thepack.label, "Mass;M_{#mu#mu} [GeV/c^{2}];Events", 100, 70, 110);
   thepack.h_hiHF = new TH1F("h_hiHF_"+thepack.label, "HF energy;hiHF;Events", 46, bins);
   thepack.h_hiHFplus = new TH1F("h_hiHFplus_"+thepack.label, "HF energy (plus);hiHFplus;Events", 47, binsplus);
   thepack.h_hiHFminus = new TH1F("h_hiHFminus_"+thepack.label, "HF energy (minus);hiHFminus;Events", 32, binsminus);
   thepack.h_hiHF2D = new TH2F("h_hiHF2D_"+thepack.label, "HF energy;hiHFplus;hiHFminus", 100,0,200,100,0,200);
   thepack.h_hiHF_PV1 = new TH1F("h_hiHF_PV1_"+thepack.label, "HF energy, nPV==1;hiHF;Events", 46, bins);
   thepack.h_hiHFplus_PV1 = new TH1F("h_hiHFplus_PV1_"+thepack.label, "HF energy (plus), nPV==1;hiHFplus;Events", 47, binsplus);
   thepack.h_hiHFminus_PV1 = new TH1F("h_hiHFminus_PV1_"+thepack.label, "HF energy (minus), nPV==1;hiHFminus;Events", 32, binsminus);
   thepack.h_hiHF2D_PV1 = new TH2F("h_hiHF2D_PV1_"+thepack.label, "HF energy, nPV==1;hiHFplus;hiHFminus", 100,0,200,100,0,200);

   Long64_t nentries = fChain->GetEntriesFast();
   cout << thepack.label << " " << nentries << endl;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      // selection
      if (nMuon<2) continue;

      bool oktrig=false;
      for (unsigned int i=0; !oktrig && i<HLT_trigName->size(); i++) 
         if (TString(HLT_trigName->at(i)).Contains("HLT_PAL3Mu12")) 
            oktrig=true;

      if (!oktrig) continue;

      if (!isTightMuon[0] || !isTightMuon[1]) continue;
      if (Muon_charge[0] == Muon_charge[1]) continue;
      if (max(Muon_pT[0],Muon_pT[1])<15) continue;
      if (min(Muon_pT[0],Muon_pT[1])<10) continue;
      if (fabs(Muon_eta[0])>2.4 || fabs(Muon_eta[1])>2.4) continue;

      TLorentzVector mu1; mu1.SetPtEtaPhiM(Muon_pT[0],Muon_eta[0],Muon_phi[0],muon_mass);
      TLorentzVector mu2; mu2.SetPtEtaPhiM(Muon_pT[1],Muon_eta[1],Muon_phi[1],muon_mass);
      TLorentzVector mumu = mu1+mu2;

      if (mumu.M()<70 || mumu.M()>110) continue;

      // eta flip !
      double myHFplus = hiHFplus;
      double myHFminus = hiHFminus;
      if (DYana::runsgn(runNum)<0) {
         myHFplus = hiHFminus;
         myHFminus = hiHFplus;
      }

      // fill histos
      thepack.h_nVertices->Fill(nVertices);
      thepack.h_mass->Fill(mumu.M());
      thepack.h_hiHF->Fill(hiHF);
      thepack.h_hiHFplus->Fill(myHFplus);
      thepack.h_hiHFminus->Fill(myHFminus);
      thepack.h_hiHF2D->Fill(myHFplus,myHFminus);
      if (nVertices==1) {
         thepack.h_hiHF_PV1->Fill(hiHF);
         thepack.h_hiHFplus_PV1->Fill(myHFplus);
         thepack.h_hiHFminus_PV1->Fill(myHFminus);
         thepack.h_hiHF2D_PV1->Fill(myHFplus,myHFminus);
      }
   }

   // normalise histos
   thepack.h_nVertices->Scale(1./thepack.h_nVertices->Integral());
   thepack.h_mass->Scale(1./thepack.h_mass->Integral());
   thepack.h_hiHF->Scale(1./thepack.h_hiHF->Integral());
   thepack.h_hiHFplus->Scale(1./thepack.h_hiHFplus->Integral());
   thepack.h_hiHFminus->Scale(1./thepack.h_hiHFminus->Integral());
   thepack.h_hiHF_PV1->Scale(1./thepack.h_hiHF_PV1->Integral());
   thepack.h_hiHFplus_PV1->Scale(1./thepack.h_hiHFplus_PV1->Integral());
   thepack.h_hiHFminus_PV1->Scale(1./thepack.h_hiHFminus_PV1->Integral());
}
