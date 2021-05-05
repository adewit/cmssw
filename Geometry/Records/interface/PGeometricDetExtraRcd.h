#ifndef PGeometricDetExtraRcd_H
#define PGeometricDetExtraRcd_H

#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "FWCore/Utilities/interface/mplVector.h"

class PGeometricDetExtraRcd : public edm::eventsetup::DependentRecordImplementation<PGeometricDetExtraRcd,edm::mpl::Vector<IdealGeometryRecord>> {};
#endif

