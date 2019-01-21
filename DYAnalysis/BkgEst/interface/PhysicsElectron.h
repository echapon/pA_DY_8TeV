#ifndef PhysicsElectron_h
#define PhysicsElectron_h

#include <TLorentzVector.h>
#include "NtupleMaker.h"

const double electron_mass = 0.000510998;

class PhysicsElectron : public NtupleElectron {
public:
	bool acceptance(double pt_, double eta_){
		if( pt > pt_ && fabs(eta) < eta_  && (fabs(eta) < 1.4442 || fabs(eta) > 1.566) ) return true;
		else return false;
	}
	TLorentzVector momentum(){
		TLorentzVector momentum_;
		momentum_.SetPtEtaPhiM(pt,eta,phi,electron_mass);
		return momentum_;
	}	

  bool WPLoose() {
    bool flag = false;
    if(fabs(etaSC)<=1.479) { //Barrel
      if( sigmaIetaIeta < 0.0103
        && fabs(dEtaIn) < 0.0105
        && fabs(dPhiIn) < 0.115
        && hOverE < 0.104
        //&& isoRho < 0.0893
        && ooEmooP < 0.102
        && fabs(d0) < 0.0261
        && fabs(dz) < 0.41
        && expectedMissingInnerHits <= 2
        && passConversionVeto )
        flag = true;
    }
    else { //Endcap
      if( sigmaIetaIeta < 0.0301
        && fabs(dEtaIn) < 0.00814
        && fabs(dPhiIn) < 0.182
        && hOverE < 0.0897
        //&& isoRho < 0.121
        && ooEmooP < 0.126
        && fabs(d0) < 0.118
        && fabs(dz) < 0.822
        && expectedMissingInnerHits <= 1
        && passConversionVeto )
        flag = true;
    }
    return flag;
  }

	bool WPMedium() {
		bool flag = false;
		if(fabs(etaSC)<=1.479) { //Barrel
			if( sigmaIetaIeta < 0.0101 
				&& fabs(dEtaIn) < 0.0103 
				&& fabs(dPhiIn) < 0.0336 
				&& hOverE < 0.0876 
				&& isoRho < 0.0766 
				&& ooEmooP < 0.0174 
				&& fabs(d0) < 0.0118 
				&& fabs(dz) < 0.373 
				&& expectedMissingInnerHits <= 2 
				&& passConversionVeto ) 
				flag = true;
		}
		else { //Endcap
			if( sigmaIetaIeta < 0.0283 
				&& fabs(dEtaIn) < 0.00733 
				&& fabs(dPhiIn) < 0.114  
				&& hOverE < 0.0678 
				&& isoRho < 0.0678 
				&& ooEmooP < 0.0898 
				&& fabs(d0) < 0.0739 
				&& fabs(dz) < 0.602 
				&& expectedMissingInnerHits <= 1 
				&& passConversionVeto ) 
				flag = true;
		}
		return flag;
	} 

	bool WPTight() {
		bool flag = false;
		if(fabs(etaSC)<=1.479) { //Barrel
			if( sigmaIetaIeta < 0.0101 
				&& fabs(dEtaIn) < 0.00926 
				&& fabs(dPhiIn) < 0.0336 
				&& hOverE < 0.0597 
				&& isoRho < 0.0354 
				&& ooEmooP < 0.012 
				&& fabs(d0) < 0.0111 
				&& fabs(dz) < 0.0466 
				&& expectedMissingInnerHits <= 2 
				&& passConversionVeto ) 
				flag = true;
		}
		else { //Endcap
			if( sigmaIetaIeta < 0.0279 
				&& fabs(dEtaIn) < 0.00724 
				&& fabs(dPhiIn) < 0.0918 
				&& hOverE < 0.0615 
				&& isoRho < 0.0646 
				&& ooEmooP < 0.00999 
				&& fabs(d0) < 0.0351 
				&& fabs(dz) < 0.417 
				&& expectedMissingInnerHits <= 1 
				&& passConversionVeto ) 
				flag = true;
		}
		return flag;
	} 

	bool trkisolation(double iso) {
      double theiso;
      theiso = dr03tkSumPt;
		if( theiso/pt < iso) return true;
		else return false;
	}
   // bool miniIsolation(double iso) {
   //    if( miniIso < iso ) return true;
   //    else return false;
   // }
};
#endif // #ifndef PhysicsElectron_h
