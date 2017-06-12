#ifndef PhysicsEvent_h
#define PhysicsEvent_h

#include "NtupleMaker.h"

class PhysicsEvent : public NtupleEvent {
public:
	bool MuonSelection() {
		bool flag1 = false;
		bool flag2 = false;
		for( std::vector<NtupleGenParticle>::const_iterator par = genparticles.begin(); par != genparticles.end(); ++par ) {
			if( par->isHardProcess && par->id == 13 ) flag1 = true;
			else if( par->isHardProcess && par->id == -13 ) flag2 = true;
		}
		return flag1&&flag2;
	};
	bool TauSelection() {
		bool flag1 = false;
		bool flag2 = false;
		for( std::vector<NtupleGenParticle>::const_iterator par = genparticles.begin(); par != genparticles.end(); ++par ) {
			if( par->isHardProcess && par->id == 15 ) flag1 = true;
			else if( par->isHardProcess && par->id == -15 ) flag2 = true;
		}
		return flag1&&flag2;
	};
	bool TriggerSelection( std::string trigger_ = "HLT_PAL3Mu12_v") {
		bool isTriggered = false;
		for( std::vector<NtupleTrigger>::const_iterator trigger = triggers.begin(); trigger != triggers.end(); ++trigger ) {
			if( trigger->name.find(trigger_) == 0 ) {
				//cout<<trigger_<<", "<<trigger->name<<endl;
				//cout<<trigger->isFired<<endl;
				isTriggered = trigger->isFired;
				break;
			}
		}
		return isTriggered;
	}
    bool GenMass() {

        int check1 = 0;
        int check2 = 0;
        TLorentzVector mom1;
        TLorentzVector mom2;

        for(vector<NtupleGenParticle>::const_iterator gen = genparticles.begin(); gen != genparticles.end(); ++gen) {
            if( gen->id==6 && gen->isHardProcess ) {
                mom1.SetPtEtaPhiE(gen->pt,gen->eta,gen->phi,gen->energy);
            }
            else if( gen->id==-6 && gen->isHardProcess ) {
                mom2.SetPtEtaPhiE(gen->pt,gen->eta,gen->phi,gen->energy);
            }
        }

        double mass = (mom1+mom2).M();
        //cout<<"Mtt = "<<mass<<endl;
        if( mass > 700 ) return true;
        else return false;
    }
};

#endif // #ifndef PhysicsEvent_h
