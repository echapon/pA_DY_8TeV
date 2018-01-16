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
using namespace std;
using namespace DYana;

void applyFR(SampleTag index) {

    cout<<"Chain"<<endl;
    bool mc = false;
    PhysicsEvent* event = new PhysicsEvent();
    TChain* chain = new TChain("kwtuple/physicsTree");
    bool isData = IsData(index);;
    chain->Add(PathTuple(index));
    chain->SetBranchAddress("event",&event);  
    cout<<"# of events = "<<chain->GetEntries()<<endl;

    vector<pair<PhysicsMuon,int>>* passingMuons = new vector<pair<PhysicsMuon,int>>;
    vector<pair<PhysicsMuon,int>>* failingMuons = new vector<pair<PhysicsMuon,int>>;
    pair<PhysicsMuon,PhysicsMuon>* tempMuons = new pair<PhysicsMuon,PhysicsMuon>;

    // int binnum = 43;
    // double bins[44] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,  200, 220, 243, 273, 320, 380, 440, 510, 600, 700,  830, 1000, 1500, 3000};

    TFile* f = new TFile("histograms/fake"+TString(Name(index))+".root","RECREATE");

    TH1D* histDijet1 = new TH1D("histDijet1","",binnum,bins);
    TH1D* histDijet2 = new TH1D("histDijet2","",binnum,bins);
    TH1D* histSameDijet1 = new TH1D("histSameDijet1","",binnum,bins);
    TH1D* histSameDijet2 = new TH1D("histSameDijet2","",binnum,bins);

    TH1D* fitDijet1 = new TH1D("fitDijet1","",37,15,200);
    TH1D* fitDijet2 = new TH1D("fitDijet2","",37,15,200);
    TH1D* fitSameDijet1 = new TH1D("fitSameDijet1","",37,15,200);
    TH1D* fitSameDijet2 = new TH1D("fitSameDijet2","",37,15,200);

    TH1D* rapDijet1 = new TH1D("rapDijet1","",48,-2.4,2.4);
    TH1D* rapDijet2 = new TH1D("rapDijet2","",48,-2.4,2.4);
    TH1D* rapSameDijet1 = new TH1D("rapSameDijet1","",48,-2.4,2.4);
    TH1D* rapSameDijet2 = new TH1D("rapSameDijet2","",48,-2.4,2.4);

// added
    TH1D* ZptDijet1 = new TH1D("ZptDijet1","",ptbinnum_meas,ptbin_meas);
    TH1D* ZptDijet2 = new TH1D("ZptDijet2","",ptbinnum_meas,ptbin_meas);
    TH1D* ZptSameDijet1 = new TH1D("ZptSameDijet1","",ptbinnum_meas,ptbin_meas);
    TH1D* ZptSameDijet2 = new TH1D("ZptSameDijet2","",ptbinnum_meas,ptbin_meas);

    TH1D* lowMrapDijet1 = new TH1D("lowMrapDijet1","",rapbinnum_1560,rapbin_1560);
    TH1D* lowMrapDijet2 = new TH1D("lowMrapDijet2","",rapbinnum_1560,rapbin_1560);
    TH1D* lowMrapSameDijet1 = new TH1D("lowMrapSameDijet1","",rapbinnum_1560,rapbin_1560);
    TH1D* lowMrapSameDijet2 = new TH1D("lowMrapSameDijet2","",rapbinnum_1560,rapbin_1560);

    TH1D* ZphistarDijet1 = new TH1D("ZphistarDijet1","",phistarnum,phistarbin);
    TH1D* ZphistarDijet2 = new TH1D("ZphistarDijet2","",phistarnum,phistarbin);
    TH1D* ZphistarSameDijet1 = new TH1D("ZphistarSameDijet1","",phistarnum,phistarbin);
    TH1D* ZphistarSameDijet2 = new TH1D("ZphistarSameDijet2","",phistarnum,phistarbin);




    TH1D* histWJets1 = new TH1D("histWJets1","",binnum,bins);
    TH1D* histWJets2 = new TH1D("histWJets2","",binnum,bins);
    TH1D* histSameWJets1 = new TH1D("histSameWJets1","",binnum,bins);
    TH1D* histSameWJets2 = new TH1D("histSameWJets2","",binnum,bins);

    TH1D* fitWJets1 = new TH1D("fitWJets1","",37,15,200);
    TH1D* fitWJets2 = new TH1D("fitWJets2","",37,15,200);
    TH1D* fitSameWJets1 = new TH1D("fitSameWJets1","",37,15,200);
    TH1D* fitSameWJets2 = new TH1D("fitSameWJets2","",37,15,200);

    TH1D* rapWJets1 = new TH1D("rapWJets1","",48,-2.4,2.4);
    TH1D* rapWJets2 = new TH1D("rapWJets2","",48,-2.4,2.4);
    TH1D* rapSameWJets1 = new TH1D("rapSameWJets1","",48,-2.4,2.4);
    TH1D* rapSameWJets2 = new TH1D("rapSameWJets2","",48,-2.4,2.4);

// added
    TH1D* ZptWJets1 = new TH1D("ZptWJets1","",ptbinnum_meas,ptbin_meas);
    TH1D* ZptWJets2 = new TH1D("ZptWJets2","",ptbinnum_meas,ptbin_meas);
    TH1D* ZptSameWJets1 = new TH1D("ZptSameWJets1","",ptbinnum_meas,ptbin_meas);
    TH1D* ZptSameWJets2 = new TH1D("ZptSameWJets2","",ptbinnum_meas,ptbin_meas);

    TH1D* lowMrapWJets1 = new TH1D("lowMrapWJets1","",rapbinnum_1560,rapbin_1560);
    TH1D* lowMrapWJets2 = new TH1D("lowMrapWJets2","",rapbinnum_1560,rapbin_1560);
    TH1D* lowMrapSameWJets1 = new TH1D("lowMrapSameWJets1","",rapbinnum_1560,rapbin_1560);
    TH1D* lowMrapSameWJets2 = new TH1D("lowMrapSameWJets2","",rapbinnum_1560,rapbin_1560);

    TH1D* ZphistarWJets1 = new TH1D("ZphistarWJets1","",phistarnum,phistarbin);
    TH1D* ZphistarWJets2 = new TH1D("ZphistarWJets2","",phistarnum,phistarbin);
    TH1D* ZphistarSameWJets1 = new TH1D("ZphistarSameWJets1","",phistarnum,phistarbin);
    TH1D* ZphistarSameWJets2 = new TH1D("ZphistarSameWJets2","",phistarnum,phistarbin);



    TH1D* histPass = new TH1D("histPass","",10,0,10);
    TH1D* histFail = new TH1D("histFail","",10,0,10);


    initFRhistos();



    double pt = 0;
    double eta = 0;
    double wt = 1.0;
    double wtsum = 0;
   double phistar = -999.0;
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
    for(int i=0; i!=nEntries; i++) {
        chain->GetEntry(i);
        if(mc) wt = event->weight;
        else wt = 1.0;
        wtsum += wt;

        leading = false; 
        passingMuons->clear();
        failingMuons->clear();

        for(unsigned j=0; j!=event->muons.size(); j++) {
            PhysicsMuon* mu_ = (PhysicsMuon*)&event->muons.at(j);
            if( mu_->tightMuonID() && mu_->acceptance(cuts::ptmin2,cuts::etamax) ) {
                if( mu_->pt > cuts::ptmin1 ) leading = true;

                if( mu_->isolation(cuts::isomax) ) passingMuons->push_back({*mu_,j});
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
                rap  = (tempMuons->first.momentum() + tempMuons->second.momentum()).Rapidity();
					phistar = Object::phistar(tempMuons->first.momentum(), tempMuons->second.momentum() );

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
								ZptDijet1->Fill(pt, weight_template);
								ZptDijet2->Fill(pt, weight_ratio);
								ZphistarDijet1->Fill(phistar, weight_template);
								ZphistarDijet2->Fill(phistar, weight_ratio);
							}
							if ( mass > 15 && mass < 60) {
								lowMrapDijet1->Fill(rap, weight_template);
								lowMrapDijet2->Fill(rap, weight_ratio);
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
								ZptSameDijet1->Fill(pt, weight_template);
								ZptSameDijet2->Fill(pt, weight_ratio);
								ZphistarSameDijet1->Fill(phistar, weight_template);
								ZphistarSameDijet2->Fill(phistar, weight_ratio);
							}
							if ( mass > 15 && mass < 60) {
								lowMrapSameDijet1->Fill(rap, weight_template);
								lowMrapSameDijet2->Fill(rap, weight_ratio);
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
                mass = (tempMuons->first.momentum() + tempMuons->second.momentum()).M();
                sign = tempMuons->first.charge * tempMuons->second.charge;
                rap  = (tempMuons->first.momentum() + tempMuons->second.momentum()).Rapidity();
					phistar = Object::phistar(tempMuons->first.momentum(), tempMuons->second.momentum() );


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
								ZptWJets1->Fill(pt, weight_template);
								ZptWJets2->Fill(pt, weight_ratio);
								ZphistarWJets1->Fill(phistar, weight_template);
								ZphistarWJets2->Fill(phistar, weight_ratio);
							}
							if ( mass > 15 && mass < 60) {
								lowMrapWJets1->Fill(rap, weight_template);
								lowMrapWJets2->Fill(rap, weight_ratio);
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
								ZptSameWJets1->Fill(pt, weight_template);
								ZptSameWJets2->Fill(pt, weight_ratio);
								ZphistarSameWJets1->Fill(phistar, weight_template);
								ZphistarSameWJets2->Fill(phistar, weight_ratio);
							}
							if ( mass > 15 && mass < 60) {
								lowMrapSameWJets1->Fill(rap, weight_template);
								lowMrapSameWJets2->Fill(rap, weight_ratio);
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
    cout<<"# of passing muons per event = "<<nPass/wtsum<<endl;
    cout<<"# of failing muons per event = "<<nFail/wtsum<<endl;
    cout<<endl;

    cout<<"Success"<<endl;
    cout<<wtsum<<endl;
}

// produce all plots
void applyFR() {
   for (int i=0; i<=QCD; i++) {
      SampleTag tag = static_cast<SampleTag>(i);
      if (tag==ALL) continue;
      cout << Name(tag) << endl;
      applyFR(tag);
   }
}
