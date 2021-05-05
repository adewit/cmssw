import FWCore.ParameterSet.Config as cms

process = cms.Process("GeometryTest")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryExtended2026D82Reco_cff")

process.source = cms.Source("EmptySource")

process.TrackerGeometricDetESModule = cms.ESProducer( "TrackerGeometricDetESModule",
                                                      fromDDD = cms.bool( True )
                                                     )

process.GeometricDetExtraESModule = cms.ESProducer( "GeometricDetExtraESModule" )
                                                     


process.es_prefer_geomdet = cms.ESPrefer("TrackerGeometricDetESModule","")
process.es_prefer_geomdetextra = cms.ESPrefer("GeometricDetExtraESModule","")

process.load("Alignment.CommonAlignmentProducer.FakeAlignmentSource_cfi")
process.preferFakeAlign = cms.ESPrefer("FakeAlignmentSource") 

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.prod = cms.EDAnalyzer("ModuleInfo",
    fromDDD = cms.bool(True),
    printDDD = cms.untracked.bool(False),
    tolerance = cms.untracked.double(1.0e-23)
)

process.p1 = cms.Path(process.prod)


