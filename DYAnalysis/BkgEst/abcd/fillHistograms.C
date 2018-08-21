#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TKey.h"
#include "../interface/defs.h"

using namespace std;
using namespace DYana;

void fillHistograms(TString sample="Data", TString trigger="PAL3Mu12") {
   TFile *f;
   if (trigger=="PAL3Mu12") {
      if (sample=="Data") f = TFile::Open("../../FitDxy/ROOTFile_Histogram_InvMass_PAL3Mu12_Data_MomCorr00_noHFrew_notnprew.root");
      else f = TFile::Open("../../FitDxy/ROOTFile_Histogram_InvMass_PAL3Mu12_Powheg_MomCorr00_rewboth_tnprew.root");
   } else {
      if (sample=="Data") f = TFile::Open("../../FitDxy/ROOTFile_Histogram_InvMass_PAL1DoubleMu0_Data_MomCorr00_noHFrew_notnprew.root");
      else f = TFile::Open("../../FitDxy/ROOTFile_Histogram_InvMass_PAL1DoubleMu0_Powheg_MomCorr00_rewboth_tnprew.root");
   }

   TFile *fout = TFile::Open("histos_ABCD_" + sample + "_" + trigger + ".root","RECREATE");

   double vtxnormchi2cut, pt1cut, pt2cut;
   if (trigger=="PAL3Mu12") {
      vtxnormchi2cut = 20;
      pt1cut = 15;
      pt2cut = 7;
   } else {
      vtxnormchi2cut = 4;
      pt1cut = 7;
      pt2cut = 7;
   }

   TIter next(f->GetListOfKeys()); TObject *obj;
   while ((obj = next())) {
      obj = ((TKey*) obj)->ReadObj();
      if (TString(obj->ClassName()) == "TTree") {
         TTree *tr = dynamic_cast<TTree*>(obj);
         TDirectory *tdir = fout->mkdir(TString(tr->GetName()).ReplaceAll("tr_",""));
         if (!tdir) {
            cout << "Error, directory " << TString(tr->GetName()).ReplaceAll("tr_","") << " already exists" << endl;
            continue;
         }
         cout << tdir->GetName() << endl;;
         tdir->cd();

         // book the histos (idx1 = Nb of iso muons; idx2 = OS/SS)
         TH1D* hMass[3][2] = {0};
         TH1D* hRap60120[3][2] = {0};
         TH1D* hRap1560[3][2] = {0};
         TH1D* hPt[3][2] = {0};
         TH1D* hPhistar[3][2] = {0};

         for (int i=0; i<3; i++) {
            for (int j=0; j<2; j++) {
               TString striso = Form("%diso",i);
               TString strsign = (j==0) ? "OS" : "SS";
               hMass[i][j] = new TH1D("h_Mass_" + striso + "_" + strsign,"",binnum,bins);
               hRap60120[i][j] = new TH1D("h_Rap60120_" + striso + "_" + strsign,"",rapbinnum_60120,rapbin_60120);
               hRap1560[i][j] = new TH1D("h_Rap1560_" + striso + "_" + strsign,"",rapbinnum_1560,rapbin_1560);
               hPt[i][j] = new TH1D("h_Pt_" + striso + "_" + strsign,"",ptbinnum_meas,ptbin_meas);
               hPhistar[i][j] = new TH1D("h_Phistar_" + striso + "_" + strsign,"",phistarnum,phistarbin);
            }
         }

         // book the branches
         float pt1, pt2, diMass, diPt, diRapidity, diPhistar, vtxnormchi2, trkiso1, trkiso2, weight=1;
         int sign, isTight1, isTight2;
         tr->SetBranchStatus("*",0);
         tr->SetBranchStatus("pt1",1); tr->SetBranchAddress("pt1",&pt1);
         tr->SetBranchStatus("pt2",1); tr->SetBranchAddress("pt2",&pt2);
         tr->SetBranchStatus("diMass",1); tr->SetBranchAddress("diMass",&diMass);
         tr->SetBranchStatus("diPt",1); tr->SetBranchAddress("diPt",&diPt);
         tr->SetBranchStatus("diRapidity",1); tr->SetBranchAddress("diRapidity",&diRapidity);
         tr->SetBranchStatus("diPhistar",1); tr->SetBranchAddress("diPhistar",&diPhistar);
         tr->SetBranchStatus("vtxnormchi2",1); tr->SetBranchAddress("vtxnormchi2",&vtxnormchi2);
         tr->SetBranchStatus("trkiso1",1); tr->SetBranchAddress("trkiso1",&trkiso1);
         tr->SetBranchStatus("trkiso2",1); tr->SetBranchAddress("trkiso2",&trkiso2);
         if (sample!="Data") {tr->SetBranchStatus("weight",1); tr->SetBranchAddress("weight",&weight);}
         tr->SetBranchStatus("sign",1); tr->SetBranchAddress("sign",&sign);
         tr->SetBranchStatus("isTight1",1); tr->SetBranchAddress("isTight1",&isTight1);
         tr->SetBranchStatus("isTight2",1); tr->SetBranchAddress("isTight2",&isTight2);

         int nentries = tr->GetEntries();
         for (int i=0; i<nentries; i++) {
            tr->GetEntry(i);
            if (isTight1+isTight2!=2) continue;
            if (vtxnormchi2>vtxnormchi2cut) continue;
            if (pt1<pt1cut) continue;
            if (pt2<pt2cut) continue;

            int niso = (trkiso1<0.3) + (trkiso2<0.3);
            int sign2 = (sign!=0);

            if (diMass>15 && diMass<600) {
               hMass[niso][sign2]->Fill(diMass,weight);
               if (diMass>15&&diMass<60) hRap1560[niso][sign2]->Fill(diRapidity-0.47,weight);
               if (diMass>60&&diMass<120) {
                  hRap60120[niso][sign2]->Fill(diRapidity-0.47,weight);
                  hPt[niso][sign2]->Fill(diPt,weight);
                  hPhistar[niso][sign2]->Fill(diPhistar,weight);
               }
            }
         }
      } // if object is a tree
   } // loop on objects

   fout->Write();
   fout->Close();
}
