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

using namespace std;
using namespace DYana;

void selectEmuEvts(SampleTag index)
{
    //Event
    TChain*       chain = new TChain("kwtuple/physicsTree");
    PhysicsEvent* event = new PhysicsEvent();
    bool isData = IsData(index);;
    chain->Add(PathTuple(index));
    chain->SetBranchAddress("event",&event);  

    vector<pair<PhysicsMuon,int>>*     passingMuons     = new vector<pair<PhysicsMuon,int>>;
    vector<pair<PhysicsElectron,int>>* passingElectrons = new vector<pair<PhysicsElectron,int>>;
    pair<PhysicsMuon,PhysicsMuon>*     dimuon           = new pair<PhysicsMuon,PhysicsMuon>;
    pair<PhysicsElectron,PhysicsMuon>* emu              = new pair<PhysicsElectron,PhysicsMuon>;
    double chi2min = 999.;

    //Histogram 
    TFile* f = new TFile("histograms/hist"+TString(Name(index))+".root","RECREATE");
    const int binsize = binnum;

    TH1D* el_etSC     = new TH1D("el_etSC","",97,30,1000); 
    TH1D* el_etaSC    = new TH1D("el_etaSC","",42,-2.1,2.1); 
    TH1D* el_phi      = new TH1D("el_phi","",64,-3.2,3.2); 
    TH1D* emu_mass    = new TH1D("emu_mass","",binsize,bins);
    TH1D* emuSS_mass  = new TH1D("emuSS_mass","",binsize,bins);
    TH1D* dimu_mass   = new TH1D("dimu_mass","",binsize,bins);
    TH1D* dimuSS_mass = new TH1D("dimuSS_mass","",binsize,bins);
    TH1D* emu_chi2    = new TH1D("emu_chi2","",100,0,20);
    TH1D* emuSS_chi2  = new TH1D("emuSS_chi2","",100,0,20);
    TH1D* dimu_chi2   = new TH1D("dimu_chi2","",100,0,20);
    TH1D* dimuSS_chi2 = new TH1D("dimuSS_chi2","",100,0,20);

    el_etSC->Sumw2();
    el_etaSC->Sumw2();
    el_phi->Sumw2();
    emu_mass->Sumw2();
    emuSS_mass->Sumw2();
    dimu_mass->Sumw2();
    dimuSS_mass->Sumw2();
    emu_chi2->Sumw2();
    emuSS_chi2->Sumw2();
    dimu_chi2->Sumw2();
    dimuSS_chi2->Sumw2();

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
    
    for(int i=0; i!=entries; i++) { 
        chain->GetEntry(i);

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
                if(mu->pt > 15) leadingMu = true;
            } 
        } 

        for(unsigned j=0; j!=event->electrons.size(); j++) {

            PhysicsElectron* el = (PhysicsElectron*)&event->electrons.at(j);

            if( EleSel(el) ) { 
                totalEl += weight;
                pair<PhysicsElectron,int> taggedel = {*el,j};
                passingElectrons->push_back(taggedel);
                if(el->pt > 15) leadingEl = true;
            } 
        } 

        if( !leadingMu && !leadingEl ) continue;  // try to change acceptance

        // if( (event->TriggerSelection("HLT_Ele22_eta2p1_WP75_Gsf_v") || event->TriggerSelection("HLT_Ele22_eta2p1_WPLoose_Gsf_v")) && (event->TriggerSelection("HLT_IsoMu20_v") || event->TriggerSelection("HLT_IsoTkMu20_v")) ) { //Single electron spectra
        if( // event->TriggerSelection("HLT_PAEle20_WPLoose_Gsf_v") && 
              event->TriggerSelection("HLT_PAL3Mu12_v") ) { //Single electron spectra
            for(unsigned j=0; j!=passingElectrons->size(); j++) { 
                PhysicsElectron el = passingElectrons->at(j).first;
                if( el.acceptance(15,2.1) ) { 
                    el_etSC->Fill(el.etSC,weight);
                    el_etaSC->Fill(el.etaSC,weight);
                    el_phi->Fill(el.phi,weight);
                    okEl++;
                } 
            } 
        } 

        if( event->TriggerSelection("HLT_PAL3Mu12_v") ) { 
            if( passingMuons->size() > 0 && passingElectrons->size() > 0 ) ++tryEmu;
            if( emuDY(event->triggerobjects, event->emus, passingElectrons, passingMuons, emu, chi2min) ) {
                double mass = ( emu->first.momentum() + emu->second.momentum() ).M();
                okEmu++;
                if( emu->first.charge * emu->second.charge < 0 ) {
                   emu_mass->Fill(mass,weight); 
                   emu_chi2->Fill(chi2min,weight); 
                } else {
                   emuSS_mass->Fill(mass,weight);
                   emuSS_chi2->Fill(chi2min,weight);
                }
            } 
            if( dimuonDY(event->triggerobjects, event->dimuons, passingMuons, dimuon, chi2min) ) { 
                double mass = ( dimuon->first.momentum() + dimuon->second.momentum() ).M();
                if( (IsDY(index)) && !event->TauSelection() ) continue;
                okDimu++;
                if( dimuon->first.charge * dimuon->second.charge <0 ) {
                   dimu_mass->Fill(mass,weight);
                   dimu_chi2->Fill(chi2min,weight);
                } else {
                   dimuSS_mass->Fill(mass,weight);
                   dimuSS_chi2->Fill(chi2min,weight);
                }
            } 
        } 
    }

    //Save 
    f->Write();
    f->Close();

    cout << __LINE__ << endl;
    // clean behind ourselves
    if (f) delete f;
    cout << __LINE__ << endl;
    if (chain) delete chain;
    cout << __LINE__ << endl;
    if (event) delete event;
    cout << __LINE__ << endl;
    if (passingMuons) delete passingMuons;
    cout << __LINE__ << endl;
    if (passingElectrons) delete passingElectrons;
    cout << __LINE__ << endl;
    if (dimuon) delete dimuon;
    cout << __LINE__ << endl;
    if (emu) delete emu;
    cout << __LINE__ << endl;
    cout << el_etSC << endl;
    // if (el_etSC) delete el_etSC;
    // cout << __LINE__ << endl;
    // if (el_etaSC) delete el_etaSC;
    // cout << __LINE__ << endl;
    // if (el_phi) delete el_phi;
    // cout << __LINE__ << endl;
    // if (emu_mass) delete emu_mass;
    // cout << __LINE__ << endl;
    // if (emuSS_mass) delete emuSS_mass;
    // cout << __LINE__ << endl;
    // if (dimu_mass) delete dimu_mass;
    // cout << __LINE__ << endl;
    // if (dimuSS_mass) delete dimuSS_mass;
    // cout << __LINE__ << endl;
    // if (emu_chi2) delete emu_chi2;
    // cout << __LINE__ << endl;
    // if (emuSS_chi2) delete emuSS_chi2;
    // cout << __LINE__ << endl;
    // if (dimu_chi2) delete dimu_chi2;
    // cout << __LINE__ << endl;
    // if (dimuSS_chi2) delete dimuSS_chi2;
    // cout << __LINE__ << endl;

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
void selectEmuEvts() {
   for (int i=0; i<=QCD; i++) {
      SampleTag tag = static_cast<SampleTag>(i);
      if (tag==ALL) continue;
      cout << Name(tag) << endl;
      selectEmuEvts(tag);
   }
}
