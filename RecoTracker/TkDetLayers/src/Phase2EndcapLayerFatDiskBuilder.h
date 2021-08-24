#ifndef TkDetLayerFatDisks_Phase2EndcapLayerFatDiskBuilder_h
#define TkDetLayerFatDisks_Phase2EndcapLayerFatDiskBuilder_h

#include "Phase2EndcapLayerFatDisk.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"

/** A concrete builder for Phase2EndcapLayerFatDisk
 */

#pragma GCC visibility push(hidden)
class Phase2EndcapLayerFatDiskBuilder {
public:
  Phase2EndcapLayerFatDiskBuilder(){};
  Phase2EndcapLayerFatDisk* build(const GeometricDet* aPhase2EndcapLayerFatDisk,
                           const TrackerGeometry* theGeomDetGeometry) __attribute__((cold));
};

#pragma GCC visibility pop
#endif
