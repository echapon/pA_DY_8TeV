#ifndef PhysicsPhoton_h
#define PhysicsPhoton_h

#include <TLorentzVector.h>
#include "NtupleMaker.h"

class PhysicsPhoton : public NtuplePhoton {
public:
	bool acceptance(double pt_, double eta_) {
		if( pt > pt_ && fabs(eta) < eta_  && (fabs(eta) < 1.4442 || fabs(eta) > 1.566) ) return true;
		else return false;
	}
	TLorentzVector momentum(){
		TLorentzVector momentum_;
		momentum_.SetPtEtaPhiM(pt,eta,phi,0);
		return momentum_;
	}
	
  bool loose() {
    if( fabs(eta) < 1.479
      && HoverE < 0.05
      && Full5x5_SigmaIEtaIEta < 0.0102
      && ChIso < 2.5
      && !hasPixelSeed )
      return true;
    else if( fabs(eta) > 1.479
      && HoverE < 0.05
      && Full5x5_SigmaIEtaIEta < 0.0274
      && ChIso < 2.5 
      && !hasPixelSeed )
      return true;
    else return false;
  }

	bool medium() {
		if( fabs(etaSC) < 1.479 
			&& HoverE < 0.05 
			&& Full5x5_SigmaIEtaIEta < 0.0102 
			&& ChIso < 1.37 
			&& NhIsoWithEA < 1.06 + 0.014 * pt + 0.000019 * pt * pt 
			&& PhIsoWithEA < 0.28 + 0.0053 * pt ) 
			return true;
		else if( fabs(etaSC) > 1.479 
			&& HoverE < 0.05 
			&& Full5x5_SigmaIEtaIEta < 0.0268 
			&& ChIso < 1.10 
			&& NhIsoWithEA < 2.69 + 0.0139 * pt + 0.000025 * pt * pt 
			&& PhIsoWithEA < 0.39 + 0.0034 * pt ) 
			return true;
		else return false;
	}
};

#endif // #ifndef PhysiccsPhoton_h
