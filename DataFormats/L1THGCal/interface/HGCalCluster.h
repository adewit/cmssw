#ifndef DataFormats_L1Trigger_HGCalCluster_h
#define DataFormats_L1Trigger_HGCalCluster_h

#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/L1Trigger/interface/BXVector.h"
#include "DataFormats/L1THGCal/interface/HGCalTriggerCell.h"
#include "DataFormats/L1THGCal/interface/HGCalClusterT.h"

namespace l1t {

  class HGCalCluster : public HGCalClusterT<l1t::HGCalTriggerCell> {
  public:
    HGCalCluster() {}
    HGCalCluster(const LorentzVector p4, int pt = 0, int eta = 0, int phi = 0);

    HGCalCluster(const edm::Ptr<l1t::HGCalTriggerCell> &tc);

    ~HGCalCluster() override;

    void setModule(uint32_t module) { module_ = module; }
    uint32_t module() const { return module_; }

    void setColumn(int column) { column_ = column; }
    void setFrame(int frame) { frame_ = frame; }
    void setWaferU(unsigned waferU) { waferU_ = waferU; }
    void setWaferV(unsigned waferV) { waferV_ = waferV; }

    int frame() const { return frame_; }
    int column() const { return column_; }
    unsigned waferU() const { return waferU_; } //probably need to change to int instead of unsigned. For next iteration (implies checksum change)
    unsigned waferV() const { return waferV_; }

  private:
    uint32_t module_;
    int column_;
    int frame_;
    unsigned waferU_;
    unsigned waferV_;
  };

  typedef BXVector<HGCalCluster> HGCalClusterBxCollection;

}  // namespace l1t

#endif
