#include "Phase2EndcapSingleRing.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TrackingTools/DetLayers/interface/DetLayerException.h"
#include "TrackingTools/DetLayers/interface/MeasurementEstimator.h"
#include "TrackingTools/GeomPropagators/interface/HelixForwardPlaneCrossing.h"
#include "TrackingTools/DetLayers/interface/rangesIntersect.h"
#include "TrackingTools/DetLayers/interface/ForwardRingDiskBuilderFromDet.h"

#include "LayerCrossingSide.h"
#include "DetGroupMerger.h"
#include "CompatibleDetToGroupAdder.h"

#include "TkDetUtil.h"
#include "DataFormats/GeometryVector/interface/VectorUtil.h"

using namespace std;

typedef GeometricSearchDet::DetWithState DetWithState;

class DetGroupElementZLess {
public:
  bool operator()(DetGroup a, DetGroup b) {
    return (fabs(a.front().det()->position().z()) < fabs(b.front().det()->position().z()));
  }
};

Phase2EndcapSingleRing::Phase2EndcapSingleRing(vector<const GeomDet*>& allDets)
    : GeometricSearchDet(true),
      theDets(allDets.begin(), allDets.end()){
 // theDets.assign(theFrontDets.begin(), theFrontDets.end());
  //theDets.insert(theDets.end(), theBackDets.begin(), theBackDets.end());
 // theDets.insert(theDets.end(), theFrontDetBrothers.begin(), theFrontDetBrothers.end());
  //theDets.insert(theDets.end(), theBackDetBrothers.begin(), theBackDetBrothers.end());

  // the dets should be already phi-ordered. TO BE CHECKED
  //sort( theFrontDets.begin(), theFrontDets.end(), DetLessPhi() );
  //sort( theBackDets.begin(), theBackDets.end(), DetLessPhi() );
  std::cout<<"theDets. size "<<theDets.size()<<std::endl;
  std::cout<<"theDets.front()->surface().position().phi() "<<theDets.front()->surface().position().phi()<<std::endl;

  theDisk = ForwardRingDiskBuilderFromDet()(theDets);

  //theFrontDisk = ForwardRingDiskBuilderFromDet()(theFrontDets);
  //theBackDisk = ForwardRingDiskBuilderFromDet()(theBackDets);

  theBinFinder = BinFinderType(theDets.front()->surface().position().phi(),theDets.size());

#ifdef EDM_ML_DEBUG
  LogDebug("TkDetLayers") << "DEBUG INFO for Phase2EndcapSingleRing";
  for (vector<const GeomDet*>::const_iterator it = theDets.begin(); it != theDets.end(); it++) {
    LogDebug("TkDetLayers") << "Det detId,phi,z,r: " << (*it)->geographicalId().rawId() << " , "
                            << (*it)->surface().position().phi() << " , " << (*it)->surface().position().z() << " , "
                            << (*it)->surface().position().perp();
  }

#endif
}

Phase2EndcapSingleRing::~Phase2EndcapSingleRing() {}

const vector<const GeometricSearchDet*>& Phase2EndcapSingleRing::components() const {
  throw DetLayerException("Phase2EndcapSingleRing doesn't have GeometricSearchDet components");
}

pair<bool, TrajectoryStateOnSurface> Phase2EndcapSingleRing::compatible(const TrajectoryStateOnSurface&,
                                                                  const Propagator&,
                                                                  const MeasurementEstimator&) const {
  edm::LogError("TkDetLayers") << "temporary dummy implementation of Phase2EndcapSingleRing::compatible()!!";
  return pair<bool, TrajectoryStateOnSurface>();
}

void Phase2EndcapSingleRing::groupedCompatibleDetsV(const TrajectoryStateOnSurface& tsos,
                                              const Propagator& prop,
                                              const MeasurementEstimator& est,
                                              std::vector<DetGroup>& result) const {
  std::cout<<"detsV -A "<<std::endl;
  SubLayerCrossings crossings;

  std::cout<<"detsV -B "<<std::endl;
  crossings = computeCrossings(tsos, prop.propagationDirection());

  std::cout<<"detsV -C "<<std::endl;
  if (!crossings.isValid())
    return;

  std::vector<DetGroup> closestResult;
  std::vector<DetGroup> closestBrotherResult;

  std::cout<<"detsV - going to add closest "<<std::endl;
  addClosest(tsos, prop, est, crossings.closest(), closestResult, closestBrotherResult);
  if (closestResult.empty())
    return;

  std::cout<<"detsV - done adding closest "<<std::endl;
  DetGroupElement closestGel(closestResult.front().front());

  std::cout<<"detsV - got closestGel "<<std::endl;
  int crossingSide = LayerCrossingSide().endcapSide(closestGel.trajectoryState(), prop);

  std::cout<<"detsV - got crossingSide "<<crossingSide<<std::endl;
  float phiWindow = tkDetUtil::computeWindowSize(closestGel.det(), closestGel.trajectoryState(), est);

  std::cout<<"detsV - got phiWindow "<<phiWindow<<std::endl;
  searchNeighbors(tsos, prop, est, crossings.closest(), phiWindow, closestResult, closestBrotherResult, false);

  std::cout<<"detsV - done searchNeighbors "<<std::endl;
  vector<DetGroup> closestCompleteResult;

  std::cout<<"detsV - going to orderAndMergeTWoLevels "<<std::endl;
  DetGroupMerger::orderAndMergeTwoLevels(
      std::move(closestResult), std::move(closestBrotherResult), result, 0, crossingSide);

  //std::cout<<"detsV - done orderAndMergeTWoLevels "<<std::endl;
  //vector<DetGroup> nextResult;
  //vector<DetGroup> nextBrotherResult;
  //searchNeighbors(tsos, prop, est, crossings.other(), phiWindow, nextResult, nextBrotherResult, true);

  //std::cout<<"detsV - going to orderAndMergeTWoLevels again "<<std::endl;
  //vector<DetGroup> nextCompleteResult;
  //DetGroupMerger::orderAndMergeTwoLevels(
  //    std::move(nextResult), std::move(nextBrotherResult), nextCompleteResult, 0, crossingSide);

  //std::cout<<"detsV - done orderAndMergeTWoLevels again "<<std::endl;
  //DetGroupMerger::orderAndMergeTwoLevels(
  //    std::move(closestCompleteResult), std::move(nextCompleteResult), result, crossings.closestIndex(), crossingSide);

  //std::cout<<"detsV - done orderAndMergeTWoLevels again and again "<<std::endl;
  //due to propagator problems, when we add single pt sub modules, we should order them in z (endcap)

  std::cout<<"TkDetLayers " << "Number of groups : " << result.size() << std::endl;
  for (auto& grp : result) {
    if (grp.empty())
      continue;
    std::cout<<"TkDetLayers " << "New group in Phase2EndcapSingleRing made by : " << std::endl;
    for (auto const& det : grp) {
      std::cout<<"TkDetLayers " << " geom det at r: " << det.det()->position().perp()
                              << " id:" << det.det()->geographicalId().rawId()
                              << " tsos at:" << det.trajectoryState().globalPosition() << std::endl;
    }
  }
}

SubLayerCrossings Phase2EndcapSingleRing::computeCrossings(const TrajectoryStateOnSurface& startingState,
                                                     PropagationDirection propDir) const {
  auto rho = startingState.transverseCurvature();

  HelixPlaneCrossing::PositionType startPos(startingState.globalPosition());
  HelixPlaneCrossing::DirectionType startDir(startingState.globalMomentum());
  HelixForwardPlaneCrossing crossing(startPos, startDir, rho, propDir);

  pair<bool, double> frontPath = crossing.pathLength(*theDisk);
  if (!frontPath.first)
    return SubLayerCrossings();

  //pair<bool, double> backPath = crossing.pathLength(*theBackDisk);
  //if (!backPath.first)
  //  return SubLayerCrossings();

  GlobalPoint gFrontPoint(crossing.position(frontPath.second));
  //GlobalPoint gBackPoint(crossing.position(backPath.second));

  int frontIndex = theBinFinder.binIndex(gFrontPoint.barePhi());
  SubLayerCrossing frontSLC(0, frontIndex, gFrontPoint);
  std::cout<<"closest DetIndex "<<frontSLC.closestDetIndex()<<std::endl;

  //int backIndex = theBackBinFinder.binIndex(gBackPoint.barePhi());
  //SubLayerCrossing backSLC(1, backIndex, gBackPoint);

  // 0ss: frontDisk has index=0, backDisk has index=1
  float frontDist = std::abs(Geom::deltaPhi(gFrontPoint.barePhi(), theDets[frontIndex]->surface().phi()));
  //float backDist = std::abs(Geom::deltaPhi(gBackPoint.barePhi(), theBackDets[backIndex]->surface().phi()));

  return SubLayerCrossings(frontSLC,frontSLC,0);
}

bool Phase2EndcapSingleRing::addClosest(const TrajectoryStateOnSurface& tsos,
                                  const Propagator& prop,
                                  const MeasurementEstimator& est,
                                  const SubLayerCrossing& crossing,
                                  vector<DetGroup>& result,
                                  vector<DetGroup>& brotherresult) const {
  std::cout<<"In addClosest"<<std::endl;
  std::cout<<"crossing "<<crossing.subLayerIndex()<<std::endl;
  const vector<const GeomDet*>& sub(subLayer(crossing.subLayerIndex()));

  std::cout<<"Got sub"<<std::endl;
  std::cout<<"sub size "<<sub.size()<<std::endl;
  std::cout<<"crossing.closestDetIndex() "<<crossing.closestDetIndex()<<std::endl;
  const GeomDet* det(sub[crossing.closestDetIndex()]);

  std::cout<<"Got GeomDet"<<std::endl;
  bool firstgroup = CompatibleDetToGroupAdder::add(*det, tsos, prop, est, result);

  std::cout<<"firstgroup "<<firstgroup<<std::endl;
  std::cout<<"theFrontDetBrothers.empty "<<theFrontDetBrothers.empty()<<std::endl;
  std::cout<<"theBackDetBrothers.empty "<<theBackDetBrothers.empty()<<std::endl;
  if (theFrontDetBrothers.empty() && theBackDetBrothers.empty())
    return firstgroup;
  // it assumes that the closestDetIndex is ok also for the brother detectors: the crossing is NOT recomputed
  const vector<const GeomDet*>& subBrothers(subLayerBrothers(crossing.subLayerIndex()));
  const GeomDet* detBrother(subBrothers[crossing.closestDetIndex()]);
  bool brothergroup = CompatibleDetToGroupAdder::add(*detBrother, tsos, prop, est, brotherresult);
  return firstgroup || brothergroup;
}

void Phase2EndcapSingleRing::searchNeighbors(const TrajectoryStateOnSurface& tsos,
                                       const Propagator& prop,
                                       const MeasurementEstimator& est,
                                       const SubLayerCrossing& crossing,
                                       float window,
                                       vector<DetGroup>& result,
                                       vector<DetGroup>& brotherresult,
                                       bool checkClosest) const {
  const GlobalPoint& gCrossingPos = crossing.position();

  const vector<const GeomDet*>& sLayer(subLayer(crossing.subLayerIndex()));
  // It assumes that what is ok for the front modules in the pt modules is ok also for the back module
  const vector<const GeomDet*>& sBrotherLayer(subLayerBrothers(crossing.subLayerIndex()));

  int closestIndex = crossing.closestDetIndex();
  int negStartIndex = closestIndex - 1;
  int posStartIndex = closestIndex + 1;

  if (checkClosest) {  // must decide if the closest is on the neg or pos side
    if (Geom::phiLess(gCrossingPos.barePhi(), sLayer[closestIndex]->surface().phi())) {
      posStartIndex = closestIndex;
    } else {
      negStartIndex = closestIndex;
    }
  }

  const BinFinderType& binFinder = theBinFinder;

  typedef CompatibleDetToGroupAdder Adder;
  int half = sLayer.size() / 2;  // to check if dets are called twice....
  for (int idet = negStartIndex; idet >= negStartIndex - half; idet--) {
    const GeomDet& neighborDet = *sLayer[binFinder.binIndex(idet)];
    if (!tkDetUtil::overlapInPhi(gCrossingPos, neighborDet, window))
      break;
    if (!Adder::add(neighborDet, tsos, prop, est, result))
      break;
    if (theFrontDetBrothers.empty() && theBackDetBrothers.empty())
      break;
    // If the two above checks are passed also the brother module will be added with no further checks
    const GeomDet& neighborBrotherDet = *sBrotherLayer[binFinder.binIndex(idet)];
    Adder::add(neighborBrotherDet, tsos, prop, est, brotherresult);
    // maybe also add shallow crossing angle test here???
  }
  for (int idet = posStartIndex; idet < posStartIndex + half; idet++) {
    const GeomDet& neighborDet = *sLayer[binFinder.binIndex(idet)];
    if (!tkDetUtil::overlapInPhi(gCrossingPos, neighborDet, window))
      break;
    if (!Adder::add(neighborDet, tsos, prop, est, result))
      break;
    if (theFrontDetBrothers.empty() && theBackDetBrothers.empty())
      break;
    // If the two above checks are passed also the brother module will be added with no further checks
    const GeomDet& neighborBrotherDet = *sBrotherLayer[binFinder.binIndex(idet)];
    Adder::add(neighborBrotherDet, tsos, prop, est, brotherresult);
    // maybe also add shallow crossing angle test here???
  }
}
