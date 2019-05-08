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

void applyFR_wjets(SampleTag index)
{
   if (index == ALL) return;

   // Event & muon
   cout<<"Event"<<endl;
   PhysicsEvent* event = new PhysicsEvent();
   vector<PhysicsMuon>* passingMuons = new vector<PhysicsMuon>;

   // the tool for HF reweighting
   HFweight hfTool;

   // the FR histos
   TFile *fFR = TFile::Open("FRhistos.root");
   TH1D *hFR = (TH1D*) fFR->Get("hFR");
   TH1D *hFR_barrel = (TH1D*) fFR->Get("hFR_barrel");
   TH1D *hFR_endcap = (TH1D*) fFR->Get("hFR_endcap");

   TFile* f = TFile::Open("histograms/histWjetsEmi"+TString(Name(index))+".root","RECREATE");

   TH1D* hmass_OS_wjets = new TH1D("hmass_OS_wjets","",binnum,bins); 
   TH1D* hmass_SS_wjets = new TH1D("hmass_SS_wjets","",binnum,bins); 
   TH1D* hmass_OS_wjets_etasplit = new TH1D("hmass_OS_wjets_etasplit","",binnum,bins); 
   TH1D* hmass_SS_wjets_etasplit = new TH1D("hmass_SS_wjets_etasplit","",binnum,bins); 

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


   for(int i=0; i!=entries; i++) {

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

      passingMuons->clear();

      for(unsigned j=0; j!=event->muons.size(); j++) {

         PhysicsMuon* mu_ = (PhysicsMuon*)&event->muons.at(j);
         if( !mu_->tightMuonID() ) continue;

         if( mu_->acceptance(cuts::ptmin1,cuts::etamax) ) {
            passingMuons->push_back(*mu_);
         }
      } // loop on muons

      if (passingMuons->size()<2) continue; // skip events without any muon


      int idx1=-1, idx2=-1;
      float iso1=99., iso2=99.;
      int ch1=0, ch2=0;


      for(unsigned j=0; j!=passingMuons->size(); j++) {
         cnt[2]++;

         PhysicsMuon mu = passingMuons->at(j);
         pt = mu.pt;
         eta = runsgn(event->run) * mu.eta;
         // iso = (mu.PfChargedHadronIsoR03 + mu.PfNeutralHadronIsoR03 + mu.PfGammaIsoR03)/mu.pt;
         iso = (mu.isolationR03_sumpt)/mu.pt;
         ch = mu.charge;


         cnt[3]++;

         if (iso<iso1) { // is it the best muon we've seen so far?
            iso2=iso1;
            idx2=idx1;
            ch2=ch1;
            iso1=iso;
            idx1=j;
            ch1=ch;
         } else if (iso<iso2) { // is it the second best muon we've seen so far?
            iso2=iso;
            idx2=j;
            ch2=ch;
         }
      } // loop on passing muons

      if (idx1<0 || idx2<0) continue;

      // we only base the wjets FR estimation on 1pass + 1fail event (exactly 1 iso)
      if ((iso1>cuts::isomax && iso2>cuts::isomax) || (iso1<=cuts::isomax && iso2<=cuts::isomax)) continue;

      // fill histos
      PhysicsMuon mu1 = passingMuons->at(idx1);
      PhysicsMuon mu2 = passingMuons->at(idx2);
      TLorentzVector tmu1,tmu2,tmumu;
      tmu1.SetPtEtaPhiM(mu1.pt,mu1.eta,mu1.phi,muon_mass);
      tmu2.SetPtEtaPhiM(mu2.pt,mu2.eta,mu2.phi,muon_mass);
      tmumu = tmu1+tmu2;
      double mass = tmumu.M();

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

      double FRweight = iso1>iso2 ? hFR->GetBinContent(hFR->FindBin(mu1.pt)) : hFR->GetBinContent(hFR->FindBin(mu2.pt));

      if (ch1 != ch2) hmass_OS_wjets->Fill(mass,FRweight*wt*wt_tnp);
      else hmass_SS_wjets->Fill(mass,FRweight*wt*wt_tnp);

      // split barrel / endcap
      if (iso1>iso2) {
         if (fabs(mu1.eta)<0.8) {
            FRweight = hFR->GetBinContent(hFR_barrel->FindBin(mu1.pt));
         } else {
            FRweight = hFR->GetBinContent(hFR_endcap->FindBin(mu1.pt));
         }
      } else {
         if (fabs(mu2.eta)<0.8) {
            FRweight = hFR->GetBinContent(hFR_barrel->FindBin(mu2.pt));
         } else {
            FRweight = hFR->GetBinContent(hFR_endcap->FindBin(mu2.pt));
         }
      }

      if (ch1 != ch2) hmass_OS_wjets_etasplit->Fill(mass,FRweight*wt*wt_tnp);
      else hmass_SS_wjets_etasplit->Fill(mass,FRweight*wt*wt_tnp);
   } // event loop


   f->Write();
   f->Close();

   cout<<"Success"<<endl;
   cout<<wtsum<<endl;
   for (int i=0; i<10; i++) cout << cnt[i] << " ";
   cout << endl;

   delete event;
   delete passingMuons;
}

void applyFR_wjets() {
   vector<SampleTag> v = allSamples();
   for (vector<SampleTag>::const_iterator it = v.begin(); it!=v.end(); it++) {
      cout << Name(*it) << endl;
      applyFR_wjets(*it);
   }
}

void applyFR_wjets(int i) {
   SampleTag tag = static_cast<SampleTag>(i);
   applyFR_wjets(tag);
}
