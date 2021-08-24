#include "Phase2EndcapLayerFatDiskBuilder.h"
#include "Phase2EndcapSubDiskBuilder.h"

using namespace edm;
using namespace std;

Phase2EndcapLayerFatDisk* Phase2EndcapLayerFatDiskBuilder::build(const GeometricDet* aPhase2EndcapLayerFatDisk,
                                                   const TrackerGeometry* theGeomDetGeometry) {
  LogTrace("TkDetLayers") << "Phase2EndcapLayerFatDiskBuilder::build";
  vector<const GeometricDet*> theSubDisks = aPhase2EndcapLayerFatDisk->components();
  LogTrace("TkDetLayers") << "theSubDisks.size(): " << theSubDisks.size();

  Phase2EndcapSubDiskBuilder myBuilder;
  vector<const Phase2EndcapSubDisk*> thePhase2EndcapSubDisks;

  for (vector<const GeometricDet*>::const_iterator it = theSubDisks.begin(); it != theSubDisks.end();
       it++) {
    thePhase2EndcapSubDisks.push_back(myBuilder.build(*it, theGeomDetGeometry));
  }

  return new Phase2EndcapLayerFatDisk(thePhase2EndcapSubDisks);
}
