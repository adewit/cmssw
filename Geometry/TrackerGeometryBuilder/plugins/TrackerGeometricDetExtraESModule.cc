#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESTransientHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/PTrackerGeometricDetExtraRcd.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "CondFormats/GeometryObjects/interface/PTrackerGeometricDetExtra.h"

#include <memory>

namespace edm {
  class ConfigurationDescriptions;
}
class PTrackerGeometricDetExtra;
class PTrackerGeometricDetExtraRcd;


class TrackerGeometricDetExtraESModule : public edm::ESProducer {
public:
  TrackerGeometricDetExtraESModule(const edm::ParameterSet&);

  using ReturnType = std::unique_ptr<PTrackerGeometricDetExtra>;

  static void fillDescriptions(edm::ConfigurationDescriptions&);

  ReturnType produce(const PTrackerGeometricDetExtraRcd&);

private:
  edm::ESGetToken<GeometricDet, IdealGeometryRecord> geometricDetToken_;
};



TrackerGeometricDetExtraESModule::TrackerGeometricDetExtraESModule(const edm::ParameterSet& ps) {
  auto cc = setWhatProduced(this);
  geometricDetToken_ = cc.consumesFrom<GeometricDet, IdealGeometryRecord>(edm::ESInputTag()); 

}

void TrackerGeometricDetExtraESModule::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  descriptions.add("TrackerGeometricDetExtra", desc);
}

TrackerGeometricDetExtraESModule::ReturnType TrackerGeometricDetExtraESModule::produce(const PTrackerGeometricDetExtraRcd& iRecord) {
  edm::LogInfo("TrackerGeom") << "TrackerGeometricDetExtraESModule::produce(const PTrackerGeometricDetExtraRcd& iRecord)";

  auto gdex = std::make_unique<PTrackerGeometricDetExtra>();

  edm::ESTransientHandle<GeometricDet> gd = iRecord.getTransientHandle(geometricDetToken_);

  std::vector<const GeometricDet*> comp;
  gd->deepComponents(comp);
 

  for (auto& i : comp) {
    PTrackerGeometricDetExtra::Item item;
    item._geographicalId = i->geographicalId();
    item._isBricked = i->isBricked();//cpv->get<std::string_view>("isBricked") == strue_esp;
    gdex->pgdes_.emplace_back(item);
  }

  return gdex;
}



DEFINE_FWK_EVENTSETUP_MODULE(TrackerGeometricDetExtraESModule);
