#ifndef analysis_H
#define analysis_H

#include <TLorentzVector.h>
#include "NtupleMaker.h"
#include "fakerate.h"
#include "PhysicsEvent.h"
#include "PhysicsMuon.h"
#include "PhysicsElectron.h"
#include "PhysicsPhoton.h"
#include "cuts.h"


//const int binnum = 45;
//const double bins[46] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,  200, 220, 243, 273, 320, 380, 440, 510, 600, 700,  830, 1000, 1200, 1500, 2000, 3000};


double deltaR(double eta1, double phi1, double eta2, double phi2) {
	double deta = fabs(eta1 - eta2);
	double dphi = fabs(phi1 - phi2);
	if(dphi>M_PI)
		dphi = 2*M_PI - dphi;
	return sqrt(deta*deta+dphi*dphi);
}

bool triggerMatch(std::vector<NtupleTriggerObject> triggerobjects, PhysicsMuon mu, std::string triggerName = cuts::trig) {
	bool flag = false;
	for( std::vector<NtupleTriggerObject>::const_iterator trig = triggerobjects.begin(); trig != triggerobjects.end(); ++trig ) {
		if( trig->name.find(triggerName)!=std::string::npos) {
			double dR = deltaR(mu.eta, mu.phi, trig->eta, trig->phi);
			if(dR<0.2) {
				flag = true;
				break;
			}
		}
	}
	return flag;
}  

double openingAngle(std::vector<NtupleDimuon> dimuons, std::pair<PhysicsMuon,int> mu1, std::pair<PhysicsMuon,int> mu2) {
    bool fail = true;
	double angle = 9999;
	int index1, index2;
	if( mu1.second < mu2.second ) {
		index1 = mu1.second;
		index2 = mu2.second;
	}
	else {
		index1 = mu2.second;
		index2 = mu1.second;
	}
	for( unsigned i=0; i!=dimuons.size(); i++){
		if( dimuons.at(i).X==index1 && dimuons.at(i).Y==index2 ) {
            fail = false;
			angle = dimuons.at(i).openingAngle;
			break;
		}
	}
    if(fail) {
        cout<<"Wrong angle:"<<index1<<", "<<index2<<endl;
        for( unsigned i=0; i!=dimuons.size(); ++i) {
            cout<<dimuons.at(i).X<<", "<<dimuons.at(i).Y<<endl;
        }
        cout<<""<<endl;
    }
	return angle;
}

double openingAngle(vector<NtupleEmu> emus, std::pair<PhysicsElectron,int> el, std::pair<PhysicsMuon,int> mu) {
	double angle = 9999;
	int index1, index2;
	index1 = mu.second;
	index2 = el.second;

	for(unsigned i=0; i!=emus.size(); i++){
		if(emus.at(i).X==index1 && emus.at(i).Y==index2) {
			angle = emus.at(i).openingAngle;
			break;
		}
	}
	return angle;
}

double vertexFitChi2(std::vector<NtupleDimuon> dimuons, std::pair<PhysicsMuon,int> mu1, std::pair<PhysicsMuon,int> mu2) {
	double chi2 = 9999;
    bool fail = true;
	int index1, index2;
	if( mu1.second < mu2.second ) {
		index1 = mu1.second;
		index2 = mu2.second;
	}
	else {
		index1 = mu2.second;
		index2 = mu1.second;
	}
	for( unsigned i=0; i!=dimuons.size(); i++){
		if( dimuons.at(i).X==index1 && dimuons.at(i).Y==index2 ) {
			chi2 = dimuons.at(i).vertexFitChi2Ndof;
            fail = false;
			break;
		}
	}
    if(fail) cout<<"Wrong"<<endl;
	//cout<<"Dimuon chi2 = "<<chi2<<endl;
	return chi2;
}

double vertexFitChi2(std::vector<NtupleEmu> emus, std::pair<PhysicsElectron,int> el, std::pair<PhysicsMuon,int> mu) {
  double chi2 = 9999;
  int index1, index2;
  index1 = mu.second;
  index2 = el.second;

  for(unsigned i=0; i!=emus.size(); i++){
    if( emus.at(i).X==index1 && emus.at(i).Y==index2 ) {
      chi2 = emus.at(i).vertexFitChi2Ndof;
      break;
    }
  }
  //cout<<"Emu chi2 = "<<chi2<<endl;
  return chi2;
}

bool dimuonDY(std::vector<NtupleTriggerObject> triggerobjects, std::vector<NtupleDimuon> dimuons, vector<pair<PhysicsMuon,int>>* muons, pair<PhysicsMuon,PhysicsMuon>* dimuon, double& chi2min) {
	bool flag = false;
	chi2min = 20;
	pair<PhysicsMuon,int> mu1;
	pair<PhysicsMuon,int> mu2;

	if(muons->size()>=2) {
		for(unsigned int i=0; i<muons->size(); i++) {
			for(unsigned int j=0; j<muons->size(); j++) {
				if(i>=j) continue;

				mu1 = muons->at(i);
				mu2 = muons->at(j);

				if( mu1.first.pt<=cuts::ptmin1 && mu2.first.pt<=cuts::ptmin2 ) continue;

				if(triggerMatch(triggerobjects, mu1.first,cuts::trig)) {
					if(openingAngle(dimuons,mu1,mu2)<M_PI-0.005) {
						if(vertexFitChi2(dimuons,mu1,mu2)<chi2min) {
							flag = true;
							chi2min = vertexFitChi2(dimuons,mu1,mu2);
							dimuon->first = mu1.first;
							dimuon->second = mu2.first;
						}
					}
				}
			}
		}
	}
	return flag;
}
bool emuDY(std::vector<NtupleTriggerObject> triggerobjects, std::vector<NtupleEmu> emus, vector<pair<PhysicsElectron,int>>* electrons, vector<pair<PhysicsMuon,int>>* muons, pair<PhysicsElectron,PhysicsMuon>* emu, double& chi2min) {
	bool flag = false;
	chi2min = 20;
	pair<PhysicsMuon,int> mu;
	pair<PhysicsElectron,int> el;

	for(unsigned int i=0; i<muons->size(); i++) {
		for(unsigned int j=0; j<electrons->size(); j++) {
			mu = muons->at(i);
			el = electrons->at(j);

			if( mu.first.pt<=cuts::ptmin1 && el.first.etSC<=cuts::ptmin2 ) continue;

			if(!triggerMatch(triggerobjects, mu.first,cuts::trig)) continue;

			if( openingAngle(emus,el,mu)<M_PI-0.005 ) {
				if( vertexFitChi2(emus,el,mu)<chi2min ) {
					flag = true;
					chi2min = vertexFitChi2(emus,el,mu);
					emu->first = el.first;
					emu->second = mu.first;
				}
			}
		}
	}
	return flag;
}

bool dijetDY(std::vector<NtupleDimuon> dimuons, vector<pair<PhysicsMuon,int>>* muons, pair<PhysicsMuon,PhysicsMuon>* dimuon) {
	bool flag = false;
	double min = 20;
	pair<PhysicsMuon,int> mu1;
	pair<PhysicsMuon,int> mu2;

	if(muons->size()>=2) {
		for(unsigned int i=0; i<muons->size(); i++) {
			for(unsigned int j=0; j<muons->size(); j++) {
				if(i>=j) continue;

				mu1 = muons->at(i);
				mu2 = muons->at(j);

				if( mu1.first.pt<=cuts::ptmin1 && mu2.first.pt<=cuts::ptmin2 ) continue;

				if(openingAngle(dimuons,mu1,mu2)<M_PI-0.005) {
					if(vertexFitChi2(dimuons,mu1,mu2)<min) {
						flag = true;
						min = vertexFitChi2(dimuons,mu1,mu2);
						dimuon->first = mu1.first;
						dimuon->second = mu2.first;
					}
				}
				
			}
		}
	}
	return flag;
}

bool wjetsDY(std::vector<NtupleDimuon> dimuons, pair<PhysicsMuon,int> mu1, pair<PhysicsMuon,int> mu2, pair<PhysicsMuon,PhysicsMuon>* dimuon) {
	bool flag = false;
	const double min = 20;


	if( mu1.first.pt>cuts::ptmin1 || mu2.first.pt>cuts::ptmin2 ) {
		if(openingAngle(dimuons,mu1,mu2)<M_PI-0.005) {
			if(vertexFitChi2(dimuons,mu1,mu2)<min) {
				flag = true;
				dimuon->first = mu1.first;
				dimuon->second = mu2.first;
			}
		}
	}

	return flag;
}


#endif
