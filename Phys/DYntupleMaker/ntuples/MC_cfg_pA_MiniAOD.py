import FWCore.ParameterSet.Config as cms
# from PhysicsTools.PatAlgos.patTemplate_cfg import *
from Configuration.StandardSequences.Eras import eras

isMC = True

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
   FileName = "file:/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/miniAOD_MC/Pyquen_DYtoLL_M_10to50_embd_pPb/DY1050_miniAOD_20170517/170517_143930/0000/miniaodtest_10.root"
else:
	FileName = "file:/cms/home/kplee/ROOTFiles_Test/ExampleAOD_Run2015Dv4_SingleMuon_Run259891.root"

process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring( FileName )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# -- Geometry and Detector Conditions (needed for a few patTuple production steps) -- #
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

# -- Global Tags -- #
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
if isMC == True:
  # rocess.GlobalTag.globaltag = cms.string('74X_mcRun2_asymptotic_v4')
  process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_pA_v4', '')
else:
  process.GlobalTag.globaltag = cms.string('74X_dataRun2_Prompt_v4') #prompt-reco global tag


# # -- HLT Filters -- #
# import HLTrigger.HLTfilters.hltHighLevel_cfi
# process.dimuonsHLTFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()

# process.dimuonsHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
# process.dimuonsHLTFilter.HLTPaths = ["HLT_Mu*","HLT_DoubleMu*","HLT_IsoMu*"]

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('ntuple_skim.root')
)

# -- FastFilters -- //
# process.goodOfflinePrimaryVertices = cms.EDFilter("VertexSelector",
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
# process.FastFilters = cms.Sequence( process.goodOfflinePrimaryVertices )

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
       # 'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff']
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

# -- Objects -- #
process.recoTree.Muon = cms.untracked.InputTag("slimmedMuons") # -- miniAOD -- #
process.recoTree.Electron = cms.untracked.InputTag("slimmedElectrons") # -- miniAOD -- #
process.recoTree.Photon = cms.untracked.InputTag("slimmedPhotons") # -- miniAOD -- #
process.recoTree.Jet = cms.untracked.InputTag("slimmedJets") # -- miniAOD -- #
process.recoTree.MET = cms.untracked.InputTag("slimmedMETs") # -- miniAOD -- #
process.recoTree.GenParticle = cms.untracked.InputTag("prunedGenParticles") # -- miniAOD -- #

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
process.recoTree.PrimaryVertex = cms.untracked.InputTag("offlineSlimmedPrimaryVertices") # -- miniAOD -- #

# -- Else -- #
process.recoTree.PileUpInfo = cms.untracked.InputTag("")

# -- Filters -- #
process.recoTree.ApplyFilter = False

# -- Store Flags -- #
process.recoTree.StoreElectronFlag = True
process.recoTree.StorePhotonFlag = True
process.recoTree.StoreGENFlag = isMC
process.recoTree.StoreGenOthersFlag = True
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
process.kwtuple.PU = cms.InputTag("")
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

# process.p.remove(process.makePatPhotons)
# process.p.remove(process.makePatJets)
# process.p.remove(process.makePatTaus)
# process.p.remove(process.makePatMETs)
# process.p.remove(process.patCandidateSummary)

# if isMC == False:
# 	process.p.remove(process.electronMatch)
# 	process.p.remove(process.muonMatch)
