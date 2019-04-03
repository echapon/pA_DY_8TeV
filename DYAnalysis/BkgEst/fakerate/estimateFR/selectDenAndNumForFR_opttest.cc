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

//void selectDenAndNumForFR_opttest_v20190211_r2(SampleTag index=TT, TString opt="nominal", TString MuonIDopt="", TString trigopt="")
//void selectDenAndNumForFR_opttest_v20190227(TString outdn="", int niso=20, double isomin=0.0, double isomax=0.2, SampleTag index=DYMuMu1030, TString opt="relPFisoR03", TString MuonIDopt="", TString trigopt="_L3Mu12")
//###void selectDenAndNumForFR_opttest_v20190327v10_part1(TString outdn="20190306v9_iso0_0p2_HFminus", int niso=20, double isomin=0.0, double isomax=0.2, int index1=TT, int index2=TbarW, TString opt="reltrkisoR03muptlt10", TString MuonIDopt="", TString trigopt="_L3Mu12")
void selectDenAndNumForFR_opttest(TString outdn="20190306v9_iso0_0p2_HFminus", int niso=20, double isomin=0.0, double isomax=0.2, int index1=TT, int index2=TbarW, TString opt="reltrkisoR03muptlt10isomax0p5", TString MuonIDopt="", TString trigopt="_L3Mu12")


//("20190306v9_iso0_0p2_HFminus", 20, 0.0, 0.2, TT, DYMuMu30_PbP, "reltrkisoR03isolt0p2", "", "_L3Mu12")
//root -l -b -q 'selectDenAndNumForFR_opttest_v20190211_r2.cc++(DYMuMu1030,"relPFisoR03","","_L3Mu12")
{

	for (int ind=index1; ind<=index2; ind++) {
		if (ind==ALL) continue;
		SampleTag index= static_cast<SampleTag>(ind);
		// Event & muon
		cout << "########### OPTION : " << opt.Data() << std::endl;
		cout << "index : " << index << "  " << TString(Name(index)) << endl;
		cout << "Event"<<endl;
		PhysicsEvent* event = new PhysicsEvent();
		vector<PhysicsMuon>* passingMuons = new vector<PhysicsMuon>;

		float hiHFminus_c2;
		// the tool for HF reweighting
		HFweight hfTool;

		//TFile* f = new TFile("histograms/histFRiso0p10"+TString(Name(index))+".root","RECREATE");
		//TFile* f = new TFile(Form("histograms_test/histFR_%s_%s%s.root",opt.Data(),Name(index),MuonIDopt.Data()),"RECREATE");
		//###TFile* f = new TFile(Form("histograms_20190215v3/histFR_%s_%s%s%s.root",opt.Data(),Name(index),MuonIDopt.Data(),trigopt.Data()),"RECREATE");
		TFile* f = new TFile(Form("histograms_%s/histFR_%s_%s%s%s.root",outdn.Data(),opt.Data(),Name(index),MuonIDopt.Data(),trigopt.Data()),"RECREATE");



		TH1D* denominator_pt = new TH1D("denominator_pt","",ptbinnum,ptbin); 
		TH1D* denominator_pt_barrel = new TH1D("denominator_pt_barrel","",ptbinnum,ptbin); 
		TH1D* denominator_pt_endcap = new TH1D("denominator_pt_endcap","",ptbinnum_endcap,ptbin_endcap);  

		TH1D* numerator_pt = new TH1D("numerator_pt","",ptbinnum,ptbin); 
		TH1D* numerator_pt_barrel = new TH1D("numerator_pt_barrel","",ptbinnum,ptbin); 
		TH1D* numerator_pt_endcap = new TH1D("numerator_pt_endcap","",ptbinnum_endcap,ptbin_endcap); 

		TH1D* denominator_eta = new TH1D("denominator_eta","",48,-2.4,2.4); 
		TH1D* numerator_eta = new TH1D("numerator_eta","",48,-2.4,2.4); 

		//	TH1D* denominator = new TH1D("denominator","",100,0,1);
		TH1D* denominator = new TH1D("denominator","",niso,isomin,isomax);
		TH1D* denominator_barrel = new TH1D("denominator_barrel","",niso,isomin,isomax);
		TH1D* denominator_endcap = new TH1D("denominator_endcap","",niso,isomin,isomax);

		//	TH1D* numerator = new TH1D("numerator","",100,0,1);
		TH1D* numerator = new TH1D("numerator","",niso,isomin,isomax);
		TH1D* numerator_barrel = new TH1D("numerator_barrel","",niso,isomin,isomax);
		TH1D* numerator_endcap = new TH1D("numerator_endcap","",niso,isomin,isomax);

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
		else if (opt=="relPFisoR03" || opt=="relPFisoR04" || opt=="SSrelPFisoR03" || opt=="SSrelPFisoR04") isomax_=0.15;
		else if (opt=="reltrkisoR03" || opt=="reltrkisoR04" || opt=="SSreltrkisoR04") isomax_=0.20;
		else if (opt=="reltrkisoR03muptlt10" || opt=="SSreltrkisoR03muptlt10") isomax_=0.20;
	   else if (opt=="reltrkisoR03muptlt10isomax0p5" || opt=="SSreltrkisoR03muptlt10isomax0p5") isomax_=0.50;
	   else if (opt=="reltrkisoR03muptlt10isomax0p4" || opt=="SSreltrkisoR03muptlt10isomax0p4") isomax_=0.40;
	   else if (opt=="reltrkisoR03muptlt10isomax0p3" || opt=="SSreltrkisoR03muptlt10isomax0p3") isomax_=0.30;
	   else if (opt=="reltrkisoR03muptlt10isomax0p1" || opt=="SSreltrkisoR03muptlt10isomax0p1") isomax_=0.10;
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
		else if (opt=="SS" || opt=="SSreltrkisoR03" || opt=="SSrelPFisoR03" || opt=="SSrelPFisoR04" || opt=="SSreltrkisoR03muptlt10" || opt=="SSreltrkisoR03muptlt10isomax0p5" || opt=="SSreltrkisoR03muptlt10isomax0p4" || opt=="SSreltrkisoR03muptlt10isomax0p3" || opt=="SSreltrkisoR03muptlt10isomax0p1") {// same sign
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
		TChain*  chain2 = new TChain("recoTree/DYTree");
		if (index==Data1 || index==Data2) {
			if (trigopt=="_L1DoubleMu0") {
				chain->Add(PathDimuTuple(index));
				chain2->Add(PathDimuTuple(index));
			}
			else {
				chain->Add(PathTuple(index));
				chain2->Add(PathTuple(index));


			}	
		}    else {
			chain->Add(PathTuple(index));
			chain2->Add(PathTuple(index));
		} 
		chain->SetBranchAddress("event",&event);
		cout<<"# of events = "<<chain->GetEntries()<<endl;

		chain2->SetBranchAddress("hiHFminus",&hiHFminus_c2);
		cout<<"# of events (chain2) = "<<chain2->GetEntries()<<endl;


		double pt = 0;
		double eta = 0;
		double iso = 0;
		double trkiso = 0.0;
		int charge = -9;
		double wt = 1.0;
		double wtsum = 0;
		int entries = chain->GetEntries();
		int cnt[10]={0};

		chain->AddFriend(chain2);
		for(int i=0; i!=entries; i++) {
			//for(int i=0; i!=100; i++) {
			if ((i%1000000==0) || (i==entries-1)) std::cout << "(" << i+1 << "/" << entries << ") started" << std::endl;

			chain->GetEntry(i);
			chain2->GetEntry(i);
			if(mc) {
				wt = event->weight;
				//			wt = wt * hfTool.weight(event->hiHFminus,HFweight::minus,true);
				wt = wt * hfTool.weight(hiHFminus_c2,HFweight::minus,true);
				if (i==0) {
					std::cout << i << " - wt ( " << event->weight << " * " << hfTool.weight(event->hiHFminus,HFweight::minus,true) << " ) : " << wt << " --- event->hiHFminus : " << event->hiHFminus << std::endl;
					std::cout << i << " ---- wt ( " << event->weight << " * " << hfTool.weight(hiHFminus_c2,HFweight::minus,true) << " ) : " << wt << " --- hiHFminus_c2 : " << hiHFminus_c2 << std::endl;
				}

				wtsum += wt;
			}

			cnt[0]++;

			if (trigopt=="_L1DoubleMu0") {
				//			std::cout << "*** Trigger selection : " << "HLT_PAL1DoubleMu0_v" << std::endl;
				if (!event->TriggerSelection("HLT_PAL1DoubleMu0_v")) continue;
			}
			else if (trigopt=="_L3Mu12") {
				//			std::cout << "*** Trigger selection : " << "HLT_PAL3Mu12_v" << std::endl;
				if (!event->TriggerSelection("HLT_PAL3Mu12_v")) continue;
			}
			else if (trigopt=="_checkAND") {
				if(!(event->TriggerSelection("HLT_PAL3Mu12_v") && event->TriggerSelection("HLT_PAL1DoubleMu0_v"))) continue;
			}
			else if (trigopt=="_checkOR") {
				if(!(event->TriggerSelection("HLT_PAL3Mu12_v") || event->TriggerSelection("HLT_PAL1DoubleMu0_v"))) continue;
			}
			else if (trigopt=="_L3Mu5") {
				if (!event->TriggerSelection("HLT_PAL3Mu5_v")) continue;
			}
			else if (trigopt=="_L3Mu7") {
				if (!event->TriggerSelection("HLT_PAL3Mu7_v")) continue;
			}
			else {
				//			std::cout << "*** Trigger selection : " << cuts::trig << std::endl;
				if(!event->TriggerSelection(cuts::trig) ) continue;
			}
			cnt[1]++;

			passingMuons->clear();

			//std::cout << "muon size : " << event->muons.size() << std::endl;
			for(unsigned j=0; j!=event->muons.size(); j++) {

				PhysicsMuon* mu_ = (PhysicsMuon*)&event->muons.at(j);
				//if (cnt[1]%10000==0) std::cout << "pt : " << mu_->pt << " , eta : " << mu_->eta <<std::endl;
				if (opt=="reltrkisoR03muptlt10" || opt=="SSreltrkisoR03muptlt10" || opt=="reltrkisoR03muptlt10isomax0p5" || opt=="SSreltrkisoR03muptlt10isomax0p5" || opt=="reltrkisoR03muptlt10isomax0p4" || opt=="SSreltrkisoR03muptlt10isomax0p4" || opt=="reltrkisoR03muptlt10isomax0p3" || opt=="SSreltrkisoR03muptlt10isomax0p3" || opt=="reltrkisoR03muptlt10isomax0p1" || opt=="SSreltrkisoR03muptlt10isomax0p1") {
					if( mu_->acceptance(cuts::ptmin3,cuts::etamax) ) {
					passingMuons->push_back(*mu_);
				}

				}
				else if( mu_->acceptance(cuts::ptmin2,cuts::etamax) ) {
					passingMuons->push_back(*mu_);
				}
			}

			// applied conditions
			if (opt=="iso0p10") isomax_=0.10;
			else if (opt=="iso0p20") isomax_=0.20;
			else if (opt=="relPFisoR03" || opt=="relPFisoR04" || opt=="SSrelPFisoR03" || opt=="SSrelPFisoR04") isomax_=0.15;
			else if (opt=="reltrkisoR03" || opt=="reltrkisoR04" || opt=="SSreltrkisoR03") isomax_=0.20;
			else if (opt=="reltrkisoR03muptlt10" || opt=="SSreltrkisoR03muptlt10") isomax_=0.20;
		   else if (opt=="reltrkisoR03muptlt10isomax0p5" || opt=="SSreltrkisoR03muptlt10isomax0p5") isomax_=0.50;
		   else if (opt=="reltrkisoR03muptlt10isomax0p4" || opt=="SSreltrkisoR03muptlt10isomax0p4") isomax_=0.40;
	   	else if (opt=="reltrkisoR03muptlt10isomax0p3" || opt=="SSreltrkisoR03muptlt10isomax0p3") isomax_=0.30;
	   	else if (opt=="reltrkisoR03muptlt10isomax0p1" || opt=="SSreltrkisoR03muptlt10isomax0p1") isomax_=0.10;
			else isomax_=cuts::isomax; //0.15


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
			if (opt=="nominal" || opt=="ZVETO_SMUwJET_MuPtlt10" || opt=="ZVETO_SMUwoJET_MuPtlt10" || opt=="ZVETO_SMUwJET_MuPtlt15" || opt=="iso0p10" || opt=="iso0p20" || opt=="new" || opt=="reltrkisoR03" || opt=="relPFisoR03" || opt=="relPFisoR04" || opt=="reltrkisoR03muptlt10" || opt=="reltrkisoR03muptlt10isomax0p5" || opt=="reltrkisoR03muptlt10isomax0p4" || opt=="reltrkisoR03muptlt10isomax0p3" || opt=="reltrkisoR03muptlt10isomax0p1") {
				//std::cout << "---------- option 00 - (Z Veto related) Nominal conditions ###############" << std::endl;

				//if (cnt[1]%10000==0) std::cout << "########### PASSING MUON : " << passingMuons->size() << std::endl;
				for(unsigned j=0; j!=passingMuons->size(); j++) {
					cnt[2]++;

					PhysicsMuon mu = passingMuons->at(j);
					pt = mu.pt;
					eta = runsgn(event->run) * mu.eta;
					if (opt=="reltrkisoR03"  || opt=="reltrkisoR03muptlt10" || opt=="reltrkisoR03muptlt10isomax0p5" || opt=="reltrkisoR03muptlt10isomax0p4" || opt=="reltrkisoR03muptlt10isomax0p3" || opt=="reltrkisoR03muptlt10isomax0p1") iso = mu.isolationR03_sumpt/mu.pt;
					//else if (opt=="reltrkisoR04") iso = mu.isolationR04_sumpt/mu.pt;
					else if (opt=="relPFisoR03") iso = (mu.PfChargedHadronIsoR03 + mu.PfNeutralHadronIsoR03 + mu.PfGammaIsoR03)/mu.pt;
					else if (opt=="relPFisoR04") iso = (mu.PfChargedHadronIsoR04 + mu.PfNeutralHadronIsoR04 + mu.PfGammaIsoR04)/mu.pt;
					else iso = (mu.PfChargedHadronIsoR03 + mu.PfNeutralHadronIsoR03 + mu.PfGammaIsoR03)/mu.pt;
					//iso = (mu.PfChargedHadronIsoR03 + mu.PfNeutralHadronIsoR03 + mu.PfGammaIsoR03)/mu.pt;
					//trkiso = mu.isolationR03_sumpt/mu.pt;
					charge = mu.charge;

					//if( !mu.highPtMuonID() ) continue;

					if (MuonIDopt=="_highPtMuonID") {
						if (!mu.highPtMuonID()) continue;
					}
					//				else if (MuonIDopt=="_tightMuonIDSub") {
					//					if (!mu.tightMuonIDSub()) continue;
					//				}
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


					//###if( !mu.isolation(cuts::isomax) ) continue; 
					if( !mu.isolation(isomax_) ) continue; 
					cnt[4]++;

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
			}// nominal
			else if (opt=="SS" || opt=="SSreltrkisoR03" || opt=="SSrelPFisoR03" || opt=="SSrelPFisoR04" || opt=="SSreltrkisoR03muptlt10" || opt=="SSreltrkisoR03muptlt10isomax0p5" || opt=="SSreltrkisoR03muptlt10isomax0p4" || opt=="SSreltrkisoR03muptlt10isomax0p3" || opt=="SSreltrkisoR03muptlt10isomax0p1") {// same sign
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
					if (opt=="SSreltrkisoR03" || opt=="SSreltrkisoR03muptlt10" || opt=="SSreltrkisoR03muptlt10isomax0p5" || opt=="SSreltrkisoR03muptlt10isomax0p4" || opt=="SSreltrkisoR03muptlt10isomax0p3" || opt=="SSreltrkisoR03muptlt10isomax0p1") iso = mu.isolationR03_sumpt/mu.pt;
					//else if (opt=="reltrkisoR04") iso = mu.isolationR04_sumpt/mu.pt;
					else if (opt=="SSrelPFisoR03") iso = (mu.PfChargedHadronIsoR03 + mu.PfNeutralHadronIsoR03 + mu.PfGammaIsoR03)/mu.pt;
					else if (opt=="SSrelPFisoR04") iso = (mu.PfChargedHadronIsoR04 + mu.PfNeutralHadronIsoR04 + mu.PfGammaIsoR04)/mu.pt;
					else iso = (mu.PfChargedHadronIsoR03 + mu.PfNeutralHadronIsoR03 + mu.PfGammaIsoR03)/mu.pt;
					//iso = (mu.PfChargedHadronIsoR03 + mu.PfNeutralHadronIsoR03 + mu.PfGammaIsoR03)/mu.pt;
					//trkiso = mu.isolationR03_sumpt/mu.pt;
					charge = mu.charge;

					//				if( !mu.highPtMuonID() ) continue;

					if (MuonIDopt=="_highPtMuonID") {
						if (!mu.highPtMuonID()) continue;
					}
					//				else if (MuonIDopt=="_tightMuonIDSub") {
					//					if (!mu.tightMuonIDSub()) continue;
					//				}
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
		for (int i=0; i<10; i++) cout << i << " : " << cnt[i] << " ";
		cout << endl;
		}
	}
/*
int main(int argc, char *argv[])
{
	if(argc==10)
	{
		selectDenAndNumForFR_opttest_v20190227(argv[1], atoi(argv[2]), atof(argv[3]), atof(argv[4]), atoi(argv[5]), atoi(argv[6]), argv[7], argv[8], argv[9]);
		//void selectDenAndNumForFR_opttest_v20190227(TString outdn="", int niso=20, double isomin=0.0, double isomax=0.2, int index1=DYMuMu1030, int index2=DYMuMu1030, TString opt="relPFisoR03", TString MuonIDopt="", TString trigopt="_L3Mu12")
		return 0;
	}
	else
	{
		std::cout << "--- Number of input variables are wrong!, please check again" << std::endl;
		return 1;
	}
}
*/

