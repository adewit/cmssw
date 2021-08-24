#ifndef TkDetLayers_Phase2EndcapFlatRingBuilder_h
#define TkDetLayers_Phase2EndcapFlatRingBuilder_h

#include "Phase2EndcapFlatRing.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"

/** A concrete builder for Phase2EndcapFlatRing 
 */

#pragma GCC visibility push(hidden)
class Phase2EndcapFlatRingBuilder {
public:
  Phase2EndcapFlatRingBuilder(){};
  Phase2EndcapFlatRing* build(const GeometricDet* aPhase2EndcapFlatRing,
                          const TrackerGeometry* theGeomDetGeometry) __attribute__((cold));
};

#pragma GCC visibility pop
#endif
