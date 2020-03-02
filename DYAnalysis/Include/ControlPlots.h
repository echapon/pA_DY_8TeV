/////////////////////////////////////////////////
// -- 2016.10.24 (v01): Include DYAnalyzer_v01.h
// -- 2016.11.04 (v02): include v02 headers
/////////////////////////////////////////////////

#pragma once

#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TFile.h>

#include <vector>

#include "DYAnalyzer.h"
#include "../BkgEst/interface/defs.h"

#define NISOBINS 3

using namespace DYana;

class ControlPlots
{

public:
	DYAnalyzer *analyzer;
	Bool_t isMC;
	std::vector<TH1D*> Histo;
	std::vector<TH2D*> Histo2D;

	TH1D *h_Pt;
	TH1D *h_eta;
	TH1D *h_phi;
	TH1D *h_mass;
	TH1D *h_mass2;
	TH1D *h_mass3bins;
	TH1D *h_massZ;
	TH1D *h_diPt;
	TH1D *h_diPt_M15to20;
	TH1D *h_diPt2_M60to120;
	TH1D *h_diPt2_M15to60;
	TH1D *h_Phistar_M60to120;
	TH1D *h_Phistar2_M60to120;
	TH1D *h_Phistar2_M15to60;
	TH1D *h_diRap;
	TH1D *h_diRap_M15to20;

	TH1D *h_diRap_M15to60;
	TH1D *h_diRap_M60to120;
	TH1D *h_diRapCM_coarse_M60to120;
	TH1D *h_diRap_M120to600;
	TH1D *h_diRap2_M15to60;
	TH1D *h_diRap2_M60to120;

	TH1D *h_lead_Pt;
	TH1D *h_sub_Pt;
	
	TH1D *h_lead_eta;
	TH1D *h_sub_eta;
	TH1D *h_lead_eta_M80to100;
	TH1D *h_sub_eta_M80to100;

	TH1D *h_lead_phi;
	TH1D *h_sub_phi;

	TH1D *h_barrel_Pt;
	TH1D *h_barrel_eta;
	TH1D *h_barrel_phi;

	TH1D *h_endcap_Pt;
	TH1D *h_endcap_eta;
	TH1D *h_endcap_phi;

	TH1D *h_mass_OS;
	TH1D *h_mass_SS;
	TH1D *h_mass2_SS;
	TH1D *h_mass3bins_SS;
	TH1D *h_diPt2_SS_M60to120;
	TH1D *h_diPt2_SS_M15to60;
	TH1D *h_Phistar2_SS_M60to120;
	TH1D *h_Phistar2_SS_M15to60;
	TH1D *h_diRap2_SS_M15to60;
	TH1D *h_diRap2_SS_M60to120;

	TH1D *h_mass_Hichi2;
	TH1D *h_mass2_Hichi2;
	TH1D *h_mass3bins_Hichi2;
	TH1D *h_diPt2_Hichi2_M60to120;
	TH1D *h_diPt2_Hichi2_M15to60;
	TH1D *h_Phistar2_Hichi2_M60to120;
	TH1D *h_Phistar2_Hichi2_M15to60;
	TH1D *h_diRap2_Hichi2_M15to60;
	TH1D *h_diRap2_Hichi2_M60to120;

	TH1D *h_mass_SS_Hichi2;
	TH1D *h_mass2_SS_Hichi2;
	TH1D *h_mass3bins_SS_Hichi2;
	TH1D *h_diPt2_SS_Hichi2_M60to120;
	TH1D *h_diPt2_SS_Hichi2_M15to60;
	TH1D *h_Phistar2_SS_Hichi2_M60to120;
	TH1D *h_Phistar2_SS_Hichi2_M15to60;
	TH1D *h_diRap2_SS_Hichi2_M15to60;
	TH1D *h_diRap2_SS_Hichi2_M60to120;

	TH1D *h_Pt_minusCharge;
	TH1D *h_eta_minusCharge;
	TH1D *h_phi_minusCharge;
	TH1D *h_Pt_plusCharge;
	TH1D *h_eta_plusCharge;
	TH1D *h_phi_plusCharge;

	TH1D *h_Pt_M15to60;
	TH1D *h_Pt_M60to120;
	TH1D *h_Pt_M120to600;

	TH1D *h_Angle;

	TH1D *h_Pt_TrigLeg;
	TH1D *h_eta_TrigLeg;
	TH1D *h_phi_TrigLeg;

	TH1D *h_Pt_OtherLeg;
	TH1D *h_eta_OtherLeg;
	TH1D *h_phi_OtherLeg;

	TH1D *h_GenMass;
   TH1D *h_GenMass_preFSR;
   TH1D *h_GenMass_postFSR;
	TH1D *h_GenPt;
	TH1D *h_GenEta;
	TH1D *h_GenPhi;

	TH1D *h_GenPt_Lead;
	TH1D *h_GenEta_Lead;
	TH1D *h_GenPhi_Lead;

	TH1D *h_GenPt_Sub;
	TH1D *h_GenEta_Sub;
	TH1D *h_GenPhi_Sub;

	TH1D *h_GenDiPt;
	TH1D *h_GenDiRap;
   // TH1D *h_GenDiRap1;
   // TH1D *h_GenDiRap2;

	TH1D *h_VtxProb;
	TH1D *h_VtxNormChi2;
	TH1D *h_VtxNormChi2_fullrange;
	TH1D *h_VtxProb_belowM600;
	TH1D *h_VtxNormChi2_belowM600;

	TH1D* h_mass_OS_BB;
	TH1D* h_mass_OS_BE;
	TH1D* h_mass_OS_EE;

   TH1D* h_PtoM;
   TH1D* h_PtoM_2060;
   TH1D* h_PtoM_60120;

	TH1D *h_muonHits;
	TH1D *h_nMatches;
	TH1D *h_RelPtError;
	TH1D *h_dxyVTX;
	TH1D *h_dzVTX;
	TH1D *h_pixelHits;
	TH1D *h_trackerLayers;
	TH1D *h_RelTrkIso;
	TH1D *h_RelTrkIso_fullrange;
	TH1D *h_RelPFIso;

   // 2D
   TH2D* h_2D_leadEta_leadPhi;
   TH2D* h_2D_leadEta_leadPt;

   // // iso plots
   // TH1D *h_maxRelTrkIso_OS;
   // TH1D *h_maxRelTrkIso_OS_M1560;
   // TH1D *h_maxRelTrkIso_OS_M60120;
   // TH1D *h_maxRelTrkIso_OS_M120600;
   // TH1D *h_maxRelTrkIso_SS;
   // TH1D *h_maxRelTrkIso_SS_M1560;
   // TH1D *h_maxRelTrkIso_SS_M60120;
   // TH1D *h_maxRelTrkIso_SS_M120600;
   // TH1D *h_maxRelPFIso_OS;
   // TH1D *h_maxRelPFIso_OS_M1560;
   // TH1D *h_maxRelPFIso_OS_M60120;
   // TH1D *h_maxRelPFIso_OS_M120600;
   // TH1D *h_maxRelPFIso_SS;
   // TH1D *h_maxRelPFIso_SS_M1560;
   // TH1D *h_maxRelPFIso_SS_M60120;
   // TH1D *h_maxRelPFIso_SS_M120600;


	ControlPlots(TString Type, DYAnalyzer *dyanalyzer)
	{
		isMC = kFALSE;
		if( Type != "Data" )
			isMC = kTRUE;

		analyzer = dyanalyzer;

		h_Pt = new TH1D("h_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_Pt );
		h_eta = new TH1D("h_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_eta );
		h_phi = new TH1D("h_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_phi );
		h_mass = new TH1D("h_mass_"+Type, "", 120, 0, 600); Histo.push_back( h_mass );
		h_mass2 = new TH1D("h_mass2_"+Type, "", binnum, bins); Histo.push_back( h_mass2 );
		h_mass3bins = new TH1D("h_mass3bins_"+Type, "", binnum3, bins3); Histo.push_back( h_mass3bins );
		h_massZ = new TH1D("h_massZ_"+Type, "", 60, 60, 120); Histo.push_back( h_massZ );
		h_diPt = new TH1D("h_diPt_"+Type, "", 100, 0, 500); Histo.push_back( h_diPt );
		h_diPt_M15to20 = new TH1D("h_diPt_M15to20_"+Type, "", 10, 0, 50); Histo.push_back( h_diPt_M15to20 );
		h_diPt2_M60to120 = new TH1D("h_diPt2_M60to120_"+Type, "", ptbinnum_meas, ptbin_meas); Histo.push_back( h_diPt2_M60to120 );
		h_diPt2_M15to60 = new TH1D("h_diPt2_M15to60_"+Type, "", ptbinnum_meas_1560, ptbin_meas_1560); Histo.push_back( h_diPt2_M15to60 );
		h_Phistar_M60to120 = new TH1D("h_Phistar_M60to120_"+Type, "", 100,0,3.); Histo.push_back( h_Phistar_M60to120 );
		h_Phistar2_M60to120 = new TH1D("h_Phistar2_M60to120_"+Type, "", phistarnum, phistarbin); Histo.push_back( h_Phistar2_M60to120 );
		h_Phistar2_M15to60 = new TH1D("h_Phistar2_M15to60_"+Type, "", phistarnum_1560, phistarbin_1560); Histo.push_back( h_Phistar2_M15to60 );
		h_diRap = new TH1D("h_diRap_"+Type, "", 60, -3, 3); Histo.push_back( h_diRap );
		h_diRap_M15to20 = new TH1D("h_diRap_M15to20_"+Type, "", 12, -3, 3); Histo.push_back( h_diRap_M15to20 );

		h_diRap_M15to60 = new TH1D("h_diRap_M15to60_"+Type, "", 24, -2.4, 2.4); Histo.push_back( h_diRap_M15to60 );
		h_diRap_M60to120 = new TH1D("h_diRap_M60to120_"+Type, "", 24, -2.4, 2.4); Histo.push_back( h_diRap_M60to120 );
		h_diRapCM_coarse_M60to120 = new TH1D("h_diRapCM_coarse_M60to120_"+Type, "", 13, -3.2, 2.); Histo.push_back( h_diRapCM_coarse_M60to120 );
		h_diRap_M120to600 = new TH1D("h_diRap_M120to600_"+Type, "", 24, -2.4, 2.4); Histo.push_back( h_diRap_M120to600 );
		h_diRap2_M15to60 = new TH1D("h_diRap2_M15to60_"+Type, "", rapbinnum_1560, rapbin_1560); Histo.push_back( h_diRap2_M15to60 );
		h_diRap2_M60to120 = new TH1D("h_diRap2_M60to120_"+Type, "", rapbinnum_60120, rapbin_60120); Histo.push_back( h_diRap2_M60to120 );

		h_lead_Pt = new TH1D("h_lead_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_lead_Pt );
		h_sub_Pt = new TH1D("h_sub_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_sub_Pt );
		
		h_lead_eta = new TH1D("h_lead_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_lead_eta );
		h_sub_eta = new TH1D("h_sub_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_sub_eta );
		h_lead_eta_M80to100 = new TH1D("h_lead_eta_M80to100_"+Type, "", 24, -2.4, 2.4); Histo.push_back( h_lead_eta_M80to100 );
		h_sub_eta_M80to100 = new TH1D("h_sub_eta_M80to100_"+Type, "", 24, -2.4, 2.4); Histo.push_back( h_sub_eta_M80to100 );

		h_lead_phi = new TH1D("h_lead_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_lead_phi );
		h_sub_phi = new TH1D("h_sub_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_sub_phi );

		h_barrel_Pt = new TH1D("h_barrel_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_barrel_Pt );
		h_barrel_eta = new TH1D("h_barrel_eta_"+Type, "",  60, -3, 3); Histo.push_back( h_barrel_eta );
		h_barrel_phi = new TH1D("h_barrel_phi_"+Type, "",  80, -4, 4); Histo.push_back( h_barrel_phi );

		h_endcap_Pt = new TH1D("h_endcap_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_endcap_Pt );
		h_endcap_eta = new TH1D("h_endcap_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_endcap_eta );
		h_endcap_phi = new TH1D("h_endcap_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_endcap_phi );

		h_mass_OS = new TH1D("h_mass_OS_"+Type, "", 120, 0, 600); Histo.push_back( h_mass_OS );
		h_mass_SS = new TH1D("h_mass_SS_"+Type, "", 120, 0, 600); Histo.push_back( h_mass_SS );
		h_mass2_SS = new TH1D("h_mass2_SS_"+Type, "", binnum, bins); Histo.push_back( h_mass2_SS );
		h_mass3bins_SS = new TH1D("h_mass3bins_SS_"+Type, "", binnum3, bins3); Histo.push_back( h_mass3bins_SS );
		h_diPt2_SS_M60to120 = new TH1D("h_diPt2_SS_M60to120_"+Type, "", ptbinnum_meas, ptbin_meas); Histo.push_back( h_diPt2_SS_M60to120 );
		h_diPt2_SS_M15to60 = new TH1D("h_diPt2_SS_M15to60_"+Type, "", ptbinnum_meas_1560, ptbin_meas_1560); Histo.push_back( h_diPt2_SS_M15to60 );
		h_Phistar2_SS_M60to120 = new TH1D("h_Phistar2_SS_M60to120_"+Type, "", phistarnum, phistarbin); Histo.push_back( h_Phistar2_SS_M60to120 );
		h_Phistar2_SS_M15to60 = new TH1D("h_Phistar2_SS_M15to60_"+Type, "", phistarnum_1560, phistarbin_1560); Histo.push_back( h_Phistar2_SS_M15to60 );
		h_diRap2_SS_M15to60 = new TH1D("h_diRap2_SS_M15to60_"+Type, "", rapbinnum_1560, rapbin_1560); Histo.push_back( h_diRap2_SS_M15to60 );
		h_diRap2_SS_M60to120 = new TH1D("h_diRap2_SS_M60to120_"+Type, "", rapbinnum_60120, rapbin_60120); Histo.push_back( h_diRap2_SS_M60to120 );

		h_mass_Hichi2 = new TH1D("h_mass_Hichi2_"+Type, "", 120, 0, 600); Histo.push_back( h_mass_Hichi2 );
		h_mass2_Hichi2 = new TH1D("h_mass2_Hichi2_"+Type, "", binnum, bins); Histo.push_back( h_mass2_Hichi2 );
		h_mass3bins_Hichi2 = new TH1D("h_mass3bins_Hichi2_"+Type, "", binnum3, bins3); Histo.push_back( h_mass3bins_Hichi2 );
		h_diPt2_Hichi2_M60to120 = new TH1D("h_diPt2_Hichi2_M60to120_"+Type, "", ptbinnum_meas, ptbin_meas); Histo.push_back( h_diPt2_Hichi2_M60to120 );
		h_diPt2_Hichi2_M15to60 = new TH1D("h_diPt2_Hichi2_M15to60_"+Type, "", ptbinnum_meas_1560, ptbin_meas_1560); Histo.push_back( h_diPt2_Hichi2_M15to60 );
		h_Phistar2_Hichi2_M60to120 = new TH1D("h_Phistar2_Hichi2_M60to120_"+Type, "", phistarnum, phistarbin); Histo.push_back( h_Phistar2_Hichi2_M60to120 );
		h_Phistar2_Hichi2_M15to60 = new TH1D("h_Phistar2_Hichi2_M15to60_"+Type, "", phistarnum_1560, phistarbin_1560); Histo.push_back( h_Phistar2_Hichi2_M15to60 );
		h_diRap2_Hichi2_M15to60 = new TH1D("h_diRap2_Hichi2_M15to60_"+Type, "", rapbinnum_1560, rapbin_1560); Histo.push_back( h_diRap2_Hichi2_M15to60 );
		h_diRap2_Hichi2_M60to120 = new TH1D("h_diRap2_Hichi2_M60to120_"+Type, "", rapbinnum_60120, rapbin_60120); Histo.push_back( h_diRap2_Hichi2_M60to120 );

		h_mass_SS_Hichi2 = new TH1D("h_mass_SS_Hichi2_"+Type, "", 120, 0, 600); Histo.push_back( h_mass_SS_Hichi2 );
		h_mass2_SS_Hichi2 = new TH1D("h_mass2_SS_Hichi2_"+Type, "", binnum, bins); Histo.push_back( h_mass2_SS_Hichi2 );
		h_mass3bins_SS_Hichi2 = new TH1D("h_mass3bins_SS_Hichi2_"+Type, "", binnum3, bins3); Histo.push_back( h_mass3bins_SS_Hichi2 );
		h_diPt2_SS_Hichi2_M60to120 = new TH1D("h_diPt2_SS_Hichi2_M60to120_"+Type, "", ptbinnum_meas, ptbin_meas); Histo.push_back( h_diPt2_SS_Hichi2_M60to120 );
		h_diPt2_SS_Hichi2_M15to60 = new TH1D("h_diPt2_SS_Hichi2_M15to60_"+Type, "", ptbinnum_meas_1560, ptbin_meas_1560); Histo.push_back( h_diPt2_SS_Hichi2_M15to60 );
		h_Phistar2_SS_Hichi2_M60to120 = new TH1D("h_Phistar2_SS_Hichi2_M60to120_"+Type, "", phistarnum, phistarbin); Histo.push_back( h_Phistar2_SS_Hichi2_M60to120 );
		h_Phistar2_SS_Hichi2_M15to60 = new TH1D("h_Phistar2_SS_Hichi2_M15to60_"+Type, "", phistarnum_1560, phistarbin_1560); Histo.push_back( h_Phistar2_SS_Hichi2_M15to60 );
		h_diRap2_SS_Hichi2_M15to60 = new TH1D("h_diRap2_SS_Hichi2_M15to60_"+Type, "", rapbinnum_1560, rapbin_1560); Histo.push_back( h_diRap2_SS_Hichi2_M15to60 );
		h_diRap2_SS_Hichi2_M60to120 = new TH1D("h_diRap2_SS_Hichi2_M60to120_"+Type, "", rapbinnum_60120, rapbin_60120); Histo.push_back( h_diRap2_SS_Hichi2_M60to120 );

		h_Pt_minusCharge = new TH1D("h_Pt_minusCharge_"+Type, "", 125, 0, 250); Histo.push_back( h_Pt_minusCharge );
		h_eta_minusCharge = new TH1D("h_eta_minusCharge_"+Type, "", 60, -3, 3); Histo.push_back( h_eta_minusCharge );
		h_phi_minusCharge = new TH1D("h_phi_minusCharge_"+Type, "", 80, -4, 4); Histo.push_back( h_phi_minusCharge );
		h_Pt_plusCharge = new TH1D("h_Pt_plusCharge_"+Type, "", 125, 0, 250); Histo.push_back( h_Pt_plusCharge );
		h_eta_plusCharge = new TH1D("h_eta_plusCharge_"+Type, "", 60, -3, 3); Histo.push_back( h_eta_plusCharge );
		h_phi_plusCharge = new TH1D("h_phi_plusCharge_"+Type, "", 80, -4, 4); Histo.push_back( h_phi_plusCharge );

		h_Pt_M15to60 = new TH1D("h_Pt_M15to60_"+Type, "", 125, 0, 250); Histo.push_back( h_Pt_M15to60 );
		h_Pt_M60to120 = new TH1D("h_Pt_M60to120_"+Type, "", 125, 0, 250); Histo.push_back( h_Pt_M60to120 );
		h_Pt_M120to600 = new TH1D("h_Pt_M120to600_"+Type, "", 125, 0, 250); Histo.push_back( h_Pt_M120to600 );

		h_Angle = new TH1D("h_Angle_"+Type, "", 80, -4, 4); Histo.push_back( h_Angle );

		h_Pt_TrigLeg = new TH1D("h_Pt_TrigLeg_"+Type, "", 100, 0, 500); Histo.push_back( h_Pt_TrigLeg );
		h_eta_TrigLeg = new TH1D("h_eta_TrigLeg_"+Type, "", 60, -3, 3); Histo.push_back( h_eta_TrigLeg );
		h_phi_TrigLeg = new TH1D("h_phi_TrigLeg_"+Type, "", 80, -4, 4); Histo.push_back( h_phi_TrigLeg );

		h_Pt_OtherLeg = new TH1D("h_Pt_OtherLeg_"+Type, "", 100, 0, 500); Histo.push_back( h_Pt_OtherLeg );
		h_eta_OtherLeg = new TH1D("h_eta_OtherLeg_"+Type, "", 60, -3, 3); Histo.push_back( h_eta_OtherLeg );
		h_phi_OtherLeg = new TH1D("h_phi_OtherLeg_"+Type, "", 80, -4, 4); Histo.push_back( h_phi_OtherLeg );

      h_GenMass = new  TH1D("h_GenMass_"+Type, "", 120, 0, 600); Histo.push_back( h_GenMass );
      // h_GenMass = new  TH1D("h_GenMass_"+Type, "", 60, 0, 600); Histo.push_back( h_GenMass );
      // h_GenMass2 = new  TH1D("h_GenMass2_"+Type, "", 60, 60, 120); Histo.push_back( h_GenMass2 );
      h_GenMass_preFSR = new  TH1D("h_GenMass_preFSR_"+Type, "", 120, 0, 600); Histo.push_back( h_GenMass_preFSR );
      h_GenMass_postFSR = new  TH1D("h_GenMass_postFSR_"+Type, "", 120, 0, 600); Histo.push_back( h_GenMass_postFSR );
		h_GenPt = new  TH1D("h_GenPt_"+Type, "", 100, 0, 500); Histo.push_back( h_GenPt );
		h_GenEta = new  TH1D("h_GenEta_"+Type, "", 200, -10, 10); Histo.push_back( h_GenEta );
		h_GenPhi = new  TH1D("h_GenPhi_"+Type, "", 80, -4, 4); Histo.push_back( h_GenPhi );

		h_GenPt_Lead = new  TH1D("h_GenPt_Lead_"+Type, "", 100, 0, 500); Histo.push_back( h_GenPt_Lead );
		h_GenEta_Lead = new  TH1D("h_GenEta_Lead_"+Type, "", 200, -10, 10); Histo.push_back( h_GenEta_Lead );
		h_GenPhi_Lead = new  TH1D("h_GenPhi_Lead_"+Type, "", 80, -4, 4); Histo.push_back( h_GenPhi_Lead );

		h_GenPt_Sub = new  TH1D("h_GenPt_Sub_"+Type, "", 100, 0, 500); Histo.push_back( h_GenPt_Sub );
		h_GenEta_Sub = new  TH1D("h_GenEta_Sub_"+Type, "", 200, -10, 10); Histo.push_back( h_GenEta_Sub );
		h_GenPhi_Sub = new  TH1D("h_GenPhi_Sub_"+Type, "", 80, -4, 4); Histo.push_back( h_GenPhi_Sub );

      h_GenDiPt = new TH1D("h_GenDiPt_"+Type, "", 100, 0, 500); Histo.push_back( h_GenDiPt );
      h_GenDiRap = new TH1D("h_GenDiRap_"+Type, "", 200, -10, 10); Histo.push_back( h_GenDiRap );
      // h_GenDiPt = new TH1D("h_GenDiPt_"+Type, "", 25, 0, 50); Histo.push_back( h_GenDiPt );
      // h_GenDiRap = new TH1D("h_GenDiRap_"+Type, "", 60, -6, 6); Histo.push_back( h_GenDiRap );
      // h_GenDiRap1 = new TH1D("h_GenDiRap1_"+Type, "", 60, -6, 6); Histo.push_back( h_GenDiRap1 );
      // h_GenDiRap2 = new TH1D("h_GenDiRap2_"+Type, "", 60, -6, 6); Histo.push_back( h_GenDiRap2 );

		h_VtxProb = new TH1D("h_VtxProb_"+Type, "", 100, 0, 1); Histo.push_back( h_VtxProb );
		h_VtxProb_belowM600 = new TH1D("h_VtxProb_belowM600_"+Type, "", 100, 0, 1); Histo.push_back( h_VtxProb_belowM600 );
		
		h_VtxNormChi2 = new TH1D("h_VtxNormChi2_"+Type, "", 20, 0, 20); Histo.push_back( h_VtxNormChi2 );
		h_VtxNormChi2_fullrange = new TH1D("h_VtxNormChi2_fullrange_"+Type, "", 100, 0, 100); Histo.push_back( h_VtxNormChi2_fullrange );
		h_VtxNormChi2_belowM600 = new TH1D("h_VtxNormChi2_belowM600_"+Type, "", 20, 0, 20); Histo.push_back( h_VtxNormChi2_belowM600 );

		h_mass_OS_BB = new TH1D("h_mass_OS_BB_"+Type, "", 120, 0, 600); Histo.push_back( h_mass_OS_BB );
		h_mass_OS_BE = new TH1D("h_mass_OS_BE_"+Type, "", 120, 0, 600); Histo.push_back( h_mass_OS_BE );
		h_mass_OS_EE = new TH1D("h_mass_OS_EE_"+Type, "", 120, 0, 600); Histo.push_back( h_mass_OS_EE );

      h_PtoM = new TH1D("h_PtoM_"+Type, "", 50, 1, 10); Histo.push_back( h_PtoM );
      h_PtoM_2060 = new TH1D("h_PtoM_2060_"+Type, "", 50, 1, 10); Histo.push_back( h_PtoM_2060 );
      h_PtoM_60120 = new TH1D("h_PtoM_60120_"+Type, "", 50, 1, 10); Histo.push_back( h_PtoM_60120 );

		h_muonHits = new TH1D("h_muonHits_"+Type, "", 70, 0, 70); Histo.push_back( h_muonHits );
		h_nMatches = new TH1D("h_nMatches_"+Type, "", 7, 0, 7); Histo.push_back( h_nMatches );
		h_RelPtError = new TH1D("h_RelPtError_"+Type, "", 50, 0, 0.5); Histo.push_back( h_RelPtError );
		h_dxyVTX = new TH1D("h_dxyVTX_"+Type, "", 100, -0.3, 0.3); Histo.push_back( h_dxyVTX );
		h_dzVTX = new TH1D("h_dzVTX_"+Type, "", 500, -1.0, 1.0); Histo.push_back( h_dzVTX );
		h_pixelHits = new TH1D("h_pixelHits_"+Type, "", 15, 0, 15); Histo.push_back( h_pixelHits );
		h_trackerLayers = new TH1D("h_trackerLayers_"+Type, "", 20, 0, 20); Histo.push_back( h_trackerLayers );
		h_RelTrkIso = new TH1D("h_RelTrkIso_"+Type, "", 100, 0, 0.2); Histo.push_back( h_RelTrkIso ); 
		h_RelTrkIso_fullrange = new TH1D("h_RelTrkIso_fullrange_"+Type, "", 100, 0, 1); Histo.push_back( h_RelTrkIso_fullrange ); 
		h_RelPFIso = new TH1D("h_RelPFIso_"+Type, "", 100, 0, 0.2); Histo.push_back( h_RelPFIso ); 

      // 2D
      h_2D_leadEta_leadPhi = new TH2D("h_2D_leadEta_leadPhi_"+Type, "", 24, -2.4, 2.4, 32, -TMath::Pi(), TMath::Pi()); Histo2D.push_back( h_2D_leadEta_leadPhi);
      h_2D_leadEta_leadPt = new TH2D("h_2D_leadEta_leadPt_"+Type, "", 24, -2.4, 2.4, 50, 0, 100); Histo2D.push_back( h_2D_leadEta_leadPt);

      // h_maxRelTrkIso_OS = new TH1D("h_maxRelTrkIso_OS_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelTrkIso_OS );
      // h_maxRelTrkIso_OS_M1560 = new TH1D("h_maxRelTrkIso_OS_M1560_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelTrkIso_OS_M1560 );
      // h_maxRelTrkIso_OS_M60120 = new TH1D("h_maxRelTrkIso_OS_M60120_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelTrkIso_OS_M60120 );
      // h_maxRelTrkIso_OS_M120600 = new TH1D("h_maxRelTrkIso_OS_M120600_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelTrkIso_OS_M120600 );
      // h_maxRelTrkIso_SS = new TH1D("h_maxRelTrkIso_SS_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelTrkIso_SS );
      // h_maxRelTrkIso_SS_M1560 = new TH1D("h_maxRelTrkIso_SS_M1560_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelTrkIso_SS_M1560 );
      // h_maxRelTrkIso_SS_M60120 = new TH1D("h_maxRelTrkIso_SS_M60120_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelTrkIso_SS_M60120 );
      // h_maxRelTrkIso_SS_M120600 = new TH1D("h_maxRelTrkIso_SS_M120600_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelTrkIso_SS_M120600 );
      // h_maxRelPFIso_OS = new TH1D("h_maxRelPFIso_OS_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelPFIso_OS );
      // h_maxRelPFIso_OS_M1560 = new TH1D("h_maxRelPFIso_OS_M1560_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelPFIso_OS_M1560 );
      // h_maxRelPFIso_OS_M60120 = new TH1D("h_maxRelPFIso_OS_M60120_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelPFIso_OS_M60120 );
      // h_maxRelPFIso_OS_M120600 = new TH1D("h_maxRelPFIso_OS_M120600_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelPFIso_OS_M120600 );
      // h_maxRelPFIso_SS = new TH1D("h_maxRelPFIso_SS_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelPFIso_SS );
      // h_maxRelPFIso_SS_M1560 = new TH1D("h_maxRelPFIso_SS_M1560_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelPFIso_SS_M1560 );
      // h_maxRelPFIso_SS_M60120 = new TH1D("h_maxRelPFIso_SS_M60120_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelPFIso_SS_M60120 );
      // h_maxRelPFIso_SS_M120600 = new TH1D("h_maxRelPFIso_SS_M120600_"+Type, "", 100, 0, 1); Histo.push_back( h_maxRelPFIso_SS_M120600 );
	}

	void FillHistograms_GenDoubleMu(NtupleHandle *ntuple, GenLepton genlep1, GenLepton genlep2, Double_t weight)
	{
		FillHistograms_GenSingleMu( ntuple, genlep1, weight );
		FillHistograms_GenSingleMu( ntuple, genlep2, weight );

		TLorentzVector gen_v1 = genlep1.Momentum;
		TLorentzVector gen_v2 = genlep2.Momentum;
		Double_t gen_M = (gen_v1 + gen_v2).M();
		Double_t gen_DiPt = (gen_v1 + gen_v2).Pt();
		Double_t gen_DiRap = (gen_v1 + gen_v2).Rapidity();

		h_GenMass->Fill( gen_M, weight );
      // h_GenMass2->Fill( gen_M, weight );

      // build pre-FSR mass
      vector<GenLepton> GenLeptonCollection_FinalState;
      Int_t NGenLeptons = ntuple->gnpair;
      for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
      {
         GenLepton genlep;
         genlep.FillFromNtuple(ntuple, i_gen);
         if( genlep.isMuon() && genlep.fromHardProcessFinalState )
         {
            GenLeptonCollection_FinalState.push_back( genlep );

            if( (Int_t)GenLeptonCollection_FinalState.size() == 2 )
               break;
         }
      }
      Double_t dRCut = 0.1;
      GenLepton genlep_postFSR1 = GenLeptonCollection_FinalState[0];
      GenLepton genlep_preFSR1 = genlep_postFSR1; // -- Copy the values of member variables -- // 
      vector< GenOthers > GenPhotonCollection1;
      analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR1, dRCut, &genlep_preFSR1, &GenPhotonCollection1);

      GenLepton genlep_postFSR2 = GenLeptonCollection_FinalState[1];
      GenLepton genlep_preFSR2 = genlep_postFSR2; // -- Copy the values of member variables -- // 
      vector< GenOthers > GenPhotonCollection2;
      analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR2, dRCut, &genlep_preFSR2, &GenPhotonCollection2);
      Double_t M_preFSR = ( genlep_preFSR1.Momentum + genlep_preFSR2.Momentum ).M();
		h_GenMass_preFSR->Fill( M_preFSR, weight );
      Double_t M_postFSR = ( genlep_postFSR1.Momentum + genlep_postFSR2.Momentum ).M();
		h_GenMass_postFSR->Fill( M_postFSR, weight );

		h_GenDiPt->Fill( gen_DiPt, weight );
		h_GenDiRap->Fill( gen_DiRap, weight );
      // h_GenDiRap1->Fill( gen_DiRap+0.465, weight );
      // h_GenDiRap2->Fill( gen_DiRap-0.465, weight );

		if( genlep1.Pt > genlep2.Pt )
		{
			h_GenPt_Lead->Fill( genlep1.Pt, weight );
			h_GenEta_Lead->Fill( genlep1.eta, weight );
			h_GenPhi_Lead->Fill( genlep1.phi, weight );

			h_GenPt_Sub->Fill( genlep2.Pt, weight );
			h_GenEta_Sub->Fill( genlep2.eta, weight );
			h_GenPhi_Sub->Fill( genlep2.phi, weight );
		}
		else
		{
			h_GenPt_Lead->Fill( genlep2.Pt, weight );
			h_GenEta_Lead->Fill( genlep2.eta, weight );
			h_GenPhi_Lead->Fill( genlep2.phi, weight );

			h_GenPt_Sub->Fill( genlep1.Pt, weight );
			h_GenEta_Sub->Fill( genlep1.eta, weight );
			h_GenPhi_Sub->Fill( genlep1.phi, weight );
		}
	}

	void FillHistograms_GenSingleMu( NtupleHandle *ntuple, GenLepton genlep, Double_t weight )
	{
		h_GenPt->Fill( genlep.Pt, weight );
		h_GenEta->Fill( genlep.eta, weight );
		h_GenPhi->Fill( genlep.phi, weight );
	}

	void FillHistograms_SingleMu(NtupleHandle *ntuple, Muon recolep, Double_t weight)
	{
		//-- Pt ,eta, phi --//
		h_Pt->Fill( recolep.Pt, weight );
		h_eta->Fill( recolep.eta, weight );
		h_phi->Fill( recolep.phi, weight );

		//-- Barrel muons --//
		if( fabs(recolep.eta) < 0.9 )
		{
			h_barrel_Pt->Fill( recolep.Pt, weight );
			h_barrel_eta->Fill( recolep.eta, weight );
			h_barrel_phi->Fill( recolep.phi, weight );
		}
		else // -- endcap -- //
		{
			h_endcap_Pt->Fill( recolep.Pt, weight );
			h_endcap_eta->Fill( recolep.eta, weight );
			h_endcap_phi->Fill( recolep.phi, weight );
		}

		//-- Pt plots for negative/positive charged muons --//
		if( recolep.charge == -1 )
		{
			h_Pt_minusCharge->Fill( recolep.Pt, weight );
			h_eta_minusCharge->Fill( recolep.eta, weight );
			h_phi_minusCharge->Fill( recolep.phi, weight );
		}
		else if( recolep.charge == 1 )
		{
			h_Pt_plusCharge->Fill( recolep.Pt, weight );
			h_eta_plusCharge->Fill( recolep.eta, weight );
			h_phi_plusCharge->Fill( recolep.phi, weight );
		}

		//-- Trigger Matched objects --//
		if( analyzer->HLT == "HLT_PAL3Mu12_v*" )
		{
			if( recolep.isTrigMatched(ntuple, "HLT_PAL3Mu12_v*") )
			{
				h_Pt_TrigLeg->Fill( recolep.Pt, weight );
				h_eta_TrigLeg->Fill( recolep.eta, weight );
				h_phi_TrigLeg->Fill( recolep.phi, weight );
			}
			else
			{
				h_Pt_OtherLeg->Fill( recolep.Pt, weight );
				h_eta_OtherLeg->Fill( recolep.eta, weight );
				h_phi_OtherLeg->Fill( recolep.phi, weight );
			}
		}
		else
		{
			if( recolep.isTrigMatched(ntuple, analyzer->HLT) )
			{
				h_Pt_TrigLeg->Fill( recolep.Pt, weight );
				h_eta_TrigLeg->Fill( recolep.eta, weight );
				h_phi_TrigLeg->Fill( recolep.phi, weight );
			}
			else
			{
				h_Pt_OtherLeg->Fill( recolep.Pt, weight );
				h_eta_OtherLeg->Fill( recolep.eta, weight );
				h_phi_OtherLeg->Fill( recolep.phi, weight );
			}
		}

		h_muonHits->Fill( recolep.muonHits, weight );
		h_nMatches->Fill( recolep.nMatches, weight );
		h_RelPtError->Fill( recolep.Best_pTError / recolep.Best_pT, weight );
		h_dxyVTX->Fill( recolep.dxyVTX, weight );
		h_dzVTX->Fill( recolep.dzVTX, weight );
		h_pixelHits->Fill( recolep.pixelHits, weight );
		h_trackerLayers->Fill( recolep.trackerLayers, weight );
		h_RelTrkIso->Fill( recolep.trkiso, weight );
		h_RelTrkIso_fullrange->Fill( recolep.trkiso, weight );
		h_RelPFIso->Fill( recolep.relPFiso, weight );
	}

	void FillHistograms_DoubleMu(NtupleHandle *ntuple, Muon recolep1, Muon recolep2, Double_t weight)
	{
		this->FillHistograms_SingleMu(ntuple, recolep1, weight);
		this->FillHistograms_SingleMu(ntuple, recolep2, weight);

		TLorentzVector reco_v1 = recolep1.Momentum;
		TLorentzVector reco_v2 = recolep2.Momentum;
      TLorentzVector reco_v12 = reco_v1 + reco_v2;
		Double_t reco_M = reco_v12.M();
		Double_t reco_Rap = reco_v12.Rapidity();
		Double_t reco_Pt = reco_v12.Pt();
      Bool_t isOS = ( recolep1.charge != recolep2.charge );

      double maxtrkiso = max(recolep1.trkiso, recolep2.trkiso);
      double maxrelPFiso = max(recolep1.relPFiso, recolep2.relPFiso);

      if (!isOS) {
			h_mass_SS->Fill( reco_M, weight );
			h_mass2_SS->Fill( reco_M, weight );
			h_mass3bins_SS->Fill( reco_M, weight );

         if( reco_M >= 15 && reco_M < 60 ) {
            h_diRap2_SS_M15to60->Fill( reco_Rap-rapshift, weight);
            h_diPt2_SS_M15to60->Fill( reco_Pt, weight );
            h_Phistar2_SS_M15to60->Fill( Object::phistar(reco_v1,reco_v2), weight );
         } else if( reco_M >= 60 && reco_M < 120 ) {
            h_diRap2_SS_M60to120->Fill( reco_Rap-rapshift, weight);
            h_diPt2_SS_M60to120->Fill( reco_Pt, weight );
            h_Phistar2_SS_M60to120->Fill( Object::phistar(reco_v1,reco_v2), weight );
         }

         // h_maxRelTrkIso_SS->Fill(maxtrkiso, weight);
         // h_maxRelPFIso_SS->Fill(maxrelPFiso, weight);
         // if( reco_M >= 15 && reco_M < 60 ) {
         //    h_maxRelTrkIso_SS_M1560->Fill(maxtrkiso, weight);
         //    h_maxRelPFIso_SS_M1560->Fill(maxrelPFiso, weight);
         // } else if( reco_M >= 60 && reco_M < 120 ) {
         //    h_maxRelTrkIso_SS_M60120->Fill(maxtrkiso, weight);
         //    h_maxRelPFIso_SS_M60120->Fill(maxrelPFiso, weight);
         // } else if( reco_M >= 120 && reco_M < 600 ) {
         //    h_maxRelTrkIso_SS_M120600->Fill(maxtrkiso, weight);
         //    h_maxRelPFIso_SS_M120600->Fill(maxrelPFiso, weight);
         // }

         return;
      }

      // from now on, we are looking at opposite sign muons.

		//Dimuon Mass/Pt/Rapidity
		h_mass->Fill( reco_M, weight );
		h_mass2->Fill( reco_M, weight );
		h_mass3bins->Fill( reco_M, weight );
		h_massZ->Fill( reco_M, weight );
		h_diPt->Fill( reco_Pt, weight );
		h_diRap->Fill( reco_Rap, weight );
      if ( reco_M >= 15 && reco_M < 20 ) {
         h_diPt_M15to20->Fill( reco_Pt, weight );
         h_diRap_M15to20->Fill( reco_Rap, weight );
      }

		// -- |y(ll)| distributions -- //
		if( reco_M >= 15 && reco_M < 60 ) {
			h_diRap_M15to60->Fill( reco_Rap, weight);
			h_diRap2_M15to60->Fill( reco_Rap-rapshift, weight);
         h_diPt2_M15to60->Fill( reco_Pt, weight );
         h_Phistar2_M15to60->Fill( Object::phistar(reco_v1,reco_v2), weight );
      } else if( reco_M >= 60 && reco_M < 120 ) {
			h_diRap_M60to120->Fill( reco_Rap, weight);
			h_diRapCM_coarse_M60to120->Fill( reco_Rap-rapshift, weight);
         h_diRap2_M60to120->Fill( reco_Rap-rapshift, weight);
         h_diPt2_M60to120->Fill( reco_Pt, weight );
         h_Phistar_M60to120->Fill( Object::phistar(reco_v1,reco_v2), weight );
         h_Phistar2_M60to120->Fill( Object::phistar(reco_v1,reco_v2), weight );

         if( reco_M >= 80 && reco_M < 100 && recolep1.Pt > 15 && recolep2.Pt > 15 ) { // mimick the selection in Fig. 105 of http://cms.cern.ch/iCMS/jsp/db_notes/noteInfo.jsp?cmsnoteid=CMS%20AN-2017/058
            if( recolep1.Pt > recolep2.Pt ) {
               h_lead_eta_M80to100->Fill( recolep1.eta, weight );
               h_sub_eta_M80to100->Fill( recolep2.eta, weight );
               h_2D_leadEta_leadPhi->Fill( recolep1.eta, recolep1.phi, weight );
               h_2D_leadEta_leadPt->Fill( recolep1.eta, recolep1.Pt, weight );
            } else {
               h_lead_eta_M80to100->Fill( recolep2.eta, weight );
               h_sub_eta_M80to100->Fill( recolep1.eta, weight );
               h_2D_leadEta_leadPhi->Fill( recolep2.eta, recolep2.phi, weight );
               h_2D_leadEta_leadPt->Fill( recolep2.eta, recolep2.Pt, weight );
            }
         }
      } else if( reco_M >= 120 && reco_M < 600 )
			h_diRap_M120to600->Fill( reco_Rap, weight);

		//leading, sub-leading muon pt,eta,phi distribution
		if( recolep1.Pt > recolep2.Pt )
		{
			h_lead_Pt->Fill( recolep1.Pt, weight );
			h_lead_eta->Fill( recolep1.eta, weight );
			h_lead_phi->Fill( recolep1.phi, weight );

			h_sub_Pt->Fill( recolep2.Pt, weight );
			h_sub_eta->Fill( recolep2.eta, weight );
			h_sub_phi->Fill( recolep2.phi, weight );
		}
		else
		{
			h_lead_Pt->Fill( recolep2.Pt, weight );
			h_lead_eta->Fill( recolep2.eta, weight );
			h_lead_phi->Fill( recolep2.phi, weight );

			h_sub_Pt->Fill( recolep1.Pt, weight );
			h_sub_eta->Fill( recolep1.eta, weight );
			h_sub_phi->Fill( recolep1.phi, weight );
		}

		//Invariant Mass
      h_mass_OS->Fill( reco_M, weight );

      Bool_t isBB = kFALSE;
      Bool_t isBE = kFALSE;
      Bool_t isEE = kFALSE;
      if( fabs(recolep1.eta) < 0.9 && fabs(recolep2.eta) < 0.9 )
         isBB = kTRUE;
      else if( fabs(recolep1.eta) > 0.9 && fabs(recolep2.eta) > 0.9 )
         isEE = kTRUE;
      else
         isBE = kTRUE;

      if( isBB == kTRUE )
         h_mass_OS_BB->Fill( reco_M, weight );
      else if( isBE == kTRUE )
         h_mass_OS_BE->Fill( reco_M, weight );
      else if( isEE == kTRUE )
         h_mass_OS_EE->Fill( reco_M, weight );

      h_PtoM->Fill( reco_Pt / reco_M, weight );

		// -- Fine binning -- //
		if( reco_M > 15 && reco_M < 60 )
		{
			h_Pt_M15to60->Fill( recolep1.Pt, weight );
         h_Pt_M15to60->Fill( recolep2.Pt, weight );
         if (reco_M > 20) h_PtoM_2060->Fill( reco_Pt / reco_M, weight );
		}
		else if( reco_M > 60 && reco_M < 120 )
		{
			h_Pt_M60to120->Fill( recolep1.Pt, weight );
			h_Pt_M60to120->Fill( recolep2.Pt, weight );
         h_PtoM_60120->Fill( reco_Pt / reco_M, weight );
		}
		else if( reco_M > 120 && reco_M < 600 )
		{
			h_Pt_M120to600->Fill( recolep1.Pt, weight );
			h_Pt_M120to600->Fill( recolep2.Pt, weight );
		}

		//Angle between two muons
		h_Angle->Fill( reco_v1.Angle( reco_v2.Vect() ), weight );

		Double_t VtxProb = 0;
		Double_t VtxNormChi2 = 0;
		DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

		h_VtxProb->Fill( VtxProb, weight );
		h_VtxNormChi2->Fill( VtxNormChi2, weight );
		h_VtxNormChi2_fullrange->Fill( VtxNormChi2, weight );

      // h_maxRelTrkIso_OS->Fill(maxtrkiso, weight);
      // h_maxRelPFIso_OS->Fill(maxrelPFiso, weight);
      // if( reco_M >= 15 && reco_M < 60 ) {
      //    h_maxRelTrkIso_OS_M1560->Fill(maxtrkiso, weight);
      //    h_maxRelPFIso_OS_M1560->Fill(maxrelPFiso, weight);
      // } else if( reco_M >= 60 && reco_M < 120 ) {
      //    h_maxRelTrkIso_OS_M60120->Fill(maxtrkiso, weight);
      //    h_maxRelPFIso_OS_M60120->Fill(maxrelPFiso, weight);
      // } else if( reco_M >= 120 && reco_M < 600 ) {
      //    h_maxRelTrkIso_OS_M120600->Fill(maxtrkiso, weight);
      //    h_maxRelPFIso_OS_M120600->Fill(maxrelPFiso, weight);
      // }
		
	}

	void FillHistograms_Hichi2(NtupleHandle *ntuple, Muon recolep1, Muon recolep2, Double_t weight) {
		TLorentzVector reco_v1 = recolep1.Momentum;
		TLorentzVector reco_v2 = recolep2.Momentum;
      TLorentzVector reco_v12 = reco_v1 + reco_v2;
		Double_t reco_M = reco_v12.M();
		Double_t reco_Rap = reco_v12.Rapidity();
		Double_t reco_Pt = reco_v12.Pt();
      Bool_t isOS = ( recolep1.charge != recolep2.charge );

      if (!isOS) {
			h_mass2_SS_Hichi2->Fill( reco_M, weight );
			h_mass3bins_SS_Hichi2->Fill( reco_M, weight );

         if( reco_M >= 15 && reco_M < 60 ) {
            h_diRap2_SS_Hichi2_M15to60->Fill( reco_Rap-rapshift, weight);
            h_diPt2_SS_Hichi2_M15to60->Fill( reco_Pt, weight );
            h_Phistar2_SS_Hichi2_M15to60->Fill( Object::phistar(reco_v1,reco_v2), weight );
         } else if( reco_M >= 60 && reco_M < 120 ) {
            h_diRap2_SS_Hichi2_M60to120->Fill( reco_Rap-rapshift, weight);
            h_diPt2_SS_Hichi2_M60to120->Fill( reco_Pt, weight );
            h_Phistar2_SS_Hichi2_M60to120->Fill( Object::phistar(reco_v1,reco_v2), weight );
         }
      } else {
			h_mass2_Hichi2->Fill( reco_M, weight );
			h_mass3bins_Hichi2->Fill( reco_M, weight );

         if( reco_M >= 15 && reco_M < 60 ) {
            h_diRap2_Hichi2_M15to60->Fill( reco_Rap-rapshift, weight);
            h_diPt2_Hichi2_M15to60->Fill( reco_Pt, weight );
            h_Phistar2_Hichi2_M15to60->Fill( Object::phistar(reco_v1,reco_v2), weight );
         } else if( reco_M >= 60 && reco_M < 120 ) {
            h_diRap2_Hichi2_M60to120->Fill( reco_Rap-rapshift, weight);
            h_diPt2_Hichi2_M60to120->Fill( reco_Pt, weight );
            h_Phistar2_Hichi2_M60to120->Fill( Object::phistar(reco_v1,reco_v2), weight );
         }
      }
   }

	void DimuonVertexProbNormChi2(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2, Double_t *VtxProb, Double_t *VtxNormChi2)
	{
		vector<double> *PtCollection1 = ntuple->vtxTrkCkt1Pt;
		vector<double> *PtCollection2 = ntuple->vtxTrkCkt2Pt;
		vector<double> *VtxProbCollection = ntuple->vtxTrkProb;

		Int_t NPt1 = (Int_t)PtCollection1->size();
		Int_t NPt2 = (Int_t)PtCollection2->size();
		Int_t NProb = (Int_t)VtxProbCollection->size();

		if( NPt1 != NPt2 || NPt2 != NProb || NPt1 != NProb ) 
			cout << "NPt1: " << NPt1 << " NPt2: " << NPt2 << " Nprob: " << NProb << endl;

		// cout << "Pt1: " << Pt1 << " Pt2: " << Pt2 << endl;

		for(Int_t i=0; i<NProb; i++)
		{
			// cout << "\tPtCollection1->at("<< i << "): " << PtCollection1->at(i) << " PtCollection2->at("<< i << "): " << PtCollection2->at(i) << endl;
			if( ( PtCollection1->at(i) == Pt1 && PtCollection2->at(i) == Pt2 )  || ( PtCollection1->at(i) == Pt2 && PtCollection2->at(i) == Pt1 ) )
			{
				*VtxProb = VtxProbCollection->at(i);
				*VtxNormChi2 = ntuple->vtxTrkChi2->at(i) / ntuple->vtxTrkNdof->at(i);
				break;
			}
		}

		return;
	}

	void WriteHistograms(TFile *fout)
	{
		fout->cd();
		for(Int_t i_hist=0; i_hist < (Int_t)Histo.size(); i_hist++) {
			Histo[i_hist]->Write();
      }
		for(Int_t i_hist=0; i_hist < (Int_t)Histo2D.size(); i_hist++) {
			Histo2D[i_hist]->Write();
      }
	}
};

class ControlPlots_Electron
{
public:
	DYAnalyzer *analyzer;
	NtupleHandle *ntuple;
	Bool_t isMC;
	std::vector<TH1D*> Histo;

	TH1D *h_Pt;
	TH1D *h_eta;
	TH1D *h_phi;
	TH1D *h_mass;
	TH1D *h_diPt;
	TH1D *h_diRap;

	TH1D *h_lead_Pt;
	TH1D *h_lead_eta;
	TH1D *h_lead_phi;

	TH1D *h_sub_Pt;
	TH1D *h_sub_eta;
	TH1D *h_sub_phi;

	TH1D *h_barrel_Pt;
	TH1D *h_barrel_eta;
	TH1D *h_barrel_phi;
	TH1D *h_barrel_mass;

	TH1D *h_endcap_Pt;
	TH1D *h_endcap_eta;
	TH1D *h_endcap_phi;
	TH1D *h_endcap_mass;

	TH1D *h_mass_OS;
	TH1D *h_mass_SS;
	TH1D *h_mass2_SS;
	TH1D *h_mass3bins_SS;

	TH1D *h_minusCharge_Pt;
	TH1D *h_minusCharge_eta;
	TH1D *h_minusCharge_phi;
	TH1D *h_plusCharge_Pt;
	TH1D *h_plusCharge_eta;
	TH1D *h_plusCharge_phi;

	TH1D *h_TrigLeg_Pt;
	TH1D *h_TrigLeg_eta;
	TH1D *h_TrigLeg_phi;

	TH1D *h_OtherLeg_Pt;
	TH1D *h_OtherLeg_eta;
	TH1D *h_OtherLeg_phi;

	TH1D *h_GenMass;

	ControlPlots_Electron(TString Type, DYAnalyzer *dyanalyzer, NtupleHandle *ntuplehandle)
	{
		isMC = kFALSE;
		if( Type != "Data" )
			isMC = kTRUE;

		analyzer = dyanalyzer;
		ntuple = ntuplehandle;


		h_Pt = new TH1D("h_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_Pt );
		h_eta = new TH1D("h_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_eta );
		h_phi = new TH1D("h_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_phi );
		h_mass = new TH1D("h_mass_"+Type, "", 100, 0, 500); Histo.push_back( h_mass );
		h_diPt = new TH1D("h_diPt_"+Type, "", 125, 0, 250); Histo.push_back( h_diPt );
		h_diRap = new TH1D("h_diRap_"+Type, "", 60, -3, 3); Histo.push_back( h_diRap );

		h_lead_Pt = new TH1D("h_lead_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_lead_Pt );
		h_lead_eta = new TH1D("h_lead_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_lead_eta );
		h_lead_phi = new TH1D("h_lead_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_lead_phi );

		h_sub_Pt = new TH1D("h_sub_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_sub_Pt );
		h_sub_eta = new TH1D("h_sub_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_sub_eta );
		h_sub_phi = new TH1D("h_sub_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_sub_phi );

		h_barrel_Pt = new TH1D("h_barrel_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_barrel_Pt );
		h_barrel_eta = new TH1D("h_barrel_eta_"+Type, "",  60, -3, 3); Histo.push_back( h_barrel_eta );
		h_barrel_phi = new TH1D("h_barrel_phi_"+Type, "",  80, -4, 4); Histo.push_back( h_barrel_phi );
		h_barrel_mass = new TH1D("h_barrel_mass_"+Type, "", 100, 0, 500); Histo.push_back( h_barrel_mass );

		h_mass_OS = new TH1D("h_mass_OS_"+Type, "", 100, 0, 500); Histo.push_back( h_mass_OS );
		h_mass_SS = new TH1D("h_mass_SS_"+Type, "", 100, 0, 500); Histo.push_back( h_mass_SS );
		h_mass2_SS = new TH1D("h_mass2_SS_"+Type, "", binnum, bins); Histo.push_back( h_mass2_SS );
		h_mass3bins_SS = new TH1D("h_mass3bins_SS_"+Type, "", binnum3, bins3); Histo.push_back( h_mass3bins_SS );

		h_minusCharge_Pt = new TH1D("h_minusCharge_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_minusCharge_Pt );
		h_minusCharge_eta = new TH1D("h_minusCharge_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_minusCharge_eta );
		h_minusCharge_phi = new TH1D("h_phi_minusCharge_"+Type, "", 80, -4, 4); Histo.push_back( h_minusCharge_phi );
		h_plusCharge_Pt = new TH1D("h_plusCharge_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_plusCharge_Pt );
		h_plusCharge_eta = new TH1D("h_plusCharge_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_plusCharge_eta );
		h_plusCharge_phi = new TH1D("h_plusCharge_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_plusCharge_phi );

		h_TrigLeg_Pt = new TH1D("h_TrigLeg_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_TrigLeg_Pt );
		h_TrigLeg_eta = new TH1D("h_TrigLeg_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_TrigLeg_eta );
		h_TrigLeg_phi = new TH1D("h_TrigLeg_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_TrigLeg_phi );

		h_OtherLeg_Pt = new TH1D("h_OtherLeg_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_OtherLeg_Pt );
		h_OtherLeg_eta = new TH1D("h_OtherLeg_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_OtherLeg_eta );
		h_OtherLeg_phi = new TH1D("h_OtherLeg_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_OtherLeg_phi );

		h_GenMass = new  TH1D("h_GenMass_"+Type, "", 100, 0, 500); Histo.push_back( h_GenMass );

	}

	void FillHistograms_GenElec(GenLepton genlep1, GenLepton genlep2, Double_t weight)
	{
		TLorentzVector gen_v1 = genlep1.Momentum;
		TLorentzVector gen_v2 = genlep2.Momentum;
		Double_t gen_M = (gen_v1 + gen_v2).M();

		h_GenMass->Fill( gen_M, weight );
	}

	void FillHistograms_SingleElec(Electron RecoObj, Double_t weight)
	{
		//-- Pt ,eta, phi --//
		h_Pt->Fill( RecoObj.Pt, weight );
		h_eta->Fill( RecoObj.eta, weight );
		h_phi->Fill( RecoObj.phi, weight );

		//-- Barrel muons --//
		if( fabs(RecoObj.eta) < 0.9 )
		{
			h_barrel_Pt->Fill( RecoObj.Pt, weight );
			h_barrel_eta->Fill( RecoObj.eta, weight );
			h_barrel_phi->Fill( RecoObj.phi, weight );
		}

		//-- Pt plots for negative/positive charged muons --//
		if( RecoObj.charge == -1 )
		{
			h_minusCharge_Pt->Fill( RecoObj.Pt, weight );
			h_minusCharge_eta->Fill( RecoObj.eta, weight );
			h_minusCharge_phi->Fill( RecoObj.phi, weight );
		}
		else if( RecoObj.charge == 1 )
		{
			h_plusCharge_Pt->Fill( RecoObj.Pt, weight );
			h_plusCharge_eta->Fill( RecoObj.eta, weight );
			h_plusCharge_phi->Fill( RecoObj.phi, weight );
		}

		TString HLT = analyzer->HLT;
		if( RecoObj.isTrigMatched(ntuple, HLT) )
		{
			h_TrigLeg_Pt->Fill( RecoObj.Pt, weight );
			h_TrigLeg_eta->Fill( RecoObj.eta, weight );
			h_TrigLeg_phi->Fill( RecoObj.phi, weight );
		}
		else
		{
			h_OtherLeg_Pt->Fill( RecoObj.Pt, weight );
			h_OtherLeg_eta->Fill( RecoObj.eta, weight );
			h_OtherLeg_phi->Fill( RecoObj.phi, weight );
		}
	}

	void FillHistograms_DoubleElec(Electron RecoObj1, Electron RecoObj2, Double_t weight)
	{
		this->FillHistograms_SingleElec(RecoObj1, weight);
		this->FillHistograms_SingleElec(RecoObj2, weight);

		TLorentzVector reco_v1 = RecoObj1.Momentum;
		TLorentzVector reco_v2 = RecoObj2.Momentum;
		Double_t reco_M = (reco_v1 + reco_v2).M();

		//Dielectron Mass/Pt/Rapidity
		Double_t reco_Rap = (reco_v1 + reco_v2).Rapidity();
		Double_t reco_Pt = (reco_v1 + reco_v2).Pt();
		h_mass->Fill( reco_M, weight );
		h_diPt->Fill( reco_Pt, weight );
		h_diRap->Fill( reco_Rap, weight );

		//leading, sub-leading electron pt,eta,phi distribution
		if( RecoObj1.Pt > RecoObj2.Pt )
		{
			h_lead_Pt->Fill( RecoObj1.Pt, weight );
			h_lead_eta->Fill( RecoObj1.eta, weight );
			h_lead_phi->Fill( RecoObj1.phi, weight );

			h_sub_Pt->Fill( RecoObj2.Pt, weight );
			h_sub_eta->Fill( RecoObj2.eta, weight );
			h_sub_phi->Fill( RecoObj2.phi, weight );
		}
		else
		{
			h_lead_Pt->Fill( RecoObj2.Pt, weight );
			h_lead_eta->Fill( RecoObj2.eta, weight );
			h_lead_phi->Fill( RecoObj2.phi, weight );

			h_sub_Pt->Fill( RecoObj1.Pt, weight );
			h_sub_eta->Fill( RecoObj1.eta, weight );
			h_sub_phi->Fill( RecoObj1.phi, weight );
		}

		//-- Mass of barrel Electrons --//
		if( fabs(RecoObj1.eta) < 0.9 && fabs(RecoObj2.eta) < 0.9 )
			h_barrel_mass->Fill( reco_M, weight );

		//Same-Sign / Opposite Invariant Mass
		if( RecoObj1.charge != RecoObj2.charge )
			h_mass_OS->Fill( reco_M, weight );
		else {
			h_mass_SS->Fill( reco_M, weight );
			h_mass2_SS->Fill( reco_M, weight );
			h_mass3bins_SS->Fill( reco_M, weight );
      }
	}

	void WriteHistograms(TFile *fout)
	{
		fout->cd();
		for(Int_t i_hist=0; i_hist < (Int_t)Histo.size(); i_hist++)
			Histo[i_hist]->Write();
	}
};

class ControlPlots_Photon
{
public:
	DYAnalyzer *analyzer;
	NtupleHandle *ntuple;
	Bool_t isMC;
	std::vector<TH1D*> Histo;

	TH1D *h_Pt;
	TH1D *h_eta;
	TH1D *h_phi;
	// TH1D *h_mass;
	// TH1D *h_diPt;
	// TH1D *h_diRap;

	// TH1D *h_lead_Pt;
	// TH1D *h_lead_eta;
	// TH1D *h_lead_phi;

	// TH1D *h_sub_Pt;
	// TH1D *h_sub_eta;
	// TH1D *h_sub_phi;

	TH1D *h_barrel_Pt;
	TH1D *h_barrel_eta;
	TH1D *h_barrel_phi;
	// TH1D *h_barrel_mass;

	TH1D *h_endcap_Pt;
	TH1D *h_endcap_eta;
	TH1D *h_endcap_phi;
	// TH1D *h_endcap_mass;

	// TH1D *h_mass_OS;
	// TH1D *h_mass_SS;

	// TH1D *h_TrigLeg_Pt;
	// TH1D *h_TrigLeg_eta;
	// TH1D *h_TrigLeg_phi;

	// TH1D *h_OtherLeg_Pt;
	// TH1D *h_OtherLeg_eta;
	// TH1D *h_OtherLeg_phi;

	// TH1D *h_GenMass;

	TH1D* h_hasPixelSeed;
	TH1D* h_etaSC;
	TH1D* h_phiSC;
	TH1D* h_HoverE;
	TH1D* h_Full5x5_SigmaIEtaIEta;
	TH1D* h_ChIso;
	TH1D* h_NhIso;
	TH1D* h_PhIso;
	TH1D* h_ChIsoWithEA;
	TH1D* h_NhIsoWithEA;
	TH1D* h_PhIsoWithEA;


	ControlPlots_Photon(TString Type, DYAnalyzer *dyanalyzer, NtupleHandle *ntuplehandle)
	{
		isMC = kFALSE;
		if( Type != "Data" )
			isMC = kTRUE;

		analyzer = dyanalyzer;
		ntuple = ntuplehandle;

		h_Pt = new TH1D("h_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_Pt );
		h_eta = new TH1D("h_eta_"+Type, "", 80, -4, 4); Histo.push_back( h_eta );
		h_phi = new TH1D("h_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_phi );
		// h_mass = new TH1D("h_mass_"+Type, "", 100, 0, 500); Histo.push_back( h_mass );
		// h_diPt = new TH1D("h_diPt_"+Type, "", 125, 0, 250); Histo.push_back( h_diPt );
		// h_diRap = new TH1D("h_diRap_"+Type, "", 60, -3, 3); Histo.push_back( h_diRap );

		// h_lead_Pt = new TH1D("h_lead_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_lead_Pt );
		// h_lead_eta = new TH1D("h_lead_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_lead_eta );
		// h_lead_phi = new TH1D("h_lead_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_lead_phi );

		// h_sub_Pt = new TH1D("h_sub_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_sub_Pt );
		// h_sub_eta = new TH1D("h_sub_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_sub_eta );
		// h_sub_phi = new TH1D("h_sub_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_sub_phi );

		h_barrel_Pt = new TH1D("h_barrel_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_barrel_Pt );
		h_barrel_eta = new TH1D("h_barrel_eta_"+Type, "",  80, -4, 4); Histo.push_back( h_barrel_eta );
		h_barrel_phi = new TH1D("h_barrel_phi_"+Type, "",  80, -4, 4); Histo.push_back( h_barrel_phi );
		// h_barrel_mass = new TH1D("h_barrel_mass_"+Type, "", 100, 0, 500); Histo.push_back( h_barrel_mass );

		h_endcap_Pt = new TH1D("h_endcap_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_endcap_Pt );
		h_endcap_eta = new TH1D("h_endcap_eta_"+Type, "",  80, -4, 4); Histo.push_back( h_endcap_eta );
		h_endcap_phi = new TH1D("h_endcap_phi_"+Type, "",  80, -4, 4); Histo.push_back( h_endcap_phi );

		// h_mass_OS = new TH1D("h_mass_OS_"+Type, "", 100, 0, 500); Histo.push_back( h_mass_OS );
		// h_mass_SS = new TH1D("h_mass_SS_"+Type, "", 100, 0, 500); Histo.push_back( h_mass_SS );

		// h_TrigLeg_Pt = new TH1D("h_TrigLeg_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_TrigLeg_Pt );
		// h_TrigLeg_eta = new TH1D("h_TrigLeg_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_TrigLeg_eta );
		// h_TrigLeg_phi = new TH1D("h_TrigLeg_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_TrigLeg_phi );

		// h_OtherLeg_Pt = new TH1D("h_OtherLeg_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_OtherLeg_Pt );
		// h_OtherLeg_eta = new TH1D("h_OtherLeg_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_OtherLeg_eta );
		// h_OtherLeg_phi = new TH1D("h_OtherLeg_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_OtherLeg_phi );

		// h_GenMass = new  TH1D("h_GenMass_"+Type, "", 100, 0, 500); Histo.push_back( h_GenMass );

		h_hasPixelSeed = new TH1D("h_hasPixelSeed_"+Type, "", 2, 0, 2); Histo.push_back( h_hasPixelSeed );
		h_etaSC = new TH1D("h_etaSC_"+Type, "", 80, -4, 4); Histo.push_back( h_etaSC );
		h_phiSC = new TH1D("h_phiSC_"+Type, "", 80, -4, 4); Histo.push_back( h_phiSC );
		h_HoverE = new TH1D("h_HoverE_"+Type, "", 500, 0, 1); Histo.push_back( h_HoverE );
		h_Full5x5_SigmaIEtaIEta = new TH1D("h_Full5x5_SigmaIEtaIEta_"+Type, "", 500, 0, 1); Histo.push_back( h_Full5x5_SigmaIEtaIEta );
		h_ChIso = new TH1D("h_ChIso_"+Type, "", 500, 0, 10); Histo.push_back( h_ChIso );
		h_NhIso = new TH1D("h_NhIso_"+Type, "", 500, 0, 10); Histo.push_back( h_NhIso );
		h_PhIso = new TH1D("h_PhIso_"+Type, "", 500, 0, 10); Histo.push_back( h_PhIso );
		h_ChIsoWithEA = new TH1D("h_ChIsoWithEA_"+Type, "", 500, 0, 10); Histo.push_back( h_ChIsoWithEA );
		h_NhIsoWithEA = new TH1D("h_NhIsoWithEA_"+Type, "", 500, 0, 10); Histo.push_back( h_NhIsoWithEA );
		h_PhIsoWithEA = new TH1D("h_PhIsoWithEA_"+Type, "", 500, 0, 10); Histo.push_back( h_PhIsoWithEA );

	}

	// void FillHistograms_GenPhoton(GenLepton genlep1, GenLepton genlep2)
	// {
	// 	Double_t AllWeight = 1;

	// 	Double_t GenWeight;
	// 	ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

	// 	Double_t PUWeight = 1;
	// 	if( isMC == kTRUE )
	// 		PUWeight = analyzer->PileUpWeightValue( ntuple->nPileUp );

	// 	AllWeight = GenWeight * PUWeight;

	// 	TLorentzVector gen_v1 = genlep1.Momentum;
	// 	TLorentzVector gen_v2 = genlep2.Momentum;
	// 	Double_t gen_M = (gen_v1 + gen_v2).M();

	// 	h_GenMass->Fill( gen_M, AllWeight );
	// }

	void FillHistograms_SinglePhoton(Photon RecoObj)
	{
		Double_t AllWeight = 1;

		Double_t GenWeight;
		ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

		Double_t PUWeight = 1;
		if( isMC == kTRUE )
			PUWeight = analyzer->PileUpWeightValue( ntuple->nPileUp );

		AllWeight = GenWeight * PUWeight;

		//-- Pt ,eta, phi --//
		h_Pt->Fill( RecoObj.Pt, AllWeight );
		h_eta->Fill( RecoObj.eta, AllWeight );
		h_phi->Fill( RecoObj.phi, AllWeight );

		//-- Barrel photons --//
		if( fabs(RecoObj.eta) < 1.479 )
		{
			h_barrel_Pt->Fill( RecoObj.Pt, AllWeight );
			h_barrel_eta->Fill( RecoObj.eta, AllWeight );
			h_barrel_phi->Fill( RecoObj.phi, AllWeight );
		}
		else
		{
			h_endcap_Pt->Fill( RecoObj.Pt, AllWeight );
			h_endcap_eta->Fill( RecoObj.eta, AllWeight );
			h_endcap_phi->Fill( RecoObj.phi, AllWeight );
		}

		// TString HLT = analyzer->HLT;
		// if( RecoObj.isTrigMatched(ntuple, HLT) )
		// {
		// 	h_TrigLeg_Pt->Fill( RecoObj.Pt, AllWeight );
		// 	h_TrigLeg_eta->Fill( RecoObj.eta, AllWeight );
		// 	h_TrigLeg_phi->Fill( RecoObj.phi, AllWeight );
		// }
		// else
		// {
		// 	h_OtherLeg_Pt->Fill( RecoObj.Pt, AllWeight );
		// 	h_OtherLeg_eta->Fill( RecoObj.eta, AllWeight );
		// 	h_OtherLeg_phi->Fill( RecoObj.phi, AllWeight );
		// }

		h_hasPixelSeed->Fill( RecoObj.hasPixelSeed, AllWeight );
		h_etaSC->Fill( RecoObj.etaSC, AllWeight );
		h_phiSC->Fill( RecoObj.phiSC, AllWeight );
		h_HoverE->Fill( RecoObj.HoverE, AllWeight );
		h_Full5x5_SigmaIEtaIEta->Fill( RecoObj.Full5x5_SigmaIEtaIEta, AllWeight );
		h_ChIso->Fill( RecoObj.ChIso, AllWeight );
		h_NhIso->Fill( RecoObj.NhIso, AllWeight );
		h_PhIso->Fill( RecoObj.PhIso, AllWeight );
		h_ChIsoWithEA->Fill( RecoObj.ChIsoWithEA, AllWeight );
		h_NhIsoWithEA->Fill( RecoObj.NhIsoWithEA, AllWeight );
		h_PhIsoWithEA->Fill( RecoObj.PhIsoWithEA, AllWeight );
	}

	// void FillHistograms_DoublePhoton(Photon RecoObj1, Photon RecoObj2)
	// {
	// 	Double_t AllWeight = 1;

	// 	Double_t GenWeight;
	// 	ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

	// 	Double_t PUWeight = 1;
	// 	if( isMC == kTRUE )
	// 		PUWeight = analyzer->PileUpWeightValue( ntuple->nPileUp );

	// 	this->FillHistograms_SinglePhoton(RecoObj1);
	// 	this->FillHistograms_SinglePhoton(RecoObj2);

	// 	TLorentzVector reco_v1 = RecoObj1.Momentum;
	// 	TLorentzVector reco_v2 = RecoObj2.Momentum;
	// 	Double_t reco_M = (reco_v1 + reco_v2).M();

	// 	//DiPhoton Mass/Pt/Rapidity
	// 	Double_t reco_Rap = (reco_v1 + reco_v2).Rapidity();
	// 	Double_t reco_Pt = (reco_v1 + reco_v2).Pt();
	// 	h_mass->Fill( reco_M, AllWeight );
	// 	h_diPt->Fill( reco_Pt, AllWeight );
	// 	h_diRap->Fill( reco_Rap, AllWeight );

	// 	//leading, sub-leading Photon pt,eta,phi distribution
	// 	if( RecoObj1.Pt > RecoObj2.Pt )
	// 	{
	// 		h_lead_Pt->Fill( RecoObj1.Pt, AllWeight );
	// 		h_lead_eta->Fill( RecoObj1.eta, AllWeight );
	// 		h_lead_phi->Fill( RecoObj1.phi, AllWeight );

	// 		h_sub_Pt->Fill( RecoObj2.Pt, AllWeight );
	// 		h_sub_eta->Fill( RecoObj2.eta, AllWeight );
	// 		h_sub_phi->Fill( RecoObj2.phi, AllWeight );
	// 	}
	// 	else
	// 	{
	// 		h_lead_Pt->Fill( RecoObj2.Pt, AllWeight );
	// 		h_lead_eta->Fill( RecoObj2.eta, AllWeight );
	// 		h_lead_phi->Fill( RecoObj2.phi, AllWeight );

	// 		h_sub_Pt->Fill( RecoObj1.Pt, AllWeight );
	// 		h_sub_eta->Fill( RecoObj1.eta, AllWeight );
	// 		h_sub_phi->Fill( RecoObj1.phi, AllWeight );
	// 	}

	// 	//-- Mass of barrel Photons --//
	// 	if( fabs(RecoObj1.eta) < 0.9 && fabs(RecoObj2.eta) < 0.9 )
	// 		h_barrel_mass->Fill( reco_M, AllWeight );

	// 	//Same-Sign / Opposite Invariant Mass
	// 	if( RecoObj1.charge != RecoObj2.charge )
	// 		h_mass_OS->Fill( reco_M, AllWeight );
	// 	else
	// 		h_mass_SS->Fill( reco_M, AllWeight );
	// }

	void WriteHistograms(TFile *fout)
	{
		fout->cd();
		for(Int_t i_hist=0; i_hist < (Int_t)Histo.size(); i_hist++)
			Histo[i_hist]->Write();
	}
};

class ControlPlots_Jet
{
public:
	DYAnalyzer *analyzer;
	NtupleHandle *ntuple;
	Bool_t isMC;
	std::vector<TH1D*> Histo;

	TH1D *h_Jet_Pt;
	TH1D *h_Jet_eta;
	TH1D *h_Jet_phi;
	TH1D *h_Jet_nJets;

	TH1D *h_Jet_Charge;
	TH1D *h_Jet_Flavor;
	TH1D *h_Jet_bTag;
	TH1D *h_Jet_CHfrac;
	TH1D *h_Jet_NHfrac;
	TH1D *h_Jet_NHEMfrac;
	TH1D *h_Jet_CHEMfrac;
	TH1D *h_Jet_CHmulti;
	TH1D *h_Jet_NHmulti;

	// TH1D *h_barrel_Pt;
	// TH1D *h_barrel_eta;
	// TH1D *h_barrel_phi;

	// TH1D *h_endcap_Pt;
	// TH1D *h_endcap_eta;
	// TH1D *h_endcap_phi;


	ControlPlots_Jet(TString Type, DYAnalyzer *dyanalyzer, NtupleHandle *ntuplehandle)
	{
		isMC = kFALSE;
		if( Type != "Data" )
			isMC = kTRUE;

		analyzer = dyanalyzer;
		ntuple = ntuplehandle;

		h_Jet_Pt = new TH1D("h_Jet_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_Jet_Pt );
		h_Jet_eta = new TH1D("h_Jet_eta_"+Type, "", 120, -6, 6); Histo.push_back( h_Jet_eta );
		h_Jet_phi = new TH1D("h_Jet_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_Jet_phi );
		h_Jet_nJets = new TH1D("h_Jet_nJets_"+Type, "", 100, 0, 100); Histo.push_back( h_Jet_nJets );

		// h_Jet_barrel_Pt = new TH1D("h_Jet_barrel_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_Jet_barrel_Pt );
		// h_Jet_barrel_eta = new TH1D("h_Jet_barrel_eta_"+Type, "",  60, -3, 3); Histo.push_back( h_Jet_barrel_eta );
		// h_Jet_barrel_phi = new TH1D("h_Jet_barrel_phi_"+Type, "",  80, -4, 4); Histo.push_back( h_Jet_barrel_phi );

		// h_Jet_endcap_Pt = new TH1D("h_Jet_endcap_Pt_"+Type, "", 125, 0, 250); Histo.push_back( h_Jet_endcap_Pt );
		// h_Jet_endcap_eta = new TH1D("h_Jet_endcap_eta_"+Type, "",  60, -3, 3); Histo.push_back( h_Jet_endcap_eta );
		// h_Jet_endcap_phi = new TH1D("h_Jet_endcap_phi_"+Type, "",  80, -4, 4); Histo.push_back( h_Jet_endcap_phi );

		h_Jet_Charge = new TH1D("h_Jet_Charge_"+Type, "", 40, -2, 2); Histo.push_back( h_Jet_Charge );
		h_Jet_Flavor = new TH1D("h_Jet_Flavor_"+Type, "", 200, -100, 100); Histo.push_back( h_Jet_Flavor );
		h_Jet_bTag = new TH1D("h_Jet_bTag_"+Type, "", 400, -2, 2); Histo.push_back( h_Jet_bTag );
		h_Jet_CHfrac = new TH1D("h_Jet_CHfrac_"+Type, "", 200, 0, 2); Histo.push_back( h_Jet_CHfrac );
		h_Jet_NHfrac = new TH1D("h_Jet_NHfrac_"+Type, "", 200, 0, 2); Histo.push_back( h_Jet_NHfrac );
		h_Jet_NHEMfrac = new TH1D("h_Jet_NHEMfrac_"+Type, "", 200, 0, 2); Histo.push_back( h_Jet_NHEMfrac );
		h_Jet_CHEMfrac = new TH1D("h_Jet_CHEMfrac_"+Type, "", 200, 0, 2); Histo.push_back( h_Jet_CHEMfrac );
		h_Jet_CHmulti = new TH1D("h_Jet_CHmulti_"+Type, "", 100, 0, 100); Histo.push_back( h_Jet_CHmulti );
		h_Jet_NHmulti = new TH1D("h_Jet_NHmulti_"+Type, "", 100, 0, 100); Histo.push_back( h_Jet_NHmulti );

	}

	void FillHistograms_SingleJet(Jet RecoObj)
	{
		Double_t AllWeight = 1;

		Double_t GenWeight;
		ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

		Double_t PUWeight = 1;
		if( isMC == kTRUE )
			PUWeight = analyzer->PileUpWeightValue( ntuple->nPileUp );

		AllWeight = GenWeight * PUWeight;

		//-- Pt ,eta, phi --//
		h_Jet_Pt->Fill( RecoObj.Pt, AllWeight );
		h_Jet_eta->Fill( RecoObj.eta, AllWeight );
		h_Jet_phi->Fill( RecoObj.phi, AllWeight );
		h_Jet_nJets->Fill( ntuple->Njets, AllWeight );

		// //-- Barrel Jet --//
		// if( fabs(RecoObj.eta) < 1.479 )
		// {
		// 	h_Jet_barrel_Pt->Fill( RecoObj.Pt, AllWeight );
		// 	h_Jet_barrel_eta->Fill( RecoObj.eta, AllWeight );
		// 	h_Jet_barrel_phi->Fill( RecoObj.phi, AllWeight );
		// }
		// else
		// {
		// 	h_Jet_endcap_Pt->Fill( RecoObj.Pt, AllWeight );
		// 	h_Jet_endcap_eta->Fill( RecoObj.eta, AllWeight );
		// 	h_Jet_endcap_phi->Fill( RecoObj.phi, AllWeight );
		// }

		h_Jet_Charge->Fill( RecoObj.Charge, AllWeight );
		h_Jet_Flavor->Fill( RecoObj.Flavor, AllWeight );
		h_Jet_bTag->Fill( RecoObj.bTag, AllWeight );
		h_Jet_CHfrac->Fill( RecoObj.CHfrac, AllWeight );
		h_Jet_NHfrac->Fill( RecoObj.NHfrac, AllWeight );
		h_Jet_NHEMfrac->Fill( RecoObj.NHEMfrac, AllWeight );
		h_Jet_CHEMfrac->Fill( RecoObj.CHEMfrac, AllWeight );
		h_Jet_CHmulti->Fill( RecoObj.CHmulti, AllWeight );
		h_Jet_NHmulti->Fill( RecoObj.NHmulti, AllWeight );

	}

	void WriteHistograms(TFile *fout)
	{
		fout->cd();
		for(Int_t i_hist=0; i_hist < (Int_t)Histo.size(); i_hist++)
			Histo[i_hist]->Write();
	}
};


class ControlPlots_MET
{
public:
	DYAnalyzer *analyzer;
	NtupleHandle *ntuple;
	Bool_t isMC;
	std::vector<TH1D*> Histo;

	TH1D* h_pfMET_pT;
	TH1D* h_pfMET_phi;
	TH1D* h_pfMET_Px;
	TH1D* h_pfMET_Py;
	TH1D* h_pfMET_SumEt;

	TH1D* h_pfMET_Type1_pT;
	TH1D* h_pfMET_Type1_phi;
	TH1D* h_pfMET_Type1_Px;
	TH1D* h_pfMET_Type1_Py;
	TH1D* h_pfMET_Type1_SumEt;

	ControlPlots_MET(TString Type, DYAnalyzer *dyanalyzer, NtupleHandle *ntuplehandle)
	{
		isMC = kFALSE;
		if( Type != "Data" )
			isMC = kTRUE;

		analyzer = dyanalyzer;
		ntuple = ntuplehandle;

		h_pfMET_pT = new TH1D("h_pfMET_pT_"+Type, "", 100, 0, 100); Histo.push_back( h_pfMET_pT );
		h_pfMET_phi = new TH1D("h_pfMET_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_pfMET_phi );
		h_pfMET_Px = new TH1D("h_pfMET_Px_"+Type, "", 100, -100, 100); Histo.push_back( h_pfMET_Px );
		h_pfMET_Py = new TH1D("h_pfMET_Py_"+Type, "", 100, -100, 100); Histo.push_back( h_pfMET_Py );
		h_pfMET_SumEt = new TH1D("h_pfMET_SumEt_"+Type, "", 200, 0, 2000); Histo.push_back( h_pfMET_SumEt );

		h_pfMET_Type1_pT = new TH1D("h_pfMET_Type1_pT_"+Type, "", 100, 0, 100); Histo.push_back( h_pfMET_Type1_pT );
		h_pfMET_Type1_phi = new TH1D("h_pfMET_Type1_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_pfMET_Type1_phi );
		h_pfMET_Type1_Px = new TH1D("h_pfMET_Type1_Px_"+Type, "", 100, -100, 100); Histo.push_back( h_pfMET_Type1_Px );
		h_pfMET_Type1_Py = new TH1D("h_pfMET_Type1_Py_"+Type, "", 100, -100, 100); Histo.push_back( h_pfMET_Type1_Py );
		h_pfMET_Type1_SumEt = new TH1D("h_pfMET_Type1_SumEt_"+Type, "", 200, 0, 2000); Histo.push_back( h_pfMET_Type1_SumEt );
	}

	void FillHistograms_MET(double AllWeight)
	{
		h_pfMET_pT->Fill( ntuple->pfMET_pT, AllWeight );
		h_pfMET_phi->Fill( ntuple->pfMET_phi, AllWeight );
		h_pfMET_Px->Fill( ntuple->pfMET_Px, AllWeight );
		h_pfMET_Py->Fill( ntuple->pfMET_Py, AllWeight );
		h_pfMET_SumEt->Fill( ntuple->pfMET_SumEt, AllWeight );

		h_pfMET_Type1_pT->Fill( ntuple->pfMET_Type1_pT, AllWeight );
		h_pfMET_Type1_phi->Fill( ntuple->pfMET_Type1_phi, AllWeight );
		h_pfMET_Type1_Px->Fill( ntuple->pfMET_Type1_Px, AllWeight );
		h_pfMET_Type1_Py->Fill( ntuple->pfMET_Type1_Py, AllWeight );
		h_pfMET_Type1_SumEt->Fill( ntuple->pfMET_Type1_SumEt, AllWeight );

	}

	void WriteHistograms(TFile *fout)
	{
		fout->cd();
		for(Int_t i_hist=0; i_hist < (Int_t)Histo.size(); i_hist++)
			Histo[i_hist]->Write();
	}
};

class ControlPlots_iso
{

public:
	DYAnalyzer *analyzer;
	Bool_t isMC;
	std::vector<TH1D*> Histo;

   TH1D *h_Mass[NISOBINS];
   TH1D *h_Rap60120[NISOBINS];
   TH1D *h_Pt[NISOBINS];
   TH1D *h_Phistar[NISOBINS];
   TH1D *h_Rap1560[NISOBINS];
   TH1D *h_Pt1560[NISOBINS];
   TH1D *h_Phistar1560[NISOBINS];
   TH1D *h_Mass_SS[NISOBINS];
   TH1D *h_Rap60120_SS[NISOBINS];
   TH1D *h_Pt_SS[NISOBINS];
   TH1D *h_Phistar_SS[NISOBINS];
   TH1D *h_Rap1560_SS[NISOBINS];
   TH1D *h_Pt1560_SS[NISOBINS];
   TH1D *h_Phistar1560_SS[NISOBINS];

   TH1D *h_Mass_hivtx[NISOBINS];
   TH1D *h_Rap60120_hivtx[NISOBINS];
   TH1D *h_Pt_hivtx[NISOBINS];
   TH1D *h_Phistar_hivtx[NISOBINS];
   TH1D *h_Rap1560_hivtx[NISOBINS];
   TH1D *h_Pt1560_hivtx[NISOBINS];
   TH1D *h_Phistar1560_hivtx[NISOBINS];
   TH1D *h_Mass_SS_hivtx[NISOBINS];
   TH1D *h_Rap60120_SS_hivtx[NISOBINS];
   TH1D *h_Pt_SS_hivtx[NISOBINS];
   TH1D *h_Phistar_SS_hivtx[NISOBINS];
   TH1D *h_Rap1560_SS_hivtx[NISOBINS];
   TH1D *h_Pt1560_SS_hivtx[NISOBINS];
   TH1D *h_Phistar1560_SS_hivtx[NISOBINS];

   const double iso1min[NISOBINS] = {
      0,0,0.2
      };
   const double iso1max[NISOBINS] = {
      0.2,0.2,100.
      };
   const double iso2min[NISOBINS] = {
      0,0.2,0.2
      };
   const double iso2max[NISOBINS] = {
      0.2,100.,100.
      };

	ControlPlots_iso(TString Type, DYAnalyzer *dyanalyzer)
	{
		isMC = kFALSE;
		if( Type != "Data" )
			isMC = kTRUE;

		analyzer = dyanalyzer;

      for (int i=0; i<NISOBINS; i++) {
         float iso11=iso1min[i], iso12=iso1max[i];
         float iso21=iso2min[i], iso22=iso2max[i];

         h_Mass[i] = new TH1D(Form("h_Mass_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::mass), binsvar(var::mass) ); Histo.push_back( h_Mass[i] );
         h_Rap60120[i] = new TH1D(Form("h_Rap60120_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::rap60120), binsvar(var::rap60120) ); Histo.push_back( h_Rap60120[i] );
         h_Pt[i] = new TH1D(Form("h_Pt_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::pt), binsvar(var::pt) ); Histo.push_back( h_Pt[i] );
         h_Phistar[i] = new TH1D(Form("h_Phistar_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::phistar), binsvar(var::phistar) ); Histo.push_back( h_Phistar[i] );
         h_Rap1560[i] = new TH1D(Form("h_Rap1560_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::rap1560), binsvar(var::rap1560) ); Histo.push_back( h_Rap1560[i] );
         h_Pt1560[i] = new TH1D(Form("h_Pt1560_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::pt1560), binsvar(var::pt1560) ); Histo.push_back( h_Pt1560[i] );
         h_Phistar1560[i] = new TH1D(Form("h_Phistar1560_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::phistar1560), binsvar(var::phistar1560) ); Histo.push_back( h_Phistar1560[i] );

         h_Mass_SS[i] = new TH1D(Form("h_Mass_SS_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::mass), binsvar(var::mass) ); Histo.push_back( h_Mass_SS[i] );
         h_Rap60120_SS[i] = new TH1D(Form("h_Rap60120_SS_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::rap60120), binsvar(var::rap60120) ); Histo.push_back( h_Rap60120_SS[i] );
         h_Pt_SS[i] = new TH1D(Form("h_Pt_SS_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::pt), binsvar(var::pt) ); Histo.push_back( h_Pt_SS[i] );
         h_Phistar_SS[i] = new TH1D(Form("h_Phistar_SS_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::phistar), binsvar(var::phistar) ); Histo.push_back( h_Phistar_SS[i] );
         h_Rap1560_SS[i] = new TH1D(Form("h_Rap1560_SS_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::rap1560), binsvar(var::rap1560) ); Histo.push_back( h_Rap1560_SS[i] );
         h_Pt1560_SS[i] = new TH1D(Form("h_Pt1560_SS_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::pt1560), binsvar(var::pt1560) ); Histo.push_back( h_Pt1560_SS[i] );
         h_Phistar1560_SS[i] = new TH1D(Form("h_Phistar1560_SS_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::phistar1560), binsvar(var::phistar1560) ); Histo.push_back( h_Phistar1560_SS[i] );

         h_Mass_hivtx[i] = new TH1D(Form("h_Mass_hivtx_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::mass), binsvar(var::mass) ); Histo.push_back( h_Mass_hivtx[i] );
         h_Rap60120_hivtx[i] = new TH1D(Form("h_Rap60120_hivtx_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::rap60120), binsvar(var::rap60120) ); Histo.push_back( h_Rap60120_hivtx[i] );
         h_Pt_hivtx[i] = new TH1D(Form("h_Pt_hivtx_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::pt), binsvar(var::pt) ); Histo.push_back( h_Pt_hivtx[i] );
         h_Phistar_hivtx[i] = new TH1D(Form("h_Phistar_hivtx_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::phistar), binsvar(var::phistar) ); Histo.push_back( h_Phistar_hivtx[i] );
         h_Rap1560_hivtx[i] = new TH1D(Form("h_Rap1560_hivtx_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::rap1560), binsvar(var::rap1560) ); Histo.push_back( h_Rap1560_hivtx[i] );
         h_Pt1560_hivtx[i] = new TH1D(Form("h_Pt1560_hivtx_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::pt1560), binsvar(var::pt1560) ); Histo.push_back( h_Pt1560_hivtx[i] );
         h_Phistar1560_hivtx[i] = new TH1D(Form("h_Phistar1560_hivtx_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::phistar1560), binsvar(var::phistar1560) ); Histo.push_back( h_Phistar1560_hivtx[i] );

         h_Mass_SS_hivtx[i] = new TH1D(Form("h_Mass_SS_hivtx_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::mass), binsvar(var::mass) ); Histo.push_back( h_Mass_SS_hivtx[i] );
         h_Rap60120_SS_hivtx[i] = new TH1D(Form("h_Rap60120_SS_hivtx_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::rap60120), binsvar(var::rap60120) ); Histo.push_back( h_Rap60120_SS_hivtx[i] );
         h_Pt_SS_hivtx[i] = new TH1D(Form("h_Pt_SS_hivtx_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::pt), binsvar(var::pt) ); Histo.push_back( h_Pt_SS_hivtx[i] );
         h_Phistar_SS_hivtx[i] = new TH1D(Form("h_Phistar_SS_hivtx_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::phistar), binsvar(var::phistar) ); Histo.push_back( h_Phistar_SS_hivtx[i] );
         h_Rap1560_SS_hivtx[i] = new TH1D(Form("h_Rap1560_SS_hivtx_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::rap1560), binsvar(var::rap1560) ); Histo.push_back( h_Rap1560_SS_hivtx[i] );
         h_Pt1560_SS_hivtx[i] = new TH1D(Form("h_Pt1560_SS_hivtx_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::pt1560), binsvar(var::pt1560) ); Histo.push_back( h_Pt1560_SS_hivtx[i] );
         h_Phistar1560_SS_hivtx[i] = new TH1D(Form("h_Phistar1560_SS_hivtx_%.1f-%.1f_%.1f-%.1f_",iso11,iso12,iso21,iso22)+Type, "", nbinsvar(var::phistar1560), binsvar(var::phistar1560) ); Histo.push_back( h_Phistar1560_SS_hivtx[i] );
      }
	}


	void FillHistograms_DoubleMu(NtupleHandle *ntuple, Muon recolep1, Muon recolep2, Double_t weight)
	{
		TLorentzVector reco_v1 = recolep1.Momentum;
		TLorentzVector reco_v2 = recolep2.Momentum;
      TLorentzVector reco_vv = reco_v1 + reco_v2;
		Double_t reco_M = reco_vv.M();
		Double_t reco_rap = reco_vv.Rapidity();
		Double_t reco_pt = reco_vv.Pt();
		Double_t reco_phistar = Object::phistar(reco_v1,reco_v2);
      Bool_t isOS = ( recolep1.charge != recolep2.charge );

      double mintrkiso = min(recolep1.trkiso, recolep2.trkiso);
      double maxtrkiso = max(recolep1.trkiso, recolep2.trkiso);

      for (int i=0; i<NISOBINS; i++) {
         if (!(mintrkiso>=iso1min[i] && mintrkiso<iso1max[i] && maxtrkiso>=iso2min[i] && maxtrkiso<iso2max[i])) continue;

         if (!isOS) {
            h_Mass_SS[i]->Fill( reco_M, weight );

            if (reco_M>60 && reco_M<120) {
               h_Rap60120_SS[i]->Fill( reco_rap-rapshift, weight );
               h_Pt_SS[i]->Fill( reco_pt, weight );
               h_Phistar_SS[i]->Fill( reco_phistar, weight );
            } else if (reco_M>15 && reco_M<60) {
               h_Rap1560_SS[i]->Fill( reco_rap-rapshift, weight );
               h_Pt1560_SS[i]->Fill( reco_pt, weight );
               h_Phistar1560_SS[i]->Fill( reco_phistar, weight );
            }
         } else {
            h_Mass[i]->Fill( reco_M, weight );

            if (reco_M>60 && reco_M<120) {
               h_Rap60120[i]->Fill( reco_rap-rapshift, weight );
               h_Pt[i]->Fill( reco_pt, weight );
               h_Phistar[i]->Fill( reco_phistar, weight );
            } else if (reco_M>15 && reco_M<60) {
               h_Rap1560[i]->Fill( reco_rap-rapshift, weight );
               h_Pt1560[i]->Fill( reco_pt, weight );
               h_Phistar1560[i]->Fill( reco_phistar, weight );
            }
         }
      }
	}

	void FillHistograms_DoubleMu_hivtx(NtupleHandle *ntuple, Muon recolep1, Muon recolep2, Double_t weight)
	{
		TLorentzVector reco_v1 = recolep1.Momentum;
		TLorentzVector reco_v2 = recolep2.Momentum;
      TLorentzVector reco_vv = reco_v1 + reco_v2;
		Double_t reco_M = reco_vv.M();
		Double_t reco_rap = reco_vv.Rapidity();
		Double_t reco_pt = reco_vv.Pt();
		Double_t reco_phistar = Object::phistar(reco_v1,reco_v2);
      Bool_t isOS = ( recolep1.charge != recolep2.charge );

      double mintrkiso = min(recolep1.trkiso, recolep2.trkiso);
      double maxtrkiso = max(recolep1.trkiso, recolep2.trkiso);

      for (int i=0; i<NISOBINS; i++) {
         if (!(mintrkiso>=iso1min[i] && mintrkiso<iso1max[i] && maxtrkiso>=iso2min[i] && maxtrkiso<iso2max[i])) continue;

         if (!isOS) {
            h_Mass_SS_hivtx[i]->Fill( reco_M, weight );

            if (reco_M>60 && reco_M<120) {
               h_Rap60120_SS_hivtx[i]->Fill( reco_rap-rapshift, weight );
               h_Pt_SS_hivtx[i]->Fill( reco_pt, weight );
               h_Phistar_SS_hivtx[i]->Fill( reco_phistar, weight );
            } else if (reco_M>15 && reco_M<60) {
               h_Rap1560_SS_hivtx[i]->Fill( reco_rap-rapshift, weight );
               h_Pt1560_SS_hivtx[i]->Fill( reco_pt, weight );
               h_Phistar1560_SS_hivtx[i]->Fill( reco_phistar, weight );
            }
         } else {
            h_Mass_hivtx[i]->Fill( reco_M, weight );

            if (reco_M>60 && reco_M<120) {
               h_Rap60120_hivtx[i]->Fill( reco_rap-rapshift, weight );
               h_Pt_hivtx[i]->Fill( reco_pt, weight );
               h_Phistar_hivtx[i]->Fill( reco_phistar, weight );
            } else if (reco_M>15 && reco_M<60) {
               h_Rap1560_hivtx[i]->Fill( reco_rap-rapshift, weight );
               h_Pt1560_hivtx[i]->Fill( reco_pt, weight );
               h_Phistar1560_hivtx[i]->Fill( reco_phistar, weight );
            }
         }
      }
	}

	void WriteHistograms(TFile *fout)
	{
		fout->cd();
		for(Int_t i_hist=0; i_hist < (Int_t)Histo.size(); i_hist++) 
			Histo[i_hist]->Write();
	}
};
