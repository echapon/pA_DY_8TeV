import FWCore.ParameterSet.Config as cms

# Z2 tune with pT-ordered showers
from Configuration.Generator.PythiaUEZ2Settings_cfi import *
from GeneratorInterface.ExternalDecays.TauolaSettings_cff import *

generator = cms.EDFilter(
    "Pythia6HadronizerFilter",
    pythiaPylistVerbosity = cms.untracked.int32(1),
    filterEfficiency = cms.untracked.double(1.0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(8160.0),
    maxEventsToPrint = cms.untracked.int32(0),
    ExternalDecays = cms.PSet(
        Tauola = cms.untracked.PSet(
            TauolaPolar,
            TauolaDefaultInputCards
            ),
        parameterSets = cms.vstring('Tauola')
        ),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        processParameters = cms.vstring(
            'MSEL=0 ! User defined processes', # or should this be 15?
            'PMAS(5,1)=4.75 ! b quark mass',
            'PMAS(6,1)=172.5 ! t quark mass',
            'MSUB(22)   =1      !ZZ production',
                        
            'MDME(174,1)=0 !Z decay into d dbar',
            'MDME(175,1)=0 !Z decay into u ubar',
            'MDME(176,1)=0 !Z decay into s sbar',
            'MDME(177,1)=0 !Z decay into c cbar',
            'MDME(178,1)=0 !Z decay into b bbar',
            'MDME(179,1)=0 !Z decay into t tbar',
            'MDME(182,1)=1 !Z decay into e- e+',
            'MDME(183,1)=1 !Z decay into nu_e nu_ebar',
            'MDME(184,1)=1 !Z decay into mu- mu+',
            'MDME(185,1)=1 !Z decay into nu_mu nu_mubar',
            'MDME(186,1)=1 !Z decay into tau- tau+',
            'MDME(187,1)=1 !Z decay into nu_tau nu_taubar',
            
            'CKIN(45)=1. !low mass cut on Z1',
            'CKIN(47)=1. !low mass cut on Z2'
            ),
        parameterSets = cms.vstring(
            'pythiaUESettings',
            'processParameters'
            )
        )
    )

configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    name = cms.untracked.string('$Source: /cvs/CMSSW/CMSSW/Configuration/GenProduction/python/EightTeV/POWHEG_PYTHIA6_Tauola_H_ZZ_4l_em_14TeV_cff.py,v $'),
    annotation = cms.untracked.string('POWHEG + PYTHIA6 + Tauola - Higgs -> ZZ -> 4l l=e/m/t at 8TeV')
    )
