# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: miniaod4 --data -s PAT --datatier MINIAOD --eventcontent MINIAOD --conditions 80X_dataRun2_Prompt_v15 --runUnscheduled --era Run2_2016_pA --filein /store/hidata/PARun2016B/PADoubleMuon/AOD/PromptReco-v1/000/284/958/00000/BCB00EE5-5BAB-E611-B3DB-02163E014319.root --fileout miniaodtest.root --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('PAT',eras.Run2_2016_pA)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('PhysicsTools.PatAlgos.slimming.metFilterPaths_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/hidata/PARun2016C/PASingleMuon/AOD/PromptReco-v1/000/285/480/00000/AAF1FE3C-09AF-E611-B0B7-02163E014168.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('miniaod4 nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.MINIAODoutput = cms.OutputModule("PoolOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(4),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('MINIAOD'),
        filterName = cms.untracked.string('')
    ),
    dropMetaData = cms.untracked.string('ALL'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    fastCloning = cms.untracked.bool(False),
    fileName = cms.untracked.string('miniaodtest.root'),
    outputCommands = process.MINIAODEventContent.outputCommands,
    overrideInputFileSplitLevels = cms.untracked.bool(True),
    SelectEvents = cms.untracked.PSet(
       SelectEvents = cms.vstring('filtersPath')
       )
)

# extend for HI
process.MINIAODoutput.outputCommands.extend(cms.untracked.vstring('keep recoCentrality*_*_*_*',
   'keep *_centralityBin_*_*',
   'keep recoClusterCompatibility*_hiClusterCompatibility_*_*'))

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_v15', '')

# Path and EndPath definitions
process.Flag_trackingFailureFilter = cms.Path(process.goodVertices+process.trackingFailureFilter)
process.Flag_goodVertices = cms.Path(process.primaryVertexFilter)
process.Flag_CSCTightHaloFilter = cms.Path(process.CSCTightHaloFilter)
process.Flag_trkPOGFilters = cms.Path(process.trkPOGFilters)
process.Flag_trkPOG_logErrorTooManyClusters = cms.Path(~process.logErrorTooManyClusters)
process.Flag_EcalDeadCellTriggerPrimitiveFilter = cms.Path(process.EcalDeadCellTriggerPrimitiveFilter)
process.Flag_ecalLaserCorrFilter = cms.Path(process.ecalLaserCorrFilter)
process.Flag_globalSuperTightHalo2016Filter = cms.Path(process.globalSuperTightHalo2016Filter)
process.Flag_eeBadScFilter = cms.Path(process.eeBadScFilter)
process.Flag_METFilters = cms.Path(process.metFilters)
process.Flag_chargedHadronTrackResolutionFilter = cms.Path(process.chargedHadronTrackResolutionFilter)
process.Flag_globalTightHalo2016Filter = cms.Path(process.globalTightHalo2016Filter)
process.Flag_CSCTightHaloTrkMuUnvetoFilter = cms.Path(process.CSCTightHaloTrkMuUnvetoFilter)
process.Flag_HBHENoiseIsoFilter = cms.Path(process.HBHENoiseFilterResultProducer+process.HBHENoiseIsoFilter)
process.Flag_hcalLaserEventFilter = cms.Path(process.hcalLaserEventFilter)
process.Flag_HBHENoiseFilter = cms.Path(process.HBHENoiseFilterResultProducer+process.HBHENoiseFilter)
process.Flag_trkPOG_toomanystripclus53X = cms.Path(~process.toomanystripclus53X)
process.Flag_EcalDeadCellBoundaryEnergyFilter = cms.Path(process.EcalDeadCellBoundaryEnergyFilter)
process.Flag_trkPOG_manystripclus53X = cms.Path(~process.manystripclus53X)
process.Flag_HcalStripHaloFilter = cms.Path(process.HcalStripHaloFilter)
process.Flag_muonBadTrackFilter = cms.Path(process.muonBadTrackFilter)
process.Flag_CSCTightHalo2015Filter = cms.Path(process.CSCTightHalo2015Filter)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.MINIAODoutput_step = cms.EndPath(process.MINIAODoutput)

### FILTERS ###
# Trigger Filter
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltFilter = hltHighLevel.clone(
      HLTPaths =  ['HLT_PAL3Mu12_v1'] , 
      TriggerResultsTag = cms.InputTag("TriggerResults","","HLT") , 
      throw = False , andOr = True , eventSetupPathsKey = cms.string( '' ) 
      )
process.triggerSelection = cms.Sequence( process.hltFilter )

process.selectMuonEvts = cms.EDFilter("PATMuonSelector", 
      src = cms.InputTag( 'patMuons' ) , 
      cut = cms.string("isPFMuon() && pt()>10.") 
      )
process.selectMuonFilter = cms.EDFilter("PATCandViewCountFilter", 
      minNumber = cms.uint32(1), 
      maxNumber = cms.uint32(9999999), 
      src = cms.InputTag("selectMuonEvts") 
      )
process.muonSelection = cms.Sequence( process.selectMuonEvts + process.selectMuonFilter )

# process.metAnaNoHF = process.metAna.clone(
#       patMETTag      = cms.InputTag("slimmedMETsNoHF"),
#       pfMETTag       = cms.InputTag("pfMetNoHF"),
#       caloMETTag     = cms.InputTag(""),
#       )
# process.metAnaSeqNoHF = cms.Sequence( process.metAnaNoHF )

# process.anaMET  = cms.EndPath( process.metAnaSeq * process.metAnaSeqNoHF )
process.filtersPath = cms.Path(
      process.triggerSelection + 
      process.muonSelection  
      # process.pfcandAnalyzer 
      )

# Schedule definition
process.schedule = cms.Schedule(
      process.filtersPath,
      process.Flag_HBHENoiseFilter,
      process.Flag_HBHENoiseIsoFilter,
      process.Flag_CSCTightHaloFilter,
      process.Flag_CSCTightHaloTrkMuUnvetoFilter,
      process.Flag_CSCTightHalo2015Filter,
      process.Flag_globalTightHalo2016Filter,
      process.Flag_globalSuperTightHalo2016Filter,
      process.Flag_HcalStripHaloFilter,
      process.Flag_hcalLaserEventFilter,
      process.Flag_EcalDeadCellTriggerPrimitiveFilter,
      process.Flag_EcalDeadCellBoundaryEnergyFilter,
      process.Flag_goodVertices,
      process.Flag_eeBadScFilter,
      process.Flag_ecalLaserCorrFilter,
      process.Flag_trkPOGFilters,
      process.Flag_chargedHadronTrackResolutionFilter,
      process.Flag_muonBadTrackFilter,
      process.Flag_trkPOG_manystripclus53X,
      process.Flag_trkPOG_toomanystripclus53X,
      process.Flag_trkPOG_logErrorTooManyClusters,
      process.Flag_METFilters,
      process.endjob_step,
      process.MINIAODoutput_step)

#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
process.load('Configuration.StandardSequences.PAT_cff')
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)

# customisation of the process.

# Automatic addition of the customisation function from PhysicsTools.PatAlgos.slimming.miniAOD_tools
from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllData 

#call to customisation function miniAOD_customizeAllData imported from PhysicsTools.PatAlgos.slimming.miniAOD_tools
process = miniAOD_customizeAllData(process)

# End of customisation functions
