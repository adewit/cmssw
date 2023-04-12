#include "PixelRodBuilder.h"

#include <iostream>
using namespace edm;
using namespace std;

PixelRod* PixelRodBuilder::build(const GeometricDet* aRod, const TrackerGeometry* theGeomDetGeometry) {
  vector<const GeometricDet*> allGeometricDets = aRod->components();

  vector<const GeomDet*> theGeomDets;
  vector<const GeometricDet*> compGeometricDets;
  for ( auto& it : allGeometricDets ) {
	compGeometricDets = it->components();
	cout << "PRB::build() type " << it->type() << endl;
	if ( it->type() == GeometricDet::ITPhase2Combined ) {
		cout << "PRB::build() det with two components " << compGeometricDets[0]->geographicalId() << " " << compGeometricDets[1]->geographicalId() << endl;
		const GeomDet* theGeomDet = theGeomDetGeometry->idToDet(compGeometricDets[0]->geographicalId());
		theGeomDets.push_back(theGeomDet);
		const GeomDet* theGeomDetBrother = theGeomDetGeometry->idToDet(compGeometricDets[1]->geographicalId());
		theGeomDets.push_back(theGeomDetBrother);
	} else if ( it->type() == GeometricDet::DetUnit ) {
		cout << "PRB::build() det with  zero components " << it->geographicalId() << endl;
		const GeomDet* theGeomDet = theGeomDetGeometry->idToDet(it->geographicalId());
		theGeomDets.push_back(theGeomDet);
	}
  }
  return new PixelRod(theGeomDets);
}
