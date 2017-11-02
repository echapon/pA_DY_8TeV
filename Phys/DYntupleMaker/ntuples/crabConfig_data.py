from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'PASingleMuon_DYtuple_PAL3Mu12_2ndpart_20170518'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'data_cfg_pA8TeV_from_miniAOD.py'
config.JobType.outputFiles = ['ntuple_skim.root']

config.Data.inputDataset = '/PASingleMuon/echapon-PASingleMuon_miniAOD_PAL3Mu12_285952_286496_20170517-a75c004fe2f95d3b9596de65492d591a/USER'
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 200
# 1st half: 285479-285832
# config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/HI/Cert_285479-285832_HI8TeV_PromptReco_pPb_Collisions16_JSON_NoL1T.txt'
# config.Data.runRange = '285479-285832'
# 2nd half: 285952-285952
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/HI/Cert_285952-286496_HI8TeV_PromptReco_Pbp_Collisions16_JSON_NoL1T.txt'
config.Data.runRange = '285952-286496'
config.Data.outLFNDirBase = '/store/group/cmst3/user/echapon/pA_8p16TeV/DYtuples/' 
config.Data.publication = False
# config.Data.outputDatasetTag = config.General.requestName

config.Site.storageSite = 'T2_CH_CERN'
