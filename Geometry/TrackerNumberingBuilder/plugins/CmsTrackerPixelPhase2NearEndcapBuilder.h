#ifndef Geometry_TrackerNumberingBuilder_CmsTrackerPixelPhase2NearEndcapBuilder_H
#define Geometry_TrackerNumberingBuilder_CmsTrackerPixelPhase2NearEndcapBuilder_H

#include "Geometry/TrackerNumberingBuilder/plugins/CmsTrackerLevelBuilder.h"
#include "FWCore/ParameterSet/interface/types.h"
#include <string>

/**
 * Class which builds the pixel phase 2 endcap
 */
template <class FilteredView>
class CmsTrackerPixelPhase2NearEndcapBuilder : public CmsTrackerLevelBuilder<FilteredView> {
public:
  CmsTrackerPixelPhase2NearEndcapBuilder() {}

private:
  void sortNS(FilteredView&, GeometricDet*) override;
  void buildComponent(FilteredView&, GeometricDet*, const std::string&) override;
};

#endif
