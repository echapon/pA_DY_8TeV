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

#include "../../../Include/NtupleHandle.h"
#include "../../../Include/Object.h"


using namespace std;
using namespace DYana;

void applyFR(SampleTag index, TString infrname, TString rmk, TString outdir) {

	 float hiHFminus_c2;
	 // the tool for HF reweighting
	 HFweight hfTool;
		
    cout<<"Chain"<<endl;
    bool mc = false;
    PhysicsEvent* event = new PhysicsEvent();
    TChain* chain = new TChain("kwtuple/physicsTree");
    bool isData = IsData(index);;
    chain->Add(PathTuple(index));
    chain->SetBranchAddress("event",&event);  
    cout<<"# of events = "<<chain->GetEntries()<<endl;

	 TChain*  chain2 = new TChain("recoTree/DYTree");
	 chain2->Add(PathTuple(index));
    chain2->SetBranchAddress("hiHFminus",&hiHFminus_c2);
    cout<<"# of events (chain2) = "<<chain2->GetEntries()<<endl;

    vector<pair<PhysicsMuon,int>>* passingMuons = new vector<pair<PhysicsMuon,int>>;
    vector<pair<PhysicsMuon,int>>* failingMuons = new vector<pair<PhysicsMuon,int>>;
    pair<PhysicsMuon,PhysicsMuon>* tempMuons = new pair<PhysicsMuon,PhysicsMuon>;

    // int binnum = 43;
    // double bins[44] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,  200, 220, 243, 273, 320, 380, 440, 510, 600, 700,  830, 1000, 1500, 3000};

    TFile* f = new TFile("histograms_"+outdir+"/fake_"+rmk+"_"+TString(Name(index))+".root","RECREATE");

    TH1D* histDijet1 = new TH1D("histDijet1","",binnum,bins);
    TH1D* histDijet2 = new TH1D("histDijet2","",binnum,bins);
    TH1D* histSameDijet1 = new TH1D("histSameDijet1","",binnum,bins);
    TH1D* histSameDijet2 = new TH1D("histSameDijet2","",binnum,bins);

    TH1D* fitDijet1 = new TH1D("fitDijet1","",50,15,500);
    TH1D* fitDijet2 = new TH1D("fitDijet2","",50,15,500);
    TH1D* fitSameDijet1 = new TH1D("fitSameDijet1","",50,15,500);
    TH1D* fitSameDijet2 = new TH1D("fitSameDijet2","",50,15,500);

    TH1D* rapDijet1 = new TH1D("rapDijet1","",48,-2.4,2.4);
    TH1D* rapDijet2 = new TH1D("rapDijet2","",48,-2.4,2.4);
    TH1D* rapSameDijet1 = new TH1D("rapSameDijet1","",48,-2.4,2.4);
    TH1D* rapSameDijet2 = new TH1D("rapSameDijet2","",48,-2.4,2.4);

// added
    TH1D* ZptDijet1 = new TH1D("ZptDijet1","",ptbinnum_meas,ptbin_meas);
    TH1D* ZptDijet2 = new TH1D("ZptDijet2","",ptbinnum_meas,ptbin_meas);
    TH1D* ZptSameDijet1 = new TH1D("ZptSameDijet1","",ptbinnum_meas,ptbin_meas);
    TH1D* ZptSameDijet2 = new TH1D("ZptSameDijet2","",ptbinnum_meas,ptbin_meas);

    TH1D* rap1560Dijet1 = new TH1D("rap1560Dijet1","",rapbinnum_1560,rapbin_1560);
    TH1D* rap1560Dijet2 = new TH1D("rap1560Dijet2","",rapbinnum_1560,rapbin_1560);
    TH1D* rap1560SameDijet1 = new TH1D("rap1560SameDijet1","",rapbinnum_1560,rapbin_1560);
    TH1D* rap1560SameDijet2 = new TH1D("rap1560SameDijet2","",rapbinnum_1560,rapbin_1560);

    TH1D* rap60120Dijet1 = new TH1D("rap60120Dijet1","",rapbinnum_60120,rapbin_60120);
    TH1D* rap60120Dijet2 = new TH1D("rap60120Dijet2","",rapbinnum_60120,rapbin_60120);
    TH1D* rap60120SameDijet1 = new TH1D("rap60120SameDijet1","",rapbinnum_60120,rapbin_60120);
    TH1D* rap60120SameDijet2 = new TH1D("rap60120SameDijet2","",rapbinnum_60120,rapbin_60120);

    TH1D* ZphistarDijet1 = new TH1D("ZphistarDijet1","",phistarnum,phistarbin);
    TH1D* ZphistarDijet2 = new TH1D("ZphistarDijet2","",phistarnum,phistarbin);
    TH1D* ZphistarSameDijet1 = new TH1D("ZphistarSameDijet1","",phistarnum,phistarbin);
    TH1D* ZphistarSameDijet2 = new TH1D("ZphistarSameDijet2","",phistarnum,phistarbin);

    TH1D* pt1560Dijet1 = new TH1D("pt1560Dijet1","",ptbinnum_meas_1560,ptbin_meas_1560);
    TH1D* pt1560Dijet2 = new TH1D("pt1560Dijet2","",ptbinnum_meas_1560,ptbin_meas_1560);
    TH1D* pt1560SameDijet1 = new TH1D("pt1560SameDijet1","",ptbinnum_meas_1560,ptbin_meas_1560);
    TH1D* pt1560SameDijet2 = new TH1D("pt1560SameDijet2","",ptbinnum_meas_1560,ptbin_meas_1560);

    TH1D* phistar1560Dijet1 = new TH1D("phistar1560Dijet1","",phistarnum_1560,phistarbin_1560);
    TH1D* phistar1560Dijet2 = new TH1D("phistar1560Dijet2","",phistarnum_1560,phistarbin_1560);
    TH1D* phistar1560SameDijet1 = new TH1D("phistar1560SameDijet1","",phistarnum_1560,phistarbin_1560);
    TH1D* phistar1560SameDijet2 = new TH1D("phistar1560SameDijet2","",phistarnum_1560,phistarbin_1560);


    TH1D* histWJets1 = new TH1D("histWJets1","",binnum,bins);
    TH1D* histWJets2 = new TH1D("histWJets2","",binnum,bins);
    TH1D* histSameWJets1 = new TH1D("histSameWJets1","",binnum,bins);
    TH1D* histSameWJets2 = new TH1D("histSameWJets2","",binnum,bins);

    TH1D* fitWJets1 = new TH1D("fitWJets1","",50,15,500);
    TH1D* fitWJets2 = new TH1D("fitWJets2","",50,15,500);
    TH1D* fitSameWJets1 = new TH1D("fitSameWJets1","",50,15,500);
    TH1D* fitSameWJets2 = new TH1D("fitSameWJets2","",50,15,500);

    TH1D* rapWJets1 = new TH1D("rapWJets1","",48,-2.4,2.4);
    TH1D* rapWJets2 = new TH1D("rapWJets2","",48,-2.4,2.4);
    TH1D* rapSameWJets1 = new TH1D("rapSameWJets1","",48,-2.4,2.4);
    TH1D* rapSameWJets2 = new TH1D("rapSameWJets2","",48,-2.4,2.4);

// added
    TH1D* ZptWJets1 = new TH1D("ZptWJets1","",ptbinnum_meas,ptbin_meas);
    TH1D* ZptWJets2 = new TH1D("ZptWJets2","",ptbinnum_meas,ptbin_meas);
    TH1D* ZptSameWJets1 = new TH1D("ZptSameWJets1","",ptbinnum_meas,ptbin_meas);
    TH1D* ZptSameWJets2 = new TH1D("ZptSameWJets2","",ptbinnum_meas,ptbin_meas);

    TH1D* rap1560WJets1 = new TH1D("rap1560WJets1","",rapbinnum_1560,rapbin_1560);
    TH1D* rap1560WJets2 = new TH1D("rap1560WJets2","",rapbinnum_1560,rapbin_1560);
    TH1D* rap1560SameWJets1 = new TH1D("rap1560SameWJets1","",rapbinnum_1560,rapbin_1560);
    TH1D* rap1560SameWJets2 = new TH1D("rap1560SameWJets2","",rapbinnum_1560,rapbin_1560);

    TH1D* rap60120WJets1 = new TH1D("rap60120WJets1","",rapbinnum_60120,rapbin_60120);
    TH1D* rap60120WJets2 = new TH1D("rap60120WJets2","",rapbinnum_60120,rapbin_60120);
    TH1D* rap60120SameWJets1 = new TH1D("rap60120SameWJets1","",rapbinnum_60120,rapbin_60120);
    TH1D* rap60120SameWJets2 = new TH1D("rap60120SameWJets2","",rapbinnum_60120,rapbin_60120);


    TH1D* ZphistarWJets1 = new TH1D("ZphistarWJets1","",phistarnum,phistarbin);
    TH1D* ZphistarWJets2 = new TH1D("ZphistarWJets2","",phistarnum,phistarbin);
    TH1D* ZphistarSameWJets1 = new TH1D("ZphistarSameWJets1","",phistarnum,phistarbin);
    TH1D* ZphistarSameWJets2 = new TH1D("ZphistarSameWJets2","",phistarnum,phistarbin);

    TH1D* pt1560WJets1 = new TH1D("pt1560WJets1","",ptbinnum_meas_1560,ptbin_meas_1560);
    TH1D* pt1560WJets2 = new TH1D("pt1560WJets2","",ptbinnum_meas_1560,ptbin_meas_1560);
    TH1D* pt1560SameWJets1 = new TH1D("pt1560SameWJets1","",ptbinnum_meas_1560,ptbin_meas_1560);
    TH1D* pt1560SameWJets2 = new TH1D("pt1560SameWJets2","",ptbinnum_meas_1560,ptbin_meas_1560);

    TH1D* phistar1560WJets1 = new TH1D("phistar1560WJets1","",phistarnum_1560,phistarbin_1560);
    TH1D* phistar1560WJets2 = new TH1D("phistar1560WJets2","",phistarnum_1560,phistarbin_1560);
    TH1D* phistar1560SameWJets1 = new TH1D("phistar1560SameWJets1","",phistarnum_1560,phistarbin_1560);
    TH1D* phistar1560SameWJets2 = new TH1D("phistar1560SameWJets2","",phistarnum_1560,phistarbin_1560);



    TH1D* histPass = new TH1D("histPass","",10,0,10);
    TH1D* histFail = new TH1D("histFail","",10,0,10);


    initFRhistos_in(infrname);



    double pt = 0;
    double eta = 0;
    double wt = 1.0;
    double wtsum = 0;
   double phistar = -999.0;
	double dimu_pt=-9.0;
	  bool leading = false;

    double FR1_template;
    double FR2_template;
    double FR1_ratio;
    double FR2_ratio;
    double weight_template;
    double weight_ratio;
    double mass;
    double sign;
    double rap;

    int nPass = 0;
    int nFail = 0;

    int nEntries = chain->GetEntries();

	 chain->AddFriend(chain2);

    for(int i=0; i!=nEntries; i++) {
        chain->GetEntry(i);
		  chain2->GetEntry(i);

		  if (wt!=1) std::cout << "wt is not one, wt : " << wt << std::endl;
        if(mc) {
			wt = event->weight;
			wt = wt * hfTool.weight(hiHFminus_c2,HFweight::minus,true);
             if (i==0) {
                std::cout << i << " - wt ( " << event->weight << " * " << hfTool.weight(event->hiHFminus,HFweight::minus,true) << " ) : " << wt << " --- event->hiHFminus : " << event->hiHFminus << std::endl;
                std::cout << i << " ---- wt ( " << event->weight << " * " << hfTool.weight(hiHFminus_c2,HFweight::minus,true) << " ) : " << wt << " --- hiHFminus_c2 : " << hiHFminus_c2 << std::endl;
             }
			}
        else wt = 1.0;
        wtsum += wt;

        leading = false; 
        passingMuons->clear();
        failingMuons->clear();

		  if (!event->TriggerSelection("HLT_PAL3Mu12_v")) continue;

        for(unsigned j=0; j!=event->muons.size(); j++) {
            PhysicsMuon* mu_ = (PhysicsMuon*)&event->muons.at(j);
//            if( mu_->tightMuonID() && mu_->acceptance(cuts::ptmin2,cuts::etamax) ) {
            if( mu_->tightMuonID() && mu_->acceptance(cuts::ptmin3,cuts::etamax) ) {
                if( mu_->pt > cuts::ptmin1 ) leading = true;

        //        if( mu_->isolation(cuts::isomax) ) passingMuons->push_back({*mu_,j});
//					 if( (mu_->isolationR03_sumpt/mu_->pt)<0.3 ) passingMuons->push_back({*mu_,j});
					 if( (mu_->isolationR03_sumpt/mu_->pt)<0.20 ) passingMuons->push_back({*mu_,j});
                else failingMuons->push_back({*mu_,j});        
            }
        }

        if( !leading ) continue;

        histPass->Fill(passingMuons->size(),wt);
        histFail->Fill(failingMuons->size(),wt);

        nPass += wt*passingMuons->size();
        nFail += wt*failingMuons->size();

        if( failingMuons->size()>1 ) {
            if( dijetDY(event->dimuons,failingMuons,tempMuons) ) {
                FR1_template = FR_template(tempMuons->first);
                FR2_template = FR_template(tempMuons->second);
                FR1_ratio = FR_ratio(tempMuons->first);
                FR2_ratio = FR_ratio(tempMuons->second);

                weight_template = wt*FR1_template*FR2_template/((1-FR1_template)*(1-FR2_template));
                weight_ratio = wt*FR1_ratio*FR2_ratio/((1-FR1_ratio)*(1-FR2_ratio));

//                weight_template = wt*FR1_template*FR2_template/((1-FR1_template)*(1-FR2_template));
//                weight_ratio = wt;


				/*
                if(weight_template > 1 || weight_ratio > 1 ) {
                    cout<<"wt_temp = "<<weight_template<<", wt_ratio= "<<weight_ratio<<endl;
                    cout<<"fr_temp1 = "<<FR1_template<<", fr_ratio1= "<<FR1_ratio<<endl;
                    cout<<"fr_temp2 = "<<FR2_template<<", fr_ratio2= "<<FR2_ratio<<endl;
                    cout<<"pt = "<<tempMuons->first.pt<<", "<<tempMuons->second.pt<<endl;
                    cout<<"eta = "<<tempMuons->first.eta<<", "<<tempMuons->second.eta<<endl;
                }*/

                mass = (tempMuons->first.momentum() + tempMuons->second.momentum()).M();
                sign = tempMuons->first.charge * tempMuons->second.charge;
                rap  = (tempMuons->first.momentum() + tempMuons->second.momentum()).Rapidity()-rapshift;// in CM frame
					phistar = Object::phistar(tempMuons->first.momentum(), tempMuons->second.momentum() );
					dimu_pt = ( tempMuons->first.momentum() + tempMuons->second.momentum() ).Pt();

                if( sign < 0 ) {
                    if( mass > bins[0] && mass < bins[binnum-1]) {
                    		histDijet1->Fill(mass, weight_template);
                    		histDijet2->Fill(mass, weight_ratio);
                    		fitDijet1->Fill(mass, weight_template);
                    		fitDijet2->Fill(mass, weight_ratio);
                        rapDijet1->Fill(rap, weight_template);
                        rapDijet2->Fill(rap, weight_ratio);
                    }
							if( mass > 60 && mass < 120) {
								ZptDijet1->Fill(dimu_pt, weight_template);
								ZptDijet2->Fill(dimu_pt, weight_ratio);
								ZphistarDijet1->Fill(phistar, weight_template);
								ZphistarDijet2->Fill(phistar, weight_ratio);
								rap60120Dijet1->Fill(rap, weight_template);
								rap60120Dijet2->Fill(rap, weight_ratio);
							}
							if ( mass > 15 && mass < 60) {
								rap1560Dijet1->Fill(rap, weight_template);
								rap1560Dijet2->Fill(rap, weight_ratio);
								pt1560Dijet1->Fill(dimu_pt, weight_template);
								pt1560Dijet2->Fill(dimu_pt, weight_ratio);
								phistar1560Dijet1->Fill(phistar, weight_template);
								phistar1560Dijet2->Fill(phistar, weight_ratio);

							}
                }
                else {
                    if( mass > bins[0] && mass < bins[binnum-1]) {
                    		histSameDijet1->Fill(mass, weight_template);
                    		histSameDijet2->Fill(mass, weight_ratio);
                    		fitSameDijet1->Fill(mass, weight_template);
                    		fitSameDijet2->Fill(mass, weight_ratio);
                        rapSameDijet1->Fill(rap, weight_template);
                        rapSameDijet2->Fill(rap, weight_ratio);
                    }
   						if( mass > 60 && mass < 120) {
								ZptSameDijet1->Fill(dimu_pt, weight_template);
								ZptSameDijet2->Fill(dimu_pt, weight_ratio);
								ZphistarSameDijet1->Fill(phistar, weight_template);
								ZphistarSameDijet2->Fill(phistar, weight_ratio);
								rap60120SameDijet1->Fill(rap, weight_template);
								rap60120SameDijet2->Fill(rap, weight_ratio);
							}
							if ( mass > 15 && mass < 60) {
								rap1560SameDijet1->Fill(rap, weight_template);
								rap1560SameDijet2->Fill(rap, weight_ratio);
								pt1560SameDijet1->Fill(dimu_pt, weight_template);
								pt1560SameDijet2->Fill(dimu_pt, weight_ratio);
								phistar1560SameDijet1->Fill(phistar, weight_template);
								phistar1560SameDijet2->Fill(phistar, weight_ratio);

							}
             }
            }
        }
        else if( failingMuons->size()==1 && passingMuons->size()==1 ) {
            if( wjetsDY(event->dimuons, failingMuons->at(0), passingMuons->at(0), tempMuons) ) {
                FR1_template = FR_template(tempMuons->first);
                FR1_ratio = FR_ratio(tempMuons->first);

                weight_template = wt*FR1_template/(1-FR1_template);
                weight_ratio = wt*FR1_ratio/(1-FR1_ratio);

//                weight_template = wt*FR1_template/(1-FR1_template);
//                weight_ratio = wt;


                mass = (tempMuons->first.momentum() + tempMuons->second.momentum()).M();
                sign = tempMuons->first.charge * tempMuons->second.charge;
                rap  = (tempMuons->first.momentum() + tempMuons->second.momentum()).Rapidity()-rapshift;// for CM frame
					phistar = Object::phistar(tempMuons->first.momentum(), tempMuons->second.momentum() );
					dimu_pt = ( tempMuons->first.momentum() + tempMuons->second.momentum() ).Pt();



                if( sign < 0 ) {
                    if( mass > bins[0] && mass < bins[binnum-1]) {
                    	histWJets1->Fill(mass, weight_template);
                    	histWJets2->Fill(mass, weight_ratio);
                    	fitWJets1->Fill(mass, weight_template);
                    	fitWJets2->Fill(mass, weight_ratio);
                        rapWJets1->Fill(rap, weight_template);
                        rapWJets2->Fill(rap, weight_ratio);
                    }
							if( mass > 60 && mass < 120) {
								ZptWJets1->Fill(dimu_pt, weight_template);
								ZptWJets2->Fill(dimu_pt, weight_ratio);
								ZphistarWJets1->Fill(phistar, weight_template);
								ZphistarWJets2->Fill(phistar, weight_ratio);
								rap60120WJets1->Fill(rap, weight_template);
								rap60120WJets2->Fill(rap, weight_ratio);
							}
							if ( mass > 15 && mass < 60) {
								rap1560WJets1->Fill(rap, weight_template);
								rap1560WJets2->Fill(rap, weight_ratio);
								pt1560WJets1->Fill(dimu_pt, weight_template);
								pt1560WJets2->Fill(dimu_pt, weight_ratio);
								phistar1560WJets1->Fill(phistar, weight_template);
								phistar1560WJets2->Fill(phistar, weight_ratio);
							}

                }
                else {
                    if( mass > bins[0] && mass < bins[binnum-1]) {
                    	histSameWJets1->Fill(mass, weight_template);
                    	histSameWJets2->Fill(mass, weight_ratio);
                    	fitSameWJets1->Fill(mass, weight_template);
                    	fitSameWJets2->Fill(mass, weight_ratio);
                        rapSameWJets1->Fill(rap, weight_template);
                        rapSameWJets2->Fill(rap, weight_ratio);
                    }
   							if( mass > 60 && mass < 120) {
								ZptSameWJets1->Fill(dimu_pt, weight_template);
								ZptSameWJets2->Fill(dimu_pt, weight_ratio);
								ZphistarSameWJets1->Fill(phistar, weight_template);
								ZphistarSameWJets2->Fill(phistar, weight_ratio);
								rap60120SameWJets1->Fill(rap, weight_template);
								rap60120SameWJets2->Fill(rap, weight_ratio);
							}
							if ( mass > 15 && mass < 60) {
								rap1560SameWJets1->Fill(rap, weight_template);
								rap1560SameWJets2->Fill(rap, weight_ratio);
								pt1560SameWJets1->Fill(dimu_pt, weight_template);
								pt1560SameWJets2->Fill(dimu_pt, weight_ratio);
								phistar1560SameWJets1->Fill(phistar, weight_template);
								phistar1560SameWJets2->Fill(phistar, weight_ratio);

							}

                }
            }
        } 
    }


    f->Write();
    f->Close();

    // clean behind ourselves
    if (f) delete f;
    if (chain) delete chain;
    if (event) delete event;
    if (passingMuons) delete passingMuons;
    if (failingMuons) delete failingMuons;
    if (tempMuons) delete tempMuons;

    cout<<"# of passing muons = "<<nPass<<endl;
    cout<<"# of failing muons = "<<nFail<<endl;
    cout<<endl;
	 std::cout << "sum of wt : " << wtsum << std::endl;

    cout<<"# of passing muons per event = "<<nPass/wtsum<<endl;
    cout<<"# of failing muons per event = "<<nFail/wtsum<<endl;
    cout<<endl;

    cout<<"Success"<<endl;
    cout<<wtsum<<endl;
}

// produce all plots
void applyFR(TString infrname, TString rmk, TString outdir) {
//   for (int i=0; i<=QCD; i++) {
//	  for (int i=DYMuMu1030; i<=QCD; i++) {
	  for (int i=0; i<=DYMuMu30_PbP; i++) {
//	  for (int i=QCD; i<=QCD; i++) {
      SampleTag tag = static_cast<SampleTag>(i);
      if (tag==ALL) continue;
      cout << Name(tag) << endl;
      applyFR(tag, infrname, rmk, outdir);
   }
}
