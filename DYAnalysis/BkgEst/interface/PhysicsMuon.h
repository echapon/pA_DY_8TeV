#ifndef PhysicsMuon_h
#define PhysicsMuon_h

#include <TLorentzVector.h>
#include "NtupleMaker.h"

const double muon_mass     = 0.1056583715;

class PhysicsMuon : public NtupleMuon {
public:
	bool acceptance(double pt_, double eta_){
		if( pt > pt_ && fabs(eta) < eta_ ) return true;
		else return false;
	}
	TLorentzVector momentum(){
		TLorentzVector momentum_;
		momentum_.SetPtEtaPhiM(pt,eta,phi,muon_mass);
		return momentum_;
	}

  bool looseMuonID() {
    // if( isPFMuon && (isGlobalMuon||isTrackerMuon) && fabs(dxyVTX) < 0.2 && fabs(dzVTX) < 0.5 )
    //   return true;
    // else
    //   return false;
    return isLooseMuon;
  }

  bool tightMuonID() {
    // if( isPFMuon && isGlobalMuon && normalizedChi2 < 10 && nValidMuonHits>0 && nMatchedStations>1 && nValidPixelHits>0 && nTrackerLayers>5 && fabs(dxyVTX) < 0.2 && fabs(dzVTX) < 0.5 )
    //   return true;
    // else
    //   return false;
    return isTightMuon;
  }

	bool highPtMuonID() {
      // if( isGlobalMuon && nValidMuonHits>0 && nMatchedStations>1 && nValidPixelHits>0 && nTrackerLayers>5 && fabs(dxyVTX)<0.2 && muonBestTrack_ptError/muonBestTrack_pt<0.3 )
      //    return true;
      // else
      //    return false;
      return isHighPtMuon;
	}
	// added for fake rate test
	bool tightMuonIDSub() {
	   // if( isGlobalMuon && nValidMuonHits>0 && nMatchedStations>1 && nValidPixelHits>0 && nTrackerLayers>5 && fabs(dxyVTX) < 0.2 && fabs(dzVTX) < 0.5 )
		//   return true;
		// else
		//   return false;
		return istightMuonSub;
	}

	bool isolation(double iso) {
      double theiso;
      theiso = PfChargedHadronIsoR03 + PfNeutralHadronIsoR03 + PfGammaIsoR03;
		if( theiso/pt < iso) return true;
		else return false;
	}
   // bool miniIsolation(double iso) {
   //    if( miniIso < iso ) return true;
   //    else return false;
   // }
};

#endif // #ifndef PhysicsMuon_h
