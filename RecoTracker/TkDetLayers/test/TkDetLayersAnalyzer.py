import FWCore.ParameterSet.Config as cms


from Configuration.Eras.Era_Phase2C17I13M9_cff import Phase2C17I13M9
process = cms.Process('USER',Phase2C17I13M9)

process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(1) )

process.load("FWCore.MessageLogger.MessageLogger_cfi")


process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load('Configuration.Geometry.GeometryExtended2026D97Reco_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag

process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic_T25', '')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'DEBUG'
# 
process.MessageLogger.debugModules = ['analyzer']
#process.MessageLogger.cerr.DEBUG = cms.untracked.PSet(
#    threshold = cms.untracked.string('DEBUG'),
#    default          = cms.untracked.PSet( limit = cms.untracked.int32(0)  )
#    TkDetLayers = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
#    )

#cat debug | grep -v MSG | grep -v "Run:" | grep -v analyzer > debug.readable

process.source = cms.Source("EmptySource")

process.analyzer = cms.EDAnalyzer("TkDetLayersAnalyzer")
process.p1 = cms.Path(process.analyzer)


