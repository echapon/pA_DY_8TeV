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

using namespace std;
using namespace DYana;

void fillHistosForFR(SampleTag index)
{
    // Event & muon
    cout<<"Event"<<endl;
    PhysicsEvent* event = new PhysicsEvent();
    vector<PhysicsMuon>* passingMuons = new vector<PhysicsMuon>;

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
    TH2D* h2d_OS_60120 = new TH2D("h2d_OS_60120","",ptbinnum,ptbin,20,0,1);
    TH2D* h2d_SS = new TH2D("h2d_SS","",ptbinnum,ptbin,20,0,1);

    // Chain
    cout<<"Chain"<<endl;
    bool mc = !IsData(index);
    TChain*       chain = new TChain("kwtuple/physicsTree");
    chain->Add(PathTuple(index));

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
        if(mc) {
            wt = event->weight;
            wt = wt * hfTool.weight(event->hiHFminus,HFweight::minus,true);
            wtsum += wt;
        }
        cnt[0]++;

        if(!event->TriggerSelection(cuts::trig) ) continue;
        cnt[1]++;

        passingMuons->clear();

        for(unsigned j=0; j!=event->muons.size(); j++) {

            PhysicsMuon* mu_ = (PhysicsMuon*)&event->muons.at(j);

            if( mu_->acceptance(cuts::ptmin1,cuts::etamax) ) {
                passingMuons->push_back(*mu_);
            }
        } // loop on muons

        if (passingMuons->size()<2) continue;

        int idx1=-1, idx2=-1;
        float iso1=99., iso2=99.;
        int ch1=0, ch2=0;


        for(unsigned j=0; j!=passingMuons->size(); j++) {
            cnt[2]++;

            PhysicsMuon mu = passingMuons->at(j);
            pt = mu.pt;
            eta = runsgn(event->run) * mu.eta;
            iso = (mu.PfChargedHadronIsoR03 + mu.PfNeutralHadronIsoR03 + mu.PfGammaIsoR03)/mu.pt;
            ch = mu.charge;


            if( !mu.highPtMuonID() ) continue;
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

        // fill the mass histos
        using namespace cuts;
        if (iso1<isomax&&iso2<isomax) {
           if (ch1*ch2<0) hmass_OS_2iso->Fill(mass,wt);
           else hmass_SS_2iso->Fill(mass,wt);
        } else if (iso1<isomax||iso2<isomax) {
           if (ch1*ch2<0) hmass_OS_1iso->Fill(mass,wt);
           else hmass_SS_1iso->Fill(mass,wt);
        } else {
           if (ch1*ch2<0) hmass_OS_0iso->Fill(mass,wt);
           else hmass_SS_0iso->Fill(mass,wt);
        }

        // fill the iso histos
        if (ch1*ch2<0 && mass>60 && mass<120 && iso1<isomax) h2d_OS_60120->Fill(mu2.pt,iso2,wt);
        if (ch1*ch2>0 && iso1<isomax) h2d_SS->Fill(mu2.pt,iso2,wt);
    }


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
