#include "PixelRodBuilder.h"

using namespace edm;
using namespace std;

PixelRod* PixelRodBuilder::build(const GeometricDet* aRod, const TrackerGeometry* theGeomDetGeometry) {
  vector<const GeometricDet*> allGeometricDets = aRod->components();

  vector<const GeomDet*> theGeomDets;
  for (vector<const GeometricDet*>::iterator it = allGeometricDets.begin(); it != allGeometricDets.end(); it++) {
    std::cout<<"The geometric det name in rod builder is "<<(*it)->name()<<std::endl;
    const GeomDet* theGeomDet = theGeomDetGeometry->idToDet((*it)->geographicalId());
    //std::cout<<"theGeomDet name in rod builder "<<theGeomDet->name()<<std::endl;
    theGeomDets.push_back(theGeomDet);
  }

  return new PixelRod(theGeomDets);
}
