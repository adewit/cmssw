#ifndef CondFormats_PTrackerGeometricDetExtra_h
#define CondFormats_PTrackerGeometricDetExtra_h

#include "CondFormats/Serialization/interface/Serializable.h"

#include <vector>
#include <string>

class PTrackerGeometricDetExtra {
public:
  PTrackerGeometricDetExtra(){};
  ~PTrackerGeometricDetExtra(){};

  struct Item {
    int _geographicalId;  // to be converted to DetId
    bool _isBricked;

    COND_SERIALIZABLE;
  };

  std::vector<Item> pgdes_;

  COND_SERIALIZABLE;
};

#endif
