#ifndef TkDetLayers_Phase2EndcapLayerFatDisk_h
#define TkDetLayers_Phase2EndcapLayerFatDisk_h

#include "TrackingTools/DetLayers/interface/RingedForwardLayer.h"
#include "Phase2EndcapSubDisk.h"
#include <array>
#include <atomic>

/** A concrete implementation for Phase 2 Endcap/Forward layer 
 *  built out of Phase2EndcapRings
 *  this classs is used for both OT and Pixel detector
 */

#pragma GCC visibility push(hidden)
class Phase2EndcapLayerFatDisk final : public RingedForwardLayer {
public:
  Phase2EndcapLayerFatDisk(std::vector<const Phase2EndcapSubDisk*>& subDisks) __attribute__((cold));
  ~Phase2EndcapLayerFatDisk() override __attribute__((cold));

  // Default implementations would not properly manage memory
  Phase2EndcapLayerFatDisk(const Phase2EndcapLayerFatDisk&) = delete;
  Phase2EndcapLayerFatDisk& operator=(const Phase2EndcapLayerFatDisk&) = delete;

  // GeometricSearchDet interface

  const std::vector<const GeomDet*>& basicComponents() const override { return theBasicComps; }

  const std::vector<const GeometricSearchDet*>& components() const override __attribute__((cold));

  void groupedCompatibleDetsV(const TrajectoryStateOnSurface& tsos,
                              const Propagator& prop,
                              const MeasurementEstimator& est,
                              std::vector<DetGroup>& result) const override __attribute__((hot));

  // DetLayer interface
  SubDetector subDetector() const override {
      return GeomDetEnumerators::subDetGeom[GeomDetEnumerators::P2PXEC];
  }

private:
  // private methods for the implementation of groupedCompatibleDets()
  BoundDisk* computeDisk(const std::vector<const Phase2EndcapSubDisk*>& subDisks) const __attribute__((cold));

  std::array<int, 2> subDiskIndicesByCrossingProximity(const TrajectoryStateOnSurface& startingState,
                                                    const Propagator& prop) const;

  //  bool isCompatible( const TrajectoryStateOnSurface& ms,
  //	     const MeasurementEstimator& est) const;

  std::array<int, 2> findTwoClosest(std::vector<GlobalPoint>) const __attribute__((hot));


  void fillSubDiskPars(int i) __attribute__((cold));

private:
  std::vector<GeomDet const*> theBasicComps;
  mutable std::atomic<std::vector<const GeometricSearchDet*>*> theComponents;
  std::vector<const Phase2EndcapSubDisk*> theComps;
  struct SubDiskPar {
    float theSubDiskZ;
  };
  std::vector<SubDiskPar> subDiskPars;
  int theSubDisksSize;
};

#pragma GCC visibility pop
#endif
