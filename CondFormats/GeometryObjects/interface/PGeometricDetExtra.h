#ifndef CondFormats_PGeometricDetExtra_h
#define CondFormats_PGeometricDetExtra_h

#include "CondFormats/Serialization/interface/Serializable.h"

#include <vector>
#include <string>

class PGeometricDetExtra {
public:
  PGeometricDetExtra(){};
  ~PGeometricDetExtra(){};

  struct Item {
    int _geographicalId;  // to be converted to DetId
    bool _isBricked;

    COND_SERIALIZABLE;
  };

  std::vector<Item> pgdes_;

  COND_SERIALIZABLE;
};

#endif
