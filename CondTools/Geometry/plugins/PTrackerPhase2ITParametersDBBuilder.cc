#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESTransientHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondFormats/GeometryObjects/interface/PTrackerPhase2ITParameters.h"
#include "DetectorDescription/DDCMS/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"

class PTrackerPhase2ITParametersDBBuilder : public edm::one::EDAnalyzer<edm::one::WatchRuns> {
public:
  PTrackerPhase2ITParametersDBBuilder(const edm::ParameterSet&);

  void beginRun(edm::Run const& iEvent, edm::EventSetup const&) override;
  void analyze(edm::Event const& iEvent, edm::EventSetup const&) override {}
  void endRun(edm::Run const& iEvent, edm::EventSetup const&) override {}
};

PTrackerPhase2ITParametersDBBuilder::PTrackerPhase2ITParametersDBBuilder(const edm::ParameterSet& iConfig) {}

void PTrackerPhase2ITParametersDBBuilder::beginRun(const edm::Run&, edm::EventSetup const& es) {
  PTrackerPhase2ITParameters* ptitp = new PTrackerPhase2ITParameters;
  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  if (!mydbservice.isAvailable()) {
    edm::LogError("PTrackerPhase2ITParametersDBBuilder") << "PoolDBOutputService unavailable";
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

  if (mydbservice->isNewTagRequest("PTrackerPhase2ITParametersRcd")) {
    mydbservice->createNewIOV<PTrackerPhase2ITParameters>(
        ptitp, mydbservice->beginOfTime(), mydbservice->endOfTime(), "PTrackerPhase2ITParametersRcd");
  } else {
    edm::LogError("PTrackerPhase2ITParametersDBBuilder")
        << "PTrackerPhase2ITParameters and PTrackerPhase2ITParametersRcd Tag already present";
  }
}

DEFINE_FWK_MODULE(PTrackerPhase2ITParametersDBBuilder);
