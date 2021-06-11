#ifndef PTrackerPhase2ITParametersRcd_H
#define PTrackerPhase2ITParametersRcd_H

#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "FWCore/Utilities/interface/mplVector.h"

class PTrackerPhase2ITParametersRcd : public edm::eventsetup::DependentRecordImplementation<PTrackerPhase2ITParametersRcd,edm::mpl::Vector<IdealGeometryRecord>> {};
#endif

