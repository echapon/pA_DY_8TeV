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
//using namespace DYana_v20180814;

double isomax_;

void selectDenAndNumForFR_opttest(SampleTag index=TT, TString opt="nominal", TString MuonIDopt="")
{

	// Event & muon
	cout << "########### OPTION : " << opt.Data() << std::endl;
	cout << "index : " << index << "  " << TString(Name(index)) << endl;
	cout << "Event"<<endl;
	PhysicsEvent* event = new PhysicsEvent();
	vector<PhysicsMuon>* passingMuons = new vector<PhysicsMuon>;

	// the tool for HF reweighting
	HFweight hfTool;

	//TFile* f = new TFile("histograms/histFRiso0p10"+TString(Name(index))+".root","RECREATE");
	//TFile* f = new TFile(Form("histograms_test/histFR_%s_%s%s.root",opt.Data(),Name(index),MuonIDopt.Data()),"RECREATE");
	TFile* f = new TFile(Form("histograms_20180814/histFR_%s_%s%s.root",opt.Data(),Name(index),MuonIDopt.Data()),"RECREATE");



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

	// applied conditions
	if (opt=="iso0p10") isomax_=0.10;
	else if (opt=="iso0p20") isomax_=0.20;
	else isomax_=cuts::isomax; //0.15
	std::cout << "---------- option 1 - isomax : " << isomax_ << std::endl;

	if (opt=="ZVETO_SMUwJET_MuPtlt10" || opt=="ZVETO_SMUwoJET_MuPtlt10" || opt=="ZVETO_SMUwJET_MuPtlt15") {

		std::cout << "---------- option 2 - (Z Veto related) apply HLT_PAL3Mu12_v trigger" << std::endl;
		if (opt=="ZVETO_SMUwJET_MuPtlt10" || opt=="ZVETO_SMUwJET_MuPtlt15") 
			std::cout << "---------- option 3 - (Z Veto related) apply jet conditions? : YES" << std::endl;
		else if (opt=="ZVETO_SMUwoJET_MuPtlt10") std::cout << "---------- option 3 - (Z Veto related) apply jet conditions? : NO" << std::endl;

		std::cout << "---------- option 4 - (Z Veto related) apply MET.pt < 20.0 GeV" << std::endl;
		std::cout << "---------- option 5 - (Z Veto related) only one muon in one event" << std::endl;
	}

	if (opt=="nominal" || opt=="ZVETO_SMUwJET_MuPtlt10" || opt=="ZVETO_SMUwoJET_MuPtlt10" || opt=="ZVETO_SMUwJET_MuPtlt15" || opt=="iso0p10" || opt=="iso0p20") {
		std::cout << "---------- option 00 - (Z Veto related) Nominal conditions ###############" << std::endl;
		if (opt=="ZVETO_SMUwJET_MuPtlt10" || opt=="ZVETO_SMUwoJET_MuPtlt10"){
			std::cout << "---------- option 6 - (Z Veto related) single muon p_{T} > 10 GeV" << std::endl;
		}
		else if (opt=="ZVETO_SMUwJET_MuPtlt15") {
			std::cout << "---------- option 6 - (Z Veto related) single muon p_{T} > 15 GeV" << std::endl;
		}

	}
	else if (opt=="SS") {// same sign
		std::cout << "---------- option 7 - collect same sign (SS) single muons" << std::endl;
	}	

	if (opt=="new") std::cout << "NEW selection" << std::endl;
	if (MuonIDopt=="_highPtMuonID") std::cout << "MuonIDopt(1) : " << MuonIDopt.Data() << std::endl;
	else if (MuonIDopt=="_tightMuonIDSub") std::cout << "MuonIDopt(2) : " << MuonIDopt.Data() << std::endl;
	else if (MuonIDopt=="_noMuonID") std::cout << "MuonIDopt(3) : " << MuonIDopt.Data() << std::endl;
	else if (MuonIDopt=="") std::cout << "MuonIDopt(0) : " << MuonIDopt.Data() << std::endl;
	else if (MuonIDopt=="_tighthighPtMuonIDCommon") std::cout << "MuonIDopt(4) : " << MuonIDopt.Data() << std::endl;

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
	double trkiso = 0.0;
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
			//if (cnt[1]%10000==0) std::cout << "pt : " << mu_->pt << " , eta : " << mu_->eta <<std::endl;
			if( mu_->acceptance(cuts::ptmin1,cuts::etamax) ) {
				passingMuons->push_back(*mu_);
			}
		}

		if (opt=="iso0p10") isomax_=0.10;
		else if (opt=="iso0p20") isomax_=0.20;
		else isomax_=cuts::isomax; //0.15
		//std::cout << "---------- option 1 - isomax : " << isomax_ << std::endl;

		if (opt=="ZVETO_SMUwJET_MuPtlt10" || opt=="ZVETO_SMUwoJET_MuPtlt10" || opt=="ZVETO_SMUwJET_MuPtlt15") {

			//std::cout << "---------- option 2 - (Z Veto related) apply HLT_PAL3Mu12_v trigger" << std::endl;
			if (!event->TriggerSelection("HLT_PAL3Mu12_v")) continue;

			bool passjets=false;
			//bool applyjets=false;
			if (opt=="ZVETO_SMUwJET_MuPtlt10" || opt=="ZVETO_SMUwJET_MuPtlt15") {
				//applyjets=true;
				for(unsigned j=0; j!=event->jets.size(); j++) {
					if (!(fabs(event->jets.at(j).eta)<2.1 && event->jets.at(j).pt>25)) continue;
					passjets=true;
				}
			}
			else passjets=true;
			//std::cout << "---------- option 3 - (Z Veto related) apply jet conditions? : " << applyjets << std::endl;

			bool passMET=false;
			for(unsigned j=0; j!=event->MET.size(); j++) {
				if (!(event->MET.at(j).pt<20.0)) continue;
				passMET=true;
			}
			//std::cout << "---------- option 4 - (Z Veto related) apply MET.pt < 20.0 GeV" << std::endl;

			if (!(passjets && passMET)) continue;

			//std::cout << "---------- option 5 - (Z Veto related) only one muon in one event" << std::endl;
			if (passingMuons->size()>1) continue;
		}
		if (opt=="nominal" || opt=="ZVETO_SMUwJET_MuPtlt10" || opt=="ZVETO_SMUwoJET_MuPtlt10" || opt=="ZVETO_SMUwJET_MuPtlt15" || opt=="iso0p10" || opt=="iso0p20" || opt=="new") {
			//std::cout << "---------- option 00 - (Z Veto related) Nominal conditions ###############" << std::endl;

			//if (cnt[1]%10000==0) std::cout << "########### PASSING MUON : " << passingMuons->size() << std::endl;
			for(unsigned j=0; j!=passingMuons->size(); j++) {
				cnt[2]++;

				PhysicsMuon mu = passingMuons->at(j);
				pt = mu.pt;
				eta = runsgn(event->run) * mu.eta;
				iso = (mu.PfChargedHadronIsoR03 + mu.PfNeutralHadronIsoR03 + mu.PfGammaIsoR03)/mu.pt;
				trkiso = mu.isolationR03_sumpt/mu.pt;
				charge = mu.charge;

				//if( !mu.highPtMuonID() ) continue;

				if (MuonIDopt=="_highPtMuonID") {
					if (!mu.highPtMuonID()) continue;
				}
				else if (MuonIDopt=="_tightMuonIDSub") {
					if (!mu.tightMuonIDSub()) continue;
				}
				else if (MuonIDopt=="") {
					if (!mu.tightMuonID()) continue;
				}
				else if (MuonIDopt=="_tighthighPtMuonIDCommon") {
					if (!(mu.tightMuonID() && mu.highPtMuonID())) continue;
				}               

				if (opt=="ZVETO_SMUwJET_MuPtlt10" || opt=="ZVETO_SMUwoJET_MuPtlt10"){ 
					//std::cout << "---------- option 6 - (Z Veto related) single muon p_{T} > 10 GeV" << std::endl;
					if (!(pt>10.0)) continue;
				}
				else if (opt=="ZVETO_SMUwJET_MuPtlt15") {
					//std::cout << "---------- option 6 - (Z Veto related) single muon p_{T} > 15 GeV" << std::endl;
					if (!(pt>15.0)) continue;
				}

				cnt[3]++;
				if (opt=="new") {
					denominator_pt->Fill(pt,wt);
					denominator_eta->Fill(eta,wt);
					denominator->Fill(trkiso,wt); 
					denominator_ptunibin->Fill(pt,wt);

					if( fabs(eta)<1.2 ) {
						denominator_barrel->Fill(trkiso,wt); 
						denominator_pt_barrel->Fill(pt,wt);
						denominator_ptunibin_barrel->Fill(pt,wt);
					} 
					else if (fabs(eta)>1.2 && fabs(eta)<2.4){
						denominator_endcap->Fill(trkiso,wt); 
						denominator_pt_endcap->Fill(pt,wt);
						denominator_ptunibin_endcap->Fill(pt,wt);
					} 
				}

				else {
					denominator_pt->Fill(pt,wt);
					denominator_eta->Fill(eta,wt);
					denominator->Fill(iso,wt); 
					denominator_ptunibin->Fill(pt,wt);

					if( fabs(eta)<1.2 ) {
						denominator_barrel->Fill(iso,wt); 
						denominator_pt_barrel->Fill(pt,wt);
						denominator_ptunibin_barrel->Fill(pt,wt);
					} 
					else if (fabs(eta)>1.2 && fabs(eta)<2.4){
						denominator_endcap->Fill(iso,wt); 
						denominator_pt_endcap->Fill(pt,wt);
						denominator_ptunibin_endcap->Fill(pt,wt);
					} 

				}
				//###if( !mu.isolation(cuts::isomax) ) continue; 
				if (opt=="new") {if(!(trkiso<0.3)) continue;}
				else if( !mu.isolation(isomax_) ) continue; 
				if (!mu.tightMuonID()) continue;
				cnt[4]++;
				if (opt=="new") {

					numerator_pt->Fill(pt,wt);
					numerator_eta->Fill(eta,wt);
					numerator->Fill(iso,wt); 
					numerator_ptunibin->Fill(pt,wt);

					if( fabs(eta)<1.2 ) {
						numerator_barrel->Fill(iso,wt); 
						numerator_pt_barrel->Fill(pt,wt);
						numerator_ptunibin_barrel->Fill(pt,wt);
					} 
					else if (fabs(eta)>1.2 && fabs(eta)<2.4){
						numerator_endcap->Fill(iso,wt); 
						numerator_pt_endcap->Fill(pt,wt);
						numerator_ptunibin_endcap->Fill(pt,wt);
					}
				}
				else {
					numerator_pt->Fill(pt,wt);
					numerator_eta->Fill(eta,wt);
					numerator->Fill(trkiso,wt); 
					numerator_ptunibin->Fill(pt,wt);

					if( fabs(eta)<1.2 ) {
						numerator_barrel->Fill(trkiso,wt); 
						numerator_pt_barrel->Fill(pt,wt);
						numerator_ptunibin_barrel->Fill(pt,wt);
					} 
					else if (fabs(eta)>1.2 && fabs(eta)<2.4){
						numerator_endcap->Fill(trkiso,wt); 
						numerator_pt_endcap->Fill(pt,wt);
						numerator_ptunibin_endcap->Fill(pt,wt);
					}

				}
			}
		}// nominal
		else if (opt=="SS") {// same sign
			//std::cout << "---------- option 7 - collect same sign (SS) single muons" << std::endl;
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

				//				if( !mu.highPtMuonID() ) continue;

				if (MuonIDopt=="_highPtMuonID") {
					if (!mu.highPtMuonID()) continue;
				}
				else if (MuonIDopt=="_tightMuonIDSub") {
					if (!mu.tightMuonIDSub()) continue;
				}
				else if (MuonIDopt=="") {
					if (!mu.tightMuonID()) continue;
				}
				else if (MuonIDopt=="_tighthighPtMuonIDCommon") {
					if (!(mu.tightMuonID() && mu.highPtMuonID())) continue;
				}               


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

					if (iniiso<isomax_) {
						numerator_pt->Fill(inipt,iniwt);
						numerator_eta->Fill(inieta,iniwt);
						numerator->Fill(iniiso,iniwt);
						numerator_ptunibin->Fill(inipt,iniwt);
					}
					if (iso<isomax_) {
						numerator_pt->Fill(pt,wt);
						numerator_eta->Fill(eta,wt);
						numerator->Fill(iso,wt);
						numerator_ptunibin->Fill(pt,wt);
					}

					if (fabs(inieta)<1.2) {
						denominator_barrel->Fill(iniiso,iniwt);
						denominator_pt_barrel->Fill(inipt,iniwt);
						denominator_ptunibin_barrel->Fill(inipt,iniwt);
						if (iniiso<isomax_) {
							numerator_barrel->Fill(iniiso,iniwt);
							numerator_pt_barrel->Fill(inipt,iniwt);
							numerator_ptunibin_barrel->Fill(inipt,iniwt);
						}
					}
					else if (fabs(inieta)>1.2 && fabs(inieta)<2.4) {
						denominator_endcap->Fill(iniiso,iniwt);
						denominator_pt_endcap->Fill(inipt,iniwt);
						denominator_ptunibin_endcap->Fill(inipt,iniwt);
						if (iniiso<isomax_) {
							numerator_endcap->Fill(iniiso,iniwt);
							numerator_pt_endcap->Fill(inipt,iniwt);
							numerator_ptunibin_endcap->Fill(inipt,iniwt);
						}
					}

					if (fabs(eta)<1.2) {
						denominator_barrel->Fill(iso,wt);
						denominator_pt_barrel->Fill(pt,wt);
						denominator_ptunibin_barrel->Fill(pt,wt);
						if (iso<isomax_) {
							numerator_barrel->Fill(iso,wt);
							numerator_pt_barrel->Fill(pt,wt);
							numerator_ptunibin_barrel->Fill(pt,wt);
						}
					}
					else if (fabs(eta)>1.2 && fabs(eta)<2.4) {
						denominator_endcap->Fill(iso,wt);
						denominator_pt_endcap->Fill(pt,wt);
						denominator_ptunibin_endcap->Fill(pt,wt);
						if (iso<isomax_) {
							numerator_endcap->Fill(iso,wt);
							numerator_pt_endcap->Fill(pt,wt);
							numerator_ptunibin_endcap->Fill(pt,wt);
						}
					}


					isStored=2;
				}
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
