//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue May 23 17:04:21 2017 by ROOT version 6.04/02
// from TTree DYTree/DYTree
// found on file: histograms/tree_DY50100.root
//////////////////////////////////////////////////////////

#ifndef MakeHFweight_h
#define MakeHFweight_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

// a simple structure with the needed TH1's
struct HistPack {
   TH1F* h_nVertices;
   TH1F* h_mass;
   TH1F* h_rap;
   TH1F* h_hiHF;
   TH1F* h_hiNtracks;
   TH1F* h_hiHFplus;
   TH1F* h_hiHFminus;
   TH2F* h_hiHF2D;
   TH1F* h_hiHF_PV1;
   TH1F* h_hiNtracks_PV1;
   TH1F* h_hiHFplus_PV1;
   TH1F* h_hiHFminus_PV1;
   TH2F* h_hiHF2D_PV1;
   TString label;
};

class MakeHFweight {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           nTotal;
   Int_t           runNum;
   Int_t           evtNum;
   Int_t           lumiBlock;
   Int_t           nMuon;
   Int_t           Nmuons;
   Double_t        PUweight;
   Int_t           nVertices;
   Int_t           hiBin;
   Float_t         hiHF;
   Float_t         hiHFplus;
   Float_t         hiHFminus;
   Float_t         hiHFplusEta4;
   Float_t         hiHFminusEta4;
   Float_t         hiZDC;
   Float_t         hiZDCplus;
   Float_t         hiZDCminus;
   Float_t         hiHFhit;
   Float_t         hiHFhitPlus;
   Float_t         hiHFhitMinus;
   Float_t         hiET;
   Float_t         hiEE;
   Float_t         hiEB;
   Float_t         hiEEplus;
   Float_t         hiEEminus;
   Int_t           hiNpix;
   Int_t           hiNpixelTracks;
   Int_t           hiNtracks;
   Int_t           hiNtracksPtCut;
   Int_t           hiNtracksEtaCut;
   Int_t           hiNtracksEtaPtCut;
   Int_t           PVtrackSize;
   Double_t        PVchi2;
   Double_t        PVndof;
   Double_t        PVnormalizedChi2;
   Double_t        PVx;
   Double_t        PVy;
   Double_t        PVz;
   Int_t           HLT_ntrig;
   Int_t           HLT_trigType[32];   //[HLT_ntrig]
   Int_t           HLT_trigFired[32];   //[HLT_ntrig]
   vector<string>  *HLT_trigName;
   vector<int>     *HLT_trigPS;
   Double_t        HLT_trigPt[32];   //[HLT_ntrig]
   Double_t        HLT_trigEta[32];   //[HLT_ntrig]
   Double_t        HLT_trigPhi[32];   //[HLT_ntrig]
   Int_t           Njets;
   Double_t        Jet_pT[27];   //[Njets]
   Double_t        Jet_eta[27];   //[Njets]
   Double_t        Jet_phi[27];   //[Njets]
   Double_t        Jet_Charge[27];   //[Njets]
   Int_t           Jet_Flavor[27];   //[Njets]
   Double_t        Jet_bTag[27];   //[Njets]
   Double_t        Jet_CHfrac[27];   //[Njets]
   Double_t        Jet_NHfrac[27];   //[Njets]
   Double_t        Jet_NHEMfrac[27];   //[Njets]
   Double_t        Jet_CHEMfrac[27];   //[Njets]
   Int_t           Jet_CHmulti[27];   //[Njets]
   Int_t           Jet_NHmulti[27];   //[Njets]
   Int_t           Nelectrons;
   Double_t        Electron_Energy[6];   //[Nelectrons]
   Double_t        Electron_pT[6];   //[Nelectrons]
   Double_t        Electron_eta[6];   //[Nelectrons]
   Double_t        Electron_phi[6];   //[Nelectrons]
   Int_t           Electron_charge[6];   //[Nelectrons]
   Double_t        Electron_gsfpT[6];   //[Nelectrons]
   Double_t        Electron_gsfPx[6];   //[Nelectrons]
   Double_t        Electron_gsfPy[6];   //[Nelectrons]
   Double_t        Electron_gsfPz[6];   //[Nelectrons]
   Double_t        Electron_gsfEta[6];   //[Nelectrons]
   Double_t        Electron_gsfPhi[6];   //[Nelectrons]
   Int_t           Electron_gsfCharge[6];   //[Nelectrons]
   Double_t        Electron_etaSC[6];   //[Nelectrons]
   Double_t        Electron_phiSC[6];   //[Nelectrons]
   Double_t        Electron_etaWidth[6];   //[Nelectrons]
   Double_t        Electron_phiWidth[6];   //[Nelectrons]
   Double_t        Electron_dEtaIn[6];   //[Nelectrons]
   Double_t        Electron_dPhiIn[6];   //[Nelectrons]
   Double_t        Electron_sigmaIEtaIEta[6];   //[Nelectrons]
   Double_t        Electron_HoverE[6];   //[Nelectrons]
   Double_t        Electron_fbrem[6];   //[Nelectrons]
   Double_t        Electron_eOverP[6];   //[Nelectrons]
   Double_t        Electron_InvEminusInvP[6];   //[Nelectrons]
   Double_t        Electron_dxyVTX[6];   //[Nelectrons]
   Double_t        Electron_dzVTX[6];   //[Nelectrons]
   Double_t        Electron_dxy[6];   //[Nelectrons]
   Double_t        Electron_dz[6];   //[Nelectrons]
   Double_t        Electron_dxyBS[6];   //[Nelectrons]
   Double_t        Electron_dzBS[6];   //[Nelectrons]
   Double_t        Electron_chIso03[6];   //[Nelectrons]
   Double_t        Electron_nhIso03[6];   //[Nelectrons]
   Double_t        Electron_phIso03[6];   //[Nelectrons]
   Char_t          Electron_hasConversion[6];   //[Nelectrons]
   Int_t           Electron_mHits[6];   //[Nelectrons]
   Double_t        Electron_EnergySC[6];   //[Nelectrons]
   Double_t        Electron_preEnergySC[6];   //[Nelectrons]
   Double_t        Electron_rawEnergySC[6];   //[Nelectrons]
   Double_t        Electron_etSC[6];   //[Nelectrons]
   Double_t        Electron_E15[6];   //[Nelectrons]
   Double_t        Electron_E25[6];   //[Nelectrons]
   Double_t        Electron_E55[6];   //[Nelectrons]
   Double_t        Electron_ChIso03FromPU[6];   //[Nelectrons]
   Double_t        Electron_RelPFIso_dBeta[6];   //[Nelectrons]
   Double_t        Electron_RelPFIso_Rho[6];   //[Nelectrons]
   Bool_t          Electron_passConvVeto[6];   //[Nelectrons]
   Int_t           Electron_passVetoID[6];   //[Nelectrons]
   Int_t           Electron_passLooseID[6];   //[Nelectrons]
   Int_t           Electron_passMediumID[6];   //[Nelectrons]
   Int_t           Electron_passTightID[6];   //[Nelectrons]
   Double_t        Electron_r9[6];   //[Nelectrons]
   Int_t           Electron_ecalDriven[6];   //[Nelectrons]
   Double_t        Electron_ambGsf0Pt[6];   //[Nelectrons]
   Double_t        Electron_ambGsf0Eta[6];   //[Nelectrons]
   Double_t        Electron_ambGsf0Phi[6];   //[Nelectrons]
   Double_t        Electron_ambGsf0Charge[6];   //[Nelectrons]
   Double_t        Electron_ambGsf1Pt[6];   //[Nelectrons]
   Double_t        Electron_ambGsf1Eta[6];   //[Nelectrons]
   Double_t        Electron_ambGsf1Phi[6];   //[Nelectrons]
   Double_t        Electron_ambGsf1Charge[6];   //[Nelectrons]
   Double_t        Electron_ambGsf2Pt[6];   //[Nelectrons]
   Double_t        Electron_ambGsf2Eta[6];   //[Nelectrons]
   Double_t        Electron_ambGsf2Phi[6];   //[Nelectrons]
   Double_t        Electron_ambGsf2Charge[6];   //[Nelectrons]
   Double_t        Electron_ambGsf3Pt[6];   //[Nelectrons]
   Double_t        Electron_ambGsf3Eta[6];   //[Nelectrons]
   Double_t        Electron_ambGsf3Phi[6];   //[Nelectrons]
   Double_t        Electron_ambGsf3Charge[6];   //[Nelectrons]
   Int_t           Muon_muonType[6];   //[nMuon]
   Int_t           isPFmuon[6];   //[nMuon]
   Int_t           isGLBmuon[6];   //[nMuon]
   Int_t           isTRKmuon[6];   //[nMuon]
   Int_t           isSTAmuon[6];   //[nMuon]
   Int_t           isLooseMuon[6];   //[nMuon]
   Int_t           isMediumMuon[6];   //[nMuon]
   Int_t           isTightMuon[6];   //[nMuon]
   Int_t           isSoftMuon[6];   //[nMuon]
   Int_t           isHighPtMuon[6];   //[nMuon]
   Int_t           Muon_nTrig[6];   //[nMuon]
   Int_t           Muon_triggerObjectType[6];   //[nMuon]
   Int_t           Muon_filterName[6];   //[nMuon]
   Double_t        Muon_phi[6];   //[nMuon]
   Double_t        Muon_eta[6];   //[nMuon]
   Double_t        Muon_pT[6];   //[nMuon]
   Double_t        Muon_cktpT[6];   //[nMuon]
   Double_t        Muon_cktPx[6];   //[nMuon]
   Double_t        Muon_cktPy[6];   //[nMuon]
   Double_t        Muon_cktPz[6];   //[nMuon]
   Double_t        Muon_cktpTError[6];   //[nMuon]
   Double_t        Muon_Px[6];   //[nMuon]
   Double_t        Muon_Py[6];   //[nMuon]
   Double_t        Muon_Pz[6];   //[nMuon]
   Double_t        Muon_trkiso[6];   //[nMuon]
   Double_t        Muon_hcaliso[6];   //[nMuon]
   Double_t        Muon_ecaliso[6];   //[nMuon]
   Double_t        Muon_trkisoR05[6];   //[nMuon]
   Double_t        Muon_hcalisoR05[6];   //[nMuon]
   Double_t        Muon_ecalisoR05[6];   //[nMuon]
   Double_t        Muon_Best_pT[6];   //[nMuon]
   Double_t        Muon_Best_pTError[6];   //[nMuon]
   Double_t        Muon_Best_Px[6];   //[nMuon]
   Double_t        Muon_Best_Py[6];   //[nMuon]
   Double_t        Muon_Best_Pz[6];   //[nMuon]
   Double_t        Muon_Best_eta[6];   //[nMuon]
   Double_t        Muon_Best_phi[6];   //[nMuon]
   Double_t        Muon_Inner_pT[6];   //[nMuon]
   Double_t        Muon_Inner_pTError[6];   //[nMuon]
   Double_t        Muon_Inner_Px[6];   //[nMuon]
   Double_t        Muon_Inner_Py[6];   //[nMuon]
   Double_t        Muon_Inner_Pz[6];   //[nMuon]
   Double_t        Muon_Inner_eta[6];   //[nMuon]
   Double_t        Muon_Inner_phi[6];   //[nMuon]
   Double_t        Muon_Outer_pT[6];   //[nMuon]
   Double_t        Muon_Outer_pTError[6];   //[nMuon]
   Double_t        Muon_Outer_Px[6];   //[nMuon]
   Double_t        Muon_Outer_Py[6];   //[nMuon]
   Double_t        Muon_Outer_Pz[6];   //[nMuon]
   Double_t        Muon_Outer_eta[6];   //[nMuon]
   Double_t        Muon_Outer_phi[6];   //[nMuon]
   Double_t        Muon_GLB_pT[6];   //[nMuon]
   Double_t        Muon_GLB_pTError[6];   //[nMuon]
   Double_t        Muon_GLB_Px[6];   //[nMuon]
   Double_t        Muon_GLB_Py[6];   //[nMuon]
   Double_t        Muon_GLB_Pz[6];   //[nMuon]
   Double_t        Muon_GLB_eta[6];   //[nMuon]
   Double_t        Muon_GLB_phi[6];   //[nMuon]
   Double_t        Muon_TuneP_pT[6];   //[nMuon]
   Double_t        Muon_TuneP_pTError[6];   //[nMuon]
   Double_t        Muon_TuneP_Px[6];   //[nMuon]
   Double_t        Muon_TuneP_Py[6];   //[nMuon]
   Double_t        Muon_TuneP_Pz[6];   //[nMuon]
   Double_t        Muon_TuneP_eta[6];   //[nMuon]
   Double_t        Muon_TuneP_phi[6];   //[nMuon]
   Double_t        Muon_PfChargedHadronIsoR05[6];   //[nMuon]
   Double_t        Muon_PfNeutralHadronIsoR05[6];   //[nMuon]
   Double_t        Muon_PfGammaIsoR05[6];   //[nMuon]
   Double_t        Muon_PfChargedHadronIsoR04[6];   //[nMuon]
   Double_t        Muon_PfNeutralHadronIsoR04[6];   //[nMuon]
   Double_t        Muon_PfGammaIsoR04[6];   //[nMuon]
   Double_t        Muon_PFSumPUIsoR04[6];   //[nMuon]
   Double_t        Muon_PfChargedHadronIsoR03[6];   //[nMuon]
   Double_t        Muon_PfNeutralHadronIsoR03[6];   //[nMuon]
   Double_t        Muon_PfGammaIsoR03[6];   //[nMuon]
   Double_t        Muon_PFSumPUIsoR03[6];   //[nMuon]
   Int_t           Muon_charge[6];   //[nMuon]
   Int_t           Muon_nChambers[6];   //[nMuon]
   Int_t           Muon_nMatches[6];   //[nMuon]
   Int_t           Muon_stationMask[6];   //[nMuon]
   Int_t           Muon_nSegments[6];   //[nMuon]
   Double_t        Muon_chi2dof[6];   //[nMuon]
   Int_t           Muon_nhits[6];   //[nMuon]
   Int_t           Muon_trackerHits[6];   //[nMuon]
   Int_t           Muon_trackerLayers[6];   //[nMuon]
   Int_t           Muon_pixelHits[6];   //[nMuon]
   Int_t           Muon_muonHits[6];   //[nMuon]
   Double_t        Muon_qoverp[6];   //[nMuon]
   Double_t        Muon_theta[6];   //[nMuon]
   Double_t        Muon_lambda[6];   //[nMuon]
   Double_t        Muon_dxy[6];   //[nMuon]
   Double_t        Muon_d0[6];   //[nMuon]
   Double_t        Muon_dsz[6];   //[nMuon]
   Double_t        Muon_dz[6];   //[nMuon]
   Double_t        Muon_dxyBS[6];   //[nMuon]
   Double_t        Muon_dszBS[6];   //[nMuon]
   Double_t        Muon_dzBS[6];   //[nMuon]
   Double_t        Muon_dxyVTX[6];   //[nMuon]
   Double_t        Muon_dszVTX[6];   //[nMuon]
   Double_t        Muon_dzVTX[6];   //[nMuon]
   Double_t        Muon_dxycktVTX[6];   //[nMuon]
   Double_t        Muon_dszcktVTX[6];   //[nMuon]
   Double_t        Muon_dzcktVTX[6];   //[nMuon]
   Double_t        Muon_vx[6];   //[nMuon]
   Double_t        Muon_vy[6];   //[nMuon]
   Double_t        Muon_vz[6];   //[nMuon]
   vector<double>  *CosAngle;
   vector<double>  *vtxTrkCkt1Pt;
   vector<double>  *vtxTrkCkt2Pt;
   vector<double>  *vtxTrkChi2;
   vector<double>  *vtxTrkProb;
   vector<double>  *vtxTrkNdof;
   vector<double>  *vtxTrkDiE1Pt;
   vector<double>  *vtxTrkDiE2Pt;
   vector<double>  *vtxTrkDiEChi2;
   vector<double>  *vtxTrkDiEProb;
   vector<double>  *vtxTrkDiENdof;
   vector<double>  *vtxTrkEMu1Pt;
   vector<double>  *vtxTrkEMu2Pt;
   vector<double>  *vtxTrkEMuChi2;
   vector<double>  *vtxTrkEMuProb;
   vector<double>  *vtxTrkEMuNdof;
   Int_t           GENnPair;
   Double_t        GENLepton_phi[22];   //[GENnPair]
   Double_t        GENLepton_eta[22];   //[GENnPair]
   Double_t        GENLepton_pT[22];   //[GENnPair]
   Double_t        GENLepton_Px[22];   //[GENnPair]
   Double_t        GENLepton_Py[22];   //[GENnPair]
   Double_t        GENLepton_Pz[22];   //[GENnPair]
   Double_t        GENLepton_mother[22];   //[GENnPair]
   Int_t           GENLepton_charge[22];   //[GENnPair]
   Int_t           GENLepton_status[22];   //[GENnPair]
   Int_t           GENLepton_ID[22];   //[GENnPair]
   Int_t           GENLepton_isPrompt[22];   //[GENnPair]
   Int_t           GENLepton_isPromptFinalState[22];   //[GENnPair]
   Int_t           GENLepton_isTauDecayProduct[22];   //[GENnPair]
   Int_t           GENLepton_isPromptTauDecayProduct[22];   //[GENnPair]
   Int_t           GENLepton_isDirectPromptTauDecayProductFinalState[22];   //[GENnPair]
   Int_t           GENLepton_isHardProcess[22];   //[GENnPair]
   Int_t           GENLepton_isLastCopy[22];   //[GENnPair]
   Int_t           GENLepton_isLastCopyBeforeFSR[22];   //[GENnPair]
   Int_t           GENLepton_isPromptDecayed[22];   //[GENnPair]
   Int_t           GENLepton_isDecayedLeptonHadron[22];   //[GENnPair]
   Int_t           GENLepton_fromHardProcessBeforeFSR[22];   //[GENnPair]
   Int_t           GENLepton_fromHardProcessDecayed[22];   //[GENnPair]
   Int_t           GENLepton_fromHardProcessFinalState[22];   //[GENnPair]
   Int_t           GENLepton_isMostlyLikePythia6Status3[22];   //[GENnPair]
   Double_t        GENEvt_weight;
   Int_t           nGenOthers;
   Double_t        GenOthers_phi[8];   //[nGenOthers]
   Double_t        GenOthers_eta[8];   //[nGenOthers]
   Double_t        GenOthers_pT[8];   //[nGenOthers]
   Double_t        GenOthers_Px[8];   //[nGenOthers]
   Double_t        GenOthers_Py[8];   //[nGenOthers]
   Double_t        GenOthers_Pz[8];   //[nGenOthers]
   Double_t        GenOthers_mother[8];   //[nGenOthers]
   Int_t           GenOthers_charge[8];   //[nGenOthers]
   Int_t           GenOthers_status[8];   //[nGenOthers]
   Int_t           GenOthers_ID[8];   //[nGenOthers]
   Int_t           GenOthers_isPrompt[8];   //[nGenOthers]
   Int_t           GenOthers_isPromptFinalState[8];   //[nGenOthers]
   Int_t           GenOthers_isTauDecayProduct[8];   //[nGenOthers]
   Int_t           GenOthers_isPromptTauDecayProduct[8];   //[nGenOthers]
   Int_t           GenOthers_isDirectPromptTauDecayProductFinalState[8];   //[nGenOthers]
   Int_t           GenOthers_isHardProcess[8];   //[nGenOthers]
   Int_t           GenOthers_isLastCopy[8];   //[nGenOthers]
   Int_t           GenOthers_isLastCopyBeforeFSR[8];   //[nGenOthers]
   Int_t           GenOthers_isPromptDecayed[8];   //[nGenOthers]
   Int_t           GenOthers_isDecayedLeptonHadron[8];   //[nGenOthers]
   Int_t           GenOthers_fromHardProcessBeforeFSR[8];   //[nGenOthers]
   Int_t           GenOthers_fromHardProcessDecayed[8];   //[nGenOthers]
   Int_t           GenOthers_fromHardProcessFinalState[8];   //[nGenOthers]
   Int_t           GenOthers_isMostlyLikePythia6Status3[8];   //[nGenOthers]
   Int_t           nPhotons;
   Int_t           Photon_hasPixelSeed[6];   //[nPhotons]
   Double_t        Photon_pT[6];   //[nPhotons]
   Double_t        Photon_eta[6];   //[nPhotons]
   Double_t        Photon_phi[6];   //[nPhotons]
   Double_t        Photon_etaSC[6];   //[nPhotons]
   Double_t        Photon_phiSC[6];   //[nPhotons]
   Double_t        Photon_HoverE[6];   //[nPhotons]
   Double_t        Photon_Full5x5_SigmaIEtaIEta[6];   //[nPhotons]
   Double_t        Photon_ChIso[6];   //[nPhotons]
   Double_t        Photon_NhIso[6];   //[nPhotons]
   Double_t        Photon_PhIso[6];   //[nPhotons]
   Double_t        Photon_ChIsoWithEA[6];   //[nPhotons]
   Double_t        Photon_NhIsoWithEA[6];   //[nPhotons]
   Double_t        Photon_PhIsoWithEA[6];   //[nPhotons]
   Int_t           nPileUp;
   Double_t        pileUpReweightIn;
   Double_t        pileUpReweight;
   Double_t        pileUpReweightPlus;
   Double_t        pileUpReweightMinus;
   Double_t        pileUpReweightInMuonPhys;
   Double_t        pileUpReweightMuonPhys;
   Double_t        pileUpReweightPlusMuonPhys;
   Double_t        pileUpReweightMinusMuonPhys;
   Double_t        pfMET_pT;
   Double_t        pfMET_phi;
   Double_t        pfMET_Px;
   Double_t        pfMET_Py;
   Double_t        pfMET_SumEt;
   Double_t        pfMET_Type1_pT;
   Double_t        pfMET_Type1_phi;
   Double_t        pfMET_Type1_Px;
   Double_t        pfMET_Type1_Py;
   Double_t        pfMET_Type1_SumEt;

   // List of branches
   TBranch        *b_nTotal;   //!
   TBranch        *b_runNum;   //!
   TBranch        *b_evtNum;   //!
   TBranch        *b_lumiBlock;   //!
   TBranch        *b_nMuon;   //!
   TBranch        *b_Nmuons;   //!
   TBranch        *b_PUweight;   //!
   TBranch        *b_nVertices;   //!
   TBranch        *b_hiBin;   //!
   TBranch        *b_hiHF;   //!
   TBranch        *b_hiHFplus;   //!
   TBranch        *b_hiHFminus;   //!
   TBranch        *b_hiHFplusEta4;   //!
   TBranch        *b_hiHFminusEta4;   //!
   TBranch        *b_hiZDC;   //!
   TBranch        *b_hiZDCplus;   //!
   TBranch        *b_hiZDCminus;   //!
   TBranch        *b_hiHFhit;   //!
   TBranch        *b_hiHFhitPlus;   //!
   TBranch        *b_hiHFhitMinus;   //!
   TBranch        *b_hiET;   //!
   TBranch        *b_hiEE;   //!
   TBranch        *b_hiEB;   //!
   TBranch        *b_hiEEplus;   //!
   TBranch        *b_hiEEminus;   //!
   TBranch        *b_hiNpix;   //!
   TBranch        *b_hiNpixelTracks;   //!
   TBranch        *b_hiNtracks;   //!
   TBranch        *b_hiNtracksPtCut;   //!
   TBranch        *b_hiNtracksEtaCut;   //!
   TBranch        *b_hiNtracksEtaPtCut;   //!
   TBranch        *b_PVtrackSize;   //!
   TBranch        *b_PVchi2;   //!
   TBranch        *b_PVndof;   //!
   TBranch        *b_PVnormalizedChi2;   //!
   TBranch        *b_PVx;   //!
   TBranch        *b_PVy;   //!
   TBranch        *b_PVz;   //!
   TBranch        *b_HLT_ntrig;   //!
   TBranch        *b_HLT_trigType;   //!
   TBranch        *b_HLT_trigFired;   //!
   TBranch        *b_HLT_trigName;   //!
   TBranch        *b_HLT_trigPS;   //!
   TBranch        *b_HLT_trigPt;   //!
   TBranch        *b_HLT_trigEta;   //!
   TBranch        *b_HLT_trigPhi;   //!
   TBranch        *b_Njets;   //!
   TBranch        *b_Jet_pT;   //!
   TBranch        *b_Jet_eta;   //!
   TBranch        *b_Jet_phi;   //!
   TBranch        *b_Jet_Charge;   //!
   TBranch        *b_Jet_Flavor;   //!
   TBranch        *b_Jet_bTag;   //!
   TBranch        *b_Jet_CHfrac;   //!
   TBranch        *b_Jet_NHfrac;   //!
   TBranch        *b_Jet_NHEMfrac;   //!
   TBranch        *b_Jet_CHEMfrac;   //!
   TBranch        *b_Jet_CHmulti;   //!
   TBranch        *b_Jet_NHmulti;   //!
   TBranch        *b_Nelectrons;   //!
   TBranch        *b_Electron_Energy;   //!
   TBranch        *b_Electron_pT;   //!
   TBranch        *b_Electron_eta;   //!
   TBranch        *b_Electron_phi;   //!
   TBranch        *b_Electron_charge;   //!
   TBranch        *b_Electron_gsfpT;   //!
   TBranch        *b_Electron_gsfPx;   //!
   TBranch        *b_Electron_gsfPy;   //!
   TBranch        *b_Electron_gsfPz;   //!
   TBranch        *b_Electron_gsfEta;   //!
   TBranch        *b_Electron_gsfPhi;   //!
   TBranch        *b_Electron_gsfCharge;   //!
   TBranch        *b_Electron_etaSC;   //!
   TBranch        *b_Electron_phiSC;   //!
   TBranch        *b_Electron_etaWidth;   //!
   TBranch        *b_Electron_phiWidth;   //!
   TBranch        *b_Electron_dEtaIn;   //!
   TBranch        *b_Electron_dPhiIn;   //!
   TBranch        *b_Electron_sigmaIEtaIEta;   //!
   TBranch        *b_Electron_HoverE;   //!
   TBranch        *b_Electron_fbrem;   //!
   TBranch        *b_Electron_eOverP;   //!
   TBranch        *b_Electron_InvEminusInvP;   //!
   TBranch        *b_Electron_dxyVTX;   //!
   TBranch        *b_Electron_dzVTX;   //!
   TBranch        *b_Electron_dxy;   //!
   TBranch        *b_Electron_dz;   //!
   TBranch        *b_Electron_dxyBS;   //!
   TBranch        *b_Electron_dzBS;   //!
   TBranch        *b_Electron_chIso03;   //!
   TBranch        *b_Electron_nhIso03;   //!
   TBranch        *b_Electron_phIso03;   //!
   TBranch        *b_Electron_hasConversion;   //!
   TBranch        *b_Electron_mHits;   //!
   TBranch        *b_Electron_EnergySC;   //!
   TBranch        *b_Electron_preEnergySC;   //!
   TBranch        *b_Electron_rawEnergySC;   //!
   TBranch        *b_Electron_etSC;   //!
   TBranch        *b_Electron_E15;   //!
   TBranch        *b_Electron_E25;   //!
   TBranch        *b_Electron_E55;   //!
   TBranch        *b_Electron_ChIso03FromPU;   //!
   TBranch        *b_Electron_RelPFIso_dBeta;   //!
   TBranch        *b_Electron_RelPFIso_Rho;   //!
   TBranch        *b_Electron_passConvVeto;   //!
   TBranch        *b_Electron_passVetoID;   //!
   TBranch        *b_Electron_passLooseID;   //!
   TBranch        *b_Electron_passMediumID;   //!
   TBranch        *b_Electron_passTightID;   //!
   TBranch        *b_Electron_r9;   //!
   TBranch        *b_Electron_ecalDriven;   //!
   TBranch        *b_Electron_ambGsf0Pt;   //!
   TBranch        *b_Electron_ambGsf0Eta;   //!
   TBranch        *b_Electron_ambGsf0Phi;   //!
   TBranch        *b_Electron_ambGsf0Charge;   //!
   TBranch        *b_Electron_ambGsf1Pt;   //!
   TBranch        *b_Electron_ambGsf1Eta;   //!
   TBranch        *b_Electron_ambGsf1Phi;   //!
   TBranch        *b_Electron_ambGsf1Charge;   //!
   TBranch        *b_Electron_ambGsf2Pt;   //!
   TBranch        *b_Electron_ambGsf2Eta;   //!
   TBranch        *b_Electron_ambGsf2Phi;   //!
   TBranch        *b_Electron_ambGsf2Charge;   //!
   TBranch        *b_Electron_ambGsf3Pt;   //!
   TBranch        *b_Electron_ambGsf3Eta;   //!
   TBranch        *b_Electron_ambGsf3Phi;   //!
   TBranch        *b_Electron_ambGsf3Charge;   //!
   TBranch        *b_Muon_muonType;   //!
   TBranch        *b_isPFmuon;   //!
   TBranch        *b_isGLBmuon;   //!
   TBranch        *b_isTRKmuon;   //!
   TBranch        *b_isSTAmuon;   //!
   TBranch        *b_isLooseMuon;   //!
   TBranch        *b_isMediumMuon;   //!
   TBranch        *b_isTightMuon;   //!
   TBranch        *b_isSoftMuon;   //!
   TBranch        *b_isHighPtMuon;   //!
   TBranch        *b_Muon_nTrig;   //!
   TBranch        *b_Muon_triggerObjectType;   //!
   TBranch        *b_Muon_filterName;   //!
   TBranch        *b_Muon_phi;   //!
   TBranch        *b_Muon_eta;   //!
   TBranch        *b_Muon_pT;   //!
   TBranch        *b_Muon_cktpT;   //!
   TBranch        *b_Muon_cktPx;   //!
   TBranch        *b_Muon_cktPy;   //!
   TBranch        *b_Muon_cktPz;   //!
   TBranch        *b_Muon_cktpTError;   //!
   TBranch        *b_Muon_Px;   //!
   TBranch        *b_Muon_Py;   //!
   TBranch        *b_Muon_Pz;   //!
   TBranch        *b_Muon_trkiso;   //!
   TBranch        *b_Muon_hcaliso;   //!
   TBranch        *b_Muon_ecaliso;   //!
   TBranch        *b_Muon_trkisoR05;   //!
   TBranch        *b_Muon_hcalisoR05;   //!
   TBranch        *b_Muon_ecalisoR05;   //!
   TBranch        *b_Muon_Best_pT;   //!
   TBranch        *b_Muon_Best_pTError;   //!
   TBranch        *b_Muon_Best_Px;   //!
   TBranch        *b_Muon_Best_Py;   //!
   TBranch        *b_Muon_Best_Pz;   //!
   TBranch        *b_Muon_Best_eta;   //!
   TBranch        *b_Muon_Best_phi;   //!
   TBranch        *b_Muon_Inner_pT;   //!
   TBranch        *b_Muon_Inner_pTError;   //!
   TBranch        *b_Muon_Inner_Px;   //!
   TBranch        *b_Muon_Inner_Py;   //!
   TBranch        *b_Muon_Inner_Pz;   //!
   TBranch        *b_Muon_Inner_eta;   //!
   TBranch        *b_Muon_Inner_phi;   //!
   TBranch        *b_Muon_Outer_pT;   //!
   TBranch        *b_Muon_Outer_pTError;   //!
   TBranch        *b_Muon_Outer_Px;   //!
   TBranch        *b_Muon_Outer_Py;   //!
   TBranch        *b_Muon_Outer_Pz;   //!
   TBranch        *b_Muon_Outer_eta;   //!
   TBranch        *b_Muon_Outer_phi;   //!
   TBranch        *b_Muon_GLB_pT;   //!
   TBranch        *b_Muon_GLB_pTError;   //!
   TBranch        *b_Muon_GLB_Px;   //!
   TBranch        *b_Muon_GLB_Py;   //!
   TBranch        *b_Muon_GLB_Pz;   //!
   TBranch        *b_Muon_GLB_eta;   //!
   TBranch        *b_Muon_GLB_phi;   //!
   TBranch        *b_Muon_TuneP_pT;   //!
   TBranch        *b_Muon_TuneP_pTError;   //!
   TBranch        *b_Muon_TuneP_Px;   //!
   TBranch        *b_Muon_TuneP_Py;   //!
   TBranch        *b_Muon_TuneP_Pz;   //!
   TBranch        *b_Muon_TuneP_eta;   //!
   TBranch        *b_Muon_TuneP_phi;   //!
   TBranch        *b_Muon_PfChargedHadronIsoR05;   //!
   TBranch        *b_Muon_PfNeutralHadronIsoR05;   //!
   TBranch        *b_Muon_PfGammaIsoR05;   //!
   TBranch        *b_Muon_PfChargedHadronIsoR04;   //!
   TBranch        *b_Muon_PfNeutralHadronIsoR04;   //!
   TBranch        *b_Muon_PfGammaIsoR04;   //!
   TBranch        *b_Muon_PFSumPUIsoR04;   //!
   TBranch        *b_Muon_PfChargedHadronIsoR03;   //!
   TBranch        *b_Muon_PfNeutralHadronIsoR03;   //!
   TBranch        *b_Muon_PfGammaIsoR03;   //!
   TBranch        *b_Muon_PFSumPUIsoR03;   //!
   TBranch        *b_Muon_charge;   //!
   TBranch        *b_Muon_nChambers;   //!
   TBranch        *b_Muon_nMatches;   //!
   TBranch        *b_Muon_stationMask;   //!
   TBranch        *b_Muon_nSegments;   //!
   TBranch        *b_Muon_chi2dof;   //!
   TBranch        *b_Muon_nhits;   //!
   TBranch        *b_Muon_trackerHits;   //!
   TBranch        *b_Muon_trackerLayers;   //!
   TBranch        *b_Muon_pixelHits;   //!
   TBranch        *b_Muon_muonHits;   //!
   TBranch        *b_Muon_qoverp;   //!
   TBranch        *b_Muon_theta;   //!
   TBranch        *b_Muon_lambda;   //!
   TBranch        *b_Muon_dxy;   //!
   TBranch        *b_Muon_d0;   //!
   TBranch        *b_Muon_dsz;   //!
   TBranch        *b_Muon_dz;   //!
   TBranch        *b_Muon_dxyBS;   //!
   TBranch        *b_Muon_dszBS;   //!
   TBranch        *b_Muon_dzBS;   //!
   TBranch        *b_Muon_dxyVTX;   //!
   TBranch        *b_Muon_dszVTX;   //!
   TBranch        *b_Muon_dzVTX;   //!
   TBranch        *b_Muon_dxycktVTX;   //!
   TBranch        *b_Muon_dszcktVTX;   //!
   TBranch        *b_Muon_dzcktVTX;   //!
   TBranch        *b_Muon_vx;   //!
   TBranch        *b_Muon_vy;   //!
   TBranch        *b_Muon_vz;   //!
   TBranch        *b_CosAngle;   //!
   TBranch        *b_vtxTrkCkt1Pt;   //!
   TBranch        *b_vtxTrkCkt2Pt;   //!
   TBranch        *b_vtxTrkChi2;   //!
   TBranch        *b_vtxTrkProb;   //!
   TBranch        *b_vtxTrkNdof;   //!
   TBranch        *b_vtxTrkDiE1Pt;   //!
   TBranch        *b_vtxTrkDiE2Pt;   //!
   TBranch        *b_vtxTrkDiEChi2;   //!
   TBranch        *b_vtxTrkDiEProb;   //!
   TBranch        *b_vtxTrkDiENdof;   //!
   TBranch        *b_vtxTrkEMu1Pt;   //!
   TBranch        *b_vtxTrkEMu2Pt;   //!
   TBranch        *b_vtxTrkEMuChi2;   //!
   TBranch        *b_vtxTrkEMuProb;   //!
   TBranch        *b_vtxTrkEMuNdof;   //!
   TBranch        *b_GENnPair;   //!
   TBranch        *b_GENLepton_phi;   //!
   TBranch        *b_GENLepton_eta;   //!
   TBranch        *b_GENLepton_pT;   //!
   TBranch        *b_GENLepton_Px;   //!
   TBranch        *b_GENLepton_Py;   //!
   TBranch        *b_GENLepton_Pz;   //!
   TBranch        *b_GENLepton_mother;   //!
   TBranch        *b_GENLepton_charge;   //!
   TBranch        *b_GENLepton_status;   //!
   TBranch        *b_GENLepton_ID;   //!
   TBranch        *b_GENLepton_isPrompt;   //!
   TBranch        *b_GENLepton_isPromptFinalState;   //!
   TBranch        *b_GENLepton_isTauDecayProduct;   //!
   TBranch        *b_GENLepton_isPromptTauDecayProduct;   //!
   TBranch        *b_GENLepton_isDirectPromptTauDecayProductFinalState;   //!
   TBranch        *b_GENLepton_isHardProcess;   //!
   TBranch        *b_GENLepton_isLastCopy;   //!
   TBranch        *b_GENLepton_isLastCopyBeforeFSR;   //!
   TBranch        *b_GENLepton_isPromptDecayed;   //!
   TBranch        *b_GENLepton_isDecayedLeptonHadron;   //!
   TBranch        *b_GENLepton_fromHardProcessBeforeFSR;   //!
   TBranch        *b_GENLepton_fromHardProcessDecayed;   //!
   TBranch        *b_GENLepton_fromHardProcessFinalState;   //!
   TBranch        *b_GENLepton_isMostlyLikePythia6Status3;   //!
   TBranch        *b_GENEvt_weight;   //!
   TBranch        *b_nGenOthers;   //!
   TBranch        *b_GenOthers_phi;   //!
   TBranch        *b_GenOthers_eta;   //!
   TBranch        *b_GenOthers_pT;   //!
   TBranch        *b_GenOthers_Px;   //!
   TBranch        *b_GenOthers_Py;   //!
   TBranch        *b_GenOthers_Pz;   //!
   TBranch        *b_GenOthers_mother;   //!
   TBranch        *b_GenOthers_charge;   //!
   TBranch        *b_GenOthers_status;   //!
   TBranch        *b_GenOthers_ID;   //!
   TBranch        *b_GenOthers_isPrompt;   //!
   TBranch        *b_GenOthers_isPromptFinalState;   //!
   TBranch        *b_GenOthers_isTauDecayProduct;   //!
   TBranch        *b_GenOthers_isPromptTauDecayProduct;   //!
   TBranch        *b_GenOthers_isDirectPromptTauDecayProductFinalState;   //!
   TBranch        *b_GenOthers_isHardProcess;   //!
   TBranch        *b_GenOthers_isLastCopy;   //!
   TBranch        *b_GenOthers_isLastCopyBeforeFSR;   //!
   TBranch        *b_GenOthers_isPromptDecayed;   //!
   TBranch        *b_GenOthers_isDecayedLeptonHadron;   //!
   TBranch        *b_GenOthers_fromHardProcessBeforeFSR;   //!
   TBranch        *b_GenOthers_fromHardProcessDecayed;   //!
   TBranch        *b_GenOthers_fromHardProcessFinalState;   //!
   TBranch        *b_GenOthers_isMostlyLikePythia6Status3;   //!
   TBranch        *b_nPhotons;   //!
   TBranch        *b_Photon_hasPixelSeed;   //!
   TBranch        *b_Photon_pT;   //!
   TBranch        *b_Photon_eta;   //!
   TBranch        *b_Photon_phi;   //!
   TBranch        *b_Photon_etaSC;   //!
   TBranch        *b_Photon_phiSC;   //!
   TBranch        *b_Photon_HoverE;   //!
   TBranch        *b_Photon_Full5x5_SigmaIEtaIEta;   //!
   TBranch        *b_Photon_ChIso;   //!
   TBranch        *b_Photon_NhIso;   //!
   TBranch        *b_Photon_PhIso;   //!
   TBranch        *b_Photon_ChIsoWithEA;   //!
   TBranch        *b_Photon_NhIsoWithEA;   //!
   TBranch        *b_Photon_PhIsoWithEA;   //!
   TBranch        *b_nPileUp;   //!
   TBranch        *b_pileUpReweightIn;   //!
   TBranch        *b_pileUpReweight;   //!
   TBranch        *b_pileUpReweightPlus;   //!
   TBranch        *b_pileUpReweightMinus;   //!
   TBranch        *b_pileUpReweightInMuonPhys;   //!
   TBranch        *b_pileUpReweightMuonPhys;   //!
   TBranch        *b_pileUpReweightPlusMuonPhys;   //!
   TBranch        *b_pileUpReweightMinusMuonPhys;   //!
   TBranch        *b_pfMET_pT;   //!
   TBranch        *b_pfMET_phi;   //!
   TBranch        *b_pfMET_Px;   //!
   TBranch        *b_pfMET_Py;   //!
   TBranch        *b_pfMET_SumEt;   //!
   TBranch        *b_pfMET_Type1_pT;   //!
   TBranch        *b_pfMET_Type1_phi;   //!
   TBranch        *b_pfMET_Type1_Px;   //!
   TBranch        *b_pfMET_Type1_Py;   //!
   TBranch        *b_pfMET_Type1_SumEt;   //!

   MakeHFweight(TTree *tree=0);
   virtual ~MakeHFweight();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(HistPack& thepack);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef MakeHFweight_cxx
MakeHFweight::MakeHFweight(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("histograms/tree_DY50100.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("histograms/tree_DY50100.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("histograms/tree_DY50100.root:/recoTree");
      dir->GetObject("DYTree",tree);

   }
   Init(tree);
}

MakeHFweight::~MakeHFweight()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MakeHFweight::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MakeHFweight::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void MakeHFweight::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   HLT_trigName = 0;
   HLT_trigPS = 0;
   CosAngle = 0;
   vtxTrkCkt1Pt = 0;
   vtxTrkCkt2Pt = 0;
   vtxTrkChi2 = 0;
   vtxTrkProb = 0;
   vtxTrkNdof = 0;
   vtxTrkDiE1Pt = 0;
   vtxTrkDiE2Pt = 0;
   vtxTrkDiEChi2 = 0;
   vtxTrkDiEProb = 0;
   vtxTrkDiENdof = 0;
   vtxTrkEMu1Pt = 0;
   vtxTrkEMu2Pt = 0;
   vtxTrkEMuChi2 = 0;
   vtxTrkEMuProb = 0;
   vtxTrkEMuNdof = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("nTotal", &nTotal, &b_nTotal);
   fChain->SetBranchAddress("runNum", &runNum, &b_runNum);
   fChain->SetBranchAddress("evtNum", &evtNum, &b_evtNum);
   fChain->SetBranchAddress("lumiBlock", &lumiBlock, &b_lumiBlock);
   fChain->SetBranchAddress("nMuon", &nMuon, &b_nMuon);
   fChain->SetBranchAddress("Nmuons", &Nmuons, &b_Nmuons);
   fChain->SetBranchAddress("PUweight", &PUweight, &b_PUweight);
   fChain->SetBranchAddress("nVertices", &nVertices, &b_nVertices);
   fChain->SetBranchAddress("hiBin", &hiBin, &b_hiBin);
   fChain->SetBranchAddress("hiHF", &hiHF, &b_hiHF);
   fChain->SetBranchAddress("hiHFplus", &hiHFplus, &b_hiHFplus);
   fChain->SetBranchAddress("hiHFminus", &hiHFminus, &b_hiHFminus);
   fChain->SetBranchAddress("hiHFplusEta4", &hiHFplusEta4, &b_hiHFplusEta4);
   fChain->SetBranchAddress("hiHFminusEta4", &hiHFminusEta4, &b_hiHFminusEta4);
   fChain->SetBranchAddress("hiZDC", &hiZDC, &b_hiZDC);
   fChain->SetBranchAddress("hiZDCplus", &hiZDCplus, &b_hiZDCplus);
   fChain->SetBranchAddress("hiZDCminus", &hiZDCminus, &b_hiZDCminus);
   fChain->SetBranchAddress("hiHFhit", &hiHFhit, &b_hiHFhit);
   fChain->SetBranchAddress("hiHFhitPlus", &hiHFhitPlus, &b_hiHFhitPlus);
   fChain->SetBranchAddress("hiHFhitMinus", &hiHFhitMinus, &b_hiHFhitMinus);
   fChain->SetBranchAddress("hiET", &hiET, &b_hiET);
   fChain->SetBranchAddress("hiEE", &hiEE, &b_hiEE);
   fChain->SetBranchAddress("hiEB", &hiEB, &b_hiEB);
   fChain->SetBranchAddress("hiEEplus", &hiEEplus, &b_hiEEplus);
   fChain->SetBranchAddress("hiEEminus", &hiEEminus, &b_hiEEminus);
   fChain->SetBranchAddress("hiNpix", &hiNpix, &b_hiNpix);
   fChain->SetBranchAddress("hiNpixelTracks", &hiNpixelTracks, &b_hiNpixelTracks);
   fChain->SetBranchAddress("hiNtracks", &hiNtracks, &b_hiNtracks);
   fChain->SetBranchAddress("hiNtracksPtCut", &hiNtracksPtCut, &b_hiNtracksPtCut);
   fChain->SetBranchAddress("hiNtracksEtaCut", &hiNtracksEtaCut, &b_hiNtracksEtaCut);
   fChain->SetBranchAddress("hiNtracksEtaPtCut", &hiNtracksEtaPtCut, &b_hiNtracksEtaPtCut);
   fChain->SetBranchAddress("PVtrackSize", &PVtrackSize, &b_PVtrackSize);
   fChain->SetBranchAddress("PVchi2", &PVchi2, &b_PVchi2);
   fChain->SetBranchAddress("PVndof", &PVndof, &b_PVndof);
   fChain->SetBranchAddress("PVnormalizedChi2", &PVnormalizedChi2, &b_PVnormalizedChi2);
   fChain->SetBranchAddress("PVx", &PVx, &b_PVx);
   fChain->SetBranchAddress("PVy", &PVy, &b_PVy);
   fChain->SetBranchAddress("PVz", &PVz, &b_PVz);
   fChain->SetBranchAddress("HLT_ntrig", &HLT_ntrig, &b_HLT_ntrig);
   fChain->SetBranchAddress("HLT_trigType", HLT_trigType, &b_HLT_trigType);
   fChain->SetBranchAddress("HLT_trigFired", HLT_trigFired, &b_HLT_trigFired);
   fChain->SetBranchAddress("HLT_trigName", &HLT_trigName, &b_HLT_trigName);
   fChain->SetBranchAddress("HLT_trigPS", &HLT_trigPS, &b_HLT_trigPS);
   fChain->SetBranchAddress("HLT_trigPt", HLT_trigPt, &b_HLT_trigPt);
   fChain->SetBranchAddress("HLT_trigEta", HLT_trigEta, &b_HLT_trigEta);
   fChain->SetBranchAddress("HLT_trigPhi", HLT_trigPhi, &b_HLT_trigPhi);
   fChain->SetBranchAddress("Njets", &Njets, &b_Njets);
   fChain->SetBranchAddress("Jet_pT", Jet_pT, &b_Jet_pT);
   fChain->SetBranchAddress("Jet_eta", Jet_eta, &b_Jet_eta);
   fChain->SetBranchAddress("Jet_phi", Jet_phi, &b_Jet_phi);
   fChain->SetBranchAddress("Jet_Charge", Jet_Charge, &b_Jet_Charge);
   fChain->SetBranchAddress("Jet_Flavor", Jet_Flavor, &b_Jet_Flavor);
   fChain->SetBranchAddress("Jet_bTag", Jet_bTag, &b_Jet_bTag);
   fChain->SetBranchAddress("Jet_CHfrac", Jet_CHfrac, &b_Jet_CHfrac);
   fChain->SetBranchAddress("Jet_NHfrac", Jet_NHfrac, &b_Jet_NHfrac);
   fChain->SetBranchAddress("Jet_NHEMfrac", Jet_NHEMfrac, &b_Jet_NHEMfrac);
   fChain->SetBranchAddress("Jet_CHEMfrac", Jet_CHEMfrac, &b_Jet_CHEMfrac);
   fChain->SetBranchAddress("Jet_CHmulti", Jet_CHmulti, &b_Jet_CHmulti);
   fChain->SetBranchAddress("Jet_NHmulti", Jet_NHmulti, &b_Jet_NHmulti);
   fChain->SetBranchAddress("Nelectrons", &Nelectrons, &b_Nelectrons);
   fChain->SetBranchAddress("Electron_Energy", Electron_Energy, &b_Electron_Energy);
   fChain->SetBranchAddress("Electron_pT", Electron_pT, &b_Electron_pT);
   fChain->SetBranchAddress("Electron_eta", Electron_eta, &b_Electron_eta);
   fChain->SetBranchAddress("Electron_phi", Electron_phi, &b_Electron_phi);
   fChain->SetBranchAddress("Electron_charge", Electron_charge, &b_Electron_charge);
   fChain->SetBranchAddress("Electron_gsfpT", Electron_gsfpT, &b_Electron_gsfpT);
   fChain->SetBranchAddress("Electron_gsfPx", Electron_gsfPx, &b_Electron_gsfPx);
   fChain->SetBranchAddress("Electron_gsfPy", Electron_gsfPy, &b_Electron_gsfPy);
   fChain->SetBranchAddress("Electron_gsfPz", Electron_gsfPz, &b_Electron_gsfPz);
   fChain->SetBranchAddress("Electron_gsfEta", Electron_gsfEta, &b_Electron_gsfEta);
   fChain->SetBranchAddress("Electron_gsfPhi", Electron_gsfPhi, &b_Electron_gsfPhi);
   fChain->SetBranchAddress("Electron_gsfCharge", Electron_gsfCharge, &b_Electron_gsfCharge);
   fChain->SetBranchAddress("Electron_etaSC", Electron_etaSC, &b_Electron_etaSC);
   fChain->SetBranchAddress("Electron_phiSC", Electron_phiSC, &b_Electron_phiSC);
   fChain->SetBranchAddress("Electron_etaWidth", Electron_etaWidth, &b_Electron_etaWidth);
   fChain->SetBranchAddress("Electron_phiWidth", Electron_phiWidth, &b_Electron_phiWidth);
   fChain->SetBranchAddress("Electron_dEtaIn", Electron_dEtaIn, &b_Electron_dEtaIn);
   fChain->SetBranchAddress("Electron_dPhiIn", Electron_dPhiIn, &b_Electron_dPhiIn);
   fChain->SetBranchAddress("Electron_sigmaIEtaIEta", Electron_sigmaIEtaIEta, &b_Electron_sigmaIEtaIEta);
   fChain->SetBranchAddress("Electron_HoverE", Electron_HoverE, &b_Electron_HoverE);
   fChain->SetBranchAddress("Electron_fbrem", Electron_fbrem, &b_Electron_fbrem);
   fChain->SetBranchAddress("Electron_eOverP", Electron_eOverP, &b_Electron_eOverP);
   fChain->SetBranchAddress("Electron_InvEminusInvP", Electron_InvEminusInvP, &b_Electron_InvEminusInvP);
   fChain->SetBranchAddress("Electron_dxyVTX", Electron_dxyVTX, &b_Electron_dxyVTX);
   fChain->SetBranchAddress("Electron_dzVTX", Electron_dzVTX, &b_Electron_dzVTX);
   fChain->SetBranchAddress("Electron_dxy", Electron_dxy, &b_Electron_dxy);
   fChain->SetBranchAddress("Electron_dz", Electron_dz, &b_Electron_dz);
   fChain->SetBranchAddress("Electron_dxyBS", Electron_dxyBS, &b_Electron_dxyBS);
   fChain->SetBranchAddress("Electron_dzBS", Electron_dzBS, &b_Electron_dzBS);
   fChain->SetBranchAddress("Electron_chIso03", Electron_chIso03, &b_Electron_chIso03);
   fChain->SetBranchAddress("Electron_nhIso03", Electron_nhIso03, &b_Electron_nhIso03);
   fChain->SetBranchAddress("Electron_phIso03", Electron_phIso03, &b_Electron_phIso03);
   fChain->SetBranchAddress("Electron_hasConversion", Electron_hasConversion, &b_Electron_hasConversion);
   fChain->SetBranchAddress("Electron_mHits", Electron_mHits, &b_Electron_mHits);
   fChain->SetBranchAddress("Electron_EnergySC", Electron_EnergySC, &b_Electron_EnergySC);
   fChain->SetBranchAddress("Electron_preEnergySC", Electron_preEnergySC, &b_Electron_preEnergySC);
   fChain->SetBranchAddress("Electron_rawEnergySC", Electron_rawEnergySC, &b_Electron_rawEnergySC);
   fChain->SetBranchAddress("Electron_etSC", Electron_etSC, &b_Electron_etSC);
   fChain->SetBranchAddress("Electron_E15", Electron_E15, &b_Electron_E15);
   fChain->SetBranchAddress("Electron_E25", Electron_E25, &b_Electron_E25);
   fChain->SetBranchAddress("Electron_E55", Electron_E55, &b_Electron_E55);
   fChain->SetBranchAddress("Electron_ChIso03FromPU", Electron_ChIso03FromPU, &b_Electron_ChIso03FromPU);
   fChain->SetBranchAddress("Electron_RelPFIso_dBeta", Electron_RelPFIso_dBeta, &b_Electron_RelPFIso_dBeta);
   fChain->SetBranchAddress("Electron_RelPFIso_Rho", Electron_RelPFIso_Rho, &b_Electron_RelPFIso_Rho);
   fChain->SetBranchAddress("Electron_passConvVeto", Electron_passConvVeto, &b_Electron_passConvVeto);
   fChain->SetBranchAddress("Electron_passVetoID", Electron_passVetoID, &b_Electron_passVetoID);
   fChain->SetBranchAddress("Electron_passLooseID", Electron_passLooseID, &b_Electron_passLooseID);
   fChain->SetBranchAddress("Electron_passMediumID", Electron_passMediumID, &b_Electron_passMediumID);
   fChain->SetBranchAddress("Electron_passTightID", Electron_passTightID, &b_Electron_passTightID);
   fChain->SetBranchAddress("Electron_r9", Electron_r9, &b_Electron_r9);
   fChain->SetBranchAddress("Electron_ecalDriven", Electron_ecalDriven, &b_Electron_ecalDriven);
   fChain->SetBranchAddress("Electron_ambGsf0Pt", Electron_ambGsf0Pt, &b_Electron_ambGsf0Pt);
   fChain->SetBranchAddress("Electron_ambGsf0Eta", Electron_ambGsf0Eta, &b_Electron_ambGsf0Eta);
   fChain->SetBranchAddress("Electron_ambGsf0Phi", Electron_ambGsf0Phi, &b_Electron_ambGsf0Phi);
   fChain->SetBranchAddress("Electron_ambGsf0Charge", Electron_ambGsf0Charge, &b_Electron_ambGsf0Charge);
   fChain->SetBranchAddress("Electron_ambGsf1Pt", Electron_ambGsf1Pt, &b_Electron_ambGsf1Pt);
   fChain->SetBranchAddress("Electron_ambGsf1Eta", Electron_ambGsf1Eta, &b_Electron_ambGsf1Eta);
   fChain->SetBranchAddress("Electron_ambGsf1Phi", Electron_ambGsf1Phi, &b_Electron_ambGsf1Phi);
   fChain->SetBranchAddress("Electron_ambGsf1Charge", Electron_ambGsf1Charge, &b_Electron_ambGsf1Charge);
   fChain->SetBranchAddress("Electron_ambGsf2Pt", Electron_ambGsf2Pt, &b_Electron_ambGsf2Pt);
   fChain->SetBranchAddress("Electron_ambGsf2Eta", Electron_ambGsf2Eta, &b_Electron_ambGsf2Eta);
   fChain->SetBranchAddress("Electron_ambGsf2Phi", Electron_ambGsf2Phi, &b_Electron_ambGsf2Phi);
   fChain->SetBranchAddress("Electron_ambGsf2Charge", Electron_ambGsf2Charge, &b_Electron_ambGsf2Charge);
   fChain->SetBranchAddress("Electron_ambGsf3Pt", Electron_ambGsf3Pt, &b_Electron_ambGsf3Pt);
   fChain->SetBranchAddress("Electron_ambGsf3Eta", Electron_ambGsf3Eta, &b_Electron_ambGsf3Eta);
   fChain->SetBranchAddress("Electron_ambGsf3Phi", Electron_ambGsf3Phi, &b_Electron_ambGsf3Phi);
   fChain->SetBranchAddress("Electron_ambGsf3Charge", Electron_ambGsf3Charge, &b_Electron_ambGsf3Charge);
   fChain->SetBranchAddress("Muon_muonType", Muon_muonType, &b_Muon_muonType);
   fChain->SetBranchAddress("isPFmuon", isPFmuon, &b_isPFmuon);
   fChain->SetBranchAddress("isGLBmuon", isGLBmuon, &b_isGLBmuon);
   fChain->SetBranchAddress("isTRKmuon", isTRKmuon, &b_isTRKmuon);
   fChain->SetBranchAddress("isSTAmuon", isSTAmuon, &b_isSTAmuon);
   fChain->SetBranchAddress("isLooseMuon", isLooseMuon, &b_isLooseMuon);
   fChain->SetBranchAddress("isMediumMuon", isMediumMuon, &b_isMediumMuon);
   fChain->SetBranchAddress("isTightMuon", isTightMuon, &b_isTightMuon);
   fChain->SetBranchAddress("isSoftMuon", isSoftMuon, &b_isSoftMuon);
   fChain->SetBranchAddress("isHighPtMuon", isHighPtMuon, &b_isHighPtMuon);
   fChain->SetBranchAddress("Muon_nTrig", Muon_nTrig, &b_Muon_nTrig);
   fChain->SetBranchAddress("Muon_triggerObjectType", Muon_triggerObjectType, &b_Muon_triggerObjectType);
   fChain->SetBranchAddress("Muon_filterName", Muon_filterName, &b_Muon_filterName);
   fChain->SetBranchAddress("Muon_phi", Muon_phi, &b_Muon_phi);
   fChain->SetBranchAddress("Muon_eta", Muon_eta, &b_Muon_eta);
   fChain->SetBranchAddress("Muon_pT", Muon_pT, &b_Muon_pT);
   fChain->SetBranchAddress("Muon_cktpT", Muon_cktpT, &b_Muon_cktpT);
   fChain->SetBranchAddress("Muon_cktPx", Muon_cktPx, &b_Muon_cktPx);
   fChain->SetBranchAddress("Muon_cktPy", Muon_cktPy, &b_Muon_cktPy);
   fChain->SetBranchAddress("Muon_cktPz", Muon_cktPz, &b_Muon_cktPz);
   fChain->SetBranchAddress("Muon_cktpTError", Muon_cktpTError, &b_Muon_cktpTError);
   fChain->SetBranchAddress("Muon_Px", Muon_Px, &b_Muon_Px);
   fChain->SetBranchAddress("Muon_Py", Muon_Py, &b_Muon_Py);
   fChain->SetBranchAddress("Muon_Pz", Muon_Pz, &b_Muon_Pz);
   fChain->SetBranchAddress("Muon_trkiso", Muon_trkiso, &b_Muon_trkiso);
   fChain->SetBranchAddress("Muon_hcaliso", Muon_hcaliso, &b_Muon_hcaliso);
   fChain->SetBranchAddress("Muon_ecaliso", Muon_ecaliso, &b_Muon_ecaliso);
   fChain->SetBranchAddress("Muon_trkisoR05", Muon_trkisoR05, &b_Muon_trkisoR05);
   fChain->SetBranchAddress("Muon_hcalisoR05", Muon_hcalisoR05, &b_Muon_hcalisoR05);
   fChain->SetBranchAddress("Muon_ecalisoR05", Muon_ecalisoR05, &b_Muon_ecalisoR05);
   fChain->SetBranchAddress("Muon_Best_pT", Muon_Best_pT, &b_Muon_Best_pT);
   fChain->SetBranchAddress("Muon_Best_pTError", Muon_Best_pTError, &b_Muon_Best_pTError);
   fChain->SetBranchAddress("Muon_Best_Px", Muon_Best_Px, &b_Muon_Best_Px);
   fChain->SetBranchAddress("Muon_Best_Py", Muon_Best_Py, &b_Muon_Best_Py);
   fChain->SetBranchAddress("Muon_Best_Pz", Muon_Best_Pz, &b_Muon_Best_Pz);
   fChain->SetBranchAddress("Muon_Best_eta", Muon_Best_eta, &b_Muon_Best_eta);
   fChain->SetBranchAddress("Muon_Best_phi", Muon_Best_phi, &b_Muon_Best_phi);
   fChain->SetBranchAddress("Muon_Inner_pT", Muon_Inner_pT, &b_Muon_Inner_pT);
   fChain->SetBranchAddress("Muon_Inner_pTError", Muon_Inner_pTError, &b_Muon_Inner_pTError);
   fChain->SetBranchAddress("Muon_Inner_Px", Muon_Inner_Px, &b_Muon_Inner_Px);
   fChain->SetBranchAddress("Muon_Inner_Py", Muon_Inner_Py, &b_Muon_Inner_Py);
   fChain->SetBranchAddress("Muon_Inner_Pz", Muon_Inner_Pz, &b_Muon_Inner_Pz);
   fChain->SetBranchAddress("Muon_Inner_eta", Muon_Inner_eta, &b_Muon_Inner_eta);
   fChain->SetBranchAddress("Muon_Inner_phi", Muon_Inner_phi, &b_Muon_Inner_phi);
   fChain->SetBranchAddress("Muon_Outer_pT", Muon_Outer_pT, &b_Muon_Outer_pT);
   fChain->SetBranchAddress("Muon_Outer_pTError", Muon_Outer_pTError, &b_Muon_Outer_pTError);
   fChain->SetBranchAddress("Muon_Outer_Px", Muon_Outer_Px, &b_Muon_Outer_Px);
   fChain->SetBranchAddress("Muon_Outer_Py", Muon_Outer_Py, &b_Muon_Outer_Py);
   fChain->SetBranchAddress("Muon_Outer_Pz", Muon_Outer_Pz, &b_Muon_Outer_Pz);
   fChain->SetBranchAddress("Muon_Outer_eta", Muon_Outer_eta, &b_Muon_Outer_eta);
   fChain->SetBranchAddress("Muon_Outer_phi", Muon_Outer_phi, &b_Muon_Outer_phi);
   fChain->SetBranchAddress("Muon_GLB_pT", Muon_GLB_pT, &b_Muon_GLB_pT);
   fChain->SetBranchAddress("Muon_GLB_pTError", Muon_GLB_pTError, &b_Muon_GLB_pTError);
   fChain->SetBranchAddress("Muon_GLB_Px", Muon_GLB_Px, &b_Muon_GLB_Px);
   fChain->SetBranchAddress("Muon_GLB_Py", Muon_GLB_Py, &b_Muon_GLB_Py);
   fChain->SetBranchAddress("Muon_GLB_Pz", Muon_GLB_Pz, &b_Muon_GLB_Pz);
   fChain->SetBranchAddress("Muon_GLB_eta", Muon_GLB_eta, &b_Muon_GLB_eta);
   fChain->SetBranchAddress("Muon_GLB_phi", Muon_GLB_phi, &b_Muon_GLB_phi);
   fChain->SetBranchAddress("Muon_TuneP_pT", Muon_TuneP_pT, &b_Muon_TuneP_pT);
   fChain->SetBranchAddress("Muon_TuneP_pTError", Muon_TuneP_pTError, &b_Muon_TuneP_pTError);
   fChain->SetBranchAddress("Muon_TuneP_Px", Muon_TuneP_Px, &b_Muon_TuneP_Px);
   fChain->SetBranchAddress("Muon_TuneP_Py", Muon_TuneP_Py, &b_Muon_TuneP_Py);
   fChain->SetBranchAddress("Muon_TuneP_Pz", Muon_TuneP_Pz, &b_Muon_TuneP_Pz);
   fChain->SetBranchAddress("Muon_TuneP_eta", Muon_TuneP_eta, &b_Muon_TuneP_eta);
   fChain->SetBranchAddress("Muon_TuneP_phi", Muon_TuneP_phi, &b_Muon_TuneP_phi);
   fChain->SetBranchAddress("Muon_PfChargedHadronIsoR05", Muon_PfChargedHadronIsoR05, &b_Muon_PfChargedHadronIsoR05);
   fChain->SetBranchAddress("Muon_PfNeutralHadronIsoR05", Muon_PfNeutralHadronIsoR05, &b_Muon_PfNeutralHadronIsoR05);
   fChain->SetBranchAddress("Muon_PfGammaIsoR05", Muon_PfGammaIsoR05, &b_Muon_PfGammaIsoR05);
   fChain->SetBranchAddress("Muon_PfChargedHadronIsoR04", Muon_PfChargedHadronIsoR04, &b_Muon_PfChargedHadronIsoR04);
   fChain->SetBranchAddress("Muon_PfNeutralHadronIsoR04", Muon_PfNeutralHadronIsoR04, &b_Muon_PfNeutralHadronIsoR04);
   fChain->SetBranchAddress("Muon_PfGammaIsoR04", Muon_PfGammaIsoR04, &b_Muon_PfGammaIsoR04);
   fChain->SetBranchAddress("Muon_PFSumPUIsoR04", Muon_PFSumPUIsoR04, &b_Muon_PFSumPUIsoR04);
   fChain->SetBranchAddress("Muon_PfChargedHadronIsoR03", Muon_PfChargedHadronIsoR03, &b_Muon_PfChargedHadronIsoR03);
   fChain->SetBranchAddress("Muon_PfNeutralHadronIsoR03", Muon_PfNeutralHadronIsoR03, &b_Muon_PfNeutralHadronIsoR03);
   fChain->SetBranchAddress("Muon_PfGammaIsoR03", Muon_PfGammaIsoR03, &b_Muon_PfGammaIsoR03);
   fChain->SetBranchAddress("Muon_PFSumPUIsoR03", Muon_PFSumPUIsoR03, &b_Muon_PFSumPUIsoR03);
   fChain->SetBranchAddress("Muon_charge", Muon_charge, &b_Muon_charge);
   fChain->SetBranchAddress("Muon_nChambers", Muon_nChambers, &b_Muon_nChambers);
   fChain->SetBranchAddress("Muon_nMatches", Muon_nMatches, &b_Muon_nMatches);
   fChain->SetBranchAddress("Muon_stationMask", Muon_stationMask, &b_Muon_stationMask);
   fChain->SetBranchAddress("Muon_nSegments", Muon_nSegments, &b_Muon_nSegments);
   fChain->SetBranchAddress("Muon_chi2dof", Muon_chi2dof, &b_Muon_chi2dof);
   fChain->SetBranchAddress("Muon_nhits", Muon_nhits, &b_Muon_nhits);
   fChain->SetBranchAddress("Muon_trackerHits", Muon_trackerHits, &b_Muon_trackerHits);
   fChain->SetBranchAddress("Muon_trackerLayers", Muon_trackerLayers, &b_Muon_trackerLayers);
   fChain->SetBranchAddress("Muon_pixelHits", Muon_pixelHits, &b_Muon_pixelHits);
   fChain->SetBranchAddress("Muon_muonHits", Muon_muonHits, &b_Muon_muonHits);
   fChain->SetBranchAddress("Muon_qoverp", Muon_qoverp, &b_Muon_qoverp);
   fChain->SetBranchAddress("Muon_theta", Muon_theta, &b_Muon_theta);
   fChain->SetBranchAddress("Muon_lambda", Muon_lambda, &b_Muon_lambda);
   fChain->SetBranchAddress("Muon_dxy", Muon_dxy, &b_Muon_dxy);
   fChain->SetBranchAddress("Muon_d0", Muon_d0, &b_Muon_d0);
   fChain->SetBranchAddress("Muon_dsz", Muon_dsz, &b_Muon_dsz);
   fChain->SetBranchAddress("Muon_dz", Muon_dz, &b_Muon_dz);
   fChain->SetBranchAddress("Muon_dxyBS", Muon_dxyBS, &b_Muon_dxyBS);
   fChain->SetBranchAddress("Muon_dszBS", Muon_dszBS, &b_Muon_dszBS);
   fChain->SetBranchAddress("Muon_dzBS", Muon_dzBS, &b_Muon_dzBS);
   fChain->SetBranchAddress("Muon_dxyVTX", Muon_dxyVTX, &b_Muon_dxyVTX);
   fChain->SetBranchAddress("Muon_dszVTX", Muon_dszVTX, &b_Muon_dszVTX);
   fChain->SetBranchAddress("Muon_dzVTX", Muon_dzVTX, &b_Muon_dzVTX);
   fChain->SetBranchAddress("Muon_dxycktVTX", Muon_dxycktVTX, &b_Muon_dxycktVTX);
   fChain->SetBranchAddress("Muon_dszcktVTX", Muon_dszcktVTX, &b_Muon_dszcktVTX);
   fChain->SetBranchAddress("Muon_dzcktVTX", Muon_dzcktVTX, &b_Muon_dzcktVTX);
   fChain->SetBranchAddress("Muon_vx", Muon_vx, &b_Muon_vx);
   fChain->SetBranchAddress("Muon_vy", Muon_vy, &b_Muon_vy);
   fChain->SetBranchAddress("Muon_vz", Muon_vz, &b_Muon_vz);
   fChain->SetBranchAddress("CosAngle", &CosAngle, &b_CosAngle);
   fChain->SetBranchAddress("vtxTrkCkt1Pt", &vtxTrkCkt1Pt, &b_vtxTrkCkt1Pt);
   fChain->SetBranchAddress("vtxTrkCkt2Pt", &vtxTrkCkt2Pt, &b_vtxTrkCkt2Pt);
   fChain->SetBranchAddress("vtxTrkChi2", &vtxTrkChi2, &b_vtxTrkChi2);
   fChain->SetBranchAddress("vtxTrkProb", &vtxTrkProb, &b_vtxTrkProb);
   fChain->SetBranchAddress("vtxTrkNdof", &vtxTrkNdof, &b_vtxTrkNdof);
   fChain->SetBranchAddress("vtxTrkDiE1Pt", &vtxTrkDiE1Pt, &b_vtxTrkDiE1Pt);
   fChain->SetBranchAddress("vtxTrkDiE2Pt", &vtxTrkDiE2Pt, &b_vtxTrkDiE2Pt);
   fChain->SetBranchAddress("vtxTrkDiEChi2", &vtxTrkDiEChi2, &b_vtxTrkDiEChi2);
   fChain->SetBranchAddress("vtxTrkDiEProb", &vtxTrkDiEProb, &b_vtxTrkDiEProb);
   fChain->SetBranchAddress("vtxTrkDiENdof", &vtxTrkDiENdof, &b_vtxTrkDiENdof);
   fChain->SetBranchAddress("vtxTrkEMu1Pt", &vtxTrkEMu1Pt, &b_vtxTrkEMu1Pt);
   fChain->SetBranchAddress("vtxTrkEMu2Pt", &vtxTrkEMu2Pt, &b_vtxTrkEMu2Pt);
   fChain->SetBranchAddress("vtxTrkEMuChi2", &vtxTrkEMuChi2, &b_vtxTrkEMuChi2);
   fChain->SetBranchAddress("vtxTrkEMuProb", &vtxTrkEMuProb, &b_vtxTrkEMuProb);
   fChain->SetBranchAddress("vtxTrkEMuNdof", &vtxTrkEMuNdof, &b_vtxTrkEMuNdof);
   fChain->SetBranchAddress("GENnPair", &GENnPair, &b_GENnPair);
   fChain->SetBranchAddress("GENLepton_phi", GENLepton_phi, &b_GENLepton_phi);
   fChain->SetBranchAddress("GENLepton_eta", GENLepton_eta, &b_GENLepton_eta);
   fChain->SetBranchAddress("GENLepton_pT", GENLepton_pT, &b_GENLepton_pT);
   fChain->SetBranchAddress("GENLepton_Px", GENLepton_Px, &b_GENLepton_Px);
   fChain->SetBranchAddress("GENLepton_Py", GENLepton_Py, &b_GENLepton_Py);
   fChain->SetBranchAddress("GENLepton_Pz", GENLepton_Pz, &b_GENLepton_Pz);
   fChain->SetBranchAddress("GENLepton_mother", GENLepton_mother, &b_GENLepton_mother);
   fChain->SetBranchAddress("GENLepton_charge", GENLepton_charge, &b_GENLepton_charge);
   fChain->SetBranchAddress("GENLepton_status", GENLepton_status, &b_GENLepton_status);
   fChain->SetBranchAddress("GENLepton_ID", GENLepton_ID, &b_GENLepton_ID);
   fChain->SetBranchAddress("GENLepton_isPrompt", GENLepton_isPrompt, &b_GENLepton_isPrompt);
   fChain->SetBranchAddress("GENLepton_isPromptFinalState", GENLepton_isPromptFinalState, &b_GENLepton_isPromptFinalState);
   fChain->SetBranchAddress("GENLepton_isTauDecayProduct", GENLepton_isTauDecayProduct, &b_GENLepton_isTauDecayProduct);
   fChain->SetBranchAddress("GENLepton_isPromptTauDecayProduct", GENLepton_isPromptTauDecayProduct, &b_GENLepton_isPromptTauDecayProduct);
   fChain->SetBranchAddress("GENLepton_isDirectPromptTauDecayProductFinalState", GENLepton_isDirectPromptTauDecayProductFinalState, &b_GENLepton_isDirectPromptTauDecayProductFinalState);
   fChain->SetBranchAddress("GENLepton_isHardProcess", GENLepton_isHardProcess, &b_GENLepton_isHardProcess);
   fChain->SetBranchAddress("GENLepton_isLastCopy", GENLepton_isLastCopy, &b_GENLepton_isLastCopy);
   fChain->SetBranchAddress("GENLepton_isLastCopyBeforeFSR", GENLepton_isLastCopyBeforeFSR, &b_GENLepton_isLastCopyBeforeFSR);
   fChain->SetBranchAddress("GENLepton_isPromptDecayed", GENLepton_isPromptDecayed, &b_GENLepton_isPromptDecayed);
   fChain->SetBranchAddress("GENLepton_isDecayedLeptonHadron", GENLepton_isDecayedLeptonHadron, &b_GENLepton_isDecayedLeptonHadron);
   fChain->SetBranchAddress("GENLepton_fromHardProcessBeforeFSR", GENLepton_fromHardProcessBeforeFSR, &b_GENLepton_fromHardProcessBeforeFSR);
   fChain->SetBranchAddress("GENLepton_fromHardProcessDecayed", GENLepton_fromHardProcessDecayed, &b_GENLepton_fromHardProcessDecayed);
   fChain->SetBranchAddress("GENLepton_fromHardProcessFinalState", GENLepton_fromHardProcessFinalState, &b_GENLepton_fromHardProcessFinalState);
   fChain->SetBranchAddress("GENLepton_isMostlyLikePythia6Status3", GENLepton_isMostlyLikePythia6Status3, &b_GENLepton_isMostlyLikePythia6Status3);
   fChain->SetBranchAddress("GENEvt_weight", &GENEvt_weight, &b_GENEvt_weight);
   fChain->SetBranchAddress("nGenOthers", &nGenOthers, &b_nGenOthers);
   fChain->SetBranchAddress("GenOthers_phi", GenOthers_phi, &b_GenOthers_phi);
   fChain->SetBranchAddress("GenOthers_eta", GenOthers_eta, &b_GenOthers_eta);
   fChain->SetBranchAddress("GenOthers_pT", GenOthers_pT, &b_GenOthers_pT);
   fChain->SetBranchAddress("GenOthers_Px", GenOthers_Px, &b_GenOthers_Px);
   fChain->SetBranchAddress("GenOthers_Py", GenOthers_Py, &b_GenOthers_Py);
   fChain->SetBranchAddress("GenOthers_Pz", GenOthers_Pz, &b_GenOthers_Pz);
   fChain->SetBranchAddress("GenOthers_mother", GenOthers_mother, &b_GenOthers_mother);
   fChain->SetBranchAddress("GenOthers_charge", GenOthers_charge, &b_GenOthers_charge);
   fChain->SetBranchAddress("GenOthers_status", GenOthers_status, &b_GenOthers_status);
   fChain->SetBranchAddress("GenOthers_ID", GenOthers_ID, &b_GenOthers_ID);
   fChain->SetBranchAddress("GenOthers_isPrompt", GenOthers_isPrompt, &b_GenOthers_isPrompt);
   fChain->SetBranchAddress("GenOthers_isPromptFinalState", GenOthers_isPromptFinalState, &b_GenOthers_isPromptFinalState);
   fChain->SetBranchAddress("GenOthers_isTauDecayProduct", GenOthers_isTauDecayProduct, &b_GenOthers_isTauDecayProduct);
   fChain->SetBranchAddress("GenOthers_isPromptTauDecayProduct", GenOthers_isPromptTauDecayProduct, &b_GenOthers_isPromptTauDecayProduct);
   fChain->SetBranchAddress("GenOthers_isDirectPromptTauDecayProductFinalState", GenOthers_isDirectPromptTauDecayProductFinalState, &b_GenOthers_isDirectPromptTauDecayProductFinalState);
   fChain->SetBranchAddress("GenOthers_isHardProcess", GenOthers_isHardProcess, &b_GenOthers_isHardProcess);
   fChain->SetBranchAddress("GenOthers_isLastCopy", GenOthers_isLastCopy, &b_GenOthers_isLastCopy);
   fChain->SetBranchAddress("GenOthers_isLastCopyBeforeFSR", GenOthers_isLastCopyBeforeFSR, &b_GenOthers_isLastCopyBeforeFSR);
   fChain->SetBranchAddress("GenOthers_isPromptDecayed", GenOthers_isPromptDecayed, &b_GenOthers_isPromptDecayed);
   fChain->SetBranchAddress("GenOthers_isDecayedLeptonHadron", GenOthers_isDecayedLeptonHadron, &b_GenOthers_isDecayedLeptonHadron);
   fChain->SetBranchAddress("GenOthers_fromHardProcessBeforeFSR", GenOthers_fromHardProcessBeforeFSR, &b_GenOthers_fromHardProcessBeforeFSR);
   fChain->SetBranchAddress("GenOthers_fromHardProcessDecayed", GenOthers_fromHardProcessDecayed, &b_GenOthers_fromHardProcessDecayed);
   fChain->SetBranchAddress("GenOthers_fromHardProcessFinalState", GenOthers_fromHardProcessFinalState, &b_GenOthers_fromHardProcessFinalState);
   fChain->SetBranchAddress("GenOthers_isMostlyLikePythia6Status3", GenOthers_isMostlyLikePythia6Status3, &b_GenOthers_isMostlyLikePythia6Status3);
   fChain->SetBranchAddress("nPhotons", &nPhotons, &b_nPhotons);
   fChain->SetBranchAddress("Photon_hasPixelSeed", Photon_hasPixelSeed, &b_Photon_hasPixelSeed);
   fChain->SetBranchAddress("Photon_pT", Photon_pT, &b_Photon_pT);
   fChain->SetBranchAddress("Photon_eta", Photon_eta, &b_Photon_eta);
   fChain->SetBranchAddress("Photon_phi", Photon_phi, &b_Photon_phi);
   fChain->SetBranchAddress("Photon_etaSC", Photon_etaSC, &b_Photon_etaSC);
   fChain->SetBranchAddress("Photon_phiSC", Photon_phiSC, &b_Photon_phiSC);
   fChain->SetBranchAddress("Photon_HoverE", Photon_HoverE, &b_Photon_HoverE);
   fChain->SetBranchAddress("Photon_Full5x5_SigmaIEtaIEta", Photon_Full5x5_SigmaIEtaIEta, &b_Photon_Full5x5_SigmaIEtaIEta);
   fChain->SetBranchAddress("Photon_ChIso", Photon_ChIso, &b_Photon_ChIso);
   fChain->SetBranchAddress("Photon_NhIso", Photon_NhIso, &b_Photon_NhIso);
   fChain->SetBranchAddress("Photon_PhIso", Photon_PhIso, &b_Photon_PhIso);
   fChain->SetBranchAddress("Photon_ChIsoWithEA", Photon_ChIsoWithEA, &b_Photon_ChIsoWithEA);
   fChain->SetBranchAddress("Photon_NhIsoWithEA", Photon_NhIsoWithEA, &b_Photon_NhIsoWithEA);
   fChain->SetBranchAddress("Photon_PhIsoWithEA", Photon_PhIsoWithEA, &b_Photon_PhIsoWithEA);
   fChain->SetBranchAddress("nPileUp", &nPileUp, &b_nPileUp);
   fChain->SetBranchAddress("pileUpReweightIn", &pileUpReweightIn, &b_pileUpReweightIn);
   fChain->SetBranchAddress("pileUpReweight", &pileUpReweight, &b_pileUpReweight);
   fChain->SetBranchAddress("pileUpReweightPlus", &pileUpReweightPlus, &b_pileUpReweightPlus);
   fChain->SetBranchAddress("pileUpReweightMinus", &pileUpReweightMinus, &b_pileUpReweightMinus);
   fChain->SetBranchAddress("pileUpReweightInMuonPhys", &pileUpReweightInMuonPhys, &b_pileUpReweightInMuonPhys);
   fChain->SetBranchAddress("pileUpReweightMuonPhys", &pileUpReweightMuonPhys, &b_pileUpReweightMuonPhys);
   fChain->SetBranchAddress("pileUpReweightPlusMuonPhys", &pileUpReweightPlusMuonPhys, &b_pileUpReweightPlusMuonPhys);
   fChain->SetBranchAddress("pileUpReweightMinusMuonPhys", &pileUpReweightMinusMuonPhys, &b_pileUpReweightMinusMuonPhys);
   fChain->SetBranchAddress("pfMET_pT", &pfMET_pT, &b_pfMET_pT);
   fChain->SetBranchAddress("pfMET_phi", &pfMET_phi, &b_pfMET_phi);
   fChain->SetBranchAddress("pfMET_Px", &pfMET_Px, &b_pfMET_Px);
   fChain->SetBranchAddress("pfMET_Py", &pfMET_Py, &b_pfMET_Py);
   fChain->SetBranchAddress("pfMET_SumEt", &pfMET_SumEt, &b_pfMET_SumEt);
   fChain->SetBranchAddress("pfMET_Type1_pT", &pfMET_Type1_pT, &b_pfMET_Type1_pT);
   fChain->SetBranchAddress("pfMET_Type1_phi", &pfMET_Type1_phi, &b_pfMET_Type1_phi);
   fChain->SetBranchAddress("pfMET_Type1_Px", &pfMET_Type1_Px, &b_pfMET_Type1_Px);
   fChain->SetBranchAddress("pfMET_Type1_Py", &pfMET_Type1_Py, &b_pfMET_Type1_Py);
   fChain->SetBranchAddress("pfMET_Type1_SumEt", &pfMET_Type1_SumEt, &b_pfMET_Type1_SumEt);
   Notify();
}

Bool_t MakeHFweight::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MakeHFweight::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MakeHFweight::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MakeHFweight_cxx
