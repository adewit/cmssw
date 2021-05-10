#ifndef PTrackerGeometricDetExtraRcd_H
#define PTrackerGeometricDetExtraRcd_H

#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "FWCore/Utilities/interface/mplVector.h"

class PTrackerGeometricDetExtraRcd : public edm::eventsetup::DependentRecordImplementation<PTrackerGeometricDetExtraRcd,edm::mpl::Vector<IdealGeometryRecord>> {};
#endif

