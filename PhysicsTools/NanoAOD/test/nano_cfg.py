import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process('NANO',eras.Run2_2017,eras.run2_nanoAOD_92X)

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('Configuration.StandardSequences.Services_cff')
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = autoCond['phase1_2017_realistic']

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring())
process.source.fileNames = [
#relvals:
# '/store/relval/CMSSW_9_3_0_pre4/RelValTTbar_13/MINIAODSIM/93X_mc2017_realistic_v1-v1/00000/1CFF7C9C-6A86-E711-A1F2-0CC47A7C35F4.root',
# '/store/relval/CMSSW_9_3_0_pre4/RelValTTbar_13/MINIAODSIM/93X_mc2017_realistic_v1-v1/00000/107D499F-6A86-E711-8A51-0025905B8592.root',
# '/store/relval/CMSSW_9_4_0_pre3/RelValProdTTbar_13/MINIAODSIM/94X_mcRun2_asymptotic_v0-v1/10000/06F85EC5-7BB9-E711-A2CB-0025905A6134.root'

#sample with LHE
#'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16MiniAODv2/WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/02FE572F-88DA-E611-8CAB-001E67792884.root'
'root://cms-xrd-global.cern.ch://store/mc/RunIIFall17MiniAOD/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/94X_mc2017_realistic_v10-v1/40000/A2100E10-51FE-E711-9D86-001E6739B019.root'
#'root://cms-xrd-global.cern.ch:///store/mc/RunIIFall17MiniAODv2/WZ_TuneCP5_13TeV-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/40000/3EFC7F98-2B42-E811-B1C3-00269E95B1B8.root'
#'root://cms-xrd-global.cern.ch//store/mc/RunIIFall17MiniAODv2/Z1JetsToNuNu_M-50_LHEZpT_250-400_TuneCP5_13TeV-amcnloFXFX-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/10000/00430106-6B42-E811-BA09-001F29089F7E.root'
#'root://cms-xrd-global.cern.ch//store/mc/RunIIFall17MiniAOD/W2JetsToLNu_LHEWpT_400-inf_TuneCP5_13TeV-amcnloFXFX-pythia8/MINIAODSIM/94X_mc2017_realistic_v11-v1/100000/38E38F15-BB1C-E811-8338-0CC47A57CDD2.root'
]

process.load("PhysicsTools.NanoAOD.nano_cff")


process.nanoPath = cms.Path(process.nanoSequenceMC)
#for data:
#process.nanoPath = cms.Path(process.nanoSequence)
#process.GlobalTag.globaltag = autoCond['run2_data']

process.out = cms.OutputModule("NanoAODOutputModule",
    fileName = cms.untracked.string('nano.root'),
    outputCommands = process.NanoAODEDMEventContent.outputCommands,
   #compressionLevel = cms.untracked.int32(9),
    #compressionAlgorithm = cms.untracked.string("LZMA"),

)
process.out1 = cms.OutputModule("NanoAODOutputModule",
    fileName = cms.untracked.string('lzma.root'),
    outputCommands = process.NanoAODEDMEventContent.outputCommands,
    compressionLevel = cms.untracked.int32(9),
    compressionAlgorithm = cms.untracked.string("LZMA"),

)
process.end = cms.EndPath(process.out+process.out1)  
