#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESTransientHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/PTrackerPhase2ITParametersRcd.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "CondFormats/GeometryObjects/interface/PTrackerPhase2ITParameters.h"

#include <memory>

namespace edm {
  class ConfigurationDescriptions;
}
class PTrackerPhase2ITParameters;
class PTrackerPhase2ITParametersRcd;

class TrackerPhase2ITParametersESModule : public edm::ESProducer {
public:
  TrackerPhase2ITParametersESModule(const edm::ParameterSet&);

  using ReturnType = std::unique_ptr<PTrackerPhase2ITParameters>;

  static void fillDescriptions(edm::ConfigurationDescriptions&);

  ReturnType produce(const PTrackerPhase2ITParametersRcd&);

private:
  edm::ESGetToken<GeometricDet, IdealGeometryRecord> geometricDetToken_;
};

TrackerPhase2ITParametersESModule::TrackerPhase2ITParametersESModule(const edm::ParameterSet& ps) {
  auto cc = setWhatProduced(this);
  geometricDetToken_ = cc.consumesFrom<GeometricDet, IdealGeometryRecord>(edm::ESInputTag());
}

void TrackerPhase2ITParametersESModule::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  descriptions.add("TrackerPhase2ITParameters", desc);
}

TrackerPhase2ITParametersESModule::ReturnType TrackerPhase2ITParametersESModule::produce(
    const PTrackerPhase2ITParametersRcd& iRecord) {
  edm::LogInfo("TrackerPhase2ITParameters")
      << "TrackerPhase2ITParametersESModule::produce(const PTrackerPhase2ITParametersRcd& iRecord)";

  auto ptitp = std::make_unique<PTrackerPhase2ITParameters>();

  edm::ESTransientHandle<GeometricDet> gd = iRecord.getTransientHandle(geometricDetToken_);

  std::vector<const GeometricDet*> comp;
  gd->deepComponents(comp);

  for (auto& i : comp) {
    ptitp->setGeographicalId(i->geographicalId());
    ptitp->setBricked(i->isBricked());
  }

  return ptitp;
}

DEFINE_FWK_EVENTSETUP_MODULE(TrackerPhase2ITParametersESModule);
