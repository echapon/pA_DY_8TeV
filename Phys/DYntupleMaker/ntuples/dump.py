# Conditions read from  CMS_CONDITIONS  via FrontierProd 
import FWCore.ParameterSet.Config as cms

process = cms.Process("DYSkim")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_10.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_11.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_12.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_13.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_14.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_15.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_16.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_17.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_18.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_19.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_20.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_22.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_23.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_24.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_25.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_26.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_27.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_28.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_29.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_30.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_31.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_32.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_33.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_34.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_35.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_36.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_37.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_38.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_39.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_40.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_41.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_6.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_7.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_8.root', 
        'file:/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/miniAOD/PASingleMuon/PASingleMuon_miniAOD_PAL3Mu12_285479_285832_20170421/170421_125709/0000/miniaodtest_9.root')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.mvaEleID_PHYS14_PU20bx25_nonTrig_V1_producer_config = cms.PSet(
    mvaName = cms.string('ElectronMVAEstimatorRun2Phys14NonTrig'),
    mvaTag = cms.string('25nsV1'),
    weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EB1_5_oldscenario2phys14_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EB2_5_oldscenario2phys14_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EE_5_oldscenario2phys14_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EB1_10_oldscenario2phys14_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EB2_10_oldscenario2phys14_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EE_10_oldscenario2phys14_BDT.weights.xml')
)

process.mvaEleID_PHYS14_PU20bx25_nonTrig_V1_wp80 = cms.PSet(
    cutFlow = cms.VPSet(cms.PSet(
        cutName = cms.string('GsfEleMVACut'),
        isIgnored = cms.bool(False),
        mvaCategoriesMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Phys14NonTrig25nsV1Categories"),
        mvaCuts = cms.vdouble(-0.253, 0.081, -0.081, 0.965, 0.917, 
            0.683),
        mvaValueMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Phys14NonTrig25nsV1Values"),
        needsAdditionalProducts = cms.bool(True)
    )),
    idName = cms.string('mvaEleID-PHYS14-PU20bx25-nonTrig-V1-wp80'),
    isPOGApproved = cms.untracked.bool(False)
)

process.mvaEleID_PHYS14_PU20bx25_nonTrig_V1_wp90 = cms.PSet(
    cutFlow = cms.VPSet(cms.PSet(
        cutName = cms.string('GsfEleMVACut'),
        isIgnored = cms.bool(False),
        mvaCategoriesMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Phys14NonTrig25nsV1Categories"),
        mvaCuts = cms.vdouble(-0.483, -0.267, -0.323, 0.933, 0.825, 
            0.337),
        mvaValueMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Phys14NonTrig25nsV1Values"),
        needsAdditionalProducts = cms.bool(True)
    )),
    idName = cms.string('mvaEleID-PHYS14-PU20bx25-nonTrig-V1-wp90'),
    isPOGApproved = cms.untracked.bool(False)
)

process.mvaEleID_Spring15_25ns_Trig_V1_producer_config = cms.PSet(
    beamSpot = cms.InputTag("offlineBeamSpot"),
    conversionsAOD = cms.InputTag("allConversions"),
    conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
    mvaName = cms.string('ElectronMVAEstimatorRun2Spring15Trig'),
    mvaTag = cms.string('25nsV1'),
    weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB1_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB2_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EE_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml')
)

process.mvaEleID_Spring15_25ns_Trig_V1_wp80 = cms.PSet(
    cutFlow = cms.VPSet(cms.PSet(
        cutName = cms.string('GsfEleMVACut'),
        isIgnored = cms.bool(False),
        mvaCategoriesMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring15Trig25nsV1Categories"),
        mvaCuts = cms.vdouble(0.988153, 0.96791, 0.841729),
        mvaValueMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring15Trig25nsV1Values"),
        needsAdditionalProducts = cms.bool(True)
    )),
    idName = cms.string('mvaEleID-Spring15-25ns-Trig-V1-wp80'),
    isPOGApproved = cms.untracked.bool(True)
)

process.mvaEleID_Spring15_25ns_Trig_V1_wp90 = cms.PSet(
    cutFlow = cms.VPSet(cms.PSet(
        cutName = cms.string('GsfEleMVACut'),
        isIgnored = cms.bool(False),
        mvaCategoriesMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring15Trig25nsV1Categories"),
        mvaCuts = cms.vdouble(0.972153, 0.922126, 0.610764),
        mvaValueMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring15Trig25nsV1Values"),
        needsAdditionalProducts = cms.bool(True)
    )),
    idName = cms.string('mvaEleID-Spring15-25ns-Trig-V1-wp90'),
    isPOGApproved = cms.untracked.bool(True)
)

process.mvaEleID_Spring15_25ns_nonTrig_V1_producer_config = cms.PSet(
    beamSpot = cms.InputTag("offlineBeamSpot"),
    conversionsAOD = cms.InputTag("allConversions"),
    conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
    mvaName = cms.string('ElectronMVAEstimatorRun2Spring15NonTrig'),
    mvaTag = cms.string('25nsV1'),
    weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB1_5_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB2_5_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EE_5_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB1_10_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB2_10_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EE_10_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml')
)

process.mvaEleID_Spring15_25ns_nonTrig_V1_wp80 = cms.PSet(
    cutFlow = cms.VPSet(cms.PSet(
        cutName = cms.string('GsfEleMVACut'),
        isIgnored = cms.bool(False),
        mvaCategoriesMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Categories"),
        mvaCuts = cms.vdouble(0.287435, 0.221846, -0.303263, 0.967083, 0.929117, 
            0.726311),
        mvaValueMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Values"),
        needsAdditionalProducts = cms.bool(True)
    )),
    idName = cms.string('mvaEleID-Spring15-25ns-nonTrig-V1-wp80'),
    isPOGApproved = cms.untracked.bool(True)
)

process.mvaEleID_Spring15_25ns_nonTrig_V1_wp90 = cms.PSet(
    cutFlow = cms.VPSet(cms.PSet(
        cutName = cms.string('GsfEleMVACut'),
        isIgnored = cms.bool(False),
        mvaCategoriesMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Categories"),
        mvaCuts = cms.vdouble(-0.083313, -0.235222, -0.67099, 0.913286, 0.805013, 
            0.358969),
        mvaValueMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Values"),
        needsAdditionalProducts = cms.bool(True)
    )),
    idName = cms.string('mvaEleID-Spring15-25ns-nonTrig-V1-wp90'),
    isPOGApproved = cms.untracked.bool(True)
)

process.mvaEleID_Spring15_25ns_nonTrig_V1_wpLoose = cms.PSet(
    cutFlow = cms.VPSet(cms.PSet(
        cutName = cms.string('GsfEleMVACut'),
        isIgnored = cms.bool(False),
        mvaCategoriesMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Categories"),
        mvaCuts = cms.vdouble(-0.265, -0.556, -0.551, -0.072, -0.286, 
            -0.267),
        mvaValueMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Values"),
        needsAdditionalProducts = cms.bool(True)
    )),
    idName = cms.string('mvaEleID-Spring15-25ns-nonTrig-V1-wpLoose'),
    isPOGApproved = cms.untracked.bool(True)
)

process.mvaEleID_Spring15_50ns_Trig_V1_producer_config = cms.PSet(
    beamSpot = cms.InputTag("offlineBeamSpot"),
    conversionsAOD = cms.InputTag("allConversions"),
    conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
    mvaName = cms.string('ElectronMVAEstimatorRun2Spring15Trig'),
    mvaTag = cms.string('50nsV1'),
    weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB1_10_oldTrigSpring15_50ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB2_10_oldTrigSpring15_50ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EE_10_oldTrigSpring15_50ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml')
)

process.mvaEleID_Spring15_50ns_Trig_V1_wp80 = cms.PSet(
    cutFlow = cms.VPSet(cms.PSet(
        cutName = cms.string('GsfEleMVACut'),
        isIgnored = cms.bool(False),
        mvaCategoriesMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring15Trig50nsV1Categories"),
        mvaCuts = cms.vdouble(0.981841, 0.946762, 0.79704),
        mvaValueMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring15Trig50nsV1Values"),
        needsAdditionalProducts = cms.bool(True)
    )),
    idName = cms.string('mvaEleID-Spring15-50ns-Trig-V1-wp80'),
    isPOGApproved = cms.untracked.bool(True)
)

process.mvaEleID_Spring15_50ns_Trig_V1_wp90 = cms.PSet(
    cutFlow = cms.VPSet(cms.PSet(
        cutName = cms.string('GsfEleMVACut'),
        isIgnored = cms.bool(False),
        mvaCategoriesMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring15Trig50nsV1Categories"),
        mvaCuts = cms.vdouble(0.953843, 0.849994, 0.514118),
        mvaValueMapName = cms.InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring15Trig50nsV1Values"),
        needsAdditionalProducts = cms.bool(True)
    )),
    idName = cms.string('mvaEleID-Spring15-50ns-Trig-V1-wp90'),
    isPOGApproved = cms.untracked.bool(True)
)

process.mvaEleID_Spring16_GeneralPurpose_V1_producer_config = cms.PSet(
    beamSpot = cms.InputTag("offlineBeamSpot"),
    conversionsAOD = cms.InputTag("allConversions"),
    conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
    mvaName = cms.string('ElectronMVAEstimatorRun2Spring16GeneralPurpose'),
    mvaTag = cms.string('V1'),
    weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring16_GeneralPurpose_V1/electronID_mva_Spring16_GeneralPurpose_V1_EB1_10.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring16_GeneralPurpose_V1/electronID_mva_Spring16_GeneralPurpose_V1_EB2_10.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring16_GeneralPurpose_V1/electronID_mva_Spring16_GeneralPurpose_V1_EE_10.weights.xml')
)

process.mvaEleID_Spring16_HZZ_V1_producer_config = cms.PSet(
    beamSpot = cms.InputTag("offlineBeamSpot"),
    conversionsAOD = cms.InputTag("allConversions"),
    conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
    mvaName = cms.string('ElectronMVAEstimatorRun2Spring16HZZ'),
    mvaTag = cms.string('V1'),
    weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EB1_5.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EB2_5.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EE_5.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EB1_10.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EB2_10.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EE_10.weights.xml')
)

process.mvaPhoID_Spring15_25ns_nonTrig_V2p1_producer_config = cms.PSet(
    esEffSigmaRRMap = cms.InputTag("photonIDValueMapProducer","phoESEffSigmaRR"),
    full5x5E1x3Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E1x3"),
    full5x5E2x2Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E2x2"),
    full5x5E2x5MaxMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5E2x5Max"),
    full5x5E5x5Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E5x5"),
    full5x5SigmaIEtaIEtaMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIEta"),
    full5x5SigmaIEtaIPhiMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIPhi"),
    mvaName = cms.string('PhotonMVAEstimatorRun2Spring15NonTrig'),
    mvaTag = cms.string('25nsV2p1'),
    phoChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoChargedIsolation"),
    phoPhotonIsolation = cms.InputTag("photonIDValueMapProducer","phoPhotonIsolation"),
    phoWorstChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoWorstChargedIsolation"),
    rho = cms.InputTag("fixedGridRhoFastjetAll"),
    useValueMaps = cms.bool(False),
    weightFileNames = cms.vstring('RecoEgamma/PhotonIdentification/data/Spring15/photon_general_MVA_Spring15_25ns_EB_V2.weights.xml', 
        'RecoEgamma/PhotonIdentification/data/Spring15/photon_general_MVA_Spring15_25ns_EE_V2.weights.xml')
)

process.mvaPhoID_Spring15_25ns_nonTrig_V2p1_wp90 = cms.PSet(
    cutFlow = cms.VPSet(cms.PSet(
        cutName = cms.string('PhoMVACut'),
        isIgnored = cms.bool(False),
        mvaCategoriesMapName = cms.InputTag("photonMVAValueMapProducer","PhotonMVAEstimatorRun2Spring15NonTrig25nsV2p1Categories"),
        mvaCuts = cms.vdouble(0.374, 0.336),
        mvaValueMapName = cms.InputTag("photonMVAValueMapProducer","PhotonMVAEstimatorRun2Spring15NonTrig25nsV2p1Values"),
        needsAdditionalProducts = cms.bool(True)
    )),
    idName = cms.string('mvaPhoID-Spring15-25ns-nonTrig-V2p1-wp90'),
    isPOGApproved = cms.untracked.bool(True)
)

process.mvaPhoID_Spring15_50ns_nonTrig_V2p1_producer_config = cms.PSet(
    esEffSigmaRRMap = cms.InputTag("photonIDValueMapProducer","phoESEffSigmaRR"),
    full5x5E1x3Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E1x3"),
    full5x5E2x2Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E2x2"),
    full5x5E2x5MaxMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5E2x5Max"),
    full5x5E5x5Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E5x5"),
    full5x5SigmaIEtaIEtaMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIEta"),
    full5x5SigmaIEtaIPhiMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIPhi"),
    mvaName = cms.string('PhotonMVAEstimatorRun2Spring15NonTrig'),
    mvaTag = cms.string('50nsV2p1'),
    phoChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoChargedIsolation"),
    phoPhotonIsolation = cms.InputTag("photonIDValueMapProducer","phoPhotonIsolation"),
    phoWorstChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoWorstChargedIsolation"),
    rho = cms.InputTag("fixedGridRhoFastjetAll"),
    useValueMaps = cms.bool(False),
    weightFileNames = cms.vstring('RecoEgamma/PhotonIdentification/data/Spring15/photon_general_MVA_Spring15_50ns_EB_V2.weights.xml', 
        'RecoEgamma/PhotonIdentification/data/Spring15/photon_general_MVA_Spring15_50ns_EE_V2.weights.xml')
)

process.mvaPhoID_Spring15_50ns_nonTrig_V2p1_wp90 = cms.PSet(
    cutFlow = cms.VPSet(cms.PSet(
        cutName = cms.string('PhoMVACut'),
        isIgnored = cms.bool(False),
        mvaCategoriesMapName = cms.InputTag("photonMVAValueMapProducer","PhotonMVAEstimatorRun2Spring15NonTrig50nsV2p1Categories"),
        mvaCuts = cms.vdouble(0.29538, 0.45837),
        mvaValueMapName = cms.InputTag("photonMVAValueMapProducer","PhotonMVAEstimatorRun2Spring15NonTrig50nsV2p1Values"),
        needsAdditionalProducts = cms.bool(True)
    )),
    idName = cms.string('mvaPhoID-Spring15-50ns-nonTrig-V2p1-wp90'),
    isPOGApproved = cms.untracked.bool(True)
)

process.mvaPhoID_Spring16_nonTrig_V1_producer_config = cms.PSet(
    effAreasConfigFile = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased.txt'),
    mvaName = cms.string('PhotonMVAEstimatorRun2Spring16NonTrig'),
    mvaTag = cms.string('V1'),
    phoChargedIsolation = cms.InputTag("egmPhotonIsolation","h+-DR030-"),
    phoIsoCutoff = cms.double(2.5),
    phoIsoPtScalingCoeff = cms.vdouble(0.0053, 0.0034),
    phoPhotonIsolation = cms.InputTag("egmPhotonIsolation","gamma-DR030-"),
    phoWorstChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoWorstChargedIsolationWithConeVeto"),
    rho = cms.InputTag("fixedGridRhoAll"),
    weightFileNames = cms.vstring('RecoEgamma/PhotonIdentification/data/Spring16/photon_general_MVA_Spring16_EB_V3.weights.xml', 
        'RecoEgamma/PhotonIdentification/data/Spring16/photon_general_MVA_Spring16_EE_V3.weights.xml')
)

process.mvaPhoID_Spring16_nonTrig_V1_wp80 = cms.PSet(
    cutFlow = cms.VPSet(cms.PSet(
        cutName = cms.string('PhoMVACut'),
        isIgnored = cms.bool(False),
        mvaCategoriesMapName = cms.InputTag("photonMVAValueMapProducer","PhotonMVAEstimatorRun2Spring16NonTrigV1Categories"),
        mvaCuts = cms.vdouble(0.68, 0.6),
        mvaValueMapName = cms.InputTag("photonMVAValueMapProducer","PhotonMVAEstimatorRun2Spring16NonTrigV1Values"),
        needsAdditionalProducts = cms.bool(True)
    )),
    idName = cms.string('mvaPhoID-Spring16-nonTrig-V1-wp80'),
    isPOGApproved = cms.untracked.bool(True)
)

process.mvaPhoID_Spring16_nonTrig_V1_wp90 = cms.PSet(
    cutFlow = cms.VPSet(cms.PSet(
        cutName = cms.string('PhoMVACut'),
        isIgnored = cms.bool(False),
        mvaCategoriesMapName = cms.InputTag("photonMVAValueMapProducer","PhotonMVAEstimatorRun2Spring16NonTrigV1Categories"),
        mvaCuts = cms.vdouble(0.2, 0.2),
        mvaValueMapName = cms.InputTag("photonMVAValueMapProducer","PhotonMVAEstimatorRun2Spring16NonTrigV1Values"),
        needsAdditionalProducts = cms.bool(True)
    )),
    idName = cms.string('mvaPhoID-Spring16-nonTrig-V1-wp90'),
    isPOGApproved = cms.untracked.bool(True)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.IsoConeDefinitions = cms.VPSet(cms.PSet(
    coneSize = cms.double(0.3),
    isolateAgainst = cms.string('h+'),
    isolationAlgo = cms.string('PhotonPFIsolationWithMapBasedVeto'),
    miniAODVertexCodes = cms.vuint32(2, 3),
    vertexIndex = cms.int32(0)
), 
    cms.PSet(
        coneSize = cms.double(0.3),
        isolateAgainst = cms.string('h0'),
        isolationAlgo = cms.string('PhotonPFIsolationWithMapBasedVeto'),
        miniAODVertexCodes = cms.vuint32(2, 3),
        vertexIndex = cms.int32(0)
    ), 
    cms.PSet(
        coneSize = cms.double(0.3),
        isolateAgainst = cms.string('gamma'),
        isolationAlgo = cms.string('PhotonPFIsolationWithMapBasedVeto'),
        miniAODVertexCodes = cms.vuint32(2, 3),
        vertexIndex = cms.int32(0)
    ))

process.mvaConfigsForEleProducer = cms.VPSet(cms.PSet(
    mvaName = cms.string('ElectronMVAEstimatorRun2Phys14NonTrig'),
    mvaTag = cms.string('25nsV1'),
    weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EB1_5_oldscenario2phys14_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EB2_5_oldscenario2phys14_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EE_5_oldscenario2phys14_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EB1_10_oldscenario2phys14_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EB2_10_oldscenario2phys14_BDT.weights.xml', 
        'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EE_10_oldscenario2phys14_BDT.weights.xml')
), 
    cms.PSet(
        beamSpot = cms.InputTag("offlineBeamSpot"),
        conversionsAOD = cms.InputTag("allConversions"),
        conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
        mvaName = cms.string('ElectronMVAEstimatorRun2Spring15NonTrig'),
        mvaTag = cms.string('25nsV1'),
        weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB1_5_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB2_5_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EE_5_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB1_10_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB2_10_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EE_10_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml')
    ), 
    cms.PSet(
        beamSpot = cms.InputTag("offlineBeamSpot"),
        conversionsAOD = cms.InputTag("allConversions"),
        conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
        mvaName = cms.string('ElectronMVAEstimatorRun2Spring15Trig'),
        mvaTag = cms.string('50nsV1'),
        weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB1_10_oldTrigSpring15_50ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB2_10_oldTrigSpring15_50ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EE_10_oldTrigSpring15_50ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml')
    ), 
    cms.PSet(
        beamSpot = cms.InputTag("offlineBeamSpot"),
        conversionsAOD = cms.InputTag("allConversions"),
        conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
        mvaName = cms.string('ElectronMVAEstimatorRun2Spring15Trig'),
        mvaTag = cms.string('25nsV1'),
        weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB1_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB2_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EE_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml')
    ), 
    cms.PSet(
        beamSpot = cms.InputTag("offlineBeamSpot"),
        conversionsAOD = cms.InputTag("allConversions"),
        conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
        mvaName = cms.string('ElectronMVAEstimatorRun2Spring16HZZ'),
        mvaTag = cms.string('V1'),
        weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EB1_5.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EB2_5.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EE_5.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EB1_10.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EB2_10.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EE_10.weights.xml')
    ), 
    cms.PSet(
        beamSpot = cms.InputTag("offlineBeamSpot"),
        conversionsAOD = cms.InputTag("allConversions"),
        conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
        mvaName = cms.string('ElectronMVAEstimatorRun2Spring16GeneralPurpose'),
        mvaTag = cms.string('V1'),
        weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring16_GeneralPurpose_V1/electronID_mva_Spring16_GeneralPurpose_V1_EB1_10.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring16_GeneralPurpose_V1/electronID_mva_Spring16_GeneralPurpose_V1_EB2_10.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/Spring16_GeneralPurpose_V1/electronID_mva_Spring16_GeneralPurpose_V1_EE_10.weights.xml')
    ))

process.mvaConfigsForPhoProducer = cms.VPSet(cms.PSet(
    esEffSigmaRRMap = cms.InputTag("photonIDValueMapProducer","phoESEffSigmaRR"),
    full5x5E1x3Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E1x3"),
    full5x5E2x2Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E2x2"),
    full5x5E2x5MaxMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5E2x5Max"),
    full5x5E5x5Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E5x5"),
    full5x5SigmaIEtaIEtaMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIEta"),
    full5x5SigmaIEtaIPhiMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIPhi"),
    mvaName = cms.string('PhotonMVAEstimatorRun2Spring15NonTrig'),
    mvaTag = cms.string('50nsV2p1'),
    phoChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoChargedIsolation"),
    phoPhotonIsolation = cms.InputTag("photonIDValueMapProducer","phoPhotonIsolation"),
    phoWorstChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoWorstChargedIsolation"),
    rho = cms.InputTag("fixedGridRhoFastjetAll"),
    useValueMaps = cms.bool(False),
    weightFileNames = cms.vstring('RecoEgamma/PhotonIdentification/data/Spring15/photon_general_MVA_Spring15_50ns_EB_V2.weights.xml', 
        'RecoEgamma/PhotonIdentification/data/Spring15/photon_general_MVA_Spring15_50ns_EE_V2.weights.xml')
), 
    cms.PSet(
        esEffSigmaRRMap = cms.InputTag("photonIDValueMapProducer","phoESEffSigmaRR"),
        full5x5E1x3Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E1x3"),
        full5x5E2x2Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E2x2"),
        full5x5E2x5MaxMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5E2x5Max"),
        full5x5E5x5Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E5x5"),
        full5x5SigmaIEtaIEtaMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIEta"),
        full5x5SigmaIEtaIPhiMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIPhi"),
        mvaName = cms.string('PhotonMVAEstimatorRun2Spring15NonTrig'),
        mvaTag = cms.string('25nsV2p1'),
        phoChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoChargedIsolation"),
        phoPhotonIsolation = cms.InputTag("photonIDValueMapProducer","phoPhotonIsolation"),
        phoWorstChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoWorstChargedIsolation"),
        rho = cms.InputTag("fixedGridRhoFastjetAll"),
        useValueMaps = cms.bool(False),
        weightFileNames = cms.vstring('RecoEgamma/PhotonIdentification/data/Spring15/photon_general_MVA_Spring15_25ns_EB_V2.weights.xml', 
            'RecoEgamma/PhotonIdentification/data/Spring15/photon_general_MVA_Spring15_25ns_EE_V2.weights.xml')
    ), 
    cms.PSet(
        effAreasConfigFile = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased.txt'),
        mvaName = cms.string('PhotonMVAEstimatorRun2Spring16NonTrig'),
        mvaTag = cms.string('V1'),
        phoChargedIsolation = cms.InputTag("egmPhotonIsolation","h+-DR030-"),
        phoIsoCutoff = cms.double(2.5),
        phoIsoPtScalingCoeff = cms.vdouble(0.0053, 0.0034),
        phoPhotonIsolation = cms.InputTag("egmPhotonIsolation","gamma-DR030-"),
        phoWorstChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoWorstChargedIsolationWithConeVeto"),
        rho = cms.InputTag("fixedGridRhoAll"),
        weightFileNames = cms.vstring('RecoEgamma/PhotonIdentification/data/Spring16/photon_general_MVA_Spring16_EB_V3.weights.xml', 
            'RecoEgamma/PhotonIdentification/data/Spring16/photon_general_MVA_Spring16_EE_V3.weights.xml')
    ))

process.egmGsfElectronIDs = cms.EDProducer("VersionedGsfElectronIdProducer",
    physicsObjectIDs = cms.VPSet(cms.PSet(
        idDefinition = cms.PSet(
            cutFlow = cms.VPSet(cms.PSet(
                cutName = cms.string('MinPtCut'),
                isIgnored = cms.bool(False),
                minPt = cms.double(5.0),
                needsAdditionalProducts = cms.bool(False)
            ), 
                cms.PSet(
                    allowedEtaRanges = cms.VPSet(cms.PSet(
                        maxEta = cms.double(1.479),
                        minEta = cms.double(0.0)
                    ), 
                        cms.PSet(
                            maxEta = cms.double(2.5),
                            minEta = cms.double(1.479)
                        )),
                    cutName = cms.string('GsfEleSCEtaMultiRangeCut'),
                    isIgnored = cms.bool(False),
                    needsAdditionalProducts = cms.bool(False),
                    useAbsEta = cms.bool(True)
                ), 
                cms.PSet(
                    barrelCutOff = cms.double(1.479),
                    cutName = cms.string('GsfEleFull5x5SigmaIEtaIEtaCut'),
                    full5x5SigmaIEtaIEtaCutValueEB = cms.double(0.011),
                    full5x5SigmaIEtaIEtaCutValueEE = cms.double(0.031),
                    isIgnored = cms.bool(False),
                    needsAdditionalProducts = cms.bool(False)
                ), 
                cms.PSet(
                    barrelCutOff = cms.double(1.479),
                    cutName = cms.string('GsfEleDEtaInSeedCut'),
                    dEtaInSeedCutValueEB = cms.double(0.004),
                    dEtaInSeedCutValueEE = cms.double(1e+30),
                    isIgnored = cms.bool(False),
                    needsAdditionalProducts = cms.bool(False)
                ), 
                cms.PSet(
                    barrelCutOff = cms.double(1.479),
                    cutName = cms.string('GsfEleDPhiInCut'),
                    dPhiInCutValueEB = cms.double(0.02),
                    dPhiInCutValueEE = cms.double(1e+30),
                    isIgnored = cms.bool(False),
                    needsAdditionalProducts = cms.bool(False)
                ), 
                cms.PSet(
                    barrelCutOff = cms.double(1.479),
                    cutName = cms.string('GsfEleHadronicOverEMCut'),
                    hadronicOverEMCutValueEB = cms.double(0.06),
                    hadronicOverEMCutValueEE = cms.double(0.065),
                    isIgnored = cms.bool(False),
                    needsAdditionalProducts = cms.bool(False)
                ), 
                cms.PSet(
                    barrelCutOff = cms.double(1.479),
                    cutName = cms.string('GsfEleEInverseMinusPInverseCut'),
                    eInverseMinusPInverseCutValueEB = cms.double(0.013),
                    eInverseMinusPInverseCutValueEE = cms.double(0.013),
                    isIgnored = cms.bool(False),
                    needsAdditionalProducts = cms.bool(False)
                ), 
                cms.PSet(
                    barrelCutOff = cms.double(1.479),
                    cutName = cms.string('GsfEleCalPFClusterIsoCut'),
                    effAreasConfigFile = cms.FileInPath('RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_HLT_ecalPFClusterIso.txt'),
                    isIgnored = cms.bool(False),
                    isRelativeIso = cms.bool(True),
                    isoCutEBHighPt = cms.double(0.16),
                    isoCutEBLowPt = cms.double(0.16),
                    isoCutEEHighPt = cms.double(0.12),
                    isoCutEELowPt = cms.double(0.12),
                    isoType = cms.int32(0),
                    needsAdditionalProducts = cms.bool(True),
                    ptCutOff = cms.double(20.0),
                    rho = cms.InputTag("fixedGridRhoFastjetCentralCalo")
                ), 
                cms.PSet(
                    barrelCutOff = cms.double(1.479),
                    cutName = cms.string('GsfEleCalPFClusterIsoCut'),
                    effAreasConfigFile = cms.FileInPath('RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_HLT_hcalPFClusterIso.txt'),
                    isIgnored = cms.bool(False),
                    isRelativeIso = cms.bool(True),
                    isoCutEBHighPt = cms.double(0.12),
                    isoCutEBLowPt = cms.double(0.12),
                    isoCutEEHighPt = cms.double(0.12),
                    isoCutEELowPt = cms.double(0.12),
                    isoType = cms.int32(1),
                    needsAdditionalProducts = cms.bool(True),
                    ptCutOff = cms.double(20.0),
                    rho = cms.InputTag("fixedGridRhoFastjetCentralCalo")
                ), 
                cms.PSet(
                    constTermEB = cms.double(0.0),
                    constTermEE = cms.double(0.0),
                    cutName = cms.string('GsfEleTrkPtIsoCut'),
                    isIgnored = cms.bool(False),
                    needsAdditionalProducts = cms.bool(False),
                    slopeStartEB = cms.double(0.0),
                    slopeStartEE = cms.double(0.0),
                    slopeTermEB = cms.double(0.08),
                    slopeTermEE = cms.double(0.08)
                ), 
                cms.PSet(
                    barrelCutOff = cms.double(1.479),
                    cutName = cms.string('GsfEleNormalizedGsfChi2Cut'),
                    isIgnored = cms.bool(False),
                    needsAdditionalProducts = cms.bool(False),
                    normalizedGsfChi2CutValueEB = cms.double(1e+30),
                    normalizedGsfChi2CutValueEE = cms.double(3.0)
                )),
            idName = cms.string('cutBasedElectronHLTPreselection-Summer16-V1')
        ),
        idMD5 = cms.string('aef5f00cc25a63b44192c6fc449f7dc0'),
        isPOGApproved = cms.untracked.bool(True)
    )),
    physicsObjectSrc = cms.InputTag("slimmedElectrons")
)


process.egmPhotonIDs = cms.EDProducer("VersionedPhotonIdProducer",
    physicsObjectIDs = cms.VPSet(cms.PSet(
        idDefinition = cms.PSet(
            cutFlow = cms.VPSet(cms.PSet(
                cutName = cms.string('MinPtCut'),
                isIgnored = cms.bool(False),
                minPt = cms.double(5.0),
                needsAdditionalProducts = cms.bool(False)
            ), 
                cms.PSet(
                    allowedEtaRanges = cms.VPSet(cms.PSet(
                        maxEta = cms.double(1.479),
                        minEta = cms.double(0.0)
                    ), 
                        cms.PSet(
                            maxEta = cms.double(2.5),
                            minEta = cms.double(1.479)
                        )),
                    cutName = cms.string('PhoSCEtaMultiRangeCut'),
                    isIgnored = cms.bool(False),
                    needsAdditionalProducts = cms.bool(False),
                    useAbsEta = cms.bool(True)
                ), 
                cms.PSet(
                    barrelCutOff = cms.double(1.479),
                    cutName = cms.string('PhoSingleTowerHadOverEmCut'),
                    hadronicOverEMCutValueEB = cms.double(0.0597),
                    hadronicOverEMCutValueEE = cms.double(0.0481),
                    isIgnored = cms.bool(False),
                    needsAdditionalProducts = cms.bool(False)
                ), 
                cms.PSet(
                    barrelCutOff = cms.double(1.479),
                    cutName = cms.string('PhoFull5x5SigmaIEtaIEtaCut'),
                    cutValueEB = cms.double(0.01031),
                    cutValueEE = cms.double(0.03013),
                    full5x5SigmaIEtaIEtaMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIEta"),
                    isIgnored = cms.bool(False),
                    needsAdditionalProducts = cms.bool(False)
                ), 
                cms.PSet(
                    C1_EB = cms.double(1.295),
                    C1_EE = cms.double(1.011),
                    C2_EB = cms.double(0),
                    C2_EE = cms.double(0.0),
                    anyPFIsoMap = cms.InputTag("photonIDValueMapProducer","phoChargedIsolation"),
                    barrelCutOff = cms.double(1.479),
                    cutName = cms.string('PhoAnyPFIsoWithEACut'),
                    effAreasConfigFile = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfChargedHadrons_90percentBased.txt'),
                    isIgnored = cms.bool(False),
                    needsAdditionalProducts = cms.bool(True),
                    rho = cms.InputTag("fixedGridRhoFastjetAll"),
                    useRelativeIso = cms.bool(False)
                ), 
                cms.PSet(
                    C1_EB = cms.double(10.91),
                    C1_EE = cms.double(5.931),
                    C2_EB = cms.double(0.0148),
                    C2_EE = cms.double(0.0163),
                    C3_EB = cms.double(1.7e-05),
                    C3_EE = cms.double(1.4e-05),
                    anyPFIsoMap = cms.InputTag("photonIDValueMapProducer","phoNeutralHadronIsolation"),
                    barrelCutOff = cms.double(1.479),
                    cutName = cms.string('PhoAnyPFIsoWithEAAndQuadScalingCut'),
                    effAreasConfigFile = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfNeutralHadrons_90percentBased.txt'),
                    isIgnored = cms.bool(False),
                    needsAdditionalProducts = cms.bool(True),
                    rho = cms.InputTag("fixedGridRhoFastjetAll"),
                    useRelativeIso = cms.bool(False)
                ), 
                cms.PSet(
                    C1_EB = cms.double(3.63),
                    C1_EE = cms.double(6.641),
                    C2_EB = cms.double(0.0047),
                    C2_EE = cms.double(0.0034),
                    anyPFIsoMap = cms.InputTag("photonIDValueMapProducer","phoPhotonIsolation"),
                    barrelCutOff = cms.double(1.479),
                    cutName = cms.string('PhoAnyPFIsoWithEACut'),
                    effAreasConfigFile = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased.txt'),
                    isIgnored = cms.bool(False),
                    needsAdditionalProducts = cms.bool(True),
                    rho = cms.InputTag("fixedGridRhoFastjetAll"),
                    useRelativeIso = cms.bool(False)
                )),
            idName = cms.string('cutBasedPhotonID-Spring16-V2p2-loose')
        ),
        idMD5 = cms.string('d6ce6a4f3476294bf0a3261e00170daf'),
        isPOGApproved = cms.untracked.bool(True)
    ), 
        cms.PSet(
            idDefinition = cms.PSet(
                cutFlow = cms.VPSet(cms.PSet(
                    cutName = cms.string('MinPtCut'),
                    isIgnored = cms.bool(False),
                    minPt = cms.double(5.0),
                    needsAdditionalProducts = cms.bool(False)
                ), 
                    cms.PSet(
                        allowedEtaRanges = cms.VPSet(cms.PSet(
                            maxEta = cms.double(1.479),
                            minEta = cms.double(0.0)
                        ), 
                            cms.PSet(
                                maxEta = cms.double(2.5),
                                minEta = cms.double(1.479)
                            )),
                        cutName = cms.string('PhoSCEtaMultiRangeCut'),
                        isIgnored = cms.bool(False),
                        needsAdditionalProducts = cms.bool(False),
                        useAbsEta = cms.bool(True)
                    ), 
                    cms.PSet(
                        barrelCutOff = cms.double(1.479),
                        cutName = cms.string('PhoSingleTowerHadOverEmCut'),
                        hadronicOverEMCutValueEB = cms.double(0.0396),
                        hadronicOverEMCutValueEE = cms.double(0.0219),
                        isIgnored = cms.bool(False),
                        needsAdditionalProducts = cms.bool(False)
                    ), 
                    cms.PSet(
                        barrelCutOff = cms.double(1.479),
                        cutName = cms.string('PhoFull5x5SigmaIEtaIEtaCut'),
                        cutValueEB = cms.double(0.01022),
                        cutValueEE = cms.double(0.03001),
                        full5x5SigmaIEtaIEtaMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIEta"),
                        isIgnored = cms.bool(False),
                        needsAdditionalProducts = cms.bool(False)
                    ), 
                    cms.PSet(
                        C1_EB = cms.double(0.441),
                        C1_EE = cms.double(0.442),
                        C2_EB = cms.double(0.0),
                        C2_EE = cms.double(0.0),
                        anyPFIsoMap = cms.InputTag("photonIDValueMapProducer","phoChargedIsolation"),
                        barrelCutOff = cms.double(1.479),
                        cutName = cms.string('PhoAnyPFIsoWithEACut'),
                        effAreasConfigFile = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfChargedHadrons_90percentBased.txt'),
                        isIgnored = cms.bool(False),
                        needsAdditionalProducts = cms.bool(True),
                        rho = cms.InputTag("fixedGridRhoFastjetAll"),
                        useRelativeIso = cms.bool(False)
                    ), 
                    cms.PSet(
                        C1_EB = cms.double(2.725),
                        C1_EE = cms.double(1.715),
                        C2_EB = cms.double(0.0148),
                        C2_EE = cms.double(0.0163),
                        C3_EB = cms.double(1.7e-05),
                        C3_EE = cms.double(1.4e-05),
                        anyPFIsoMap = cms.InputTag("photonIDValueMapProducer","phoNeutralHadronIsolation"),
                        barrelCutOff = cms.double(1.479),
                        cutName = cms.string('PhoAnyPFIsoWithEAAndQuadScalingCut'),
                        effAreasConfigFile = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfNeutralHadrons_90percentBased.txt'),
                        isIgnored = cms.bool(False),
                        needsAdditionalProducts = cms.bool(True),
                        rho = cms.InputTag("fixedGridRhoFastjetAll"),
                        useRelativeIso = cms.bool(False)
                    ), 
                    cms.PSet(
                        C1_EB = cms.double(2.571),
                        C1_EE = cms.double(3.863),
                        C2_EB = cms.double(0.0047),
                        C2_EE = cms.double(0.0034),
                        anyPFIsoMap = cms.InputTag("photonIDValueMapProducer","phoPhotonIsolation"),
                        barrelCutOff = cms.double(1.479),
                        cutName = cms.string('PhoAnyPFIsoWithEACut'),
                        effAreasConfigFile = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased.txt'),
                        isIgnored = cms.bool(False),
                        needsAdditionalProducts = cms.bool(True),
                        rho = cms.InputTag("fixedGridRhoFastjetAll"),
                        useRelativeIso = cms.bool(False)
                    )),
                idName = cms.string('cutBasedPhotonID-Spring16-V2p2-medium')
            ),
            idMD5 = cms.string('c739cfd0b6287b8586da187c06d4053f'),
            isPOGApproved = cms.untracked.bool(True)
        ), 
        cms.PSet(
            idDefinition = cms.PSet(
                cutFlow = cms.VPSet(cms.PSet(
                    cutName = cms.string('MinPtCut'),
                    isIgnored = cms.bool(False),
                    minPt = cms.double(5.0),
                    needsAdditionalProducts = cms.bool(False)
                ), 
                    cms.PSet(
                        allowedEtaRanges = cms.VPSet(cms.PSet(
                            maxEta = cms.double(1.479),
                            minEta = cms.double(0.0)
                        ), 
                            cms.PSet(
                                maxEta = cms.double(2.5),
                                minEta = cms.double(1.479)
                            )),
                        cutName = cms.string('PhoSCEtaMultiRangeCut'),
                        isIgnored = cms.bool(False),
                        needsAdditionalProducts = cms.bool(False),
                        useAbsEta = cms.bool(True)
                    ), 
                    cms.PSet(
                        barrelCutOff = cms.double(1.479),
                        cutName = cms.string('PhoSingleTowerHadOverEmCut'),
                        hadronicOverEMCutValueEB = cms.double(0.0269),
                        hadronicOverEMCutValueEE = cms.double(0.0213),
                        isIgnored = cms.bool(False),
                        needsAdditionalProducts = cms.bool(False)
                    ), 
                    cms.PSet(
                        barrelCutOff = cms.double(1.479),
                        cutName = cms.string('PhoFull5x5SigmaIEtaIEtaCut'),
                        cutValueEB = cms.double(0.00994),
                        cutValueEE = cms.double(0.03),
                        full5x5SigmaIEtaIEtaMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIEta"),
                        isIgnored = cms.bool(False),
                        needsAdditionalProducts = cms.bool(False)
                    ), 
                    cms.PSet(
                        C1_EB = cms.double(0.202),
                        C1_EE = cms.double(0.034),
                        C2_EB = cms.double(0.0),
                        C2_EE = cms.double(0.0),
                        anyPFIsoMap = cms.InputTag("photonIDValueMapProducer","phoChargedIsolation"),
                        barrelCutOff = cms.double(1.479),
                        cutName = cms.string('PhoAnyPFIsoWithEACut'),
                        effAreasConfigFile = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfChargedHadrons_90percentBased.txt'),
                        isIgnored = cms.bool(False),
                        needsAdditionalProducts = cms.bool(True),
                        rho = cms.InputTag("fixedGridRhoFastjetAll"),
                        useRelativeIso = cms.bool(False)
                    ), 
                    cms.PSet(
                        C1_EB = cms.double(0.264),
                        C1_EE = cms.double(0.586),
                        C2_EB = cms.double(0.0148),
                        C2_EE = cms.double(0.0163),
                        C3_EB = cms.double(1.7e-05),
                        C3_EE = cms.double(1.4e-05),
                        anyPFIsoMap = cms.InputTag("photonIDValueMapProducer","phoNeutralHadronIsolation"),
                        barrelCutOff = cms.double(1.479),
                        cutName = cms.string('PhoAnyPFIsoWithEAAndQuadScalingCut'),
                        effAreasConfigFile = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfNeutralHadrons_90percentBased.txt'),
                        isIgnored = cms.bool(False),
                        needsAdditionalProducts = cms.bool(True),
                        rho = cms.InputTag("fixedGridRhoFastjetAll"),
                        useRelativeIso = cms.bool(False)
                    ), 
                    cms.PSet(
                        C1_EB = cms.double(2.362),
                        C1_EE = cms.double(2.617),
                        C2_EB = cms.double(0.0047),
                        C2_EE = cms.double(0.0034),
                        anyPFIsoMap = cms.InputTag("photonIDValueMapProducer","phoPhotonIsolation"),
                        barrelCutOff = cms.double(1.479),
                        cutName = cms.string('PhoAnyPFIsoWithEACut'),
                        effAreasConfigFile = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased.txt'),
                        isIgnored = cms.bool(False),
                        needsAdditionalProducts = cms.bool(True),
                        rho = cms.InputTag("fixedGridRhoFastjetAll"),
                        useRelativeIso = cms.bool(False)
                    )),
                idName = cms.string('cutBasedPhotonID-Spring16-V2p2-tight')
            ),
            idMD5 = cms.string('bdb623bdb1a15c13545020a919dd9530'),
            isPOGApproved = cms.untracked.bool(True)
        )),
    physicsObjectSrc = cms.InputTag("slimmedPhotons")
)


process.egmPhotonIsolation = cms.EDProducer("CITKPFIsolationSumProducer",
    isolationConeDefinitions = cms.VPSet(cms.PSet(
        coneSize = cms.double(0.3),
        isolateAgainst = cms.string('h+'),
        isolationAlgo = cms.string('PhotonPFIsolationWithMapBasedVeto'),
        miniAODVertexCodes = cms.vuint32(2, 3),
        vertexIndex = cms.int32(0)
    ), 
        cms.PSet(
            coneSize = cms.double(0.3),
            isolateAgainst = cms.string('h0'),
            isolationAlgo = cms.string('PhotonPFIsolationWithMapBasedVeto'),
            miniAODVertexCodes = cms.vuint32(2, 3),
            vertexIndex = cms.int32(0)
        ), 
        cms.PSet(
            coneSize = cms.double(0.3),
            isolateAgainst = cms.string('gamma'),
            isolationAlgo = cms.string('PhotonPFIsolationWithMapBasedVeto'),
            miniAODVertexCodes = cms.vuint32(2, 3),
            vertexIndex = cms.int32(0)
        )),
    srcForIsolationCone = cms.InputTag("packedPFCandidates"),
    srcToIsolate = cms.InputTag("slimmedPhotons")
)


process.electronMVAValueMapProducer = cms.EDProducer("ElectronMVAValueMapProducer",
    mvaConfigurations = cms.VPSet(cms.PSet(
        mvaName = cms.string('ElectronMVAEstimatorRun2Phys14NonTrig'),
        mvaTag = cms.string('25nsV1'),
        weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EB1_5_oldscenario2phys14_BDT.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EB2_5_oldscenario2phys14_BDT.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EE_5_oldscenario2phys14_BDT.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EB1_10_oldscenario2phys14_BDT.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EB2_10_oldscenario2phys14_BDT.weights.xml', 
            'RecoEgamma/ElectronIdentification/data/PHYS14/EIDmva_EE_10_oldscenario2phys14_BDT.weights.xml')
    ), 
        cms.PSet(
            beamSpot = cms.InputTag("offlineBeamSpot"),
            conversionsAOD = cms.InputTag("allConversions"),
            conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
            mvaName = cms.string('ElectronMVAEstimatorRun2Spring15NonTrig'),
            mvaTag = cms.string('25nsV1'),
            weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB1_5_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB2_5_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EE_5_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB1_10_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB2_10_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EE_10_oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal_BDT.weights.xml')
        ), 
        cms.PSet(
            beamSpot = cms.InputTag("offlineBeamSpot"),
            conversionsAOD = cms.InputTag("allConversions"),
            conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
            mvaName = cms.string('ElectronMVAEstimatorRun2Spring15Trig'),
            mvaTag = cms.string('50nsV1'),
            weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB1_10_oldTrigSpring15_50ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB2_10_oldTrigSpring15_50ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EE_10_oldTrigSpring15_50ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml')
        ), 
        cms.PSet(
            beamSpot = cms.InputTag("offlineBeamSpot"),
            conversionsAOD = cms.InputTag("allConversions"),
            conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
            mvaName = cms.string('ElectronMVAEstimatorRun2Spring15Trig'),
            mvaTag = cms.string('25nsV1'),
            weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB1_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EB2_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring15/EIDmva_EE_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml')
        ), 
        cms.PSet(
            beamSpot = cms.InputTag("offlineBeamSpot"),
            conversionsAOD = cms.InputTag("allConversions"),
            conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
            mvaName = cms.string('ElectronMVAEstimatorRun2Spring16HZZ'),
            mvaTag = cms.string('V1'),
            weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EB1_5.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EB2_5.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EE_5.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EB1_10.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EB2_10.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring16_HZZ_V1/electronID_mva_Spring16_HZZ_V1_EE_10.weights.xml')
        ), 
        cms.PSet(
            beamSpot = cms.InputTag("offlineBeamSpot"),
            conversionsAOD = cms.InputTag("allConversions"),
            conversionsMiniAOD = cms.InputTag("reducedEgamma","reducedConversions"),
            mvaName = cms.string('ElectronMVAEstimatorRun2Spring16GeneralPurpose'),
            mvaTag = cms.string('V1'),
            weightFileNames = cms.vstring('RecoEgamma/ElectronIdentification/data/Spring16_GeneralPurpose_V1/electronID_mva_Spring16_GeneralPurpose_V1_EB1_10.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring16_GeneralPurpose_V1/electronID_mva_Spring16_GeneralPurpose_V1_EB2_10.weights.xml', 
                'RecoEgamma/ElectronIdentification/data/Spring16_GeneralPurpose_V1/electronID_mva_Spring16_GeneralPurpose_V1_EE_10.weights.xml')
        )),
    src = cms.InputTag("gedGsfElectrons"),
    srcMiniAOD = cms.InputTag("slimmedElectrons","","@skipCurrentProcess")
)


process.electronRegressionValueMapProducer = cms.EDProducer("ElectronRegressionValueMapProducer",
    ebReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsEB"),
    ebReducedRecHitCollectionMiniAOD = cms.InputTag("reducedEgamma","reducedEBRecHits"),
    eeReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsEE"),
    eeReducedRecHitCollectionMiniAOD = cms.InputTag("reducedEgamma","reducedEERecHits"),
    esReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsES"),
    esReducedRecHitCollectionMiniAOD = cms.InputTag("reducedEgamma","reducedESRecHits"),
    src = cms.InputTag("gedGsfElectrons"),
    srcMiniAOD = cms.InputTag("slimmedElectrons","","@skipCurrentProcess"),
    useFull5x5 = cms.bool(False)
)


process.photonIDValueMapProducer = cms.EDProducer("PhotonIDValueMapProducer",
    ebReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsEB"),
    ebReducedRecHitCollectionMiniAOD = cms.InputTag("reducedEgamma","reducedEBRecHits"),
    eeReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsEE"),
    eeReducedRecHitCollectionMiniAOD = cms.InputTag("reducedEgamma","reducedEERecHits"),
    esReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsES"),
    esReducedRecHitCollectionMiniAOD = cms.InputTag("reducedEgamma","reducedESRecHits"),
    particleBasedIsolation = cms.InputTag("particleBasedIsolation","gedPhotons"),
    pfCandidates = cms.InputTag("particleFlow"),
    pfCandidatesMiniAOD = cms.InputTag("packedPFCandidates"),
    src = cms.InputTag("gedPhotons"),
    srcMiniAOD = cms.InputTag("slimmedPhotons","","@skipCurrentProcess"),
    vertices = cms.InputTag("offlinePrimaryVertices"),
    verticesMiniAOD = cms.InputTag("offlineSlimmedPrimaryVertices")
)


process.photonMVAValueMapProducer = cms.EDProducer("PhotonMVAValueMapProducer",
    mvaConfigurations = cms.VPSet(cms.PSet(
        esEffSigmaRRMap = cms.InputTag("photonIDValueMapProducer","phoESEffSigmaRR"),
        full5x5E1x3Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E1x3"),
        full5x5E2x2Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E2x2"),
        full5x5E2x5MaxMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5E2x5Max"),
        full5x5E5x5Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E5x5"),
        full5x5SigmaIEtaIEtaMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIEta"),
        full5x5SigmaIEtaIPhiMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIPhi"),
        mvaName = cms.string('PhotonMVAEstimatorRun2Spring15NonTrig'),
        mvaTag = cms.string('50nsV2p1'),
        phoChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoChargedIsolation"),
        phoPhotonIsolation = cms.InputTag("photonIDValueMapProducer","phoPhotonIsolation"),
        phoWorstChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoWorstChargedIsolation"),
        rho = cms.InputTag("fixedGridRhoFastjetAll"),
        useValueMaps = cms.bool(False),
        weightFileNames = cms.vstring('RecoEgamma/PhotonIdentification/data/Spring15/photon_general_MVA_Spring15_50ns_EB_V2.weights.xml', 
            'RecoEgamma/PhotonIdentification/data/Spring15/photon_general_MVA_Spring15_50ns_EE_V2.weights.xml')
    ), 
        cms.PSet(
            esEffSigmaRRMap = cms.InputTag("photonIDValueMapProducer","phoESEffSigmaRR"),
            full5x5E1x3Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E1x3"),
            full5x5E2x2Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E2x2"),
            full5x5E2x5MaxMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5E2x5Max"),
            full5x5E5x5Map = cms.InputTag("photonIDValueMapProducer","phoFull5x5E5x5"),
            full5x5SigmaIEtaIEtaMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIEta"),
            full5x5SigmaIEtaIPhiMap = cms.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIPhi"),
            mvaName = cms.string('PhotonMVAEstimatorRun2Spring15NonTrig'),
            mvaTag = cms.string('25nsV2p1'),
            phoChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoChargedIsolation"),
            phoPhotonIsolation = cms.InputTag("photonIDValueMapProducer","phoPhotonIsolation"),
            phoWorstChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoWorstChargedIsolation"),
            rho = cms.InputTag("fixedGridRhoFastjetAll"),
            useValueMaps = cms.bool(False),
            weightFileNames = cms.vstring('RecoEgamma/PhotonIdentification/data/Spring15/photon_general_MVA_Spring15_25ns_EB_V2.weights.xml', 
                'RecoEgamma/PhotonIdentification/data/Spring15/photon_general_MVA_Spring15_25ns_EE_V2.weights.xml')
        ), 
        cms.PSet(
            effAreasConfigFile = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased.txt'),
            mvaName = cms.string('PhotonMVAEstimatorRun2Spring16NonTrig'),
            mvaTag = cms.string('V1'),
            phoChargedIsolation = cms.InputTag("egmPhotonIsolation","h+-DR030-"),
            phoIsoCutoff = cms.double(2.5),
            phoIsoPtScalingCoeff = cms.vdouble(0.0053, 0.0034),
            phoPhotonIsolation = cms.InputTag("egmPhotonIsolation","gamma-DR030-"),
            phoWorstChargedIsolation = cms.InputTag("photonIDValueMapProducer","phoWorstChargedIsolationWithConeVeto"),
            rho = cms.InputTag("fixedGridRhoAll"),
            weightFileNames = cms.vstring('RecoEgamma/PhotonIdentification/data/Spring16/photon_general_MVA_Spring16_EB_V3.weights.xml', 
                'RecoEgamma/PhotonIdentification/data/Spring16/photon_general_MVA_Spring16_EE_V3.weights.xml')
        )),
    src = cms.InputTag("gedPhotons"),
    srcMiniAOD = cms.InputTag("slimmedPhotons","","@skipCurrentProcess")
)


process.photonRegressionValueMapProducer = cms.EDProducer("PhotonRegressionValueMapProducer",
    ebReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsEB"),
    ebReducedRecHitCollectionMiniAOD = cms.InputTag("reducedEgamma","reducedEBRecHits"),
    eeReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsEE"),
    eeReducedRecHitCollectionMiniAOD = cms.InputTag("reducedEgamma","reducedEERecHits"),
    esReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsES"),
    esReducedRecHitCollectionMiniAOD = cms.InputTag("reducedEgamma","reducedESRecHits"),
    src = cms.InputTag("gedPhotons"),
    srcMiniAOD = cms.InputTag("slimmedPhotons","","@skipCurrentProcess"),
    useFull5x5 = cms.bool(False)
)


process.recoTree = cms.EDAnalyzer("DYntupleMaker",
    ApplyFilter = cms.untracked.bool(False),
    BDiscriminant_ssv = cms.untracked.double(2.05),
    BDiscriminant_tcheff = cms.untracked.double(0.7),
    BDiscriminant_tchpur = cms.untracked.double(0.7),
    BeamSpot = cms.untracked.InputTag("offlineBeamSpot"),
    DebugLevel = cms.untracked.int32(0),
    Electron = cms.untracked.InputTag("slimmedElectrons"),
    FilterType = cms.untracked.int32(0),
    GenEventInfo = cms.untracked.InputTag("generator"),
    GenParticle = cms.untracked.InputTag(""),
    GsfTrack = cms.untracked.InputTag("electronGsfTracks"),
    Jet = cms.untracked.InputTag("slimmedJets"),
    MET = cms.untracked.InputTag("slimmedMETs"),
    Muon = cms.untracked.InputTag("slimmedMuons"),
    Photon = cms.untracked.InputTag("slimmedPhotons"),
    PileUpInfo = cms.untracked.InputTag("addPileupInfo"),
    PrimaryVertex = cms.untracked.InputTag("offlineSlimmedPrimaryVertices"),
    StoreElectronFlag = cms.untracked.bool(True),
    StoreGENFlag = cms.untracked.bool(False),
    StoreGenOthersFlag = cms.untracked.bool(False),
    StoreHLTReportFlag = cms.untracked.bool(True),
    StoreJetFlag = cms.untracked.bool(True),
    StoreMETFlag = cms.untracked.bool(True),
    StorePhotonFlag = cms.untracked.bool(True),
    StorePriVtxFlag = cms.untracked.bool(True),
    StoreTTFlag = cms.untracked.bool(False),
    Track = cms.untracked.InputTag("generalTracks"),
    TriggerObject = cms.untracked.InputTag("selectedPatTrigger"),
    TriggerResults = cms.untracked.InputTag("TriggerResults","","HLT"),
    conversionsInputTag = cms.untracked.InputTag("reducedEgamma","reducedConversions"),
    effAreaChHadFile = cms.untracked.FileInPath('RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfChargedHadrons_V2.txt'),
    effAreaNeuHadFile = cms.untracked.FileInPath('RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfNeutralHadrons_V2.txt'),
    effAreaPhoFile = cms.untracked.FileInPath('RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfPhotons_V2.txt'),
    eleLooseIdMap = cms.untracked.InputTag("egmGsfElectronIDs","cutBasedElectronID-Summer16-80X-V1-loose"),
    eleMediumIdMap = cms.untracked.InputTag("egmGsfElectronIDs","cutBasedElectronID-Summer16-80X-V1-medium"),
    eleTightIdMap = cms.untracked.InputTag("egmGsfElectronIDs","cutBasedElectronID-Summer16-80X-V1-tight"),
    eleVetoIdMap = cms.untracked.InputTag("egmGsfElectronIDs","cutBasedElectronID-Summer16-80X-V1-veto"),
    full5x5SigmaIEtaIEtaMap = cms.untracked.InputTag("photonIDValueMapProducer","phoFull5x5SigmaIEtaIEta"),
    isMC = cms.untracked.bool(False),
    pfMET = cms.untracked.InputTag("pfMet"),
    phoChargedIsolation = cms.untracked.InputTag("photonIDValueMapProducer","phoChargedIsolation"),
    phoNeutralHadronIsolation = cms.untracked.InputTag("photonIDValueMapProducer","phoNeutralHadronIsolation"),
    phoPhotonIsolation = cms.untracked.InputTag("photonIDValueMapProducer","phoPhotonIsolation"),
    processName = cms.untracked.string('HLT'),
    rho = cms.untracked.InputTag("fixedGridRhoFastjetAll")
)


process.egmGsfElectronIDSequence = cms.Sequence(process.electronMVAValueMapProducer+process.egmGsfElectronIDs+process.electronRegressionValueMapProducer)


process.egmPhotonIsolationMiniAODSequence = cms.Sequence(process.egmPhotonIsolation)


process.egmPhotonIDSequence = cms.Sequence(process.egmPhotonIsolationMiniAODSequence+process.photonIDValueMapProducer+process.photonMVAValueMapProducer+process.egmPhotonIDs+process.photonRegressionValueMapProducer)


process.p = cms.Path(process.egmGsfElectronIDSequence+process.photonIDValueMapProducer+process.recoTree)


process.MessageLogger = cms.Service("MessageLogger",
    FrameworkJobReport = cms.untracked.PSet(
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        optionalPSet = cms.untracked.bool(True)
    ),
    categories = cms.untracked.vstring('FwkJob', 
        'FwkReport', 
        'FwkSummary', 
        'Root_NoDictionary'),
    cerr = cms.untracked.PSet(
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
            optionalPSet = cms.untracked.bool(True)
        ),
        FwkReport = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True),
            reportEvery = cms.untracked.int32(1000)
        ),
        FwkSummary = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True),
            reportEvery = cms.untracked.int32(1)
        ),
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        Root_NoDictionary = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
            optionalPSet = cms.untracked.bool(True)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000)
        ),
        noTimeStamps = cms.untracked.bool(False),
        optionalPSet = cms.untracked.bool(True),
        threshold = cms.untracked.string('INFO')
    ),
    cerr_stats = cms.untracked.PSet(
        optionalPSet = cms.untracked.bool(True),
        output = cms.untracked.string('cerr'),
        threshold = cms.untracked.string('WARNING')
    ),
    cout = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    debugModules = cms.untracked.vstring(),
    debugs = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    default = cms.untracked.PSet(

    ),
    destinations = cms.untracked.vstring('warnings', 
        'errors', 
        'infos', 
        'debugs', 
        'cout', 
        'cerr'),
    errors = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    fwkJobReports = cms.untracked.vstring('FrameworkJobReport'),
    infos = cms.untracked.PSet(
        Root_NoDictionary = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
            optionalPSet = cms.untracked.bool(True)
        ),
        optionalPSet = cms.untracked.bool(True),
        placeholder = cms.untracked.bool(True)
    ),
    statistics = cms.untracked.vstring('cerr_stats'),
    suppressDebug = cms.untracked.vstring(),
    suppressInfo = cms.untracked.vstring(),
    suppressWarning = cms.untracked.vstring(),
    warnings = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    )
)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('ntuple_skim.root')
)


process.CSCGeometryESModule = cms.ESProducer("CSCGeometryESModule",
    alignmentsLabel = cms.string(''),
    appendToDataLabel = cms.string(''),
    applyAlignment = cms.bool(True),
    debugV = cms.untracked.bool(False),
    useCentreTIOffsets = cms.bool(False),
    useDDD = cms.bool(False),
    useGangedStripsInME1a = cms.bool(False),
    useOnlyWiresInME1a = cms.bool(False),
    useRealWireGeometry = cms.bool(True)
)


process.CaloGeometryBuilder = cms.ESProducer("CaloGeometryBuilder",
    SelectedCalos = cms.vstring('HCAL', 
        'ZDC', 
        'CASTOR', 
        'EcalBarrel', 
        'EcalEndcap', 
        'EcalPreshower', 
        'TOWER')
)


process.CaloTopologyBuilder = cms.ESProducer("CaloTopologyBuilder")


process.CaloTowerGeometryFromDBEP = cms.ESProducer("CaloTowerGeometryFromDBEP",
    applyAlignment = cms.bool(False),
    hcalTopologyConstants = cms.PSet(
        maxDepthHB = cms.int32(2),
        maxDepthHE = cms.int32(3),
        mode = cms.string('HcalTopologyMode::LHC')
    )
)


process.CaloTowerTopologyEP = cms.ESProducer("CaloTowerTopologyEP")


process.CastorDbProducer = cms.ESProducer("CastorDbProducer")


process.CastorGeometryFromDBEP = cms.ESProducer("CastorGeometryFromDBEP",
    applyAlignment = cms.bool(False)
)


process.DTGeometryESModule = cms.ESProducer("DTGeometryESModule",
    alignmentsLabel = cms.string(''),
    appendToDataLabel = cms.string(''),
    applyAlignment = cms.bool(True),
    fromDDD = cms.bool(False)
)


process.EcalBarrelGeometryFromDBEP = cms.ESProducer("EcalBarrelGeometryFromDBEP",
    applyAlignment = cms.bool(True)
)


process.EcalElectronicsMappingBuilder = cms.ESProducer("EcalElectronicsMappingBuilder")


process.EcalEndcapGeometryFromDBEP = cms.ESProducer("EcalEndcapGeometryFromDBEP",
    applyAlignment = cms.bool(True)
)


process.EcalLaserCorrectionService = cms.ESProducer("EcalLaserCorrectionService")


process.EcalPreshowerGeometryFromDBEP = cms.ESProducer("EcalPreshowerGeometryFromDBEP",
    applyAlignment = cms.bool(True)
)


process.EcalTrigTowerConstituentsMapBuilder = cms.ESProducer("EcalTrigTowerConstituentsMapBuilder",
    MapFile = cms.untracked.string('Geometry/EcalMapping/data/EndCap_TTMap.txt')
)


process.GlobalTrackingGeometryESProducer = cms.ESProducer("GlobalTrackingGeometryESProducer")


process.HcalAlignmentEP = cms.ESProducer("HcalAlignmentEP")


process.HcalGeometryFromDBEP = cms.ESProducer("HcalGeometryFromDBEP",
    applyAlignment = cms.bool(True),
    hcalTopologyConstants = cms.PSet(
        maxDepthHB = cms.int32(2),
        maxDepthHE = cms.int32(3),
        mode = cms.string('HcalTopologyMode::LHC')
    )
)


process.MuonDetLayerGeometryESProducer = cms.ESProducer("MuonDetLayerGeometryESProducer")


process.ParabolicParametrizedMagneticFieldProducer = cms.ESProducer("AutoParametrizedMagneticFieldProducer",
    label = cms.untracked.string('ParabolicMf'),
    valueOverride = cms.int32(-1),
    version = cms.string('Parabolic')
)


process.RPCGeometryESModule = cms.ESProducer("RPCGeometryESModule",
    compatibiltyWith11 = cms.untracked.bool(True),
    useDDD = cms.untracked.bool(False)
)


process.SiStripRecHitMatcherESProducer = cms.ESProducer("SiStripRecHitMatcherESProducer",
    ComponentName = cms.string('StandardMatcher'),
    NSigmaInside = cms.double(3.0),
    PreFilter = cms.bool(False)
)


process.StripCPEfromTrackAngleESProducer = cms.ESProducer("StripCPEESProducer",
    ComponentName = cms.string('StripCPEfromTrackAngle'),
    ComponentType = cms.string('StripCPEfromTrackAngle'),
    parameters = cms.PSet(
        mLC_P0 = cms.double(-0.326),
        mLC_P1 = cms.double(0.618),
        mLC_P2 = cms.double(0.3),
        mTEC_P0 = cms.double(-1.885),
        mTEC_P1 = cms.double(0.471),
        mTIB_P0 = cms.double(-0.742),
        mTIB_P1 = cms.double(0.202),
        mTID_P0 = cms.double(-1.427),
        mTID_P1 = cms.double(0.433),
        mTOB_P0 = cms.double(-1.026),
        mTOB_P1 = cms.double(0.253),
        maxChgOneMIP = cms.double(6000.0),
        useLegacyError = cms.bool(False)
    )
)


process.TrackerRecoGeometryESProducer = cms.ESProducer("TrackerRecoGeometryESProducer")


process.TransientTrackBuilderESProducer = cms.ESProducer("TransientTrackBuilderESProducer",
    ComponentName = cms.string('TransientTrackBuilder')
)


process.VolumeBasedMagneticFieldESProducer = cms.ESProducer("VolumeBasedMagneticFieldESProducerFromDB",
    debugBuilder = cms.untracked.bool(False),
    label = cms.untracked.string(''),
    valueOverride = cms.int32(-1)
)


process.ZdcGeometryFromDBEP = cms.ESProducer("ZdcGeometryFromDBEP",
    applyAlignment = cms.bool(False)
)


process.fakeForIdealAlignment = cms.ESProducer("FakeAlignmentProducer",
    appendToDataLabel = cms.string('fakeForIdeal')
)


process.hcalDDDRecConstants = cms.ESProducer("HcalDDDRecConstantsESModule",
    appendToDataLabel = cms.string('')
)


process.hcalDDDSimConstants = cms.ESProducer("HcalDDDSimConstantsESModule",
    appendToDataLabel = cms.string('')
)


process.hcalTopologyIdeal = cms.ESProducer("HcalTopologyIdealEP",
    Exclude = cms.untracked.string(''),
    appendToDataLabel = cms.string('')
)


process.hcal_db_producer = cms.ESProducer("HcalDbProducer",
    dump = cms.untracked.vstring(''),
    file = cms.untracked.string('')
)


process.idealForDigiCSCGeometry = cms.ESProducer("CSCGeometryESModule",
    alignmentsLabel = cms.string('fakeForIdeal'),
    appendToDataLabel = cms.string('idealForDigi'),
    applyAlignment = cms.bool(False),
    debugV = cms.untracked.bool(False),
    useCentreTIOffsets = cms.bool(False),
    useDDD = cms.bool(False),
    useGangedStripsInME1a = cms.bool(False),
    useOnlyWiresInME1a = cms.bool(False),
    useRealWireGeometry = cms.bool(True)
)


process.idealForDigiDTGeometry = cms.ESProducer("DTGeometryESModule",
    alignmentsLabel = cms.string('fakeForIdeal'),
    appendToDataLabel = cms.string('idealForDigi'),
    applyAlignment = cms.bool(False),
    fromDDD = cms.bool(False)
)


process.idealForDigiTrackerGeometry = cms.ESProducer("TrackerDigiGeometryESModule",
    alignmentsLabel = cms.string('fakeForIdeal'),
    appendToDataLabel = cms.string('idealForDigi'),
    applyAlignment = cms.bool(False),
    fromDDD = cms.bool(False)
)


process.siPixelQualityESProducer = cms.ESProducer("SiPixelQualityESProducer",
    ListOfRecordToMerge = cms.VPSet(cms.PSet(
        record = cms.string('SiPixelQualityFromDbRcd'),
        tag = cms.string('')
    ), 
        cms.PSet(
            record = cms.string('SiPixelDetVOffRcd'),
            tag = cms.string('')
        ))
)


process.siStripBackPlaneCorrectionDepESProducer = cms.ESProducer("SiStripBackPlaneCorrectionDepESProducer",
    BackPlaneCorrectionDeconvMode = cms.PSet(
        label = cms.untracked.string('deconvolution'),
        record = cms.string('SiStripBackPlaneCorrectionRcd')
    ),
    BackPlaneCorrectionPeakMode = cms.PSet(
        label = cms.untracked.string('peak'),
        record = cms.string('SiStripBackPlaneCorrectionRcd')
    ),
    LatencyRecord = cms.PSet(
        label = cms.untracked.string(''),
        record = cms.string('SiStripLatencyRcd')
    )
)


process.siStripGainESProducer = cms.ESProducer("SiStripGainESProducer",
    APVGain = cms.VPSet(cms.PSet(
        Label = cms.untracked.string(''),
        NormalizationFactor = cms.untracked.double(1.0),
        Record = cms.string('SiStripApvGainRcd')
    ), 
        cms.PSet(
            Label = cms.untracked.string(''),
            NormalizationFactor = cms.untracked.double(1.0),
            Record = cms.string('SiStripApvGain2Rcd')
        )),
    AutomaticNormalization = cms.bool(False),
    appendToDataLabel = cms.string(''),
    printDebug = cms.untracked.bool(False)
)


process.siStripLorentzAngleDepESProducer = cms.ESProducer("SiStripLorentzAngleDepESProducer",
    LatencyRecord = cms.PSet(
        label = cms.untracked.string(''),
        record = cms.string('SiStripLatencyRcd')
    ),
    LorentzAngleDeconvMode = cms.PSet(
        label = cms.untracked.string('deconvolution'),
        record = cms.string('SiStripLorentzAngleRcd')
    ),
    LorentzAnglePeakMode = cms.PSet(
        label = cms.untracked.string('peak'),
        record = cms.string('SiStripLorentzAngleRcd')
    )
)


process.siStripQualityESProducer = cms.ESProducer("SiStripQualityESProducer",
    ListOfRecordToMerge = cms.VPSet(cms.PSet(
        record = cms.string('SiStripDetVOffRcd'),
        tag = cms.string('')
    ), 
        cms.PSet(
            record = cms.string('SiStripDetCablingRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('RunInfoRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadChannelRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadFiberRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadModuleRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadStripRcd'),
            tag = cms.string('')
        )),
    PrintDebugOutput = cms.bool(False),
    ReduceGranularity = cms.bool(False),
    ThresholdForReducedGranularity = cms.double(0.3),
    UseEmptyRunInfo = cms.bool(False),
    appendToDataLabel = cms.string('')
)


process.sistripconn = cms.ESProducer("SiStripConnectivity")


process.stripCPEESProducer = cms.ESProducer("StripCPEESProducer",
    ComponentName = cms.string('stripCPE'),
    ComponentType = cms.string('SimpleStripCPE'),
    parameters = cms.PSet(

    )
)


process.trackerGeometryDB = cms.ESProducer("TrackerDigiGeometryESModule",
    alignmentsLabel = cms.string(''),
    appendToDataLabel = cms.string(''),
    applyAlignment = cms.bool(True),
    fromDDD = cms.bool(False)
)


process.trackerNumberingGeometryDB = cms.ESProducer("TrackerGeometricDetESModule",
    appendToDataLabel = cms.string(''),
    fromDDD = cms.bool(False)
)


process.trackerTopology = cms.ESProducer("TrackerTopologyEP",
    appendToDataLabel = cms.string('')
)


process.GlobalTag = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string(''),
        authenticationSystem = cms.untracked.int32(0),
        messageLevel = cms.untracked.int32(0),
        security = cms.untracked.string('')
    ),
    DumpStat = cms.untracked.bool(False),
    ReconnectEachRun = cms.untracked.bool(False),
    RefreshAlways = cms.untracked.bool(False),
    RefreshEachRun = cms.untracked.bool(False),
    RefreshOpenIOVs = cms.untracked.bool(False),
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
    globaltag = cms.string('80X_dataRun2_Prompt_v15'),
    pfnPostfix = cms.untracked.string(''),
    pfnPrefix = cms.untracked.string(''),
    snapshotTime = cms.string('9999-12-31 23:59:59.000'),
    toGet = cms.VPSet(cms.PSet(
        connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
        record = cms.string('L1TUtmTriggerMenuRcd'),
        tag = cms.string('L1Menu_HeavyIons2016_v2_m2_xml')
    ), 
        cms.PSet(
            connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
            record = cms.string('L1TGlobalPrescalesVetosRcd'),
            tag = cms.string('L1TGlobalPrescalesVetos_Stage2v0_hlt')
        ))
)


process.eegeom = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('EcalMappingRcd')
)


process.es_hardcode = cms.ESSource("HcalHardcodeCalibrations",
    GainWidthsForTrigPrims = cms.bool(False),
    HERecalibration = cms.bool(False),
    HEreCalibCutoff = cms.double(20.0),
    HFRecalibration = cms.bool(False),
    iLumi = cms.double(-1.0),
    testHFQIE10 = cms.bool(True),
    toGet = cms.untracked.vstring('GainWidths')
)


process.prefer("es_hardcode")


