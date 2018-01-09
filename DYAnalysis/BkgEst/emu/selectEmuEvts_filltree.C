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
#include "Include/DYAnalyzer.h"
#include "Include/NtupleHandle.h"
#include "Include/Object.h"
#include "Include/Object.h"

const Int_t nLep = 4;
const Int_t eleID = 11;
const Int_t muID = 13;

using namespace std;
using namespace DYana;

void selectEmuEvts_filltree(SampleTag index)
{
    //Event
    TChain*       chain = new TChain("kwtuple/physicsTree");
    PhysicsEvent* event = new PhysicsEvent();
    bool isData = IsData(index);;
    chain->Add(PathTuple(index));
    chain->SetBranchAddress("event",&event);  
    TChain *chain2 = new TChain("recoTree/DYTree");
    chain2->Add(PathTuple(index));
    bool doflip = switcheta(index);
    NtupleHandle *ntuple = new NtupleHandle( chain2, doflip );
    ntuple->TurnOnBranches_GenLepton();

    DYAnalyzer *analyzer = new DYAnalyzer( "PAL3Mu12" );
    TString BaseLocation = "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/";
    // -- Each ntuple directory & corresponding Tags -- //
    // -- GenWeights are already taken into account in nEvents -- //
    vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents; vector< SampleTag > STags;
    analyzer->SetupMCsamples_v20170830("Powheg", &ntupleDirectory, &Tag, &Xsec, &nEvents, &STags);

    vector<pair<PhysicsMuon,int>>*     passingMuons     = new vector<pair<PhysicsMuon,int>>;
    vector<pair<PhysicsElectron,int>>* passingElectrons = new vector<pair<PhysicsElectron,int>>;
    pair<PhysicsMuon,PhysicsMuon>*     dimuon           = new pair<PhysicsMuon,PhysicsMuon>;
    pair<PhysicsElectron,PhysicsMuon>* emu              = new pair<PhysicsElectron,PhysicsMuon>;
    double chi2min = 999.;

    //Histogram 
    TString tagname = Name(index);
    TFile* f = new TFile("histograms/histAndTree"+tagname+".root","RECREATE");
    const int binsize = binnum;

    TH1D* el_etSC     = new TH1D("el_etSC","",97,30,1000); 
    TH1D* el_etaSC    = new TH1D("el_etaSC","",42,-2.1,2.1); 
    TH1D* el_phi      = new TH1D("el_phi","",64,-3.2,3.2); 
    TH1D* emu_mass    = new TH1D("emu_mass","",binsize,bins);
    TH1D* emuSS_mass  = new TH1D("emuSS_mass","",binsize,bins);
    TH1D* dimu_mass   = new TH1D("dimu_mass","",binsize,bins);
    TH1D* dimuSS_mass = new TH1D("dimuSS_mass","",binsize,bins);
    TH1D* emu_pt    = new TH1D("emu_pt","",ptbinnum_meas,ptbin_meas);
    TH1D* emuSS_pt  = new TH1D("emuSS_pt","",ptbinnum_meas,ptbin_meas);
    TH1D* dimu_pt   = new TH1D("dimu_pt","",ptbinnum_meas,ptbin_meas);
    TH1D* dimuSS_pt = new TH1D("dimuSS_pt","",ptbinnum_meas,ptbin_meas);
    TH1D* emu_phistar    = new TH1D("emu_phistar","",phistarnum,phistarbin);
    TH1D* emuSS_phistar  = new TH1D("emuSS_phistar","",phistarnum,phistarbin);
    TH1D* dimu_phistar   = new TH1D("dimu_phistar","",phistarnum,phistarbin);
    TH1D* dimuSS_phistar = new TH1D("dimuSS_phistar","",phistarnum,phistarbin);
    TH1D* emu_rap1560    = new TH1D("emu_rap1560","",rapbinnum_1560,rapbin_1560);
    TH1D* emuSS_rap1560  = new TH1D("emuSS_rap1560","",rapbinnum_1560,rapbin_1560);
    TH1D* dimu_rap1560   = new TH1D("dimu_rap1560","",rapbinnum_1560,rapbin_1560);
    TH1D* dimuSS_rap1560 = new TH1D("dimuSS_rap1560","",rapbinnum_1560,rapbin_1560);
    TH1D* emu_rap60120    = new TH1D("emu_rap60120","",rapbinnum_60120,rapbin_60120);
    TH1D* emuSS_rap60120  = new TH1D("emuSS_rap60120","",rapbinnum_60120,rapbin_60120);
    TH1D* dimu_rap60120   = new TH1D("dimu_rap60120","",rapbinnum_60120,rapbin_60120);
    TH1D* dimuSS_rap60120 = new TH1D("dimuSS_rap60120","",rapbinnum_60120,rapbin_60120);
    TH1D* emu_chi2    = new TH1D("emu_chi2","",100,0,20);
    TH1D* emuSS_chi2  = new TH1D("emuSS_chi2","",100,0,20);
    TH1D* dimu_chi2   = new TH1D("dimu_chi2","",100,0,20);
    TH1D* dimuSS_chi2 = new TH1D("dimuSS_chi2","",100,0,20);

    TTree *skimTree_p = new TTree("skimTree","skimTree");
    Int_t nLep_;
    Int_t lepID_[nLep];
    Float_t lepPt_[nLep];
    Float_t lepPhi_[nLep];
    Float_t lepEta_[nLep];
    Int_t lepChg_[nLep];
    Float_t lepIso_[nLep];
    Float_t emuPt_;
    Float_t emuRap_;
    Float_t emuPhi_;
    Float_t emuMass_;
    Int_t emuSgn_;
    Float_t emuPhistar_;
    Float_t emuDpt_;
    Float_t emuDeta_;
    Float_t emuDphi_;
    Float_t emuDR_;
    skimTree_p->Branch("nLep", &nLep_, "nLep/I");
    skimTree_p->Branch("lepID", lepID_, Form("lepID[%d]/I",nLep));
    skimTree_p->Branch("lepPt", lepPt_, Form("lepPt[%d]/F", nLep));
    skimTree_p->Branch("lepPhi", lepPhi_, Form("lepPhi[%d]/F", nLep));
    skimTree_p->Branch("lepEta", lepEta_, Form("lepEta[%d]/F", nLep));
    skimTree_p->Branch("lepChg", lepChg_, Form("lepChg[%d]/I", nLep));
    skimTree_p->Branch("lepIso", lepIso_, Form("lepIso[%d]/F", nLep)); 

    skimTree_p->Branch("emuPt", &emuPt_, "emuPt/F"); 
    skimTree_p->Branch("emuRap", &emuRap_, "emuRap/F"); 
    skimTree_p->Branch("emuPhi", &emuPhi_, "emuPhi/F"); 
    skimTree_p->Branch("emuMass", &emuMass_, "emuMass/F"); 
    skimTree_p->Branch("emuSgn", &emuSgn_, "emuSgn/I"); 
    skimTree_p->Branch("emuPhistar", &emuPhistar_, "emuPhistar/F"); 
    skimTree_p->Branch("emuDpt", &emuDpt_, "emuDpt/F"); 
    skimTree_p->Branch("emuDeta", &emuDeta_, "emuDeta/F"); 
    skimTree_p->Branch("emuDphi", &emuDphi_, "emuDphi/F"); 
    skimTree_p->Branch("emuDR", &emuDR_, "emuDR/F"); 

    cout<<"# of events = "<<chain->GetEntries()<<endl;

    bool leadingMu = false;
    bool leadingEl = false;
    double weight = 1.0;
    double weightedSum = 0;

    int tryEmu = 0;
    int totalMu = 0;
    int totalEl = 0;
    int okEl = 0;
    int okEmu = 0;
    int okDimu = 0;

    //Iteration
    //int entries = 100000;
    int entries = chain->GetEntries();
    cout << entries << " " << chain2->GetEntries() << endl;
    
    for(int i=0; i!=entries; i++) { 
       ntuple->GetEvent(i);
       Bool_t GenFlag = kFALSE;
       GenFlag = analyzer->SeparateDYLLSample_isHardProcess(tagname, ntuple);
       if (!GenFlag) continue;
       chain->GetEntry(i);
       event->switcheta(doflip);

        if( !isData ) {
            if( event->weight>0 ) weight = 1.0;
            else weight = -1.0;
        }
        weightedSum += weight;

        // if( (IsDY(index)) && !event->TauSelection() ) continue;

        if( !event->TriggerSelection("HLT_PAL3Mu12_v") ) continue;

        leadingMu = false; 
        leadingEl = false; 

        passingMuons->clear();
        passingElectrons->clear();

        for(unsigned j=0; j!=event->muons.size(); j++) {

            PhysicsMuon* mu = (PhysicsMuon*)&event->muons.at(j);

            if( MuSel(mu) ) {
                totalMu += weight;
				//cout<<mu->pt<<endl;
                pair<PhysicsMuon,int> taggedmu = {*mu,j};
                passingMuons->push_back(taggedmu);
                if(mu->pt > cuts::ptmin1) leadingMu = true;
            } 
        } 

        for(unsigned j=0; j!=event->electrons.size(); j++) {

            PhysicsElectron* el = (PhysicsElectron*)&event->electrons.at(j);

            if( EleSel(el) ) { 
                totalEl += weight;
                pair<PhysicsElectron,int> taggedel = {*el,j};
                passingElectrons->push_back(taggedel);
                if(el->pt > cuts::ptmin1) leadingEl = true;
            } 
        } 


        // if( (event->TriggerSelection("HLT_Ele22_eta2p1_WP75_Gsf_v") || event->TriggerSelection("HLT_Ele22_eta2p1_WPLoose_Gsf_v")) && (event->TriggerSelection("HLT_IsoMu20_v") || event->TriggerSelection("HLT_IsoTkMu20_v")) ) { //Single electron spectra
        if( // event->TriggerSelection("HLT_PAEle20_WPLoose_Gsf_v") && 
              event->TriggerSelection("HLT_PAL3Mu12_v") ) { //Single electron spectra
            for(unsigned j=0; j!=passingElectrons->size(); j++) { 
                PhysicsElectron el = passingElectrons->at(j).first;
                if( el.acceptance(cuts::ptmin1,2.1) ) { 
                    el_etSC->Fill(el.etSC,weight);
                    el_etaSC->Fill(el.etaSC,weight);
                    el_phi->Fill(el.phi,weight);
                    okEl++;
                } 
            } 
        } 

        if( event->TriggerSelection("HLT_PAL3Mu12_v") ) { 
           // fill the tree
           //store electrons and muons in out tree
           int lepIter = 0;
           for(unsigned j=0; j!=passingMuons->size()&&j<2; j++){
              PhysicsMuon mu = passingMuons->at(j).first;
              lepPt_[lepIter] = mu.pt;
              lepPhi_[lepIter] = mu.phi;
              lepEta_[lepIter] = mu.eta;
              lepChg_[lepIter] = mu.charge;
              lepID_[lepIter] = muID;
              lepIso_[lepIter] = (mu.PfChargedHadronIsoR03 + mu.PfNeutralHadronIsoR03 + mu.PfGammaIsoR03)/mu.pt;
              ++lepIter;
           }
           for(unsigned j=0; j!=passingElectrons->size()&&j<2; j++){
              PhysicsElectron el = passingElectrons->at(j).first;
              lepPt_[lepIter] = el.pt;
              lepPhi_[lepIter] = el.phi;
              lepEta_[lepIter] = el.eta;
              lepChg_[lepIter] = el.charge;
              lepID_[lepIter] = eleID;
              lepIso_[lepIter] = el.dr03tkSumPt;
              ++lepIter;
           }
           if(lepIter<2) continue;
           nLep_ = lepIter;

           // emu variables
           if (passingMuons->size()>0&&passingElectrons->size()>0) {
              TLorentzVector tele, tmu, temu;
              PhysicsMuon mu = passingMuons->at(0).first;
              PhysicsElectron el = passingElectrons->at(0).first;
              tele.SetPtEtaPhiM(el.pt,el.eta,el.phi,electron_mass);
              tmu.SetPtEtaPhiM(mu.pt,mu.eta,mu.phi,muon_mass);
              temu = tele+tmu;
              emuPt_ = temu.Pt();
              emuRap_ = temu.Rapidity();
              emuPhi_ = temu.Phi();
              emuMass_ = temu.M();
              emuSgn_ = mu.charge+el.charge;
              emuPhistar_ = Object::phistar(tele,tmu);
              emuDpt_ = fabs(tmu.Pt()-tele.Pt());
              emuDeta_ = fabs(tmu.Eta()-tele.Eta());
              emuDphi_ = fabs(tmu.DeltaPhi(tele));
              emuDR_ = tmu.DeltaR(tele);
           } else {
              emuPt_=-999;
              emuRap_=-999;
              emuPhi_=-999;
              emuMass_=-999;
              emuSgn_=-999;
              emuPhistar_=-999;
              emuDpt_=-999;
              emuDeta_=-999;
              emuDphi_=-999;
              emuDR_=-999;
           }

            if( passingMuons->size() > 0 && passingElectrons->size() > 0 ) ++tryEmu;
            if( emuDY(event->triggerobjects, event->emus, passingElectrons, passingMuons, emu, chi2min) ) {
                double mass = ( emu->first.momentum() + emu->second.momentum() ).M();
                double pt = ( emu->first.momentum() + emu->second.momentum() ).Pt();
                double rapidity = ( emu->first.momentum() + emu->second.momentum() ).Rapidity()-rapshift;
                double phistar = Object::phistar(emu->first.momentum(), emu->second.momentum() );
                okEmu++;
                if( emu->first.charge * emu->second.charge < 0 ) {
                   emu_mass->Fill(mass,weight); 
                   emu_chi2->Fill(chi2min,weight); 
                   if (mass>60&&mass<120) {
                      emu_pt->Fill(pt,weight); 
                      emu_phistar->Fill(phistar,weight); 
                      emu_rap60120->Fill(rapidity,weight); 
                   } else if (mass>15 && mass<60) {
                      emu_rap1560->Fill(rapidity,weight); 
                   }
                } else {
                   emuSS_mass->Fill(mass,weight);
                   emuSS_chi2->Fill(chi2min,weight);
                   if (mass>60&&mass<120) {
                      emuSS_pt->Fill(pt,weight); 
                      emuSS_phistar->Fill(phistar,weight); 
                      emuSS_rap60120->Fill(rapidity,weight); 
                   } else if (mass>15 && mass<60) {
                      emuSS_rap1560->Fill(rapidity,weight); 
                   }
                } // if opp. sign
            } // if emuDY 
            if( dimuonDY(event->triggerobjects, event->dimuons, passingMuons, dimuon, chi2min) ) { 
                double mass = ( dimuon->first.momentum() + dimuon->second.momentum() ).M();
                double pt = ( dimuon->first.momentum() + dimuon->second.momentum() ).Pt();
                double rapidity = ( dimuon->first.momentum() + dimuon->second.momentum() ).Rapidity()-rapshift;
                double phistar = Object::phistar(dimuon->first.momentum(), dimuon->second.momentum() );
                if( (IsDY(index)) && !event->TauSelection() ) continue;
                okDimu++;
                if( dimuon->first.charge * dimuon->second.charge <0 ) {
                   dimu_mass->Fill(mass,weight);
                   dimu_chi2->Fill(chi2min,weight);
                   if (mass>60&&mass<120) {
                      dimu_pt->Fill(pt,weight); 
                      dimu_phistar->Fill(phistar,weight); 
                      dimu_rap60120->Fill(rapidity,weight); 
                   } else if (mass>15 && mass<60) {
                      dimu_rap1560->Fill(rapidity,weight); 
                   }
                } else {
                   dimuSS_mass->Fill(mass,weight);
                   dimuSS_chi2->Fill(chi2min,weight);
                   if (mass>60&&mass<120) {
                      dimuSS_pt->Fill(pt,weight); 
                      dimuSS_phistar->Fill(phistar,weight); 
                      dimuSS_rap60120->Fill(rapidity,weight); 
                   } else if (mass>15 && mass<60) {
                      dimuSS_rap1560->Fill(rapidity,weight); 
                   }
                } // if opp. sign
            } // if dimuonDY
        } // if PAL3Mu12 
        skimTree_p->Fill();
    } // event loop

    //Save 
    f->Write();
    f->Close();

    // clean behind ourselves
    if (f) delete f;
    if (chain) delete chain;
    if (event) delete event;
    if (passingMuons) delete passingMuons;
    if (passingElectrons) delete passingElectrons;
    if (dimuon) delete dimuon;
    if (emu) delete emu;
    // if (el_etSC) delete el_etSC;
    // if (el_etaSC) delete el_etaSC;
    // if (el_phi) delete el_phi;
    // if (emu_mass) delete emu_mass;
    // if (emuSS_mass) delete emuSS_mass;
    // if (dimu_mass) delete dimu_mass;
    // if (dimuSS_mass) delete dimuSS_mass;
    // if (emu_chi2) delete emu_chi2;
    // if (emuSS_chi2) delete emuSS_chi2;
    // if (dimu_chi2) delete dimu_chi2;
    // if (dimuSS_chi2) delete dimuSS_chi2;

    cout<<"# of emu tries = "<<tryEmu<<endl;
    cout<<"# of passing muons = "<<totalMu<<endl;
    cout<<"# of passing electrons = "<<totalEl<<endl;
    cout<<"# of OK electrons = "<<okEl<<endl;
    cout<<"# of OK emu = "<<okEmu<<endl;
    cout<<"# of OK dimu = "<<okDimu<<endl;
    cout<<"Weighted sum = "<<weightedSum<<endl;
    cout<<"Success"<<endl;
}

// produce all plots
void selectEmuEvts_filltree() {
   for (int i=0; i<=QCD; i++) {
      SampleTag tag = static_cast<SampleTag>(i);
      if (tag==ALL) continue;
      cout << Name(tag) << endl;
      selectEmuEvts_filltree(tag);
   }
}
