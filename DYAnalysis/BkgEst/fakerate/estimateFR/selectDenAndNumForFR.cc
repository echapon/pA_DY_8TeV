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
#include "../../interface/analysis.h"
#include "../../interface/defs.h"
#include "../../../HIstuff/HFweight.h"

using namespace std;
using namespace DYana;

void selectDenAndNumForFR(SampleTag index)
{

    // Event & muon
    cout << "index : " << index << "  " << TString(Name(index)) << endl;
	 cout << "Event"<<endl;
    PhysicsEvent* event = new PhysicsEvent();
    vector<PhysicsMuon>* passingMuons = new vector<PhysicsMuon>;

    // the tool for HF reweighting
    HFweight hfTool;

    TFile* f = new TFile("histograms/histFR"+TString(Name(index))+".root","RECREATE");
    TH1D* denominator_pt = new TH1D("denominator_pt","",ptbinnum,ptbin); 
    TH1D* denominator_pt_barrel = new TH1D("denominator_pt_barrel","",ptbinnum,ptbin); 
    TH1D* denominator_pt_endcap = new TH1D("denominator_pt_endcap","",ptbinnum_endcap,ptbin_endcap);  

    TH1D* numerator_pt = new TH1D("numerator_pt","",ptbinnum,ptbin); 
    TH1D* numerator_pt_barrel = new TH1D("numerator_pt_barrel","",ptbinnum,ptbin); 
    TH1D* numerator_pt_endcap = new TH1D("numerator_pt_endcap","",ptbinnum_endcap,ptbin_endcap); 

    TH1D* denominator_eta = new TH1D("denominator_eta","",48,-2.4,2.4); 
    TH1D* numerator_eta = new TH1D("numerator_eta","",48,-2.4,2.4); 

    TH1D* denominator = new TH1D("denominator","",100,0,1);
    TH1D* denominator_barrel = new TH1D("denominator_barrel","",100,0,1);
    TH1D* denominator_endcap = new TH1D("denominator_endcap","",100,0,1);

    TH1D* numerator = new TH1D("numerator","",100,0,1);
    TH1D* numerator_barrel = new TH1D("numerator_barrel","",100,0,1);
    TH1D* numerator_endcap = new TH1D("numerator_endcap","",100,0,1);

    denominator_pt->Sumw2();
    denominator_pt_barrel->Sumw2();
    denominator_pt_endcap->Sumw2();

    numerator_pt->Sumw2();
    numerator_pt_barrel->Sumw2();
    numerator_pt_endcap->Sumw2();

    denominator_eta->Sumw2();
    numerator_eta->Sumw2();

    denominator->Sumw2();
    denominator_barrel->Sumw2();
    denominator_endcap->Sumw2();

    numerator->Sumw2();
    numerator_barrel->Sumw2();
    numerator_endcap->Sumw2();

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
        }

        for(unsigned j=0; j!=passingMuons->size(); j++) {
            cnt[2]++;

            PhysicsMuon mu = passingMuons->at(j);
            pt = mu.pt;
            eta = runsgn(event->run) * mu.eta;
            iso = (mu.PfChargedHadronIsoR03 + mu.PfNeutralHadronIsoR03 + mu.PfGammaIsoR03)/mu.pt;

            if( !mu.highPtMuonID() ) continue;
            cnt[3]++;

            denominator_pt->Fill(pt,wt);
            denominator_eta->Fill(eta,wt);
            denominator->Fill(iso,wt); 

            if( fabs(eta)<1.2 ) {
                denominator_barrel->Fill(iso,wt); 
                denominator_pt_barrel->Fill(pt,wt);
            } 
            else {
                denominator_endcap->Fill(iso,wt); 
                denominator_pt_endcap->Fill(pt,wt);
            } 

            if( !mu.isolation(cuts::isomax) ) continue; 
            cnt[4]++;

            numerator_pt->Fill(pt,wt);
            numerator_eta->Fill(eta,wt);
            numerator->Fill(iso,wt); 

            if( fabs(eta)<1.2 ) {
                numerator_barrel->Fill(iso,wt); 
                numerator_pt_barrel->Fill(pt,wt);
            } 
            else {
                numerator_endcap->Fill(iso,wt); 
                numerator_pt_endcap->Fill(pt,wt); 
            }
        }
    }


    f->Write();
    f->Close();

    cout<<"Success"<<endl;
    cout<<wtsum<<endl;
    for (int i=0; i<10; i++) cout << cnt[i] << " ";
    cout << endl;
}
