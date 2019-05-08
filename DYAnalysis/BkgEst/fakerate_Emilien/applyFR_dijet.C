#include <TStyle.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <THStack.h>
#include <TMath.h>
#include <TText.h>
#include <TPad.h>
#include <TPaveText.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TColor.h>
#include <TLatex.h>
#include <TEfficiency.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "../interface/analysis.h"
#include "../interface/defs.h"
#include "../interface/cuts.h"
#include "../../HIstuff/HFweight.h"
#include "../../Include/tnp_weight.h"

using namespace std;
using namespace DYana;

void applyFR_dijet(SampleTag index)
{
   if (index == ALL) return;

   // Event & muon
   cout<<"Event"<<endl;
   PhysicsEvent* event = new PhysicsEvent();

   // the tool for HF reweighting
   HFweight hfTool;

   // the FR histos
   TFile *fFR = TFile::Open("FRhistos.root");
   TH1D *hFR = (TH1D*) fFR->Get("hFR");
   TH1D *hFR_barrel = (TH1D*) fFR->Get("hFR_barrel");
   TH1D *hFR_endcap = (TH1D*) fFR->Get("hFR_endcap");

   TFile* f = TFile::Open("histograms/histDijetEmi"+TString(Name(index))+".root","RECREATE");

   TH1D* hmass_OS_dijet = new TH1D("hmass_OS_dijet","",binnum,bins); 
   TH1D* hmass_SS_dijet = new TH1D("hmass_SS_dijet","",binnum,bins); 
   TH1D* hmass_OS_dijet_etasplit = new TH1D("hmass_OS_dijet_etasplit","",binnum,bins); 
   TH1D* hmass_SS_dijet_etasplit = new TH1D("hmass_SS_dijet_etasplit","",binnum,bins); 

   // Chain
   cout<<"Chain"<<endl;
   bool mc = !IsData(index);
   TChain*       chain = new TChain("kwtuple/physicsTree");
   chain->Add(PathTuple(index));

   // event->hiHF is not filled for some samples: get it from the recoTree
   TChain *chain4HF = new TChain("recoTree/DYTree");
   chain4HF->Add(PathTuple(index));
   float hiHFminus;
   chain4HF->SetBranchAddress("hiHFminus",&hiHFminus);
   chain4HF->SetBranchStatus("*",0);
   chain4HF->SetBranchStatus("hiHFminus",1);

   chain->SetBranchAddress("event",&event);  
   cout<<"# of events = "<<chain->GetEntries()<<endl;

   double pt = 0;
   double eta = 0;
   double iso = 0;
   int ch = 0;
   double wt = 1.0;
   double wtsum = 0;
   int entries = chain->GetEntries();
   int cnt[10]={0};

   vector<pair<PhysicsMuon,int>>* passingMuons = new vector<pair<PhysicsMuon,int>>;
   vector<pair<PhysicsMuon,int>>* failingMuons = new vector<pair<PhysicsMuon,int>>;
   pair<PhysicsMuon,PhysicsMuon>* tempMuons = new pair<PhysicsMuon,PhysicsMuon>;
   bool leading = false;


   for(int i=0; i!=entries; i++) {
      // if (i>10000) break;

      chain->GetEntry(i);
      chain4HF->GetEntry(i);
      if(mc) {
         wt = (event->weight>0) ? 1 : -1;
         wt = wt * hfTool.weight(hiHFminus,HFweight::minus,true);
         // if (i<10) cout << hiHFminus << " " << wt << endl;
         wtsum += wt;
      }
      cnt[0]++;

      if(!event->TriggerSelection(cuts::trig) ) continue;
      cnt[1]++;

      leading = false; 
      passingMuons->clear();
      failingMuons->clear();

      for(unsigned j=0; j!=event->muons.size(); j++) {
         PhysicsMuon* mu_ = (PhysicsMuon*)&event->muons.at(j);
         if( mu_->tightMuonID() && mu_->acceptance(cuts::ptmin2,cuts::etamax) ) {
            if( mu_->pt > cuts::ptmin1 ) leading = true;

            if( mu_->isolation(cuts::isomax) ) passingMuons->push_back({*mu_,j});
            else failingMuons->push_back({*mu_,j});        
         }
      }

      if( !leading ) continue;

      if(!( failingMuons->size()>1 && dijetDY(event->dimuons,failingMuons,tempMuons) )) continue;

      // fill histos
      PhysicsMuon mu1 = tempMuons->first;
      PhysicsMuon mu2 = tempMuons->second;
      TLorentzVector tmu1,tmu2,tmumu;
      tmu1.SetPtEtaPhiM(mu1.pt,mu1.eta,mu1.phi,muon_mass);
      tmu2.SetPtEtaPhiM(mu2.pt,mu2.eta,mu2.phi,muon_mass);
      tmumu = tmu1+tmu2;
      double mass = tmumu.M();
      int ch1 = mu1.charge;
      int ch2 = mu2.charge;

      // tnp weight
      double wt_tnp = 1.;
      if (mc) {
         wt_tnp *= tnp_weight_muid_ppb(mu1.pt,mu1.eta,0) * tnp_weight_muid_ppb(mu2.pt,mu2.eta,0);
         if (mu2.pt>=15. && mu1.pt>=15.) { // both muons could trigger
            double eff_data = (1 - (1 - tnp_weight_L3Mu12_ppb(mu1.eta,200)) * (1 - tnp_weight_L3Mu12_ppb(mu2.eta,200)) );
            double eff_mc = (1 - (1 - tnp_weight_L3Mu12_ppb(mu1.eta,300)) * (1 - tnp_weight_L3Mu12_ppb(mu2.eta,300)) );
            wt_tnp *= eff_data/eff_mc;
         } else if (mu1.pt<15) {
            wt_tnp *= tnp_weight_L3Mu12_ppb(mu2.eta,0);
         } else if (mu2.pt<15) {
            wt_tnp *= tnp_weight_L3Mu12_ppb(mu1.eta,0);
         }
      }

      double FRweight1 = hFR->GetBinContent(hFR->FindBin(mu1.pt));
      double FRweight2 = hFR->GetBinContent(hFR->FindBin(mu2.pt));
      // cout << iso1 << " " << iso2 << "; " << FRweight1 << " " << FRweight2 << endl;

      if (ch1 != ch2) hmass_OS_dijet->Fill(mass,FRweight1*FRweight2*wt*wt_tnp);
      else hmass_SS_dijet->Fill(mass,FRweight1*FRweight2*wt*wt_tnp);

      // split barrel / endcap
      if (fabs(mu1.eta)<0.8) FRweight1 = hFR_barrel->GetBinContent(hFR_barrel->FindBin(mu1.pt));
      else FRweight1 = hFR_endcap->GetBinContent(hFR_endcap->FindBin(mu1.pt));
      if (fabs(mu2.eta)<0.8) FRweight2 = hFR_barrel->GetBinContent(hFR_barrel->FindBin(mu2.pt));
      else FRweight2 = hFR_endcap->GetBinContent(hFR_endcap->FindBin(mu2.pt));

      if (ch1 != ch2) hmass_OS_dijet_etasplit->Fill(mass,FRweight1*FRweight2*wt*wt_tnp);
      else hmass_SS_dijet_etasplit->Fill(mass,FRweight1*FRweight2*wt*wt_tnp);
   } // event loop


   f->Write();
   f->Close();

   cout<<"Success"<<endl;
   cout<<wtsum<<endl;
   for (int i=0; i<10; i++) cout << cnt[i] << " ";
   cout << endl;

   if (event) delete event;
   if (passingMuons) delete passingMuons;
   if (failingMuons) delete failingMuons;
   if (tempMuons) delete tempMuons;
}

void applyFR_dijet() {
   vector<SampleTag> v = allSamples();
   for (vector<SampleTag>::const_iterator it = v.begin(); it!=v.end(); it++) {
      cout << Name(*it) << endl;
      applyFR_dijet(*it);
   }
}

void applyFR_dijet(int i) {
   SampleTag tag = static_cast<SampleTag>(i);
   applyFR_dijet(tag);
}
