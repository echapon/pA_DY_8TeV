from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'DYtoMuMu_M-30_pPb_Pyquen_20170926'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'MC_cfg_pA_MiniAOD.py'
config.JobType.outputFiles = ['ntuple_skim.root']

config.Data.inputDataset = '/DYtoMuMu_M-30_pPb-EmbEPOS_8p16_Pyquen/echapon-DYtoMuMu_M-30_pPb_Pyquen_miniAOD_20170926-9c5325f5ba9d8bced523058642976d17/USER'
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.outLFNDirBase = '/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/' 
config.Data.publication = False
# config.Data.outputDatasetTag = config.General.requestName

config.Site.storageSite = 'T2_CH_CERN'
