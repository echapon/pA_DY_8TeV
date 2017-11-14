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

void selectDenAndNumForFR_SS(SampleTag index)
{

	// Event & muon
	cout << "index : " << index << "  " << TString(Name(index)) << endl;
	cout << "Event"<<endl;
	PhysicsEvent* event = new PhysicsEvent();
	vector<PhysicsMuon>* passingMuons = new vector<PhysicsMuon>;

	// the tool for HF reweighting
	HFweight hfTool;

	//TFile* f = new TFile("histograms/histFR"+TString(Name(index))+".root","RECREATE");
	TFile* f = new TFile("histograms/histFRQCD"+TString(Name(index))+".root","RECREATE");


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

	// for test, added
	TH1D* denominator_ptunibin = new TH1D("denominator_ptunibin","",97,15,500); 
	TH1D* denominator_ptunibin_barrel = new TH1D("denominator_ptunibin_barrel","",97,15,500); 
	TH1D* denominator_ptunibin_endcap = new TH1D("denominator_ptunibin_endcap","",97,15,500);  

	TH1D* numerator_ptunibin = new TH1D("numerator_ptunibin","",97,15,500); 
	TH1D* numerator_ptunibin_barrel = new TH1D("numerator_ptunibin_barrel","",97,15,500); 
	TH1D* numerator_ptunibin_endcap = new TH1D("numerator_ptunibin_endcap","",97,15,500); 

	denominator_ptunibin->Sumw2();
	denominator_ptunibin_barrel->Sumw2();
	denominator_ptunibin_endcap->Sumw2();

	numerator_ptunibin->Sumw2();
	numerator_ptunibin_barrel->Sumw2();
	numerator_ptunibin_endcap->Sumw2();


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
	int charge = -9;
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

		int inicharge = 0;
		double inipt = -9.0;
		double inieta = -9.0;
		double iniwt = -9.0;
		double iniiso = -9.0;
		int isStored=0;

		for(unsigned j=0; j!=passingMuons->size(); j++) {

			cnt[2]++;

			if (isStored==2) continue;

			PhysicsMuon mu = passingMuons->at(j);
			pt = mu.pt;
			eta = runsgn(event->run) * mu.eta;
			iso = (mu.PfChargedHadronIsoR03 + mu.PfNeutralHadronIsoR03 + mu.PfGammaIsoR03)/mu.pt;
			charge = mu.charge;

			if( !mu.highPtMuonID() ) continue;

			cnt[3]++;
			if (isStored==0) {
				inicharge = charge;				
				inipt = pt;
				inieta = eta;
				iniwt = wt;
				iniiso = iso;
				isStored=1;
			}

			else if (isStored==1 && inicharge==charge) {
				denominator_pt->Fill(inipt,iniwt);
				denominator_eta->Fill(inieta,iniwt);
				denominator->Fill(iniiso,iniwt); 
				denominator_ptunibin->Fill(inipt,iniwt);

				denominator_pt->Fill(pt,wt);
				denominator_eta->Fill(eta,wt);
				denominator->Fill(iso,wt); 
				denominator_ptunibin->Fill(pt,wt);

				if (fabs(inieta)<1.2 && fabs(eta)<1.2) {
					denominator_barrel->Fill(iniiso,iniwt); 
					denominator_pt_barrel->Fill(inipt,iniwt);
					denominator_ptunibin_barrel->Fill(inipt,iniwt);

					denominator_barrel->Fill(iso,wt); 
					denominator_pt_barrel->Fill(pt,wt);
					denominator_ptunibin_barrel->Fill(pt,wt);
				}
				else if (fabs(inieta)>1.2 && fabs(eta)>1.2) {
					denominator_endcap->Fill(iniiso,iniwt); 
					denominator_pt_endcap->Fill(inipt,iniwt);
					denominator_ptunibin_endcap->Fill(inipt,iniwt);

					denominator_endcap->Fill(iso,wt); 
					denominator_pt_endcap->Fill(pt,wt);
					denominator_ptunibin_endcap->Fill(pt,wt);
				}
/*
				if (iniiso<0.15 && iso<0.15) {
					numerator_pt->Fill(inipt,iniwt);
					numerator_eta->Fill(inieta,iniwt);
					numerator->Fill(iniiso,iniwt); 
					numerator_ptunibin->Fill(inipt,iniwt);

					numerator_pt->Fill(pt,wt);
					numerator_eta->Fill(eta,wt);
					numerator->Fill(iso,wt); 
					numerator_ptunibin->Fill(pt,wt);

					if (fabs(inieta)<1.2 && fabs(eta)<1.2) {
						numerator_barrel->Fill(iniiso,iniwt); 
						numerator_pt_barrel->Fill(inipt,iniwt);
						numerator_ptunibin_barrel->Fill(inipt,iniwt);

						numerator_barrel->Fill(iso,wt); 
						numerator_pt_barrel->Fill(pt,wt);
						numerator_ptunibin_barrel->Fill(pt,wt);
					}
					else if (fabs(inieta)>1.2 && fabs(eta)>1.2) {
						numerator_endcap->Fill(iniiso,iniwt); 
						numerator_pt_endcap->Fill(inipt,iniwt);
						numerator_ptunibin_endcap->Fill(inipt,iniwt);

						numerator_endcap->Fill(iso,wt); 
						numerator_pt_endcap->Fill(pt,wt);
						numerator_ptunibin_endcap->Fill(pt,wt);
					}

				}
*/
				if (iniiso<0.15) {
					numerator_pt->Fill(inipt,iniwt);
					numerator_eta->Fill(inieta,iniwt);
					numerator->Fill(iniiso,iniwt); 
					numerator_ptunibin->Fill(inipt,iniwt);
				}
				if (iso<0.15) {
					numerator_pt->Fill(pt,wt);
					numerator_eta->Fill(eta,wt);
					numerator->Fill(iso,wt); 
					numerator_ptunibin->Fill(pt,wt);
				}
				
				
				if (iniiso<0.15 && fabs(inieta)<1.2) {
						numerator_barrel->Fill(iniiso,iniwt); 
						numerator_pt_barrel->Fill(inipt,iniwt);
						numerator_ptunibin_barrel->Fill(inipt,iniwt);
				}
				if (iso<0.15 && fabs(eta)<1.2) {
						numerator_barrel->Fill(iso,wt); 
						numerator_pt_barrel->Fill(pt,wt);
						numerator_ptunibin_barrel->Fill(pt,wt);
					
				}

				if (iniiso<0.15 && fabs(inieta)>1.2) {
						numerator_endcap->Fill(iniiso,iniwt); 
						numerator_pt_endcap->Fill(inipt,iniwt);
						numerator_ptunibin_endcap->Fill(inipt,iniwt);
				}
				if (iso<0.15 && fabs(eta)>1.2) {
						numerator_endcap->Fill(iso,wt); 
						numerator_pt_endcap->Fill(pt,wt);
						numerator_ptunibin_endcap->Fill(pt,wt);
					
				}
				isStored=2;
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
