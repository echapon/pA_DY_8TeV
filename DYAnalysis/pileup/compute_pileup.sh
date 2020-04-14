# https://twiki.cern.ch/twiki/bin/view/CMS/HiEWQ2017#Computing_PileUp

brilcalc lumi --xing --normtag /afs/cern.ch/user/l/lumipro/public/Normtags/normtag_PHYSICS.json -i /afs/cern.ch/user/a/anstahll/work/public/LUMI/processedLumis_PASingleMuon_285952-286496_HI8TeV_PromptReco_pPb.json  -o pileup_pPb.csv
brilcalc lumi --xing --normtag /afs/cern.ch/user/l/lumipro/public/Normtags/normtag_PHYSICS.json -i /afs/cern.ch/user/a/anstahll/work/public/LUMI/processedLumis_PASingleMuon_285479-285832_HI8TeV_PromptReco_Pbp.json  -o pileup_Pbp.csv

cp /afs/cern.ch/user/a/anstahll/work/public/PileUp/estimatePileup.py .
# cmsrel in some CMSSW here. eg 8_0_28
./estimatePileup.py --csvInput pileup_pPb.csv pileup_pPb_JSON.txt
./estimatePileup.py --csvInput pileup_Pbp.csv pileup_Pbp_JSON.txt

xsection=2120000 # from Table V of https://arxiv.org/pdf/1710.07098.pdf
pileupCalc.py -i /afs/cern.ch/user/a/anstahll/work/public/LUMI/processedLumis_PASingleMuon_285952-286496_HI8TeV_PromptReco_pPb.json --inputLumiJSON ./pileup_pPb_JSON.txt --calcMode true --minBiasXsec $xsection --maxPileupBin 2 --numPileupBins 100 ./MyDataPileupHistogram_pPb.root
pileupCalc.py -i /afs/cern.ch/user/a/anstahll/work/public/LUMI/processedLumis_PASingleMuon_285479-285832_HI8TeV_PromptReco_Pbp.json --inputLumiJSON ./pileup_Pbp_JSON.txt --calcMode true --minBiasXsec $xsection --maxPileupBin 2 --numPileupBins 100 ./MyDataPileupHistogram_Pbp.root
