import FWCore.ParameterSet.Config as cms
# from PhysicsTools.PatAlgos.patTemplate_cfg import *
from Configuration.StandardSequences.Eras import eras

isMC = False

process = cms.Process("DYSkim",eras.Run2_2016_pA)

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Options and Output Report
process.options   = cms.untracked.PSet( 
  wantSummary = cms.untracked.bool(True) 
)
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

## Source
FileName = ""
if isMC == True:
	FileName = "file:/cms/home/kplee/ROOTFiles_Test/ExampleMINIAOD_Spring15_DYLL_M50_25ns.root"
else:
   # FileName = "file:/cms/home/kplee/ROOTFiles_Test/ExampleMiniAOD_Run2015Dv1_76X_SingleMuon_Run260627.root"
   FileName =  "file:/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/miniaod_prod/CMSSW_8_0_26_patch2/test/miniaodtest.root"


process.source = cms.Source("PoolSource",
   # fileNames = cms.untracked.vstring( FileName )
	fileNames = cms.untracked.vstring( 
      "file:/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/miniaod_prod/CMSSW_8_0_28/test/miniaodtest.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_10.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_11.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_12.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_13.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_14.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_15.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_16.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_17.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_18.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_19.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_20.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_22.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_23.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_24.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_25.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_26.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_27.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_28.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_29.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_30.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_31.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_32.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_33.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_34.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_35.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_36.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_37.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_38.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_39.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_40.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_41.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_6.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_7.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_8.root",
      # "file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_9.root",
      )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# -- Geometry and Detector Conditions (needed for a few patTuple production steps) -- #
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

# -- Global Tags -- #
if isMC == True:
	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
	process.GlobalTag.globaltag = cms.string('MCRUN2_74_V9A::All')
else:
   # process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
   # process.GlobalTag.globaltag = cms.string('76X_dataRun2_v15') #prompt-reco global tag
# Global Tag:
   process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
   from Configuration.AlCa.GlobalTag import GlobalTag
   process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_v15', '')
   process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")
   process.GlobalTag.toGet = cms.VPSet(
         cms.PSet(
            record = cms.string('L1TUtmTriggerMenuRcd'),
            tag = cms.string("L1Menu_HeavyIons2016_v2_m2_xml"),
            connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
            ),
         cms.PSet(
            record = cms.string('L1TGlobalPrescalesVetosRcd'),
            tag = cms.string("L1TGlobalPrescalesVetos_Stage2v0_hlt"),
            connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
            )
         )


# # -- HLT Filters -- #
# import HLTrigger.HLTfilters.hltHighLevel_cfi
# process.dimuonsHLTFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()

# process.dimuonsHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
# process.dimuonsHLTFilter.HLTPaths = ["HLT_Mu*","HLT_DoubleMu*","HLT_IsoMu*"]

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('ntuple_skim.root')
)

# -- FastFilters -- //
# process.myGoodOfflinePrimaryVertices = cms.EDFilter("VertexSelector",
#    # src = cms.InputTag("offlinePrimaryVertices"),
#    src = cms.InputTag("offlineSlimmedPrimaryVertices"), # -- miniAOD -- #
#    cut = cms.string("!isFake && ndof > 4 && abs(z) < 24 && position.Rho < 2"), # tracksSize() > 3 for the older cut
#    filter = cms.bool(True),   # otherwise it won't filter the events, just produce an empty vertex collection.
# )

# process.noscraping = cms.EDFilter("FilterOutScraping",
#    applyfilter = cms.untracked.bool(True),
#    debugOn = cms.untracked.bool(False),
#    numtrack = cms.untracked.uint32(10),
#    thresh = cms.untracked.double(0.25)
# )

# process.FastFilters = cms.Sequence( process.goodOfflinePrimaryVertices + process.noscraping )
# process.FastFilters = cms.Sequence( process.myGoodOfflinePrimaryVertices )

#########################
# -- for electron ID -- #
#########################
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
# turn on VID producer, indicate data format  to be
# DataFormat.AOD or DataFormat.MiniAOD, as appropriate 
dataFormat = DataFormat.MiniAOD
switchOnVIDElectronIdProducer(process, dataFormat)

# define which IDs we want to produce
my_id_modules = [#'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_25ns_V1_cff',
                   #'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_PHYS14_PU20bx25_nonTrig_V1_cff',
       # 'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff',
       'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronHLTPreselecition_Summer16_V1_cff',
       'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Summer16_80X_V1_cff']

#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

###################################
# -- (reco) Photon Information -- #
###################################
# Several photon variables can not be found inside of a photon object
# and it is easiest to compute them upstream with a dedicated producer,
# such as this standard producer used for photon ID.
#    The producer computes full5x5 cluster shapes and PF isolation variables.
#
# Do not forget to add this producer to the path below!
#
process.load("RecoEgamma/PhotonIdentification/PhotonIDValueMapProducer_cfi")

#
# Set up photon ID (VID framework)
#

from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
# turn on VID producer, indicate data format  to be
switchOnVIDPhotonIdProducer(process, dataFormat)

# define which IDs we want to produce
my_id_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Spring16_V2p2_cff']

#add them to the VID producer
for idmod in my_id_modules:
   setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)


# -- load the PAT config -- //
# process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
# from PhysicsTools.PatAlgos.tools.coreTools import *
# if isMC==False:
#    removeMCMatching(process, ['All'])

#################
# -- DY Tree -- #
#################
from Phys.DYntupleMaker.DYntupleMaker_cfi import *
from Phys.DYntupleMaker.PUreweight2012_cff import *

process.recoTree = DYntupleMaker.clone()
process.recoTree.isMC = isMC
# process.recoTree.DebugLevel = cms.untracked.int32(99)

# -- Objects -- #
process.recoTree.Muon = cms.untracked.InputTag("slimmedMuons") # -- miniAOD -- #
process.recoTree.Electron = cms.untracked.InputTag("slimmedElectrons") # -- miniAOD -- #
process.recoTree.Photon = cms.untracked.InputTag("slimmedPhotons") # -- miniAOD -- #
process.recoTree.Jet = cms.untracked.InputTag("slimmedJets") # -- miniAOD -- #
process.recoTree.MET = cms.untracked.InputTag("slimmedMETs") # -- miniAOD -- #
if isMC:
   process.recoTree.GenParticle = cms.untracked.InputTag("prunedGenParticles") # -- miniAOD -- #
else:
   process.recoTree.GenParticle = cms.untracked.InputTag("")

# -- for electrons -- #
process.recoTree.rho = cms.untracked.InputTag("fixedGridRhoFastjetAll")
process.recoTree.conversionsInputTag = cms.untracked.InputTag("reducedEgamma:reducedConversions") # -- miniAOD -- #
# process.recoTree.eleVetoIdMap = cms.untracked.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto")
# process.recoTree.eleLooseIdMap = cms.untracked.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose")
# process.recoTree.eleMediumIdMap = cms.untracked.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium")
# process.recoTree.eleTightIdMap = cms.untracked.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight")
process.recoTree.eleVetoIdMap = cms.untracked.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-veto")
process.recoTree.eleLooseIdMap = cms.untracked.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose")
process.recoTree.eleMediumIdMap = cms.untracked.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-medium")
process.recoTree.eleTightIdMap = cms.untracked.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-tight")

# -- for photons -- #
process.recoTree.full5x5SigmaIEtaIEtaMap   = cms.untracked.InputTag("photonIDValueMapProducer:phoFull5x5SigmaIEtaIEta")
process.recoTree.phoChargedIsolation = cms.untracked.InputTag("photonIDValueMapProducer:phoChargedIsolation")
process.recoTree.phoNeutralHadronIsolation = cms.untracked.InputTag("photonIDValueMapProducer:phoNeutralHadronIsolation")
process.recoTree.phoPhotonIsolation = cms.untracked.InputTag("photonIDValueMapProducer:phoPhotonIsolation")
process.recoTree.effAreaChHadFile = cms.untracked.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfChargedHadrons_V2.txt")
process.recoTree.effAreaNeuHadFile= cms.untracked.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfNeutralHadrons_V2.txt")
process.recoTree.effAreaPhoFile   = cms.untracked.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfPhotons_V2.txt")

# -- for Track & Vertex -- #
# process.recoTree.PrimaryVertex = cms.untracked.InputTag("offlinePrimaryVertices") #  #
process.recoTree.PrimaryVertex = cms.untracked.InputTag("offlineSlimmedPrimaryVertices") # -- miniAOD -- #

# -- Else -- #
process.recoTree.PileUpInfo = cms.untracked.InputTag("addPileupInfo")

# -- Filters -- #
process.recoTree.ApplyFilter = False

# -- Store Flags -- #
process.recoTree.StoreElectronFlag = True
process.recoTree.StorePhotonFlag = True
process.recoTree.StoreGENFlag = isMC
process.recoTree.StoreGenOthersFlag = False
process.recoTree.StoreJetFlag = True
process.recoTree.StoreMETFlag = True


#######################
# Kyungwook's tuplizer #
#######################
from Physics.NtupleMaker.NtupleMaker_cfi import *
process.kwtuple = NtupleMaker.clone()
process.kwtuple.isMC = isMC
# process.kwtuple.DebugLevel = cms.untracked.int32(99)

# -- Objects -- #
process.kwtuple.Muons = cms.InputTag("slimmedMuons") # -- miniAOD -- #
process.kwtuple.Electrons = cms.InputTag("slimmedElectrons") # -- miniAOD -- #
process.kwtuple.Photons = cms.InputTag("slimmedPhotons") # -- miniAOD -- #
process.kwtuple.Jets = cms.InputTag("slimmedJets") # -- miniAOD -- #
process.kwtuple.MET = cms.InputTag("slimmedMETs") # -- miniAOD -- #
if isMC:
   process.kwtuple.GenParticles = cms.InputTag("prunedGenParticles") # -- miniAOD -- #
else:
   process.kwtuple.GenParticles = cms.InputTag("")

# -- for electrons -- #
process.kwtuple.rho = cms.InputTag("fixedGridRhoFastjetAll")
process.kwtuple.Conversions = cms.InputTag("reducedEgamma:reducedConversions") # -- miniAOD -- #
process.kwtuple.eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-medium")
process.kwtuple.eleTightIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-tight")

# -- for photons -- #
process.kwtuple.phoChargedIsolation = cms.InputTag("photonIDValueMapProducer:phoChargedIsolation")
process.kwtuple.phoNeutralHadronIsolation = cms.InputTag("photonIDValueMapProducer:phoNeutralHadronIsolation")
process.kwtuple.phoPhotonIsolation = cms.InputTag("photonIDValueMapProducer:phoPhotonIsolation")
process.kwtuple.effAreaChHadFile = cms.untracked.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfChargedHadrons_V2.txt")
process.kwtuple.effAreaNeuHadFile= cms.untracked.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfNeutralHadrons_V2.txt")
process.kwtuple.effAreaPhoFile   = cms.untracked.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfPhotons_V2.txt")

# -- for Track & Vertex -- #
# process.kwtuple.PrimaryVertex = cms.InputTag("offlinePrimaryVertices") #  #
process.kwtuple.Vertex = cms.InputTag("offlineSlimmedPrimaryVertices") # -- miniAOD -- #

# -- Else -- #
process.kwtuple.PU = cms.InputTag("addPileupInfo")
process.kwtuple.TriggerObjects = cms.InputTag("selectedPatTrigger")
process.kwtuple.miniRho = cms.InputTag("fixedGridRhoAll")
process.kwtuple.PFCandidates = cms.InputTag("packedPFCandidates")


####################
# -- Let it run -- #
####################
process.p = cms.Path(
  # process.FastFilters *
  # process.patCandidates *
  process.egmGsfElectronIDSequence *
  process.photonIDValueMapProducer *
  # process.patDefaultSequence
  process.recoTree *
  process.kwtuple
)
