#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESTransientHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondFormats/GeometryObjects/interface/PTrackerAdditionalParametersPerDet.h"
#include "DetectorDescription/DDCMS/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"

class PTrackerAdditionalParametersPerDetDBBuilder : public edm::one::EDAnalyzer<edm::one::WatchRuns> {
public:
  PTrackerAdditionalParametersPerDetDBBuilder(const edm::ParameterSet&);

  void beginRun(edm::Run const& iEvent, edm::EventSetup const&) override;
  void analyze(edm::Event const& iEvent, edm::EventSetup const&) override {}
  void endRun(edm::Run const& iEvent, edm::EventSetup const&) override {}
};

PTrackerAdditionalParametersPerDetDBBuilder::PTrackerAdditionalParametersPerDetDBBuilder(const edm::ParameterSet& iConfig) {}

void PTrackerAdditionalParametersPerDetDBBuilder::beginRun(const edm::Run&, edm::EventSetup const& es) {
  PTrackerAdditionalParametersPerDet* ptitp = new PTrackerAdditionalParametersPerDet;
  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  if (!mydbservice.isAvailable()) {
    edm::LogError("PTrackerAdditionalParametersPerDetDBBuilder") << "PoolDBOutputService unavailable";
    return;
  }

  edm::ESTransientHandle<GeometricDet> gd;
  es.get<IdealGeometryRecord>().get(gd);

  std::vector<const GeometricDet*> comp;
  gd->deepComponents(comp);

  for (auto& i : comp) {
    ptitp->setGeographicalId(i->geographicalId());
    ptitp->setBricked(i->isBricked());
  }

  if (mydbservice->isNewTagRequest("PTrackerAdditionalParametersPerDetRcd")) {
    mydbservice->createNewIOV<PTrackerAdditionalParametersPerDet>(
        ptitp, mydbservice->beginOfTime(), mydbservice->endOfTime(), "PTrackerAdditionalParametersPerDetRcd");
  } else {
    edm::LogError("PTrackerAdditionalParametersPerDetDBBuilder")
        << "PTrackerAdditionalParametersPerDet and PTrackerAdditionalParametersPerDetRcd Tag already present";
  }
}

DEFINE_FWK_MODULE(PTrackerAdditionalParametersPerDetDBBuilder);
