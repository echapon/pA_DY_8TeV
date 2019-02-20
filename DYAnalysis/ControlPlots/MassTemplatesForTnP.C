#define MassTemplatesForTnP_cxx
#include "MassTemplatesForTnP.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <RooRealVar.h>
#include <RooDataHist.h>
#include <RooHistPdf.h>
#include <RooWorkspace.h>
#include "../FitDxy/KDEProducer.cc"

const int nbins = 58;
const double ael[nbins] = {0  ,
   0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
   0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
   1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,
   2.1,2.1,2.1,2.1,2.1,2.1,
   0  ,1.2,2.1,
   0  ,0.3,0.6,0.9,1.2,1.6
};
const double aeh[nbins] = {2.4,
   2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,
   1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,
   2.1,2.1,2.1,2.1,2.1,2.1,2.1,2.1,2.1,2.1,2.1,2.1,2.1,2.1,
   2.4,2.4,2.4,2.4,2.4,2.4,
   1.2,2.1,2.4,
   0.3,0.6,0.9,1.2,1.6,2.1
};
const double ptl[nbins] = {7  ,
   7  ,12 ,15 ,20 ,25 ,30 ,35 ,40 ,45 ,50, 60 ,70 ,80 ,100,
   7  ,12 ,15 ,20 ,25 ,30 ,35 ,40 ,45 ,50, 60 ,70 ,80 ,100,
   7  ,12 ,15 ,20 ,25 ,30 ,35 ,40 ,45 ,50, 60 ,70 ,80 ,100,
   7  ,12 ,15 ,25 ,40 ,80 ,
   7  ,7  ,7  ,
   7  ,7  ,7  ,7  ,7  ,7
};
const double pth[nbins] = {200,
   12 ,15 ,20 ,25 ,30 ,35 ,40 ,45 ,50, 60 ,70 ,80 ,100,200,
   12 ,15 ,20 ,25 ,30 ,35 ,40 ,45 ,50, 60 ,70 ,80 ,100,200,
   12 ,15 ,20 ,25 ,30 ,35 ,40 ,45 ,50, 60 ,70 ,80 ,100,200,
   12 ,15 ,25 ,40 ,80 ,200,
   200,200,200,
   200,200,200,200,200,200
};

const int nmassbins = 500;
const double masslow = 60;//75;
const double masshigh = 130;//120;

const double tagptcut = 15;
const double tagetacut = 2.4;
const double relPFisocut = 0.15; //1e99;
const double reltkisocut = 1e99; //0.20;

const double maxdr = 0.2;
const double mumass =   105.6583745e-3;

void MassTemplatesForTnP::Loop(const char* filename)
{
//   In a ROOT session, you can do:
//      root> .L MassTemplatesForTnP.C
//      root> MassTemplatesForTnP t
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
   fChain->SetBranchStatus("weight*",1);  // weights are not KDE-friendly... ignore
   // weight = 1.;
   fChain->SetBranchStatus("gen*",1);  // activate branchname
   fChain->SetBranchStatus("pt*",1);  // activate branchname
   fChain->SetBranchStatus("eta*",1);  // activate branchname
   fChain->SetBranchStatus("phi*",1);  // activate branchname
   fChain->SetBranchStatus("relPFiso*",1);  // activate branchname
   fChain->SetBranchStatus("trkiso*",1);  // activate branchname
   fChain->SetBranchStatus("isTight*",1);  // activate branchname
   fChain->SetBranchStatus("diMass",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   // book file
   TFile *fout = TFile::Open(filename,"RECREATE");

   // book histos
   TH1D* hpass[nbins];
   TH1D* hfail[nbins];

   TH1F *hdr = new TH1F("hdr","#Delta R (gen #mu, reco #mu)",100,0,1);

   for (int ihist=0; ihist<nbins; ihist++) {
      hpass[ihist] = new TH1D(Form("hpass_abseta_%.0f_%.0f_pt_%.0f_%.0f",ael[ihist]*10.,aeh[ihist]*10.,ptl[ihist],pth[ihist]),
            Form("mass for passing: %.1f<|#eta|%.1f, %.1f<p_{T}<%.1f GeV",ael[ihist],aeh[ihist],ptl[ihist],pth[ihist]),
            nmassbins, masslow, masshigh);
      hfail[ihist] = new TH1D(Form("hfail_abseta_%.0f_%.0f_pt_%.0f_%.0f",ael[ihist]*10.,aeh[ihist]*10.,ptl[ihist],pth[ihist]),
            Form("mass for failing: %.1f<|#eta|%.1f, %.1f<p_{T}<%.1f GeV",ael[ihist],aeh[ihist],ptl[ihist],pth[ihist]),
            nmassbins, masslow, masshigh);
   }

   RooWorkspace w("ws_templates");

   vector<double> valspass[nbins];
   vector<double> weightspass[nbins];
   vector<double> valsfail[nbins];
   vector<double> weightsfail[nbins];

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      // if not in the histo mass range, skip event
      if (diMass<masslow || diMass>masshigh) continue;
      
      // gen-truth matching
      TLorentzVector genmu1, genmu2, recomu1, recomu2;
      genmu1.SetPtEtaPhiM(genpt1,geneta1,genphi1,mumass);
      genmu2.SetPtEtaPhiM(genpt2,geneta2,genphi2,mumass);
      recomu1.SetPtEtaPhiM(pt1,eta1,phi1,mumass);
      recomu2.SetPtEtaPhiM(pt2,eta2,phi2,mumass);

      double dr1 = min(recomu1.DeltaR(genmu1),recomu1.DeltaR(genmu2));
      double dr2 = min(recomu2.DeltaR(genmu1),recomu2.DeltaR(genmu2));
      hdr->Fill(dr1,weight);
      hdr->Fill(dr2,weight);

      if (dr1>maxdr || dr2>maxdr) continue;

      double reltkiso1 = trkiso1 / pt1;
      double reltkiso2 = trkiso2 / pt2;

      if (pt1>tagptcut && isTight1 && fabs(eta1)<tagetacut && relPFiso1 < relPFisocut && reltkiso1 < reltkisocut) { // mu1 is a tag
         // does the probe pass or fail isolation?
         bool passiso = (relPFiso2 < relPFisocut && reltkiso2 < reltkisocut);
         double aeta = fabs(eta2);

         for (int ihist=0; ihist<nbins; ihist++) {
            if (aeta >= ael[ihist] && aeta < aeh[ihist] && pt2 >= ptl[ihist] && pt2 < pth[ihist]) {
               if (passiso) {
                  (hpass[ihist])->Fill(diMass,weight);
                  valspass[ihist].push_back(diMass);
                  weightspass[ihist].push_back(weight);
               } else {
                  (hfail[ihist])->Fill(diMass,weight);
                  valsfail[ihist].push_back(diMass);
                  weightsfail[ihist].push_back(weight);
               }
            }
         }
      }

      if (pt2>tagptcut && isTight2 && fabs(eta2)<tagetacut && relPFiso2 < relPFisocut && reltkiso2 < reltkisocut) { // mu2 is a tag
         // does the probe pass or fail isolation?
         bool passiso = (relPFiso1 < relPFisocut && reltkiso1 < reltkisocut);
         double aeta = fabs(eta1);

         for (int ihist=0; ihist<nbins; ihist++) {
            if (aeta >= ael[ihist] && aeta < aeh[ihist] && pt1 >= ptl[ihist] && pt1 < pth[ihist]) {
               if (passiso) {
                  (hpass[ihist])->Fill(diMass,weight);
                  valspass[ihist].push_back(diMass);
                  weightspass[ihist].push_back(weight);
               } else {
                  (hfail[ihist])->Fill(diMass,weight);
                  valsfail[ihist].push_back(diMass);
                  weightsfail[ihist].push_back(weight);
               }
            }
         }
      }
   }

   // now make the RooHistPdf's
   RooRealVar var("mass","m_{#mu#mu} [GeV/c^{2}]",masslow,masshigh,"");
   for (int ihist=0; ihist<nbins; ihist++) {
      int ihist0 = ihist;
      while (ihist0>0 && valspass[ihist0].size()<10) {
         ihist0 = ihist0 - 1;
      }

      KDEProducer kdeprodpass(&(valspass[ihist0]),&(weightspass[ihist0]),1,nmassbins,masslow,masshigh,5,false);
      TH1D *hkdepass = (TH1D*) kdeprodpass.getAPDF(Form("%s_kde",hpass[ihist]->GetName()), Form("KDE density for %s",hpass[ihist]->GetTitle()), nmassbins, masslow, masshigh);
      RooDataHist *rhpass = new RooDataHist(Form("%s_roodatahist",hpass[ihist]->GetName()), hpass[ihist]->GetTitle(),RooArgList(var),hkdepass);
      RooHistPdf *pdfpass = new RooHistPdf(Form("%s_roohistpdf",hpass[ihist]->GetName()), hpass[ihist]->GetTitle(),RooArgList(var),*rhpass);
      w.import(*pdfpass);

      ihist0 = ihist;
      while (ihist0>0 && valsfail[ihist0].size()<10) {
         ihist0 = ihist0 - 1;
      }
      KDEProducer kdeprodfail(&(valsfail[ihist0]),&(weightsfail[ihist0]),1,nmassbins,masslow,masshigh,5,false);
      TH1D *hkdefail = (TH1D*) kdeprodfail.getAPDF(Form("%s_kde",hfail[ihist]->GetName()), Form("KDE density for %s",hfail[ihist]->GetTitle()), nmassbins, masslow, masshigh);
      RooDataHist *rhfail = new RooDataHist(Form("%s_roodatahist",hfail[ihist]->GetName()), hfail[ihist]->GetTitle(),RooArgList(var),hkdefail);
      RooHistPdf *pdffail = new RooHistPdf(Form("%s_roohistpdf",hfail[ihist]->GetName()), hfail[ihist]->GetTitle(),RooArgList(var),*rhfail);
      w.import(*pdffail);
   }

   // close file
   w.Write();
   fout->Write();
   fout->Close();
}
