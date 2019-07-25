
import FWCore.ParameterSet.Config as cms

process = cms.Process("PrimaryVertexValidation")

import FWCore.PythonUtilities.LumiList as LumiList

lumiSecs = cms.untracked.VLuminosityBlockRange()
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source("PoolSource",
                    lumisToProcess = lumiSecs,
                            secondaryFileNames =secFiles,
                            fileNames = readFiles
)
readFiles.extend( [
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/06C7B58F-38AD-E711-A2DC-001E67D89532.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/0C8153F2-4FAD-E711-9DE3-FA163EBDE49C.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/1065DCAC-29AD-E711-9CF6-008CFAC93C94.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/183F678E-7FAE-E711-956B-008CFA1661B4.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/18519417-68AD-E711-997A-008CFAC93B38.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/1CE9EE94-58AD-E711-BD6D-0CC47A4D769A.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/1CFB7679-39AD-E711-B32E-001517FB228C.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/24FB5D80-39AD-E711-B6E3-001E67A3EC00.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/2A276978-36AD-E711-9FA8-A4BF010114F4.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/2C6E4B91-38AD-E711-B45B-0CC47AD98C8A.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/2CE61047-3FAE-E711-9B76-001E675A6D10.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/3A0896E8-48AD-E711-9059-0CC47AA98B8E.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/3AEF6C95-38AD-E711-8AC6-0CC47AD98CF4.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/3CF70608-31AD-E711-96A9-008CFAE453B8.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/3E4CD1AC-29AD-E711-BE2D-008CFAC93DC4.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/4065D56E-43AD-E711-8AD9-0CC47A13CDA0.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/44F7837F-39AD-E711-974E-001E675A68BF.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/5432012C-42AD-E711-9758-0CC47AD99044.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/54534E4B-DCAD-E711-A2EC-008CFAC93FF4.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/5CAD7F2F-42AD-E711-9409-001E675A659A.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/6008D9FC-48AD-E711-84E0-008CFAC940BC.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/6008F8E7-48AD-E711-8E3E-001E67DDC254.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/624DC682-39AD-E711-B1AF-001E67A401B3.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/72C47C23-42AD-E711-87BA-B083FED18595.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/741BE60D-1CAE-E711-BC4E-0CC47AD98F74.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/7E37082D-42AD-E711-A524-0CC47A0AD3BC.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/7E6427C8-2CAE-E711-90CA-44A84224BE51.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/82293D88-39AD-E711-9ED8-001E67A42A71.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/84B65EB4-43AD-E711-AD85-0025907254C8.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/8CAF6C33-38AD-E711-B1FF-0CC47A4D7638.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/8CEFE505-FCAD-E711-A13C-0025905A6134.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/8E59472A-42AD-E711-A8F7-0CC47AD98BEE.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/9AE0DC92-38AD-E711-B121-0CC47AD98F64.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/A21130B6-38AD-E711-B986-002590D9D894.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/B0E422D3-29AD-E711-B3B6-008CFAC9403C.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/B259610A-38AD-E711-9DA3-5065F381D2C1.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/B8CC118F-39AD-E711-915F-A4BF010261D4.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/CC451978-39AD-E711-9B63-001E6758651B.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/D4F03D48-96AD-E711-A9F9-008CFAE451A8.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/D60C4A7C-23AE-E711-A4B6-FA163E65CA6E.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/D8D67588-39AD-E711-91E0-001E67DDD0B9.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/E22AD822-15AE-E711-B756-0CC47A4D7670.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/E298C4FE-48AD-E711-B66C-008CFAC93C64.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/E47F2088-39AD-E711-8DE4-001E67A3F49D.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/E4D5382F-42AD-E711-9B72-0CC47AA98B8E.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/E6A63B31-42AD-E711-8A50-A4BF01026229.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/EE0C226A-41AD-E711-989C-24BE05C616E1.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/F2BEE71F-42AD-E711-BBE1-FA163E464B01.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/F46939E9-14AE-E711-8FE4-FA163E1092C2.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/F6D8BAE9-4FAD-E711-BD1C-008CFAC93C64.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/F6EE2CB5-38AD-E711-8B95-0CC47AD98D6C.root',
'/store/data/Run2017A/ZeroBias/ALCARECO/TkAlMinBias-09Oct2017-v1/00000/F87C0757-03AE-E711-A359-002590AB3A82.root'
] )
lumiSecs.extend( [
'296173:1-296173:max'
] )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20000 / 1) )
process.source.skipEvents=cms.untracked.uint32(0*1000/1)



process.options = cms.untracked.PSet(
   wantSummary = cms.untracked.bool(False),
   Rethrow = cms.untracked.vstring("ProductNotFound"), # make this exception fatal
   fileMode  =  cms.untracked.string('NOMERGE') # no ordering needed, but calls endRun/beginRun etc. at file boundaries
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.statistics.append('cout')


process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")
process.load("Configuration.Geometry.GeometryDB_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")


import Alignment.CommonAlignment.tools.trackselectionRefitting as trackselRefit
process.seqTrackselRefit = trackselRefit.getSequence(process, 'ALCARECOTkAlMinBias',
                                                     TTRHBuilder='WithAngleAndTemplate',
                                                     usePixelQualityFlag=True,
                                                     openMassWindow=False,
                                                     cosmicsDecoMode=True,
                                                     cosmicsZeroTesla=False,
                                                     momentumConstraint=None,
                                                     cosmicTrackSplitting=False,
                                                     use_d0cut=False,
                                                    )




#Global tag
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag,"94X_dataRun2_TestReReco17_forTkDPG_v1")


import CalibTracker.Configuration.Common.PoolDBESSource_cfi

process.conditionsInTrackerAlignmentErrorExtendedRcd = CalibTracker.Configuration.Common.PoolDBESSource_cfi.poolDBESSource.clone(
     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
     toGet = cms.VPSet(cms.PSet(record = cms.string('TrackerAlignmentErrorExtendedRcd'),
                               tag = cms.string('TrackerAlignmentErrorsExtended_Upgrade2017_design_v0')
                               )
                      )
    )
process.prefer_conditionsInTrackerAlignmentErrorExtendedRcd = cms.ESPrefer("PoolDBESSource", "conditionsInTrackerAlignmentErrorExtendedRcd")



process.HighPurityTrackSelector.trackQualities = cms.vstring()
process.HighPurityTrackSelector.pMin     = cms.double(0.)
process.AlignmentTrackSelector.pMin      = cms.double(0.)
process.AlignmentTrackSelector.ptMin     = cms.double(0.)
process.AlignmentTrackSelector.nHitMin2D = cms.uint32(0)
process.AlignmentTrackSelector.nHitMin   = cms.double(0.)
process.AlignmentTrackSelector.d0Min     = cms.double(-999999.0)
process.AlignmentTrackSelector.d0Max     = cms.double(+999999.0)
process.AlignmentTrackSelector.dzMin     = cms.double(-999999.0)
process.AlignmentTrackSelector.dzMax     = cms.double(+999999.0)

isDA = True
isMC = True

###################################################################
#  Runs and events
###################################################################
runboundary = 1
isMultipleRuns=False
if(isinstance(runboundary, (list, tuple))):
     isMultipleRuns=True
     print "Multiple Runs are selected"
       
if(isMultipleRuns):
     process.source.firstRun = cms.untracked.uint32(int(runboundary[0]))
else:
     process.source.firstRun = cms.untracked.uint32(int(runboundary)) 

###################################################################
# JSON Filtering
###################################################################
if isMC:
     print ">>>>>>>>>> testPVValidation_cfg.py: msg%-i: This is simulation!"
     runboundary = 1
else:
     print ">>>>>>>>>> testPVValidation_cfg.py: msg%-i: This is real DATA!"
     if ('None'):
          print ">>>>>>>>>> testPVValidation_cfg.py: msg%-i: JSON filtering with: None "
          import FWCore.PythonUtilities.LumiList as LumiList
          process.source.lumisToProcess = LumiList.LumiList(filename ='None').getVLuminosityBlockRange()

####################################################################
# Produce the Transient Track Record in the event
####################################################################
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

####################################################################
# Load and Configure event selection
####################################################################
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
                                           src = cms.InputTag("offlinePrimaryVertices"),
                                           cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
                                           filter = cms.bool(True)
                                           )

process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  src =  cms.untracked.InputTag("ALCARECOTkAlMinBias"),
                                  debugOn = cms.untracked.bool(False),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.25)
                                  )


process.load("Alignment.CommonAlignment.filterOutLowPt_cfi")
process.filterOutLowPt.src = "ALCARECOTkAlMinBias"
process.filterOutLowPt.ptmin = 3.
process.filterOutLowPt.runControl = False
if(isMultipleRuns):
     process.filterOutLowPt.runControlNumber.extend((runboundary))
else:
     process.filterOutLowPt.runControlNumber = [runboundary]
                                
if isMC:
     process.goodvertexSkim = cms.Sequence(process.noscraping + process.filterOutLowPt)
else:
     process.goodvertexSkim = cms.Sequence(process.primaryVertexFilter + process.noscraping + process.filterOutLowPt)

####################################################################
# Deterministic annealing clustering
####################################################################
if isDA:
     print ">>>>>>>>>> testPVValidation_cfg.py: msg%-i: Running DA Algorithm!"
     process.PVValidation = cms.EDAnalyzer("PrimaryVertexValidation",
                                           TrackCollectionTag = cms.InputTag("FinalTrackRefitter"),
                                           VertexCollectionTag = cms.InputTag("offlinePrimaryVertices"),
                                           Debug = cms.bool(False),
                                           storeNtuple = cms.bool(False),
                                           useTracksFromRecoVtx = cms.bool(False),
                                           isLightNtuple = cms.bool(True),
                                           askFirstLayerHit = cms.bool(False),
                                           probePt  = cms.untracked.double(3.),
                                           probeEta = cms.untracked.double(2.7),
                                           doBPix   = cms.untracked.bool(True),
                                           doFPix   = cms.untracked.bool(True),
                                           numberOfBins = cms.untracked.int32(48),
                                           runControl = cms.untracked.bool(False),
                                           runControlNumber = cms.untracked.vuint32(runboundary),
                                           
                                           TkFilterParameters = cms.PSet(algorithm=cms.string('filter'),                           
                                                                         maxNormalizedChi2 = cms.double(5.0),                        # chi2ndof < 5                  
                                                                         minPixelLayersWithHits = cms.int32(2),                      # PX hits > 2                       
                                                                         minSiliconLayersWithHits = cms.int32(5),                    # TK hits > 5  
                                                                         maxD0Significance = cms.double(5.0),                        # fake cut (requiring 1 PXB hit)     
                                                                         minPt = cms.double(0.0),                                    # better for softish events                        
                                                                         maxEta = cms.double(5.0),                                   # as per recommendation in PR #18330
                                                                         trackQuality = cms.string("any")
                                                                         ),

                                           ## MM 04.05.2017 (use settings as in: https://github.com/cms-sw/cmssw/pull/18330)
                                           TkClusParameters=cms.PSet(algorithm=cms.string('DA_vect'),
                                                                     TkDAClusParameters = cms.PSet(coolingFactor = cms.double(0.6),  # moderate annealing speed
                                                                                                   Tmin = cms.double(2.0),           # end of vertex splitting
                                                                                                   Tpurge = cms.double(2.0),         # cleaning
                                                                                                   Tstop = cms.double(0.5),          # end of annealing
                                                                                                   vertexSize = cms.double(0.006),   # added in quadrature to track-z resolutions
                                                                                                   d0CutOff = cms.double(3.),        # downweight high IP tracks
                                                                                                   dzCutOff = cms.double(3.),        # outlier rejection after freeze-out (T<Tmin)
                                                                                                   zmerge = cms.double(1e-2),        # merge intermediat clusters separated by less than zmerge
                                                                                                   uniquetrkweight = cms.double(0.8) # require at least two tracks with this weight at T=Tpurge
                                                                                                   )
                                                                     )
                                           )

####################################################################
# GAP clustering
####################################################################
else:
     print ">>>>>>>>>> testPVValidation_cfg.py: msg%-i: Running GAP Algorithm!"
     process.PVValidation = cms.EDAnalyzer("PrimaryVertexValidation",
                                           TrackCollectionTag = cms.InputTag("FinalTrackRefitter"),
                                           VertexCollectionTag = cms.InputTag("offlinePrimaryVertices"),
                                           Debug = cms.bool(False),
                                           isLightNtuple = cms.bool(True),
                                           storeNtuple = cms.bool(False),
                                           useTracksFromRecoVtx = cms.bool(False),
                                           askFirstLayerHit = cms.bool(False),
                                           probePt = cms.untracked.double(3.),
                                           probeEta = cms.untracked.double(2.5),
                                           doBPix   = cms.untracked.bool(True),
                                           doFPix   = cms.untracked.bool(True),
                                           numberOfBins = cms.untracked.int32(48),
                                           runControl = cms.untracked.bool(False),
                                           runControlNumber = cms.untracked.vuint32(int(1)),

                                           TkFilterParameters = cms.PSet(algorithm=cms.string('filter'),
                                                                         maxNormalizedChi2 = cms.double(5.0),                        # chi2ndof < 20
                                                                         minPixelLayersWithHits=cms.int32(2),                        # PX hits > 2
                                                                         minSiliconLayersWithHits = cms.int32(5),                    # TK hits > 5
                                                                         maxD0Significance = cms.double(5.0),                        # fake cut (requiring 1 PXB hit)
                                                                         minPt = cms.double(0.0),                                    # better for softish events
                                                                         maxEta = cms.double(5.0),                                   # as per recommendation in PR #18330
                                                                         trackQuality = cms.string("any")
                                                                         ),

                                           TkClusParameters = cms.PSet(algorithm   = cms.string('gap'),
                                                                       TkGapClusParameters = cms.PSet(zSeparation = cms.double(0.2)  # 0.2 cm max separation betw. clusters
                                                                                                      )
                                                                       )
                                           )



process.TFileService = cms.Service("TFileService",
    fileName = cms.string('test.root')
)



process.p = cms.Path(process.goodvertexSkim*
                     process.seqTrackselRefit*
                     process.PVValidation)

