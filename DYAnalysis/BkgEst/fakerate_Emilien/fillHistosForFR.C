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
#include "../../HIstuff/HFweight.h"
#include "../../Include/tnp_weight.h"

using namespace std;
using namespace DYana;

void fillHistosForFR(SampleTag index)
{
   if (index == ALL) return;

   // Event & muon
   cout<<"Event"<<endl;
   PhysicsEvent* event = new PhysicsEvent();
   vector<PhysicsMuon>* passingMuons = new vector<PhysicsMuon>;
   vector<pair<PhysicsMuon,int>>* passingMuons2 = new vector<pair<PhysicsMuon,int>>;
   pair<PhysicsMuon,PhysicsMuon>* tempMuons = new pair<PhysicsMuon,PhysicsMuon>;

   // the tool for HF reweighting
   HFweight hfTool;


   TFile* f = TFile::Open("histograms/histFREmi"+TString(Name(index))+".root","RECREATE");


   // for ABCD
   TH1D* hmass_OS_2iso = new TH1D("hmass_OS_2iso","",binnum,bins); 
   TH1D* hmass_OS_1iso = new TH1D("hmass_OS_1iso","",binnum,bins); 
   TH1D* hmass_OS_0iso = new TH1D("hmass_OS_0iso","",binnum,bins); 
   TH1D* hmass_SS_2iso = new TH1D("hmass_SS_2iso","",binnum,bins); 
   TH1D* hmass_SS_1iso = new TH1D("hmass_SS_1iso","",binnum,bins); 
   TH1D* hmass_SS_0iso = new TH1D("hmass_SS_0iso","",binnum,bins); 

   // for template fitting
   const double isomaxrange = 1;
   TH2D* h2d_OS_60120 = new TH2D("h2d_OS_60120","",ptbinnum,ptbin,20,0,isomaxrange);
   TH2D* h2d_SS = new TH2D("h2d_SS","",ptbinnum,ptbin,20,0,1);
   TH2D* h2d_1mu_HLTL3Mu12 = new TH2D("h2d_1mu_HLTL3Mu12","",ptbinnum,ptbin,20,0,isomaxrange);
   TH2D* h2d_1mu_HLTL3Mu12_barrel = new TH2D("h2d_1mu_HLTL3Mu12_barrel","",ptbinnum,ptbin,20,0,isomaxrange);
   TH2D* h2d_1mu_HLTL3Mu12_endcap = new TH2D("h2d_1mu_HLTL3Mu12_endcap","",ptbinnum_endcap,ptbin_endcap,20,0,isomaxrange);

   // 2D (pt, iso) histos with SS selection
   TH2D* h2d_2muSS_HLTL3Mu12 = new TH2D("h2d_2muSS_HLTL3Mu12","",ptbinnum,ptbin,20,0,isomaxrange);
   TH2D* h2d_2muSS_HLTL3Mu12_barrel = new TH2D("h2d_2muSS_HLTL3Mu12_barrel","",ptbinnum,ptbin,20,0,isomaxrange);
   TH2D* h2d_2muSS_HLTL3Mu12_endcap = new TH2D("h2d_2muSS_HLTL3Mu12_endcap","",ptbinnum_endcap,ptbin_endcap,20,0,isomaxrange);
   TH2D* h2d_2muSS_lochi2_HLTL3Mu12 = new TH2D("h2d_2muSS_lochi2_HLTL3Mu12","",ptbinnum,ptbin,20,0,isomaxrange);
   TH2D* h2d_2muSS_lochi2_HLTL3Mu12_barrel = new TH2D("h2d_2muSS_lochi2_HLTL3Mu12_barrel","",ptbinnum,ptbin,20,0,isomaxrange);
   TH2D* h2d_2muSS_lochi2_HLTL3Mu12_endcap = new TH2D("h2d_2muSS_lochi2_HLTL3Mu12_endcap","",ptbinnum_endcap,ptbin_endcap,20,0,isomaxrange);
   TH2D* h2d_2muSS_hichi2_HLTL3Mu12 = new TH2D("h2d_2muSS_hichi2_HLTL3Mu12","",ptbinnum,ptbin,20,0,isomaxrange);
   TH2D* h2d_2muSS_hichi2_HLTL3Mu12_barrel = new TH2D("h2d_2muSS_hichi2_HLTL3Mu12_barrel","",ptbinnum,ptbin,20,0,isomaxrange);
   TH2D* h2d_2muSS_hichi2_HLTL3Mu12_endcap = new TH2D("h2d_2muSS_hichi2_HLTL3Mu12_endcap","",ptbinnum_endcap,ptbin_endcap,20,0,isomaxrange);

   // addition of Mu7: FIXME need to check several triggers and get the prescale, from the other tree...
   TH2D* h2d_1mu_HLTL3Mu712 = new TH2D("h2d_1mu_HLTL3Mu712","",ptbinnum,ptbin,20,0,isomaxrange);
   TH2D* h2d_1mu_HLTL3Mu712_barrel = new TH2D("h2d_1mu_HLTL3Mu712_barrel","",ptbinnum,ptbin,20,0,isomaxrange);
   TH2D* h2d_1mu_HLTL3Mu712_endcap = new TH2D("h2d_1mu_HLTL3Mu712_endcap","",ptbinnum_endcap,ptbin_endcap,20,0,isomaxrange);

   // MET
   TH1D *hMET = new TH1D("hMET","",100,0,200);
   TH1D *hMET_type1 = new TH1D("hMET_type1","",100,0,200);

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
   int idx1=-1, idx2=-1;
   double iso1=99., iso2=99.;
   double pt1, pt2, eta1, eta2;
   int ch1=0, ch2=0;
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
      passingMuons2->clear();

      for(unsigned j=0; j!=event->muons.size(); j++) {

         PhysicsMuon* mu_ = (PhysicsMuon*)&event->muons.at(j);
         // if( !mu_->tightMuonID() ) continue;
         if( !mu_->looseMuonID() ) continue; // ID+ISO CHECK

         // if( mu_->acceptance(cuts::ptmin1,cuts::etamax) ) {
         if( mu_->acceptance(cuts::ptmin2,cuts::etamax) ) {
            passingMuons->push_back(*mu_);
            passingMuons2->push_back({*mu_,j});
         }
      } // loop on muons

      if (passingMuons->size()<1) continue; // skip events without any muon

      if (passingMuons->size()==1) { 
         ///////////////////////////////////////
         //         single muon histos        //
         ///////////////////////////////////////
         // if (event->MET.at(0).pt < 20) { // MET cut to reject Wjets
            PhysicsMuon mu = passingMuons->at(0);
            pt = mu.pt;
            eta = runsgn(event->run) * mu.eta;
            // iso = (mu.PfChargedHadronIsoR03 + mu.PfNeutralHadronIsoR03 + mu.PfGammaIsoR03)/mu.pt;
            iso = (mu.isolationR03_sumpt)/mu.pt;
            ch = mu.charge;

            // overflow
            if (iso >= isomaxrange) iso = isomaxrange-1e-5;
            if (!mu.tightMuonID()) iso = 1.5*cuts::isomax; // ID+ISO CHECK

            // tnp weight
            double wt_tnp = 1.;
            wt_tnp *= mc ? tnp_weight_muid_ppb(pt,eta,0) : 1;
            wt_tnp *= mc ? tnp_weight_L3Mu12_ppb(eta,0) : 1;

            h2d_1mu_HLTL3Mu12->Fill(pt,iso,wt_tnp);
            if (fabs(eta)<1.2) h2d_1mu_HLTL3Mu12_barrel->Fill(pt,iso,wt_tnp);
            else h2d_1mu_HLTL3Mu12_endcap->Fill(pt,iso,wt_tnp);

            hMET->Fill(event->MET.at(0).pt,wt_tnp);
            hMET_type1->Fill(event->MET.at(0).Type1_pt,wt_tnp);
         // }
      } else { 
         ///////////////////////////////////////
         //         double muon histos        //
         ///////////////////////////////////////
         idx1=-1; idx2=-1;
         iso1=99.; iso2=99.;
         ch1=0; ch2=0;

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

         double wt_tnp_iso1 = mc ? tnp_weight_isotk_ppb(mu1.pt,mu1.eta,0) : 1.;
         double wt_tnp_iso2 = mc ? tnp_weight_isotk_ppb(mu2.pt,mu2.eta,0) : 1.;

         // fill the mass histos
         using namespace cuts;
         if (iso1<isomax&&iso2<isomax) {
            if (ch1*ch2<0) hmass_OS_2iso->Fill(mass, wt*wt_tnp*wt_tnp_iso1*wt_tnp_iso2);
            else hmass_SS_2iso->Fill(mass, wt*wt_tnp*wt_tnp_iso1*wt_tnp_iso2);
         } else if (iso1<isomax||iso2<isomax) {
            if (ch1*ch2<0) hmass_OS_1iso->Fill(mass, wt*wt_tnp*wt_tnp_iso1/wt_tnp_iso2);
            else hmass_SS_1iso->Fill(mass, wt*wt_tnp*wt_tnp_iso1/wt_tnp_iso2);
         } else {
            if (ch1*ch2<0) hmass_OS_0iso->Fill(mass, wt*wt_tnp/wt_tnp_iso1/wt_tnp_iso2);
            else hmass_SS_0iso->Fill(mass, wt*wt_tnp/wt_tnp_iso1/wt_tnp_iso2);
         }

         // fill the iso histos
         if (ch1*ch2<0 && mass>60 && mass<120 && iso1<isomax) h2d_OS_60120->Fill(mu2.pt,iso2,wt*wt_tnp*wt_tnp_iso1);
         if (ch1*ch2>0 && iso1<isomax) h2d_SS->Fill(mu2.pt,iso2,wt*wt_tnp*wt_tnp_iso1);

         // iso histos for SS
         double chi2min=20;
         if (dimuonDY(event->triggerobjects,event->dimuons,passingMuons2,tempMuons,chi2min,false)) { // low dimuon vertex chi2
            PhysicsMuon mu1 = tempMuons->first;
            PhysicsMuon mu2 = tempMuons->second;
            ch1 = mu1.charge;
            ch2 = mu2.charge;
            if (ch1 != ch2) continue; // keep only SS

            pt1 = mu1.pt;
            eta1 = runsgn(event->run) * mu1.eta;
            iso1 = (mu1.isolationR03_sumpt)/mu1.pt;
            pt2 = mu2.pt;
            eta2 = runsgn(event->run) * mu2.eta;
            iso2 = (mu2.isolationR03_sumpt)/mu2.pt;

            // overflow
            if (iso1 >= isomaxrange) iso1 = isomaxrange-1e-5;
            if (iso2 >= isomaxrange) iso2 = isomaxrange-1e-5;
            if (!mu1.tightMuonID()) iso1 = 1.5*cuts::isomax; // ID+ISO CHECK
            if (!mu2.tightMuonID()) iso2 = 1.5*cuts::isomax; // ID+ISO CHECK

            // tnp weight
            double wt_tnp = 1.;
            wt_tnp *= mc ? tnp_weight_muid_ppb(pt1,eta1,0) * tnp_weight_muid_ppb(pt2,eta2,0) : 1;
            wt_tnp *= mc ? tnp_weight_L3Mu12_ppb(eta1,0) : 1;

            h2d_2muSS_HLTL3Mu12->Fill(pt1,iso1,wt_tnp);
            h2d_2muSS_HLTL3Mu12->Fill(pt2,iso2,wt_tnp);
            if (fabs(eta1)<1.2) h2d_2muSS_HLTL3Mu12_barrel->Fill(pt1,iso1,wt_tnp);
            else h2d_2muSS_HLTL3Mu12_endcap->Fill(pt1,iso1,wt_tnp);
            if (fabs(eta2)<1.2) h2d_2muSS_HLTL3Mu12_barrel->Fill(pt2,iso2,wt_tnp);
            else h2d_2muSS_HLTL3Mu12_endcap->Fill(pt2,iso2,wt_tnp);

            h2d_2muSS_lochi2_HLTL3Mu12->Fill(pt1,iso1,wt_tnp);
            h2d_2muSS_lochi2_HLTL3Mu12->Fill(pt2,iso2,wt_tnp);
            if (fabs(eta1)<1.2) h2d_2muSS_lochi2_HLTL3Mu12_barrel->Fill(pt1,iso1,wt_tnp);
            else h2d_2muSS_lochi2_HLTL3Mu12_endcap->Fill(pt1,iso1,wt_tnp);
            if (fabs(eta2)<1.2) h2d_2muSS_lochi2_HLTL3Mu12_barrel->Fill(pt2,iso2,wt_tnp);
            else h2d_2muSS_lochi2_HLTL3Mu12_endcap->Fill(pt2,iso2,wt_tnp);
         } else if (dimuonDY(event->triggerobjects,event->dimuons,passingMuons2,tempMuons,chi2min,true)) { // high dimuon vertex chi2
            PhysicsMuon mu1 = tempMuons->first;
            PhysicsMuon mu2 = tempMuons->second;
            ch1 = mu1.charge;
            ch2 = mu2.charge;
            if (ch1 != ch2) continue; // keep only SS

            pt1 = mu1.pt;
            eta1 = runsgn(event->run) * mu1.eta;
            iso1 = (mu1.isolationR03_sumpt)/mu1.pt;
            pt2 = mu2.pt;
            eta2 = runsgn(event->run) * mu2.eta;
            iso2 = (mu2.isolationR03_sumpt)/mu2.pt;

            // overflow
            if (iso1 >= isomaxrange) iso1 = isomaxrange-1e-5;
            if (iso2 >= isomaxrange) iso2 = isomaxrange-1e-5;
            if (!mu1.tightMuonID()) iso1 = 1.5*cuts::isomax; // ID+ISO CHECK
            if (!mu2.tightMuonID()) iso2 = 1.5*cuts::isomax; // ID+ISO CHECK

            // tnp weight
            double wt_tnp = 1.;
            wt_tnp *= mc ? tnp_weight_muid_ppb(pt1,eta1,0) * tnp_weight_muid_ppb(pt2,eta2,0) : 1;
            wt_tnp *= mc ? tnp_weight_L3Mu12_ppb(eta1,0) : 1;

            h2d_2muSS_HLTL3Mu12->Fill(pt1,iso1,wt_tnp);
            h2d_2muSS_HLTL3Mu12->Fill(pt2,iso2,wt_tnp);
            if (fabs(eta1)<1.2) h2d_2muSS_HLTL3Mu12_barrel->Fill(pt1,iso1,wt_tnp);
            else h2d_2muSS_HLTL3Mu12_endcap->Fill(pt1,iso1,wt_tnp);
            if (fabs(eta2)<1.2) h2d_2muSS_HLTL3Mu12_barrel->Fill(pt2,iso2,wt_tnp);
            else h2d_2muSS_HLTL3Mu12_endcap->Fill(pt2,iso2,wt_tnp);

            h2d_2muSS_hichi2_HLTL3Mu12->Fill(pt1,iso1,wt_tnp);
            h2d_2muSS_hichi2_HLTL3Mu12->Fill(pt2,iso2,wt_tnp);
            if (fabs(eta1)<1.2) h2d_2muSS_hichi2_HLTL3Mu12_barrel->Fill(pt1,iso1,wt_tnp);
            else h2d_2muSS_hichi2_HLTL3Mu12_endcap->Fill(pt1,iso1,wt_tnp);
            if (fabs(eta2)<1.2) h2d_2muSS_hichi2_HLTL3Mu12_barrel->Fill(pt2,iso2,wt_tnp);
            else h2d_2muSS_hichi2_HLTL3Mu12_endcap->Fill(pt2,iso2,wt_tnp);
         }
      } // >=2 muon case
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

void fillHistosForFR() {
   vector<SampleTag> v = allSamples();
   for (vector<SampleTag>::const_iterator it = v.begin(); it!=v.end(); it++) {
      cout << Name(*it) << endl;
      fillHistosForFR(*it);
   }
}

void fillHistosForFR(int i) {
   SampleTag tag = static_cast<SampleTag>(i);
   fillHistosForFR(tag);
}
