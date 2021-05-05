#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESTransientHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DetectorDescription/DDCMS/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "DetectorDescription/DDCMS/interface/DDFilteredView.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/PGeometricDetExtraRcd.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "CondFormats/GeometryObjects/interface/PGeometricDetExtra.h"

#include <memory>

namespace edm {
  class ConfigurationDescriptions;
}
class PGeometricDetExtra;
class PGeometricDetExtraRcd;

namespace cms {
  class DDCompactView;
  class DDFilteredView;
}

namespace{
   const std::string strue_esp{"true"};
 template <typename DDView>
  std::string getString_esp(const char* s, DDView const& ev) {
    DDValue val(s);
    std::vector<const DDsvalues_type*> result;
    ev.specificsV(result);
    std::vector<const DDsvalues_type*>::iterator it = result.begin();
    bool foundIt = false;
    for (; it != result.end(); ++it) {
      foundIt = DDfetch(*it, val);
      if (foundIt)
        break;
    }
    if (foundIt) {
      const std::vector<std::string>& temp = val.strings();
      if (temp.size() != 1) {
        throw cms::Exception("Configuration") << "I need 1 " << s << " tags";
      }
      return temp[0];
    }
    return "NotFound";
  }
}



class GeometricDetExtraESModule : public edm::ESProducer {
public:
  GeometricDetExtraESModule(const edm::ParameterSet&);

  using ReturnType = std::unique_ptr<PGeometricDetExtra>;

  static void fillDescriptions(edm::ConfigurationDescriptions&);

  ReturnType produce(const PGeometricDetExtraRcd&);

private:
  edm::ESGetToken<DDCompactView, IdealGeometryRecord> cpvTokenDDD_;
  edm::ESGetToken<cms::DDCompactView, IdealGeometryRecord> cpvTokenDD4Hep_;
  bool fromDD4Hep_;
};



GeometricDetExtraESModule::GeometricDetExtraESModule(const edm::ParameterSet& ps) {
  fromDD4Hep_ = ps.getParameter<bool>("fromDD4Hep");
  auto cc = setWhatProduced(this);
  cpvTokenDD4Hep_ = cc.consumesFrom<cms::DDCompactView, IdealGeometryRecord>(edm::ESInputTag());
  cpvTokenDDD_ = cc.consumesFrom<DDCompactView, IdealGeometryRecord>(edm::ESInputTag());

#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("TrackerGeom") << "GeometricDetExtraESModule::GeometricDetExtraESModule called with dd4hep: "
				  << fromDD4Hep_;
#endif
}

void GeometricDetExtraESModule::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<bool>("fromDD4Hep", false);
  descriptions.add("GeometricDetExtra", desc);
}

GeometricDetExtraESModule::ReturnType GeometricDetExtraESModule::produce(const PGeometricDetExtraRcd& iRecord) {
  edm::LogInfo("TrackerGeom") << "GeometricDetExtraESModule::produce(const PGeometricDetExtraRcd& iRecord)";

  auto gdex = std::make_unique<PGeometricDetExtra>();
   DDSpecificsFilter filter;


  if (fromDD4Hep_) {
#ifdef EDM_ML_DEBUG
    edm::LogVerbatim("TrackerGeom") << "GeometricDetExtraESModule::produce try to access cms::DDCompactView";
#endif
    edm::ESTransientHandle<cms::DDCompactView> cpv = iRecord.getTransientHandle(cpvTokenDD4Hep_);
   

    //DDFilteredView fv((*cpv), filter);

    PGeometricDetExtra::Item item;
    item._geographicalId = 1;
    item._isBricked = true ;//cpv->get<std::string_view>("isBricked") == strue_esp;
    gdex->pgdes_.emplace_back(item);
  } else {
#ifdef EDM_ML_DEBUG
    edm::LogVerbatim("TrackerGeom") << "GeometricDetExtraESModule::produce try to access DDCompactView";
#endif
    edm::ESTransientHandle<DDCompactView> cpv = iRecord.getTransientHandle(cpvTokenDDD_);
    DDFilteredView fv((*cpv), filter);
    //Should re-build all tracker components to get the detIDs *and* to be able to read `isBricked` from the XML?
    PGeometricDetExtra::Item item;
    item._geographicalId = 1;
    item._isBricked = getString_esp("isBricked", fv) == strue_esp;
    gdex->pgdes_.emplace_back(item); //This should then become a vector of {isBricked, detID, ... } pairs

  }

  return gdex;
}



DEFINE_FWK_EVENTSETUP_MODULE(GeometricDetExtraESModule);
